#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "VertexBufferLayout.h"
#include "Texture.h"

int main(void)
{
    std::unique_ptr<helpers::ILogger> TheLogger;
    std::unique_ptr<helpers::ITimeHelper> TheTimeHelper;
    ASSERT(sizeof(unsigned int) == sizeof(GLuint));

    TheTimeHelper = std::make_unique<helpers::TimeHelper>();
    TheLogger = std::make_unique<helpers::Logger>(TheTimeHelper.get());

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
        //Position 2dVec,TextureCord 2dVec
        float vertexes[] = {
             -0.5f, -0.5f, 0.0f, 0.0f, //Bottom left
              0.5f, -0.5f, 1.0f, 0.0f, //Right side
              0.5f,  0.5f, 1.0f, 1.0f, //Top right
             -0.5f,  0.5f, 0.0f, 1.0f  //Left side
        };

        unsigned int indices[] = {
            0, 1, 2,
            2, 3, 0
        };
        
        GLCALL(glEnable(GL_BLEND));
        GLCALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA)); //Set Blending
        
        VertexArray vertexArray;
        VertexBuffer vertexBuffer(vertexes, 4 * 4 * sizeof(float));

        VertexBufferLayout layout;
        layout.Push<float>(2); //Push position 
        layout.Push<float>(2); //Push texture cords
        vertexArray.AddBuffer(vertexBuffer, layout);

        IndexBuffer indexBuffer(indices, 6);

        Shader shader(TheLogger.get(), "res/shaders/Basic.glsl");
        shader.Bind();
        shader.SetUniform4f("u_Color", 0.0f, 0.0f, 0.0f, 0.0f);

        Texture texture("res/textures/face.png");
        texture.Bind(0);
        shader.SetUniform1i("u_Texture", 0);

        vertexArray.Unbind();
        shader.Unbind();
        vertexBuffer.Unbind();
        indexBuffer.Unbind();
        
        Renderer renderer;

        float redChannel = 0.0f;
        float increment = 0.05f;
        while (!glfwWindowShouldClose(window))
        {
            renderer.Clear();

            shader.Bind();
            shader.SetUniform4f("u_Color", redChannel, 0.0f, 0.0f, 0.0f);

            renderer.Draw(vertexArray, indexBuffer, shader);

            if (redChannel > 1.0f)
            {
                increment = -0.005f;
            }
            else if (redChannel < 0.0f)
            {
                increment = 0.005f;
            }

            redChannel += increment;

            glfwSwapBuffers(window);
            glfwPollEvents();
        }
    }

    glfwTerminate();
    return 0;
}