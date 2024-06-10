/*
 * Filename: db_manager.cc
 * Created on: June  7, 2024
 * Author: Lucas Araújo <araujolucas@dcc.ufmg.br>
 */

#include "db_manager.h"
#include "config.h"
#include "log_manager.h"
#include "sql_queries.h"
#include <cstdlib>
#include <filesystem>
#include <spdlog/common.h>
#include <sqlite3.h>

DBManager::DBManager()
    : m_logger(LogManager::GetInstance())
{
    // Check if path to database exists and try to create it if it doesn't
    try
    {
        if (not std::filesystem::exists(config::DATABASE_PATH))
        {
            std::filesystem::create_directories(config::DATABASE_PATH);
            this->m_logger.Log("Database directory created", spdlog::level::debug);
        }
    }
    catch (std::filesystem::filesystem_error& fse)
    {
        this->m_logger.Log("Error creating database directory: " +
                               std::string(fse.what()),
                           spdlog::level::err);
    }

    // Open database
    bool rc = sqlite3_open(config::DATABASE_FULL_PATH.c_str(), &this->m_db);
    if (rc)
    {
        this->m_logger.Log("Can't open database: " +
                               std::string(sqlite3_errmsg(this->m_db)),
                           spdlog::level::debug);

        sqlite3_close(this->m_db);
        this->m_db = nullptr;
    }
    else
    {
        std::cout << "Database opened successfully" << std::endl;
        this->m_logger.Log("Opened database successfully", spdlog::level::debug);
    }

    this->CreateTables();
}

DBManager::~DBManager() noexcept
{
    if (this->m_db)
    {
        sqlite3_close(this->m_db);
        this->m_logger.Log("Database closed", spdlog::level::debug);
    }
}

DBManager& DBManager::GetInstance() noexcept
{
    static DBManager instance;
    return instance;
}

bool DBManager::ExecuteQuery(const std::string& query) noexcept
{
    if (not this->m_db)
    {
        this->m_logger.Log("Database is not open", spdlog::level::err);
        return false;
    }

    char* errMsg = nullptr;

    this->m_logger.Log("Executing query: " + query, spdlog::level::debug);

    int rc = sqlite3_exec(this->m_db, query.c_str(), nullptr, nullptr, &errMsg);

    if (rc != SQLITE_OK)
    {
        sqlite3_free(errMsg);
        return false;
    }

    return true;
}

bool DBManager::ExecuteQueryWithResult(
    const std::string&                 query,
    std::function<void(sqlite3_stmt*)> callback) const
{
    if (not m_db)
    {
        return false;
    }

    sqlite3_stmt* stmt;
    int           rc = sqlite3_prepare_v2(m_db, query.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK)
    {
        return false;
    }

    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW)
    {
        callback(stmt);
    }

    if (rc != SQLITE_DONE)
    {
        sqlite3_finalize(stmt);
        return false;
    }

    sqlite3_finalize(stmt);
    return true;
}

void DBManager::ResetDatabase() noexcept
{
    // Drop all tables
    this->ExecuteQuery(query::DELETE_TABLE_CREDIT_CARD_PAYMENT);
    this->ExecuteQuery(query::DELETE_TABLE_CREDIT_CARD_DEBT);
    this->ExecuteQuery(query::DELETE_TABLE_CREDIT_CARD);
    this->ExecuteQuery(query::DELETE_TABLE_WALLET_TRANSACTION);
    this->ExecuteQuery(query::DELETE_TABLE_TRANSFER);
    this->ExecuteQuery(query::DELETE_TABLE_CATEGORY);
    this->ExecuteQuery(query::DELETE_TABLE_WALLET);
}

void DBManager::CreateTables()
{
    if (not this->ExecuteQuery(query::CREATE_TABLE_WALLET))
    {
        this->m_logger.Log("Error creating table Wallet", spdlog::level::err);
        throw std::runtime_error("Error creating table Wallet");
    }

    if (not this->ExecuteQuery(query::CREATE_TABLE_CATEGORY))
    {
        this->m_logger.Log("Error creating table Category", spdlog::level::err);
        throw std::runtime_error("Error creating table Category");
    }

    if (not this->ExecuteQuery(query::CREATE_TABLE_WALLET_TRANSACTION))
    {
        this->m_logger.Log("Error creating table WalletTransaction",
                           spdlog::level::err);
        throw std::runtime_error("Error creating table WalletTransaction");
    }

    if (not this->ExecuteQuery(query::CREATE_TABLE_TRANSFER))
    {
        this->m_logger.Log("Error creating table Transfer", spdlog::level::err);
        throw std::runtime_error("Error creating table Transfer");
    }

    if (not this->ExecuteQuery(query::CREATE_TABLE_CREDIT_CARD))
    {
        this->m_logger.Log("Error creating table CreditCard", spdlog::level::err);
        throw std::runtime_error("Error creating table CreditCard");
    }

    if (not this->ExecuteQuery(query::CREATE_TABLE_CREDIT_CARD_DEBT))
    {
        this->m_logger.Log("Error creating table CreditCardDebt", spdlog::level::err);
        throw std::runtime_error("Error creating table CreditCardDebt");
    }

    if (not this->ExecuteQuery(query::CREATE_TABLE_CREDIT_CARD_PAYMENT))
    {
        this->m_logger.Log("Error creating table CreditCardPayment",
                           spdlog::level::err);
        throw std::runtime_error("Error creating table CreditCardPayment");
    }
}
