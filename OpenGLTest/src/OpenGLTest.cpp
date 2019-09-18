#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"

std::tuple<std::string, std::string> ParseShader(const std::string& filepath)
{
    std::ifstream stream(filepath);

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

unsigned int CompileShader(unsigned int type, const std::string& source)
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
        GLCALL(char* message = (char *)alloca(length * sizeof(char)));
        GLCALL(glGetShaderInfoLog(id, length, &length, message));
        TheLogger->Log(helpers::StringFormater::Format(
            "Failed to compile %s shader :( \n %s", 
            (type == GL_VERTEX_SHADER ? "vertex" : "fragment"), 
            message));
        GLCALL(glDeleteShader(id));
        return 0;
    }

    return id;
}

unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
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

int main(void)
{
    Setup();

    GLFWwindow* window;

    if (!glfwInit())
    {
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(640, 480, "Open GL Test", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    glfwSwapInterval(1);

    if (glewInit() != GLEW_OK)
    {
        return -1;
    }

    DEBUGLOG(helpers::StringFormater::Format("OpenGL Version: %s", (const char*)glGetString(GL_VERSION)));

    {
        float positions[] = {
             -0.5f, -0.5f,
              0.5f, -0.5f,
              0.5f,  0.5f,
             -0.5f,  0.5f
        };

        unsigned int indices[] = {
            0, 1, 2,
            2, 3, 0
        };
        
        VertexArray vertexArray;

        VertexBuffer vertexBuffer(positions, 4 * 2 * sizeof(float));

        VertexBufferLayout layout;
        layout.Push<float>(2);  
        vertexArray.AddBuffer(vertexBuffer, layout);

        IndexBuffer indexBuffer(indices, 6);

        auto [vertexShaderSource, fragmentShaderSource] = ParseShader("res/shaders/Basic.glsl");
        unsigned int shader = CreateShader(vertexShaderSource, fragmentShaderSource);
        GLCALL(glUseProgram(shader));

        GLCALL(int location = glGetUniformLocation(shader, "u_Color"));
        ASSERT(location != -1);
        GLCALL(glUniform4f(location, 0.8f, 0.3f, 0.8f, 1.0f));

        vertexArray.Unbind();
        GLCALL(glUseProgram(0));
        vertexBuffer.Unbind();
        indexBuffer.Unbind();
        
        float redChannel = 0.0f;
        float increment = 0.05f;
        while (!glfwWindowShouldClose(window))
        {
            GLCALL(glClear(GL_COLOR_BUFFER_BIT));

            GLCALL(glUseProgram(shader));
            GLCALL(glUniform4f(location, redChannel, 0.3f, 0.8f, 1.0f));

            vertexArray.Bind();

            indexBuffer.Bind();

            GLCALL(glDrawElements(GL_TRIANGLES, indexBuffer.GetCount(), GL_UNSIGNED_INT, nullptr));

            if (redChannel > 1.0f)
            {
                increment = -0.05f;
            }
            else if (redChannel < 0.0f)
            {
                increment = 0.05f;
            }

            redChannel += increment;

            GLCALL(glfwSwapBuffers(window));

            GLCALL(glfwPollEvents());
        }

        GLCALL(glDeleteProgram(shader));
    }

    glfwTerminate();
    return 0;
}