#pragma once
#ifndef OPENGLTEST_SRC_Texture_h__
#define OPENGLTEST_SRC_Texture_h__
#include "Renderer.h"

class Texture
{
private:
    unsigned int m_RendererId;
    std::string m_FilePath;
    unsigned char* m_LocalBuffer;
    int m_Width, m_Height, m_BitsPerPixel;
public:
    Texture(const std::string& path);
    ~Texture();

    void Bind(unsigned int slot = 0) const;
    void Unbind() const;

    inline int GetWidth() const
    {
        return m_Width;
    }

    inline int GetHeight() const
    {
        return m_Height;
    }
};

#endif // OPENGLTEST_SRC_Texture_h__