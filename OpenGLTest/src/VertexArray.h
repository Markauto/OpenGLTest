#pragma once
#ifndef OPENGLTEST_SRC_VertexArray_h__
#define OPENGLTEST_SRC_VertexArray_h__

#include "VertexBuffer.h"

class VertexBufferLayout;

class VertexArray
{
private:
    unsigned int m_RendererId;
public:
    VertexArray();
    ~VertexArray();
    void AddBuffer(const VertexBuffer& vertexBuffer, const VertexBufferLayout& layout);
    void Bind() const;
    void Unbind() const;
private:

};

#endif // OPENGLTEST_SRC_VertexArray_h__