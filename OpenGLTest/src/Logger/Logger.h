#pragma once
#ifndef OPENGLTEST_SRC_LOGGER_LOGGER_H_
#define OPENGLTEST_SRC_LOGGER_LOGGER_H_

#include "ILogger.h"
#include "../TimeHelper/ITimeHelper.h"
namespace helpers
{
    class Logger :
        public helpers::ILogger
    {
    public:
        Logger(helpers::ITimeHelper* timeHelper);
        void Log(const std::string& LogMessage) const override;
    private:
        helpers::ITimeHelper* TimeHelper;
    };
}

#endif //OPENGLTEST_SRC_LOGGER_LOGGER_H_