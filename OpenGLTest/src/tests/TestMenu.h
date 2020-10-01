#pragma once
#include "Test.h"

#include <vector>
#include <string>
#include <functional>

#include "Logger/ILogger.h"
#include "StringFormater/StringFormater.h"

namespace test
{
    class TestMenu sealed : public Test
    {
    public:
        TestMenu(Test*& currentTestPointers, helpers::ILogger* logger);

        virtual void OnImGuiRender() override;
        
        template<typename T>
        void RegisterTest(const std::string& name)
        {
            TheLogger->Log(helpers::StringFormater::Format("Registering test %s", name));

            m_Tests.push_back(std::make_pair(name, []()
                {
                    return new T();
                }));
        }
    private:
        Test*& m_CurrentTest;
        helpers::ILogger* TheLogger;
        std::vector<std::pair<std::string, std::function<Test* ()>>> m_Tests;
    };
}