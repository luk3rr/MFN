/*
 * Filename: log_manager.h
 * Created on: June  7, 2024
 * Author: Lucas Araújo <araujolucas@dcc.ufmg.br>
 *
 * This file contains the declaration of the LogManager class.
 * This class is responsible for logging messages to the console and to a file.
 */

#ifndef LOG_MANAGER_H_
#define LOG_MANAGER_H_

#include <source_location>
#include <string>

#include "config.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/spdlog.h"

/**
 * @brief The LogManager class is a singleton class that is responsible for logging
 *messages to the console and to a file.
 **/
class LogManager
{
    private:
        std::shared_ptr<spdlog::logger> m_logger;

        /**
         * @brief Default constructor
         **/
        LogManager() noexcept;

    public:
        /**
         * @brief Default destructor
         **/
        ~LogManager() noexcept;

        /**
         * @brief Get the singleton instance of the LogManager class.
         * @return The singleton instance of the LogManager class.
         **/
        static LogManager& GetInstance() noexcept;

        /**
         * @brief Log a message to the console and to a file.
         *
         * @param message The message to be logged.
         * @param level The level of the message.
         **/
        void Log(const std::string&          message,
                 spdlog::level::level_enum   level = spdlog::level::info,
                 const std::source_location& location =
                     std::source_location::current()) noexcept;
};

#endif // LOG_MANAGER_H_
