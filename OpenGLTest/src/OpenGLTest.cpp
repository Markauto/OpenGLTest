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

        Shader shader("res/shaders/Basic.glsl");
        shader.Bind();
        shader.SetUniform4f("u_Color", 0.8f, 0.3f, 0.8f, 1.0f);

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
            shader.SetUniform4f("u_Color", redChannel, 0.3f, 0.8f, 1.0f);

            renderer.Draw(vertexArray, indexBuffer, shader);

            if (redChannel > 1.0f)
            {
                increment = -0.05f;
            }
            else if (redChannel < 0.0f)
            {
                increment = 0.05f;
            }

            redChannel += increment;

            glfwSwapBuffers(window);
            glfwPollEvents();
        }
    }

    glfwTerminate();
    return 0;
}