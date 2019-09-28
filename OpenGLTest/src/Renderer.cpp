#include "Renderer.h"

void GLClearError()
{
    while (glGetError() != GL_NO_ERROR);
}


bool GLLogCall(const char* function, const char* file, int line)
{
    bool noErrors = true;
    while (GLenum error = glGetError())
    {
        helpers::TimeHelper timeHelper;
        helpers::Logger logger(&timeHelper);
        logger.Log(helpers::StringFormater::Format(
            "[OpenGL Error] (%s): %s : %s : %s", error, function, file, line));
        noErrors = false;
    }
    return noErrors;
}

Renderer::Renderer(const ImVec4& clearColour) : m_ClearColour(clearColour)
{
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
    GLCALL(glClearColor(m_ClearColour.x, m_ClearColour.y, m_ClearColour.z, m_ClearColour.w));
    GLCALL(glClear(GL_COLOR_BUFFER_BIT));
}
