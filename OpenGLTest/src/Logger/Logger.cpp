#include "Logger.h"
#include <iostream>

helpers::Logger::Logger(helpers::ITimeHelper* timeHelper) : TimeHelper(timeHelper)
{
}

void helpers::Logger::Log(const std::string& logMessage) const
{
    std::cout << TimeHelper->NowString() << ": " << logMessage << std::endl;
}
