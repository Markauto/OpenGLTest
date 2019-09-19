#include "Renderer.h"
std::unique_ptr<helpers::ILogger> TheLogger;
std::unique_ptr<helpers::ITimeHelper> TheTimeHelper;

void Setup()
{
    ASSERT(sizeof(unsigned int) == sizeof(GLuint));

    TheTimeHelper = std::make_unique<helpers::TimeHelper>();
    TheLogger = std::make_unique<helpers::Logger>(TheTimeHelper.get());
}

void GLClearError()
{
    while (glGetError() != GL_NO_ERROR);
}


bool GLLogCall(const char* function, const char* file, int line)
{
    bool noErrors = true;
    while (GLenum error = glGetError())
    {
        TheLogger->Log(helpers::StringFormater::Format(
            "[OpenGL Error] (%s): %s : %s : %s", error, function, file, line));
        noErrors = false;
    }
    return noErrors;
}

void Renderer::Draw(const VertexArray& vertexArray, const IndexBuffer& indexBuffer, const Shader& shader)
{
    shader.Bind();
    vertexArray.Bind();
    indexBuffer.Bind();
    GLCALL(glDrawElements(GL_TRIANGLES, indexBuffer.GetCount(), GL_UNSIGNED_INT, nullptr));
}

void Renderer::Clear() const
{
    GLCALL(glClear(GL_COLOR_BUFFER_BIT));
}