#pragma once
#ifndef OPENGLTEST_SRC_TIMEHELPER_ITIMEHELPER_H_
#define OPENGLTEST_SRC_TIMEHELPER_ITIMEHELPER_H_
#include <string>

namespace helpers
{
    class ITimeHelper
    {
    public:
        virtual std::string NowString() const = 0;
    };
}
#endif //OPENGLTEST_SRC_TIMEHELPER_ITIMEHELPER_H_