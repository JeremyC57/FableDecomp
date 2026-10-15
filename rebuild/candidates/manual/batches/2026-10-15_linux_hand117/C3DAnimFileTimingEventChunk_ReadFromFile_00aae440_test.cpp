#include <cstdio>
#include <cstring>
struct CCharString {
    char m_text[32];
    CCharString& operator=(const CCharString& o) { std::strcpy(m_text, o.m_text); return *this; }
};
class CDataInputStream {
public:
    const char* m_name;
    unsigned long m_position;
    const unsigned char* m_pData;
    long m_remaining;
    int m_overflowCalls;
    CCharString ReadNullTerminatedString() { CCharString s; std::strcpy(s.m_text, m_name); return s; }
    void ReadWithSrcChunkOverflow(unsigned char* buffer, long size) { ++m_overflowCalls; float f = 9.0f; std::memcpy(buffer, &f, size); }
    void Read(void* buffer, long size)
    {
        long long newPosition = (long long)(m_position + size);
        if (newPosition < 0 || newPosition > 0x7fffffff)
            return;
        if (m_remaining >= size) {
            std::memcpy(buffer, m_pData, size);
            m_pData += size;
            m_remaining -= size;
            m_position += size;
        }
        else
            ReadWithSrcChunkOverflow((unsigned char*)buffer, size);
    }
    float ReadFLOAT() { float value = 0.0f; Read(&value, 4); return value; }
};
class CChunkedFileLoader { };
struct C3DAnimFileTimingEventChunk {
    CCharString Name;
    float Time;
    void ReadFromFile(CDataInputStream& in, unsigned long size, CChunkedFileLoader& loader)
    {
        Name = in.ReadNullTerminatedString();
        Time = in.ReadFLOAT();
    }
};
int main()
{
    float t = 2.25f;
    unsigned char data[4]; std::memcpy(data, &t, 4);
    CDataInputStream in; in.m_name = "Strike"; in.m_position = 10; in.m_pData = data; in.m_remaining = 4; in.m_overflowCalls = 0;
    CChunkedFileLoader loader;
    C3DAnimFileTimingEventChunk c;
    c.ReadFromFile(in, 8, loader);
    if (std::strcmp(c.Name.m_text, "Strike") != 0 || c.Time != 2.25f || in.m_position != 14 || in.m_remaining != 0 || in.m_pData != data + 4) {
        std::printf("00aae440_TEST FAIL\n"); return 1;
    }
    c.ReadFromFile(in, 8, loader);   // chunk exhausted -> overflow reader
    if (c.Time != 9.0f || in.m_overflowCalls != 1) { std::printf("00aae440_TEST FAIL\n"); return 1; }
    std::printf("00aae440_TEST PASS\n");
    return 0;
}
