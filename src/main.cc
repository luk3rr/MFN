/*
 * Filename: main.cc
 * Created on: June  7, 2024
 * Author: Lucas Araújo <araujolucas@dcc.ufmg.br>
 */

#include <iostream>

#include "config.h"
#include "db_manager.h"
#include "log_manager.h"

int main()
{
    LogManager& logger = LogManager::GetInstance();
    DBManager& db = DBManager::GetInstance();

    logger.Log("Hello, World!", spdlog::level::info);
    db.ExecuteQuery("SELECT * FROM users");

    return EXIT_SUCCESS;
}
