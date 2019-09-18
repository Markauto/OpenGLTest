#pragma once
#ifndef OPENGLTEST_SRC_VERTEXBUFFER_H_
#define OPENGLTEST_SRC_VERTEXBUFFER_H_

class VertexBuffer
{
public:
    VertexBuffer(const void* data, unsigned int size);
    ~VertexBuffer();

    void Bind() const;
    void Unbind() const;

private:
    unsigned int m_RendererId;
};

#endif // OPENGLTEST_SRC_VERTEXBUFFER_H_