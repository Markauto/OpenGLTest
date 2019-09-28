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

#include "glm/glm/glm.hpp"
#include "glm/glm/gtc/matrix_transform.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

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

    const char* glsl_version = "#version 410 core";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(960, 540, "Open GL Test", NULL, NULL);
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


    //Position 2dVec,TextureCord 2dVec
    float vertexes[] = {
        -50.0f, -50.0f, 0.0f, 0.0f, //Bottom left
         50.0f, -50.0f, 1.0f, 0.0f, //Right side
         50.0f,  50.0f, 1.0f, 1.0f, //Top right
        -50.0f,  50.0f, 0.0f, 1.0f  //Left side
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

    glm::mat4 projectionMatrix = glm::ortho(0.0f, 960.0f, // left edge, right edge
        0.0f, 540.0f, // bottom edge, top edge
        -1.0f, 1.0f); // near plane, far plane
    glm::mat4 viewMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));


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

    ImVec4 clearColour = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    Renderer renderer(clearColour);

    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);
    
    glm::vec3 transalationA(200.0f, 200.0f, 0);
    glm::vec3 transalationB(400.0f, 200.0f, 0);

    float redChannel = 0.0f;
    float increment = 0.05f;
    while (!glfwWindowShouldClose(window))
    {
        renderer.Clear();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        {
            glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), transalationA);
            glm::mat4 modelViewProjectionMatrix = projectionMatrix * viewMatrix * modelMatrix;
            shader.Bind();
            shader.SetUniform4f("u_Color", redChannel, 0.0f, 0.0f, 0.0f);
            shader.SetUniformMat4f("u_ModelViewProjectionMatrix", modelViewProjectionMatrix);
            renderer.Draw(vertexArray, indexBuffer, shader);
        }

        {
            glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), transalationB);
            glm::mat4 modelViewProjectionMatrix = projectionMatrix * viewMatrix * modelMatrix;

            shader.Bind();
            shader.SetUniform4f("u_Color", redChannel, 0.0f, 0.0f, 0.0f);
            shader.SetUniformMat4f("u_ModelViewProjectionMatrix", modelViewProjectionMatrix);
            renderer.Draw(vertexArray, indexBuffer, shader);
        }

        if (redChannel > 1.0f)
        {
            increment = -0.005f;
        }
        else if (redChannel < 0.0f)
        {
            increment = 0.005f;
        }

        redChannel += increment;

        // Show debug window
        {
            ImGui::Begin("Debug");
            ImGui::SliderFloat3("Translate model A", &transalationA.x, 0.0f, 960.0f);          
            ImGui::SliderFloat3("Translate model B", &transalationB.x, 0.0f, 960.0f);          
            ImGui::ColorEdit3("clear color", (float*)& clearColour); 
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            ImGui::End();
        }

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        renderer.SetClearColour(clearColour);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwTerminate();
    return 0;
}