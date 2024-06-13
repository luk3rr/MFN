/*
 * Filename: utils.cc
 * Created on: June 12, 2024
 * Author: Lucas Araújo <araujolucas@dcc.ufmg.br>
 */

#include "utils.h"

namespace utils
{
    std::tm String2Date(const std::string& dateStr)
    {
        std::tm            tm = {};
        std::istringstream ss(dateStr);
        ss >> std::get_time(&tm, "%Y/%m/%d");
        return tm;
    }

    std::string Date2String(const std::tm& tm)
    {
        std::ostringstream ss;
        ss << std::put_time(&tm, "%Y/%m/%d");
        return ss.str();
    }
} // namespace utils
