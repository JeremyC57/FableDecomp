// Behaviour test for 0x00AAE2C0 C3DAnimFileMovementVectorChunk::ReadFromFile (model copy + stub stream/base).
#include <cstdio>
#include <string.h>
#ifndef _MSC_VER
typedef long long __int64;
#endif

struct CChunkedFileLoader { int dummy; };
struct CDataInputStream {
    virtual void v0() {}
    virtual void v1() {}
    virtual unsigned long GetPosition() { return m_position; }
    unsigned long m_position;
    long m_08;
    const unsigned char* m_pData;
    long m_10;
    long m_remaining;
    int m_overflowCalls;
    void ReadWithSrcChunkOverflow(unsigned char* buffer, long size)
    {
        float v = 9.5f; memcpy(buffer, &v, size); m_position += size; ++m_overflowCalls;
    }
    static bool IsValidPosition(__int64 position) { return position >= 0 && position <= 0x7fffffff; }
    void Read(void* buffer, long size)
    {
        __int64 newPosition = (__int64)(m_position + size);
        if (!IsValidPosition(newPosition))
            return;
        if (m_remaining >= size) {
            memcpy(buffer, m_pData, size);
            m_pData += size;
            m_remaining -= size;
            m_position += size;
        }
        else
            ReadWithSrcChunkOverflow((unsigned char*)buffer, size);
    }
    float ReadFloat() { float value; Read(&value, sizeof(value)); return value; }
};

struct C3DVector { float X; float Y; float Z; };

static unsigned long g_baseSize = 0; static CDataInputStream* g_baseIn = 0; static CChunkedFileLoader* g_baseLoader = 0;
struct CChunkedFileSuperChunk {
    virtual void v0() {}
    virtual void ReadFromFile(CDataInputStream& in, unsigned long chunkSize, CChunkedFileLoader& loader)
    {
        g_baseIn = &in; g_baseSize = chunkSize; g_baseLoader = &loader;
    }
};

struct C3DAnimFileMovementVectorChunk : CChunkedFileSuperChunk {
    C3DVector Vector;
    virtual void ReadFromFile(CDataInputStream& in, unsigned long chunkSize, CChunkedFileLoader& loader);
};

void C3DAnimFileMovementVectorChunk::ReadFromFile(CDataInputStream& in, unsigned long chunkSize, CChunkedFileLoader& loader)
{
    unsigned long end = in.GetPosition() + chunkSize;
    Vector.X = in.ReadFloat();
    Vector.Y = in.ReadFloat();
    Vector.Z = in.ReadFloat();
    CChunkedFileSuperChunk::ReadFromFile(in, end - in.GetPosition(), loader);
}

int main()
{
    float src[3] = { 1.5f, -2.0f, 3.25f };
    CDataInputStream in; in.m_position = 100; in.m_pData = (const unsigned char*)src; in.m_remaining = 8; in.m_overflowCalls = 0;
    CChunkedFileLoader loader;
    C3DAnimFileMovementVectorChunk chunk;
    chunk.ReadFromFile(in, 40, loader);
    // X, Y from the chunk fast path, Z from the overflow reader (only 8 bytes left in the chunk)
    if (chunk.Vector.X != 1.5f || chunk.Vector.Y != -2.0f || chunk.Vector.Z != 9.5f || in.m_overflowCalls != 1) {
        std::printf("00aae2c0_TEST FAIL\n"); return 1;
    }
    if (in.m_position != 112 || g_baseIn != &in || g_baseSize != 28 || g_baseLoader != &loader) {
        std::printf("00aae2c0_TEST FAIL\n"); return 1;
    }
    std::printf("00aae2c0_TEST PASS\n");
    return 0;
}
