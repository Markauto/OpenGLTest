#include "Renderer.h"
#include "IndexBuffer.h"

IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int count)
    : m_Count(count)
{
    GLCALL(glGenBuffers(1, &m_RendererId));
    GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererId));
    GLCALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW));
}

IndexBuffer::~IndexBuffer()
{
    GLCALL(glDeleteBuffers(1, &m_RendererId));
}

void IndexBuffer::Bind() const 
{
    GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererId));
}

void IndexBuffer::Unbind() const
{
    GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}
