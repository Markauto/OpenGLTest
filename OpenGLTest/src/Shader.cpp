#include "Shader.h"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <string>

#include "GL/glew.h"
#include "Renderer.h"
#include "Logger/ILogger.h"


Shader::Shader(helpers::ILogger* logger, const std::string& filePath) :
    m_Logger(logger), m_FilePath(filePath), m_RendererId(0)
{
    auto [vertexShaderSource, fragmentShaderSource] = ParseShader();
    m_RendererId = CreateShader(vertexShaderSource, fragmentShaderSource);
}

Shader::~Shader()
{
    GLCALL(glDeleteProgram(m_RendererId));
}

unsigned int Shader::CompileShader(unsigned int type, const std::string& source)
{
    GLCALL(unsigned int id = glCreateShader(type));
    const char* src = source.c_str();
    GLCALL(glShaderSource(id, 1, &src, nullptr));
    GLCALL(glCompileShader(id));

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE)
    {
        int length;
        GLCALL(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
        GLCALL(char* message = (char*)alloca(length * sizeof(char)));
        GLCALL(glGetShaderInfoLog(id, length, &length, message));
        m_Logger->Log(helpers::StringFormater::Format(
            "Failed to compile %s shader :( \n %s",
            (type == GL_VERTEX_SHADER ? "vertex" : "fragment"),
            message));
        GLCALL(glDeleteShader(id));
        return 0;
    }

    return id;
}

unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    GLCALL(unsigned int program = glCreateProgram());
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    GLCALL(glAttachShader(program, vs));
    GLCALL(glAttachShader(program, fs));
    GLCALL(glLinkProgram(program));
    GLCALL(glValidateProgram(program));

    GLCALL(glDeleteShader(vs));
    GLCALL(glDeleteShader(fs));

    return program;
}

std::tuple<std::string, std::string> Shader::ParseShader()
{
    std::ifstream stream(m_FilePath);

    enum class ShaderType
    {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };

    std::string line;
    std::stringstream ss[2];
    ShaderType type = ShaderType::NONE;
    while (getline(stream, line))
    {
        if (line.find("#shader") != std::string::npos)
        {
            if (line.find("vertex") != std::string::npos)
            {
                type = ShaderType::VERTEX;
            }
            else if (line.find("fragment") != std::string::npos)
            {
                type = ShaderType::FRAGMENT;
            }
        }
        else
        {
            ss[(int)type] << line << '\n';
        }
    }

    return { ss[0].str(), ss[1].str() };
}

void Shader::Bind() const
{
    GLCALL(glUseProgram(m_RendererId));
}

void Shader::Unbind() const
{
    GLCALL(glUseProgram(0));
}

void Shader::SetUniform1i(const std::string& name, int value)
{
    GLCALL(glUniform1i(GetUniformLocation(name), value));
}

void Shader::SetUniform1f(const std::string& name, float value)
{
    GLCALL(glUniform1f(GetUniformLocation(name), value));
}

void Shader::SetUniform4f(const std::string& name, float value0, float value1, float value2, float value3)
{
    GLCALL(glUniform4f(GetUniformLocation(name), value0, value1, value2, value3));
}

void Shader::SetUniformMat4f(const std::string& name, const glm::mat4& matrix)
{
    GLCALL(glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]));
}

int Shader::GetUniformLocation(const std::string& name)
{
    if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
    {
        return m_UniformLocationCache[name];
    }

    GLCALL(int location = glGetUniformLocation(m_RendererId, name.c_str()));
    if (location == -1)
    {
        m_Logger->Log(helpers::StringFormater::Format("Warning: uniform '%s' doesn't exist!", name));
    }

    m_UniformLocationCache[name] = location;

    return location;
}
