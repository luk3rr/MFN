/*
 * Filename: config.h
 * Created on: June  6, 2024
 * Author: Lucas Araújo <araujolucas@dcc.ufmg.br>
 *
 * This file contains the configuration of the project.
 */

#ifndef CONFIG_H_
#define CONFIG_H_

#include <cstdint>
#include <ctime>
#include <filesystem>
#include <iomanip>
#include <iostream>
#include <sstream>

/**
 * @brief Configuration of the project.
 **/
namespace config
{
    constexpr uint64_t WORLD_GDP = 10e13; // 100 trillion dollars

    // Paths
    const std::string HOME_PATH = std::getenv("HOME") + std::string("/");

    // Log configuration
    const std::string LOG_PATH      = "/tmp/";
    const std::string LOG_FILE      = "mfn.log";
    const std::string LOG_FULL_PATH = LOG_PATH + LOG_FILE;

    // Database configuration
    const std::string DATABASE_PATH      = HOME_PATH + ".config/mfn/";
    const std::string DATABASE_FILE      = "mfn.db";
    const std::string DATABASE_FULL_PATH = DATABASE_PATH + DATABASE_FILE;

    /**
     * @brief Enumerates the possible effects that can be applied to the text.
     **/
    enum Effects : uint16_t
    {
        reset           = 0,
        bold_bright     = 1,
        underline       = 4,
        inverse         = 7,
        bold_bright_off = 21,
        underline_off   = 24,
        inverse_off     = 27
    };

    /**
     * @brief Enumerates the possible colors that can be applied to the text.
     **/
    enum Colors : uint16_t
    {
        black = 30,
        red,
        green,
        yellow,
        blue,
        magenta,
        cyan,
        white
    };

    /**
     * @brief Enumerates the possible transaction types.
     **/
    enum TransactionType : uint16_t
    {
        Debit = 50,
        Credit,
        Transfer
    };

    /**
     * @brief Namespace that contains utility functions
     **/
    namespace utils
    {
        inline void PrintColor(uint16_t color1, uint16_t color2, std::string text);

        inline void PrintColor(uint16_t color, std::string text);

        inline void PrintColorNoLine(uint16_t color, std::string text);

        inline void
        PrintColorNoLine(uint16_t color1, uint16_t color2, std::string text);

        inline void ClearConsole();

        inline void SelectDate(std::string& date);
    }; // namespace utils
} // namespace config

#endif // CONFIG_H_
