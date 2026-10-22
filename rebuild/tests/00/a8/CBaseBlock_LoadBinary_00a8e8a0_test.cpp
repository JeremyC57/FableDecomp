#include <cstdio>
#include <cstring>
struct CMemoryDataInputStream {
    void* m_pVtable;
    unsigned long m_position;
    long StreamSize;
    const unsigned char* m_pData;
    long SourceChunkStreamPos;
    long m_remaining;
    const unsigned char* m_pNext;   // test-only: next chunk for the overflow path
    int m_overflowCalls;
    void ReadWithSrcChunkOverflow(unsigned char* buffer, long size)
    {
        ++m_overflowCalls;
        for (long i = 0; i < size; ++i) {
            if (m_remaining == 0) { m_pData = m_pNext; m_remaining = 100; }
            buffer[i] = *m_pData++; --m_remaining;
        }
        m_position += size;
    }
    static bool IsValidPosition(long long position) { return position >= 0 && position <= 0x7fffffff; }
    void Read(void* buffer, long size)
    {
        long long newPosition = (long long)(unsigned int)(m_position + size);
        if (!IsValidPosition(newPosition))
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
    unsigned int ReadULong() { unsigned int v; Read(&v, sizeof(v)); return v; }
    unsigned char ReadUChar() { unsigned char v; Read(&v, sizeof(v)); return v; }
    bool ReadBool() { unsigned char v; Read(&v, sizeof(v)); return v != 0; }
};
struct CBaseBlock {
    unsigned int  PrimitiveCount;
    unsigned int  StartIndex;
    bool          IsStrip;
    bool          DegenerateTriangles;
    unsigned char ChangeFlags;
    void LoadBinary(CMemoryDataInputStream* stream)
    {
        PrimitiveCount = stream->ReadULong();
        StartIndex = stream->ReadULong();
        IsStrip = stream->ReadBool();
        ChangeFlags = stream->ReadUChar();
        DegenerateTriangles = stream->ReadBool();
    }
};
int main()
{
    // 4+4+1+1+1 = 11 bytes; first chunk holds 6, rest in the next chunk (forces the overflow path)
    unsigned char a[6] = { 0x10, 0, 0, 0, 0x22, 0 };
    unsigned char b[5] = { 0, 0, 7, 0x5a, 0 };
    CMemoryDataInputStream s; std::memset(&s, 0, sizeof(s));
    s.m_pData = a; s.m_remaining = 6; s.m_pNext = b;
    CBaseBlock blk; std::memset(&blk, 0xcd, sizeof(blk));
    blk.LoadBinary(&s);
    if (blk.PrimitiveCount != 0x10 || blk.StartIndex != 0x22 || blk.IsStrip != true || blk.ChangeFlags != 0x5a
        || blk.DegenerateTriangles != false || s.m_position != 11 || s.m_overflowCalls != 1) {
        std::printf("00a8e8a0_TEST FAIL\n"); return 1;
    }
    std::printf("00a8e8a0_TEST PASS\n");
    return 0;
}