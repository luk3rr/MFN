/*
 * Filename: utils.h
 * Created on: June 12, 2024
 * Author: Lucas Araújo <araujolucas@dcc.ufmg.br>
 */

#ifndef UTILS_H_
#define UTILS_H_

#include <iomanip>
#include <sstream>
#include <string>

namespace utils
{
    /**
     * @brief Converts a string in the format "YYYY/MM/DD" to a std::tm object
     * @param dateStr The string to be converted
     * @return A std::tm object representing the date
     **/
    std::tm String2Date(const std::string& dateStr);

    /**
     * @brief Converts a std::tm object to a string in the format "YYYY/MM/DD"
     * @param tm The std::tm object to be converted
     * @return A string representing the date
     **/
    std::string Date2String(const std::tm& tm);
} // namespace utils

#endif // UTILS_H_
