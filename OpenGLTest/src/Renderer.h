#pragma once
#ifndef OPENGLTEST_SRC_RENDERER_H_
#define OPENGLTEST_SRC_RENDERER_H_

#include <memory>
#include <GL/glew.h>

#include "Logger/ILogger.h"
#include "TimeHelper/TimeHelper.h"
#include "Logger/Logger.h"
#include "StringFormater/StringFormater.h"

// TODO: Move setup into factory classes or summet
extern std::unique_ptr<helpers::ILogger> TheLogger;
extern std::unique_ptr<helpers::ITimeHelper> TheTimeHelper;

#define ASSERT(x) if (!(x)) __debugbreak()
#define GLCALL(x) GLClearError(); \
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__));
#ifdef DEBUG_LOGGING

#define DEBUGLOG(x) TheLogger->Log(x)

#else

#define DEBUGLOG(x) 

#endif// LOGGER_ENABLED

void Setup();
void GLClearError();
bool GLLogCall(const char* function, const char* file, int line);

#endif // OPENGLTEST_SRC_RENDERER_H_