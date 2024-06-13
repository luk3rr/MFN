/*
 * Filename: credit_card_manager.cc
 * Created on: June 11, 2024
 * Author: Lucas Araújo <araujolucas@dcc.ufmg.br>
 */

#include "credit_card_manager.h"
#include "config.h"
#include "db_manager.h"
#include <cmath>
#include <cstdint>
#include <fmt/format.h>
#include <string>

CreditCardManager::CreditCardManager() noexcept
    : m_dbManager(DBManager::GetInstance()),
      m_logManager(LogManager::GetInstance())
{ }

CreditCardManager::~CreditCardManager() noexcept { }

void CreditCardManager::GetCreditCards(
    std::vector<std::string>& creditCards) const noexcept
{
    creditCards.clear();

    std::string query = "SELECT number FROM CreditCard;";

    this->m_dbManager.ExecuteQueryWithResult(query, [&creditCards](sqlite3_stmt* stmt) {
        creditCards.push_back(
            std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0))));
    });
}

bool CreditCardManager::GetCreditCardInfo(const std::string& cardNumber,
                                          std::string&       cardName,
                                          double_t&          maxDebt,
                                          uint16_t& billingDueDay) const noexcept
{
    if (not this->CreditCardExists(cardNumber))
    {
        this->m_logManager.Log(
            fmt::format("Credit card '{}' does not exist.", cardNumber));

        return false;
    }

    std::string query =
        fmt::format("SELECT name, max_debt, billing_due_day FROM CreditCard "
                    "WHERE number = '{}';",
                    cardNumber);

    return this->m_dbManager.ExecuteQueryWithResult(
        query,
        [&cardName, &maxDebt, &billingDueDay](sqlite3_stmt* stmt) {
            cardName = std::string(
                reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0)));
            maxDebt       = sqlite3_column_double(stmt, 1);
            billingDueDay = sqlite3_column_int(stmt, 2);
        });
}

bool CreditCardManager::AddCreditCard(const std::string& cardNumber,
                                      const uint16_t     billingDueDay,
                                      const std::string& cardName,
                                      const double_t     maxDebt) noexcept
{
    if (this->CreditCardExists(cardNumber))
    {
        this->m_logManager.Log(
            fmt::format("Credit card '{}' already exists.", cardNumber));
        return false;
    }

    if (maxDebt <= 0)
    {
        this->m_logManager.Log(fmt::format("Invalid max debt: {}", maxDebt));
        return false;
    }

    if (billingDueDay < config::MIN_BILLING_DAY or
        billingDueDay > config::MAX_BILLING_DAY)
    {
        this->m_logManager.Log(
            fmt::format("Invalid billing due day: {}. It must be between {} and {}.",
                        billingDueDay,
                        config::MIN_BILLING_DAY,
                        config::MAX_BILLING_DAY));
        return false;
    }

    std::string query =
        fmt::format("INSERT INTO CreditCard (number, name, "
                    "max_debt, billing_due_day) VALUES ('{}', '{}', '{}', {});",
                    cardNumber,
                    cardName,
                    maxDebt,
                    billingDueDay);

    if (this->m_dbManager.ExecuteQuery(query))
    {
        this->m_logManager.Log(fmt::format("Credit card '{}' added.", cardName));
    }
    else
    {
        this->m_logManager.Log(
            fmt::format("Failed to add credit card '{}'.", cardName));
        return false;
    }

    return true;
}

bool CreditCardManager::AddDebt(const std::string& cardNumber,
                                const std::string& category,
                                const std::string& date,
                                const double_t     totalAmount,
                                const std::string& description,
                                const uint16_t     installments) noexcept
{
    if (not this->CreditCardExists(cardNumber))
    {
        this->m_logManager.Log(
            fmt::format("Credit card '{}' does not exist.", cardNumber));
        return false;
    }

    if (totalAmount <= 0)
    {
        this->m_logManager.Log(fmt::format("Invalid total amount: {}", totalAmount));
        return false;
    }

    if (installments < 1)
    {
        this->m_logManager.Log(
            fmt::format("Invalid number of installments: {}", installments));
        return false;
    }

    if (not this->HasEnoughCredit(cardNumber, totalAmount))
    {
        this->m_logManager.Log(
            fmt::format("Credit card '{}' has not enough credit for debt of {}.",
                        cardNumber,
                        totalAmount));
        return false;
    }

    // Get category id
    std::string getCategoryId =
        fmt::format("SELECT id FROM Category WHERE name = '{}';", category);

    uint32_t category_id = 0;

    this->m_dbManager.ExecuteQueryWithResult(getCategoryId,
                                             [&category_id](sqlite3_stmt* stmt) {
                                                 category_id =
                                                     sqlite3_column_int(stmt, 0);
                                             });

    // Insert debt
    std::string insertDebt =
        fmt::format("INSERT INTO CreditCardDebt (crc_number, category_id, "
                    "date, total_amount, description) VALUES "
                    "('{}', '{}', {}, '{}', '{}');",
                    cardNumber,
                    category_id,
                    date,
                    totalAmount,
                    description);

    if (this->m_dbManager.ExecuteQuery(insertDebt))
    {

        // Get debt id most recently added
        std::string getDebtId =
            "SELECT debt_id FROM CreditCardDebt ORDER BY debt_id DESC LIMIT 1;";

        uint32_t debt_id = 0;

        this->m_dbManager.ExecuteQueryWithResult(
            getDebtId,
            [&debt_id](sqlite3_stmt* stmt) { debt_id = sqlite3_column_int(stmt, 0); });

        // Insert installments into CreditCardPayment
        double_t installmentAmount = totalAmount / installments;

        for (uint32_t i = 1; i <= installments; i++)
        {
            // Get due date of the installment
            std::string dueDate = this->GetInstallmentDueDate(cardNumber, date, i);

            std::string insertInstallment =
                fmt::format("INSERT INTO CreditCardPayment (debt_id, date, amount, "
                            "installment) VALUES "
                            "({}, {}, {}, {});",
                            debt_id,
                            dueDate,
                            installmentAmount,
                            i);

            // Insert installment
            this->m_dbManager.ExecuteQuery(insertInstallment);
        }
    }
    else
    {
        this->m_logManager.Log(
            fmt::format("Failed to add debt for credit card '{}'.", cardNumber));
        return false;
    }

    return true;
}

bool CreditCardManager::CreditCardExists(const std::string& cardNumber) const noexcept
{
    // Query to get the number of credit cards with the given number
    std::string query =
        fmt::format("SELECT COUNT(*) FROM CreditCard WHERE number = '{}';", cardNumber);

    int count = 0;

    this->m_dbManager.ExecuteQueryWithResult(query, [&count](sqlite3_stmt* stmt) {
        count = sqlite3_column_int(stmt, 0);
    });

    return count > 0;
}

double_t CreditCardManager::GetMaxDebt(const std::string& cardNumber) const
{
    if (not this->CreditCardExists(cardNumber))
    {
        this->m_logManager.Log(
            fmt::format("Credit card '{}' does not exist.", cardNumber));

        throw std::invalid_argument(
            fmt::format("Credit card '{}' does not exist.", cardNumber));
    }

    // Query to get the max debt of the credit card
    std::string query =
        fmt::format("SELECT max_debt FROM CreditCard WHERE number = '{}';", cardNumber);

    double_t maxDebt = 0;

    this->m_dbManager.ExecuteQueryWithResult(query, [&maxDebt](sqlite3_stmt* stmt) {
        maxDebt = sqlite3_column_double(stmt, 0);
    });

    return maxDebt;
}

double_t CreditCardManager::GetTotalPendingDebt(const std::string& cardNumber) const
{
    if (not this->CreditCardExists(cardNumber))
    {
        this->m_logManager.Log(
            fmt::format("Credit card '{}' does not exist.", cardNumber));

        throw std::invalid_argument(
            fmt::format("Credit card '{}' does not exist.", cardNumber));
    }

    // CreditCardPayment.wallet NULL means that the installment has not been paid yet
    std::string query =
        fmt::format("SELECT SUM(CreditCardPayment.amount) AS total_amount "
                    "FROM CreditCardDebt "
                    "INNER JOIN CreditCardPayment "
                    "ON CreditCardDebt.debt_id = CreditCardPayment.debt_id "
                    "WHERE CreditCardDebt.crc_number = '{}' "
                    "AND CreditCardPayment.wallet IS NULL;",
                    cardNumber);

    double_t totalPendingDebt = 0;

    this->m_dbManager.ExecuteQueryWithResult(query,
                                             [&totalPendingDebt](sqlite3_stmt* stmt) {
                                                 totalPendingDebt =
                                                     sqlite3_column_double(stmt, 0);
                                             });

    return totalPendingDebt;
}

bool CreditCardManager::HasEnoughCredit(const std::string& cardNumber,
                                        const double_t     expense) const
{
    double_t maxDebt          = this->GetMaxDebt(cardNumber);
    double_t totalPendingDebt = this->GetTotalPendingDebt(cardNumber);

    return maxDebt - totalPendingDebt >= expense;
}

std::string CreditCardManager::GetInstallmentDueDate(const std::string& cardNumber,
                                                     const std::string& purchaseDate,
                                                     uint16_t installmentNumber) const
{
    if (not this->CreditCardExists(cardNumber))
    {
        this->m_logManager.Log(
            fmt::format("Credit card '{}' does not exist.", cardNumber));

        throw std::invalid_argument(
            fmt::format("Credit card '{}' does not exist.", cardNumber));
    }

    // Query to get the due date of the installment
    std::string query =
        fmt::format("SELECT billing_due_day FROM CreditCard WHERE number = '{}';",
                    cardNumber);

    uint16_t billingDueDay = 0;

    this->m_dbManager.ExecuteQueryWithResult(query,
                                             [&billingDueDay](sqlite3_stmt* stmt) {
                                                 billingDueDay =
                                                     sqlite3_column_int(stmt, 0);
                                             });

    std::tm purchaseDateTm = utils::String2Date(purchaseDate);

    // Getting the due date of the installment
    std::chrono::system_clock::time_point purchaseTimePoint =
        std::chrono::system_clock::from_time_t(std::mktime(&purchaseDateTm));
    std::chrono::system_clock::time_point dueDateTimePoint =
        purchaseTimePoint + std::chrono::months(installmentNumber);

    // Getting the due date tm
    std::time_t dueDateTime = std::chrono::system_clock::to_time_t(dueDateTimePoint);
    std::tm*    dueDateTm   = std::localtime(&dueDateTime);

    // Adjusting the day of the due date
    dueDateTm->tm_mday = billingDueDay;

    return utils::Date2String(*dueDateTm);
}
