#include "Texture.h"
#include "Vendor/stb_image/stb_image.h"

Texture::Texture(const std::string& path) :
    m_RendererId(0), m_FilePath(path), m_LocalBuffer(nullptr), 
    m_Width(0), m_Height(0), m_BitsPerPixel(0)
{
    stbi_set_flip_vertically_on_load(1); //Makes texture upside down cos open GL expects pixels to start at the bottom
    m_LocalBuffer = stbi_load(path.c_str(), &m_Width, &m_Height, &m_BitsPerPixel, 4 /*RGBA*/);

    GLCALL(glGenTextures(1, &m_RendererId));
    GLCALL(glBindTexture(GL_TEXTURE_2D, m_RendererId));

    GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

    GLCALL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_LocalBuffer));
    Unbind();

    if (m_LocalBuffer)
    {
        stbi_image_free(m_LocalBuffer);
    }
}

Texture::~Texture()
{
    GLCALL(glDeleteTextures(1, &m_RendererId));
}

void Texture::Bind(unsigned int slot /*= 0*/) const
{
    GLCALL(glActiveTexture(GL_TEXTURE0 + slot));
    GLCALL(glBindTexture(GL_TEXTURE_2D, m_RendererId));
}

void Texture::Unbind() const
{
    GLCALL(glBindTexture(GL_TEXTURE_2D, 0));
}
