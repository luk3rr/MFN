/*
 * Filename: db_manager.cc
 * Created on: June  7, 2024
 * Author: Lucas Araújo <araujolucas@dcc.ufmg.br>
 */

#include "db_manager.h"
#include "config.h"
#include "log_manager.h"
#include <filesystem>
#include <spdlog/common.h>
#include <sqlite3.h>

DBManager::DBManager() noexcept
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
    int   rc     = sqlite3_exec(this->m_db, query.c_str(), nullptr, nullptr, &errMsg);
    if (rc != SQLITE_OK)
    {
        this->m_logger.Log("SQL error: " + std::string(errMsg), spdlog::level::err);
        sqlite3_free(errMsg);
        return false;
    }

    return true;
}
