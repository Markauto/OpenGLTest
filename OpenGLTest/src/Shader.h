#pragma once
#ifndef OPENGLTEST_SRC_Shader_h__
#define OPENGLTEST_SRC_Shader_h__

#include <string>
#include <unordered_map>

class Shader
{
private:
    unsigned int m_RendererId;
    std::string m_FilePath;
    std::unordered_map<std::string, int> m_UniformLocationCache;
public:
    Shader(const std::string& filePath);
    
    ~Shader();

    void Bind() const;
    void Unbind() const;

    // Set uniforms
    void SetUniform4f(const std::string& name, float value0, float value1, float value2, float value3);
    void SetUniform1f(const std::string& name, float value0);
private:
    int GetUniformLocation(const std::string& name);
    unsigned int CompileShader(unsigned int type, const std::string& source);
    unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
    std::tuple<std::string, std::string> ParseShader();
};
#endif // OPENGLTEST_SRC_Shader_h__