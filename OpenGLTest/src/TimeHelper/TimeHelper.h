#pragma once
#ifndef OPENGLTEST_SRC_TIMEHELPER_TIMEHELPER_H_
#define OPENGLTEST_SRC_TIMEHELPER_TIMEHELPER_H_
#include <string>
#include "ITimeHelper.h"
namespace helpers
{
    class TimeHelper : public ITimeHelper
    {
    public:
        std::string NowString() const override;
    };
}
#endif //OPENGLTEST_SRC_TIMEHELPER_TIMEHELPER_H_