#include "TestMenu.h"
#include "imgui/imgui.h"

namespace test
{

   TestMenu::TestMenu(Test*& currentTestPointers, helpers::ILogger* theLogger) :
        m_CurrentTest(currentTestPointers), TheLogger(theLogger)
    {

    }

    void TestMenu::OnImGuiRender()
    {
        for (auto& test : m_Tests)
        {
            if (ImGui::Button(test.first.c_str()))
            {
                m_CurrentTest = test.second();
            }
            
        }
    }
}