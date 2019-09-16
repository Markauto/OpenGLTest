#include "TimeHelper.h"
#include <chrono>
#include <time.h>  

std::string helpers::TimeHelper::NowString() const
{
    const auto now = std::chrono::system_clock::now();
    const auto nowTimeT = std::chrono::system_clock::to_time_t(now);
    char str[26];
    ctime_s(str, sizeof str, &nowTimeT);
    str[24] = ' ';
    return str;
}
