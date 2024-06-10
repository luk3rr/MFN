/*
 * Filename: db_manager.h
 * Created on: June  7, 2024
 * Author: Lucas Araújo <araujolucas@dcc.ufmg.br>
 */

#ifndef DB_MANAGER_H_
#define DB_MANAGER_H_

#include <iostream>
#include <sqlite3.h>
#include <string>

#include "log_manager.h"

/**
 * @brief Class responsible for managing the database
 **/
class DBManager
{
    private:
        sqlite3*    m_db;
        LogManager& m_logger;

        /**
         * @brief Default constructor
         **/
        DBManager();

    public:
        /**
         * @brief Get the singleton instance of the class
         * @return DBManager& Singleton instance of the class
         **/
        static DBManager& GetInstance() noexcept;

        /**
         * @brief Default destructor
         **/
        ~DBManager() noexcept;

        /**
         * @brief Execute a SQL command
         * @param query SQL command to be executed
         * @return bool SQLITE_OK if the command was executed successfully
         **/
        bool ExecuteQuery(const std::string& query) noexcept;

        bool ExecuteQueryWithResult(const std::string&                 query,
                                    std::function<void(sqlite3_stmt*)> callback) const;

        void ResetDatabase() noexcept;

    private:
        /**
         * @brief Create the tables in the database
         **/
        void CreateTables();
};
#endif // DB_MANAGER_H_
