#include <cstdio>
#include <cstring>
#include <cstdlib>
class CDataInputStream {
public:
    unsigned long m_position;
    const unsigned char* m_pData;
    long m_remaining;
    int m_overflowCalls;
    void ReadWithSrcChunkOverflow(unsigned char* buffer, long size) { ++m_overflowCalls; std::memset(buffer, 0xEE, size); }
    void Read(void* buffer, long size)
    {
        if (size <= 0)
            return;
        long long newPosition = (long long)(m_position + size);
        if (newPosition < 0 || newPosition > 0x7fffffff)
            return;
        if (size <= m_remaining) {
            std::memcpy(buffer, m_pData, size);
            m_pData += size;
            m_remaining -= size;
            m_position += size;
        }
        else
            ReadWithSrcChunkOverflow((unsigned char*)buffer, size);
    }
};
class CChunkedFileLoader { };
template <class T>
struct CArray {
    T* m_pBegin; T* m_pEnd; T* m_pEndOfStorage;
    void resize(unsigned long n, const T& value)
    {
        m_pBegin = (T*)std::realloc(m_pBegin, n ? n : 1);
        for (unsigned long i = 0; i < n; ++i) m_pBegin[i] = value;
        m_pEnd = m_pBegin + n; m_pEndOfStorage = m_pEnd;
    }
};
inline unsigned char* GetData(CArray<unsigned char>& a)
{
    if (a.m_pEnd - a.m_pBegin != 0)
        return a.m_pBegin;
    return reinterpret_cast<unsigned char*>(&a);
}
struct C3DAnimFileXSequenceChunk {
    CArray<unsigned char> CompressedData;
    void ReadFromFile(CDataInputStream& in, unsigned long size, CChunkedFileLoader& loader)
    {
        CompressedData.resize(size, 0);
        in.Read(GetData(CompressedData), size);
    }
};
int main()
{
    const unsigned char src[6] = { 1, 2, 3, 4, 5, 6 };
    CDataInputStream in; in.m_position = 0; in.m_pData = src; in.m_remaining = 6; in.m_overflowCalls = 0;
    CChunkedFileLoader loader;
    C3DAnimFileXSequenceChunk c; c.CompressedData.m_pBegin = 0; c.CompressedData.m_pEnd = 0; c.CompressedData.m_pEndOfStorage = 0;
    c.ReadFromFile(in, 5, loader);
    int ok = c.CompressedData.m_pEnd - c.CompressedData.m_pBegin == 5 && std::memcmp(c.CompressedData.m_pBegin, src, 5) == 0 &&
             in.m_position == 5 && in.m_remaining == 1 && in.m_pData == src + 5;
    c.ReadFromFile(in, 3, loader);   // only 1 byte left -> overflow reader
    ok = ok && in.m_overflowCalls == 1 && c.CompressedData.m_pBegin[2] == 0xEE;
    c.ReadFromFile(in, 0, loader);   // empty: nothing read
    ok = ok && in.m_overflowCalls == 1 && in.m_position == 5;
    std::free(c.CompressedData.m_pBegin);
    if (!ok) { std::printf("00aa48d0_TEST FAIL\n"); return 1; }
    std::printf("00aa48d0_TEST PASS\n");
    return 0;
}