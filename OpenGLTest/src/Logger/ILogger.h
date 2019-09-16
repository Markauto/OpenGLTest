#pragma once
#ifndef OPENGLTEST_SRC_LOGGER_ILOGGER_H_
#define OPENGLTEST_SRC_LOGGER_ILOGGER_H_
#include <string>
namespace helpers
{
    class ILogger
    {
    public:
        virtual void Log(const std::string& logMessage) const = 0;
    };
}

#endif //OPENGLTEST_SRC_LOGGER_ILOGGER_H_