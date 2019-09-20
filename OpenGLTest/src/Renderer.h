#pragma once
#ifndef OPENGLTEST_SRC_RENDERER_H_
#define OPENGLTEST_SRC_RENDERER_H_

#include <memory>
#include <GL/glew.h>

#include "Logger/ILogger.h"
#include "TimeHelper/TimeHelper.h"
#include "Logger/Logger.h"
#include "StringFormater/StringFormater.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "VertexArray.h"
#include "Logger/Logger.h"

#define ASSERT(x) if (!(x)) __debugbreak()
#define GLCALL(x) GLClearError(); \
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__));
#ifdef DEBUG_LOGGING

#define DEBUGLOG(x) TheLogger->Log(x)

#else

#define DEBUGLOG(x) 

#endif// LOGGER_ENABLED

void GLClearError();
bool GLLogCall(const char* function, const char* file, int line);

class Renderer
{
public:
    void Draw(const VertexArray& vertexArray, const IndexBuffer& indexBuffer, const Shader& shader);
    void Clear() const;
};

#endif // OPENGLTEST_SRC_RENDERER_H_