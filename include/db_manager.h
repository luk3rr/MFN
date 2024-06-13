/*
 * Filename: db_manager.h
 * Created on: June  7, 2024
 * Author: Lucas Araújo <araujolucas@dcc.ufmg.br>
 *
 * This file contains the declaration of the DBManager class. This class is
 * responsible for managing the database.
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

        /**
         * @brief Execute a SQL command and return the result
         * @param query SQL command to be executed
         * @param callback Function to be called with the result of the query
         * @return bool SQLITE_OK if the command was executed successfully
         **/
        bool ExecuteQueryWithResult(const std::string&                 query,
                                    std::function<void(sqlite3_stmt*)> callback) const;

        /**
         * @brief Reset the database
         * NOTE: This method will delete all the data in the database. Used for testing
         *purposes
         **/
        void ResetDatabase() noexcept;

    private:
        /**
         * @brief Create the tables in the database
         **/
        void CreateTables();
};
#endif // DB_MANAGER_H_
