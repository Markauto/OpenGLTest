#include "Logger.h"
#include "../StringFormater/StringFormater.h"
#include <iostream>

helpers::Logger::Logger(helpers::ITimeHelper* timeHelper) : TimeHelper(timeHelper)
{
}

void helpers::Logger::Log(const std::string& logMessage) const
{
    std::cout << helpers::StringFormater::Format("%s: %s", TimeHelper->NowString(), logMessage) << std::endl;
}
