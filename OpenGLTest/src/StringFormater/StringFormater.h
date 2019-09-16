#pragma once
#ifndef OPENGLTEST_SRC_STRINGFORMATER_STRINGFORMATER_H_
#define OPENGLTEST_SRC_STRINGFORMATER_STRINGFORMATER_H_
#include <string>
#include <initializer_list>

namespace helpers
{
    class StringFormater
    {
    public:
        template<typename T>
        static std::string Format(std::string formatString,
            const std::initializer_list<T>& args)
        {
            std::string formattedString;
            for (auto arg : args)
            {
                auto index = formatString.find("%s");
                if (index != std::string::npos)
                {
                    formattedString = formatString.replace(index, 2, arg);
                }
            }

            return formattedString;
        }

        template<typename T, typename... Args>
        static std::string Format(std::string formatString, const T &t, const Args & ... args)
        {      
            ReplaceFirstInstance(formatString, t); 
            FormatFast(formatString, args...);
            return formatString;
        }

        template <typename T>
        static std::string Format(std::string formatString, const T &t)
        {
            ReplaceFirstInstance(formatString, t);
            return formatString;
        }

    private:
        template<typename T, typename... Args>
        static void FormatFast(std::string &formatString, const T& t, const Args & ... args)
        {
            ReplaceFirstInstance(formatString, t);
            FormatFast(formatString, args...);
        }

        template <typename T>
        static void FormatFast(std::string &formatString, const T& t)
        {
            ReplaceFirstInstance(formatString, t);
        }

        template<typename T>
        static void ReplaceFirstInstance(std::string &formatString, const T &t)
        {
            const auto &size = formatString.size();
            for (int i = 0; i < size; i++)
            {
                if (i == size - 1)
                {
                    break;
                }

                if (formatString[i] != '%' || formatString[i + 1] != 's')
                {
                    continue;
                }

                formatString.erase(i, 2);
                formatString.insert(i, to_string(t));
                break;
            }
        }

        template<typename T>
        static std::string to_string(const T& val)
        {
            if constexpr (std::is_same<T, std::string>::value)
            {
                return val;
            }
            else if constexpr (std::is_same< T, const char *>::value)
            {
                return val;
            }
            else if constexpr (std::is_array<T>::value)
            {
                return val;
            }
            else if constexpr (std::is_same<T, char*>::value)
            {
                return val;
            }
            else
            {
                return std::move(std::to_string(val));
            }
        }
    };
};
#endif //OPENGLTEST_SRC_STRINGFORMATER_ISTRINGFORMATER_H_