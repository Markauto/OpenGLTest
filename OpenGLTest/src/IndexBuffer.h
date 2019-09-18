#pragma once
#ifndef OPENGLTEST_SRC_INDEXBUFFER_H_
#define OPENGLTEST_SRC_INDEXBUFFER_H_

class IndexBuffer
{
private:
    unsigned int m_RendererId;
    unsigned int m_Count;
public:
    IndexBuffer(const unsigned int* data, unsigned int count);
    ~IndexBuffer();

    void Bind() const;
    void Unbind() const;

    inline unsigned int GetCount() const
    {
        return m_Count;
    }
private:

};

#endif // OPENGLTEST_SRC_INDEXBUFFER_H_