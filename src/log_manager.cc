/*
 * Filename: log_manager.cc
 * Created on: June  7, 2024
 * Author: Lucas Araújo <araujolucas@dcc.ufmg.br>
 */

#include "log_manager.h"

LogManager::LogManager() noexcept
{
    try
    {
        this->m_logger = spdlog::basic_logger_mt("file_logger", config::LOG_FULL_PATH);
        this->m_logger->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%^%l%$] %v");
        this->m_logger->set_level(spdlog::level::trace);
    }
    catch (const spdlog::spdlog_ex& ex)
    {
        std::cerr << "Log initialization failed: " << ex.what() << std::endl;
    }
}

LogManager::~LogManager() noexcept
{
    spdlog::drop_all();
}

LogManager& LogManager::GetInstance() noexcept
{
    static LogManager instance;
    return instance;
}

void LogManager::Log(const std::string&          message,
                     spdlog::level::level_enum   level,
                     const std::source_location& location) noexcept
{
    // Get the function that called the log
    std::string funcName = location.function_name();
    funcName             = funcName.substr(0, funcName.find('('));
    funcName             = funcName.substr(funcName.rfind(' ') + 1);

    // Log the message
    m_logger->log(level, "[{}] {} ", funcName, message);
}
