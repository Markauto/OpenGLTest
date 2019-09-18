#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include "Logger/ILogger.h"
#include "Logger/Logger.h"
#include "TimeHelper/TimeHelper.h"
#include "StringFormater/StringFormater.h"

#define ASSERT(x) if (!(x)) __debugbreak()
#define GLCALL(x) GLClearError(); \
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__));
#ifdef DEBUG_LOGGING

#define DEBUGLOG(x) TheLogger->Log(x)
 
#else

#define DEBUGLOG(x) 

#endif// LOGGER_ENABLED


// TODO: Move setup into factory classes or summet
std::unique_ptr<helpers::ILogger> TheLogger;
std::unique_ptr<helpers::ITimeHelper> TheTimeHelper;

void Setup()
{
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

    window = glfwCreateWindow(800, 600, "Open GL Test", NULL, NULL);
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

    unsigned int buffer;
    GLCALL(glGenBuffers(1, &buffer));
    GLCALL(glBindBuffer(GL_ARRAY_BUFFER, buffer));
    GLCALL(glBufferData(GL_ARRAY_BUFFER, 4 * 2 * sizeof(float) , positions, GL_STATIC_DRAW));

    GLCALL(glEnableVertexAttribArray(0));
    GLCALL(glVertexAttribPointer(0, 2, GL_FLOAT, false, sizeof(float) * 2, 0));

    unsigned int indexBufferObject;
    GLCALL(glGenBuffers(1, &indexBufferObject));
    GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferObject));
    GLCALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW));

    auto [vertexShaderSource, fragmentShaderSource] = ParseShader("res/shaders/Basic.glsl");    
    unsigned int shader = CreateShader(vertexShaderSource, fragmentShaderSource);
    GLCALL(glUseProgram(shader));

    GLCALL(int location = glGetUniformLocation(shader, "u_Color"));
    ASSERT(location != -1);
    GLCALL(glUniform4f(location, 0.8f, 0.3f, 0.8f, 1.0f));

    GLCALL(glUseProgram(0));
    GLCALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
    GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
    

    float redChannel = 0.0f;
    float increment = 0.05f;
    while (!glfwWindowShouldClose(window))
    {
        GLCALL(glClear(GL_COLOR_BUFFER_BIT));
        
        GLCALL(glUseProgram(shader));
        GLCALL(glUniform4f(location, redChannel, 0.3f, 0.8f, 1.0f));

        GLCALL(glBindBuffer(GL_ARRAY_BUFFER, buffer));
        GLCALL(glEnableVertexAttribArray(0));
        GLCALL(glVertexAttribPointer(0, 2, GL_FLOAT, false, sizeof(float) * 2, 0));

        GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferObject));

        
        GLCALL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));        

        if (redChannel > 1.0f)
        {
            increment = -0.05f;
        }
        else if(redChannel < 0.0f)
        {
            increment = 0.05f;
        }

        redChannel += increment;

        GLCALL(glfwSwapBuffers(window));

        GLCALL(glfwPollEvents());
    }

    GLCALL(glDeleteProgram(shader));

    glfwTerminate();
    return 0;
}