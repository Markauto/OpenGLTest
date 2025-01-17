#include "VertexArray.h"
#include "Renderer.h"
#include "VertexBufferLayout.h"

VertexArray::VertexArray()
{
    GLCALL(glGenVertexArrays(1, &m_RendererId));
    GLCALL(glBindVertexArray(m_RendererId));
}

VertexArray::~VertexArray()
{
    GLCALL(glDeleteVertexArrays(1, &m_RendererId));
}

void VertexArray::AddBuffer(const VertexBuffer& vertexBuffer, const VertexBufferLayout& layout)
{
    Bind();
    vertexBuffer.Bind();
    const auto& elements = layout.GetElements();
    unsigned int offset = 0;
    for (unsigned int i = 0; i < elements.size(); i++)
    {
        const auto& element = elements[i];
        GLCALL(glEnableVertexAttribArray(i));
        GLCALL(glVertexAttribPointer(i, element.count, element.type, 
            element.normalized, layout.GetStride(), (const void*)offset)); //Links buffer with vertex array object

        offset += element.count * VertexBufferLayoutElement::GetSizeOfType(element.type);
    }
    
}

void VertexArray::Bind() const
{
    GLCALL(glBindVertexArray(m_RendererId));
}

void VertexArray::Unbind() const
{
    GLCALL(glBindVertexArray(0));
}
