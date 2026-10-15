// Behaviour test for 0x00AA4D80 C3DMeshFileXTextureIndexChunk::ReadFromFile
#include <cstdio>
#include <cstring>
struct CDataInputStream {
    unsigned long m_position;
    const unsigned char* m_pData;
    long m_remaining;
    int overflowCalls;
    void ReadWithSrcChunkOverflow(unsigned char* buffer, long size) { ++overflowCalls; std::memset(buffer, 0x5A, size); m_position += size; }
    static bool IsValidPosition(long long position) { return position >= 0 && position <= 0x7fffffff; }
    void Read(void* buffer, long size)
    {
        long long newPosition = (long long)(unsigned long)(m_position + size);
        if (!IsValidPosition(newPosition)) return;
        if (m_remaining >= size) {
            std::memcpy(buffer, m_pData, size);
            m_pData += size; m_remaining -= size; m_position += size;
        } else
            ReadWithSrcChunkOverflow((unsigned char*)buffer, size);
    }
};
struct C3DMeshFileXTextureIndexChunk {
    unsigned int Index;
    void ReadFromFile(CDataInputStream& in)
    {
        unsigned int index = 0xDEAD;
        in.Read(&index, sizeof(index));
        Index = index;
    }
};
int main()
{
    unsigned char data[8] = { 0x78, 0x56, 0x34, 0x12, 0, 0, 0, 0 };
    CDataInputStream in; in.m_position = 100; in.m_pData = data; in.m_remaining = 8; in.overflowCalls = 0;
    C3DMeshFileXTextureIndexChunk c; c.Index = 0;
    c.ReadFromFile(in);
    bool ok = c.Index == 0x12345678u && in.m_position == 104 && in.m_remaining == 4 && in.m_pData == data + 4 && in.overflowCalls == 0;
    in.m_remaining = 2; c.ReadFromFile(in);
    ok = ok && in.overflowCalls == 1 && c.Index == 0x5A5A5A5Au && in.m_position == 108;
    in.m_position = 0x7ffffffe; in.m_remaining = 8; c.ReadFromFile(in);
    ok = ok && in.m_position == 0x7ffffffe && in.m_remaining == 8 && c.Index == 0xDEAD;
    if (!ok) { std::printf("00aa4d80_TEST FAIL\n"); return 1; }
    std::printf("00aa4d80_TEST PASS\n");
    return 0;
}
