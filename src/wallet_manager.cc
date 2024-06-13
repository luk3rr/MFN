/*
 * Filename: wallet_manager.cc
 * Created on: June  8, 2024
 * Author: Lucas Araújo <araujolucas@dcc.ufmg.br>
 */

#include "wallet_manager.h"
#include "category_manager.h"
#include "db_manager.h"
#include "sql_queries.h"
#include <cmath>
#include <cstddef>
#include <fmt/core.h>
#include <fmt/format.h>
#include <stdexcept>
#include <vector>

WalletManager::WalletManager() noexcept
    : m_dbManager(DBManager::GetInstance()),
      m_logManager(LogManager::GetInstance())
{ }

WalletManager::~WalletManager() noexcept { }

void WalletManager::GetWallets(std::vector<std::string>& wallets) noexcept
{
    wallets.clear();

    std::string query = "SELECT name FROM Wallet;";

    this->m_dbManager.ExecuteQueryWithResult(query, [&wallets](sqlite3_stmt* stmt) {
        wallets.push_back(
            std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0))));
    });
}

void WalletManager::GetWallets(std::vector<std::string>& wallets,
                               std::vector<double_t>&    balances) noexcept
{
    wallets.clear();
    balances.clear();

    std::string query = "SELECT name, balance FROM Wallet;";

    this->m_dbManager.ExecuteQueryWithResult(
        query,
        [&wallets, &balances](sqlite3_stmt* stmt) {
            wallets.push_back(std::string(
                reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0))));
            balances.push_back(sqlite3_column_double(stmt, 1));
        });
}

void WalletManager::CreateWallet(const std::string& walletName,
                                 const double_t     initialBalance) noexcept
{
    std::string query =
        fmt::format("INSERT INTO Wallet (name, balance) VALUES ('{}', '{}')",
                    walletName,
                    initialBalance);

    if (this->WalletExists(walletName))
    {
        this->m_logManager.Log("Wallet '" + walletName + "' already exists.");
    }
    else
    {
        this->m_dbManager.ExecuteQuery(query);
        this->m_logManager.Log("Wallet '" + walletName + "' created.");
    }
}

void WalletManager::DeleteWallet(const std::string& walletName) noexcept
{
    std::string query =
        fmt::format("DELETE FROM Wallet WHERE name = '{}';", walletName);

    if (this->WalletExists(walletName))
    {
        this->m_dbManager.ExecuteQuery(query);
        this->m_logManager.Log("Wallet '" + walletName + "' deleted.");
    }
    else
    {
        this->m_logManager.Log("Wallet '" + walletName + "' does not exist.");
    }
}

void WalletManager::Expense(const std::string& walletName,
                            const std::string& category,
                            const std::string& date,
                            const std::string& description,
                            const double_t     amount) noexcept
{
    // Check if wallet exists
    if (not this->WalletExists(walletName))
    {
        this->m_logManager.Log("Wallet '" + walletName + "' does not exist.");
        return;
    }

    // Check if amount is valid
    if (amount <= 0)
    {
        this->m_logManager.Log("Invalid income amount.");
        return;
    }

    // Check if wallet has sufficient balance
    double_t balance = this->GetBalance(walletName);

    if (balance < amount)
    {
        this->m_logManager.Log("Insufficient balance in wallet '" + walletName + "'.");
        return;
    }

    // Check if category exists
    if (not this->m_categoryManager.CategoryExists(category))
    {
        this->m_logManager.Log("Category '" + category +
                               "' does not exist. Creating it.");
        this->m_categoryManager.CreateCategory(category);
    }

    // Insert transaction
    try
    {
        std::string query = fmt::format("INSERT INTO WalletTransaction (wallet, "
                                        "category_id, type, date, amount, description) "
                                        "VALUES ('{}', {}, '{}', '{}', {}, '{}');",
                                        walletName,
                                        this->m_categoryManager.GetCategoryID(category),
                                        "EXPENSE",
                                        date,
                                        amount,
                                        description);

        if (not this->m_dbManager.ExecuteQuery(query))
        {
            this->m_logManager.Log("Failed to register expense of " +
                                       std::to_string(amount) + " in wallet '" +
                                       walletName + "'.",
                                   spdlog::level::err);
            return;
        }
    }
    catch (std::runtime_error& re)
    {
        this->m_logManager.Log(re.what(), spdlog::level::err);
        return;
    }

    // Update wallet balance
    this->UpdateBalance(walletName, balance - amount);

    this->m_logManager.Log("Expense of " + std::to_string(amount) + " in wallet '" +
                           walletName + "' registered.");
}

void WalletManager::Income(const std::string& walletName,
                           const std::string& category,
                           const std::string& date,
                           const std::string& description,
                           const double_t     amount) noexcept
{
    // Check if wallet exists
    if (not this->WalletExists(walletName))
    {
        this->m_logManager.Log("Wallet '" + walletName + "' does not exist.");
        return;
    }

    // Check if amount is valid
    if (amount <= 0)
    {
        this->m_logManager.Log("Invalid income amount.");
        return;
    }

    // Check if category exists
    if (not this->m_categoryManager.CategoryExists(category))
    {
        this->m_logManager.Log("Category '" + category +
                               "' does not exist. Creating it.");
        this->m_categoryManager.CreateCategory(category);
    }

    // Insert transaction
    try
    {
        std::string query = fmt::format("INSERT INTO WalletTransaction (wallet, "
                                        "category_id, type, date, amount, description) "
                                        "VALUES ('{}', {}, '{}', '{}', {}, '{}');",
                                        walletName,
                                        this->m_categoryManager.GetCategoryID(category),
                                        "INCOME",
                                        date,
                                        amount,
                                        description);

        this->m_dbManager.ExecuteQuery(query);
    }
    catch (std::runtime_error& re)
    {
        this->m_logManager.Log(re.what(), spdlog::level::err);
        return;
    }

    // Update wallet balance
    double_t balance = this->GetBalance(walletName);
    this->UpdateBalance(walletName, balance + amount);

    this->m_logManager.Log("Income of " + std::to_string(amount) + " in wallet '" +
                           walletName + "' registered.");
}

void WalletManager::Transfer(const std::string& fromWallet,
                             const std::string& toWallet,
                             const std::string& date,
                             const double_t     amount) noexcept
{
    // Check if wallets exist
    if (not this->WalletExists(fromWallet))
    {
        this->m_logManager.Log("Source wallet '" + fromWallet + "' does not exist.");
        return;
    }

    if (not this->WalletExists(toWallet))
    {
        this->m_logManager.Log("Destination wallet '" + toWallet + "' does not exist.");
        return;
    }

    if (fromWallet == toWallet)
    {
        this->m_logManager.Log("Source and destination wallets are the same.");
        return;
    }

    if (amount <= 0)
    {
        this->m_logManager.Log("Invalid transfer amount.");
        return;
    }

    // Check if fromWallet has sufficient balance
    double_t balance = this->GetBalance(fromWallet);

    if (balance < amount)
    {
        this->m_logManager.Log("Insufficient balance in source wallet '" + fromWallet +
                               "'.");
        return;
    }

    // Insert transaction
    try
    {
        std::string query = fmt::format("INSERT INTO Transfer (sender_wallet, "
                                        "receiver_wallet, date, amount) "
                                        "VALUES ('{}', {}, '{}', '{}');",
                                        fromWallet,
                                        toWallet,
                                        date,
                                        amount);

        this->m_dbManager.ExecuteQuery(query);
    }
    catch (std::runtime_error& re)
    {
        this->m_logManager.Log(re.what(), spdlog::level::err);
        return;
    }

    // Update wallet balances
    this->UpdateBalance(fromWallet, balance - amount);
    this->UpdateBalance(toWallet, this->GetBalance(toWallet) + amount);

    this->m_logManager.Log("Transfer of " + std::to_string(amount) + " from wallet '" +
                           fromWallet + "' to wallet '" + toWallet + "' registered.");
}

bool WalletManager::WalletExists(const std::string& walletName) noexcept
{
    std::string query =
        "SELECT COUNT(*) FROM Wallet WHERE name = '" + walletName + "';";

    int count = 0;

    this->m_dbManager.ExecuteQueryWithResult(query, [&count](sqlite3_stmt* stmt) {
        count = sqlite3_column_int(stmt, 0);
    });

    return count > 0;
}

void WalletManager::UpdateBalance(const std::string& walletName,
                                  const double_t     newBalance) noexcept
{
    std::string query = fmt::format("UPDATE Wallet SET balance = {} WHERE name = '{}';",
                                    newBalance,
                                    walletName);

    if (this->WalletExists(walletName))
    {
        this->m_dbManager.ExecuteQuery(query);
        this->m_logManager.Log("Balance for wallet '" + walletName + "' updated.");
    }
    else
    {
        this->m_logManager.Log("Wallet '" + walletName + "' does not exist.");
    }
}

double_t WalletManager::GetBalance(const std::string& walletName) noexcept
{
    std::string query =
        fmt::format("SELECT balance FROM Wallet WHERE name = '{}';", walletName);

    double_t balance = 0.0;

    this->m_dbManager.ExecuteQueryWithResult(query, [&balance](sqlite3_stmt* stmt) {
        balance = sqlite3_column_double(stmt, 0);
    });

    return balance;
}
