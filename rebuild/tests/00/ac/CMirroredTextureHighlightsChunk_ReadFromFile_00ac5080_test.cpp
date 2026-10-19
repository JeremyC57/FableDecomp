// behaviour copy of CMirroredTextureHighlightsChunk::ReadFromFile: count, resize(count, 0), then count unsigned longs
#include <cstdio>
#include <cstring>
struct CDataInputStream {
    unsigned long m_position; const unsigned char* m_pData; long m_remaining;
    void Read(void* buffer, long size)
    {
        if ((long)m_position + size < 0) return;
        if (m_remaining >= size) { std::memcpy(buffer, m_pData, size); m_pData += size; m_remaining -= size; m_position += size; }
    }
    unsigned long ReadULong() { unsigned long value = 0; Read(&value, 4); return value; }
};
struct CArray {
    unsigned long store[16]; unsigned long* m_begin; unsigned long n;
    CArray() : m_begin(store), n(0) {}
    void resize(unsigned long count, const unsigned long& v) { for (unsigned long i = 0; i < count; ++i) store[i] = v; n = count; }
    unsigned long& operator[](unsigned long i) { return *(m_begin + i); }
};
struct CChunk {
    CArray TriangleVertexIndexList;
    void ReadFromFile(CDataInputStream& in)
    {
        unsigned long count = in.ReadULong();
        TriangleVertexIndexList.resize(count, 0);
        for (unsigned long i = 0; i < count; ++i)
            TriangleVertexIndexList[i] = in.ReadULong();
    }
};
int main()
{
    unsigned char data[16];
    unsigned int words[4] = { 3, 10, 20, 30 };
    for (int w = 0; w < 4; ++w) for (int b = 0; b < 4; ++b) data[w * 4 + b] = (unsigned char)(words[w] >> (8 * b));
    CDataInputStream in; in.m_position = 0; in.m_pData = data; in.m_remaining = 16;
    CChunk c; c.ReadFromFile(in);
    if (c.TriangleVertexIndexList.n != 3 || c.TriangleVertexIndexList[0] != 10 || c.TriangleVertexIndexList[2] != 30 || in.m_remaining != 0)
    { std::printf("00ac5080_TEST FAIL\n"); return 1; }
    std::printf("00ac5080_TEST PASS\n");
    return 0;
}