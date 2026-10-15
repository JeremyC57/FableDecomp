// Behaviour test for 0x00AA77A0 CChunkedFileRawDataChunk::ReadFromFile: new buffer of the chunk size filled from the stream.
#include <cstdio>
#include <cstring>
#ifdef _MSC_VER
typedef __int64 S64;
#else
typedef long long S64;
#endif
static int g_overflowReads = 0;
struct CDataInputStream {
    unsigned long m_position; const unsigned char* m_pData; long m_remaining;
    void ReadWithSrcChunkOverflow(unsigned char*, long) { ++g_overflowReads; }
    void Read(void* buffer, long size)
    {
        if (size <= 0) return;
        S64 newPosition = (S64)(unsigned int)(m_position + size);
        if (!(newPosition >= 0 && newPosition <= 0x7fffffff)) return;
        if (size <= m_remaining) {
            std::memcpy(buffer, m_pData, size); m_pData += size; m_remaining -= size; m_position += size;
        } else ReadWithSrcChunkOverflow((unsigned char*)buffer, size);
    }
};
struct CChunkedFileRawDataChunk {
    void* Data; unsigned long ByteSize;
    void ReadFromFile(CDataInputStream& stream, unsigned long size)
    {
        delete[] (unsigned char*)Data;
        Data = new unsigned char[size];
        ByteSize = size;
        stream.Read(Data, (long)size);
    }
};
int main()
{
    static const unsigned char src[8] = { 1, 2, 3, 4, 5, 6, 7, 8 };
    CDataInputStream st; st.m_position = 100; st.m_pData = src; st.m_remaining = 8;
    CChunkedFileRawDataChunk c; c.Data = new unsigned char[2]; c.ByteSize = 2;
    c.ReadFromFile(st, 5);
    const unsigned char* d = (const unsigned char*)c.Data;
    if (c.ByteSize != 5 || d[0] != 1 || d[4] != 5 || st.m_position != 105 || st.m_remaining != 3 || st.m_pData != src + 5) {
        std::printf("00aa77a0_TEST FAIL\n"); return 1;
    }
    c.ReadFromFile(st, 6);                               // more than the chunk holds: overflow path
    if (g_overflowReads != 1 || st.m_position != 105) { std::printf("00aa77a0_TEST FAIL\n"); return 1; }
    delete[] (unsigned char*)c.Data;
    std::printf("00aa77a0_TEST PASS\n");
    return 0;
}
