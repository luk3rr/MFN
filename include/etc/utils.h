/*
 * Filename: utils.h
 * Created on: June 12, 2024
 * Author: Lucas Araújo <araujolucas@dcc.ufmg.br>
 */

#ifndef UTILS_H_
#define UTILS_H_

#include <cstdint>
#include <iomanip>
#include <sstream>
#include <string>

/**
 * @brief Namespace that contains utility functions
 **/
namespace utils
{
    /**
     * @brief Converts a string in the format "YYYY/MM/DD" to a std::tm object
     * @param dateStr The string to be converted
     * @return A std::tm object representing the date
     **/
    inline std::tm String2Date(const std::string& dateStr)
    {
        std::tm            tm = {};
        std::istringstream ss(dateStr);
        ss >> std::get_time(&tm, "%Y-%m-%d");
        return tm;
    }

    /**
     * @brief Converts a std::tm object to a string in the format "YYYY/MM/DD"
     * @param tm The std::tm object to be converted
     * @return A string representing the date
     **/
    inline std::string Date2String(const std::tm& tm)
    {
        std::ostringstream ss;
        ss << std::put_time(&tm, "%Y-%m-%d");
        return ss.str();
    }
} // namespace utils

#endif // UTILS_H_
