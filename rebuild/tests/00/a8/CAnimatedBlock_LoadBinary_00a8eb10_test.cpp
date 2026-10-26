#include <cstdio>
#include <cstring>
class CMemoryDataInputStream { public:
    void* m_pVtable;
    unsigned int m_position;
    int StreamSize;
    const unsigned char* m_pData;
    int SourceChunkStreamPos;
    int m_remaining;
    const unsigned char* m_pNext;
    int m_overflowCalls;
    void ReadWithSrcChunkOverflow(unsigned char* buffer, int size)
    {
        ++m_overflowCalls;
        for (int i = 0; i < size; ++i) {
            if (m_remaining == 0) { m_pData = m_pNext; m_remaining = 100; }
            buffer[i] = *m_pData++; --m_remaining;
        }
        m_position += size;
    }
    static bool IsValidPosition(long long position) { return position >= 0 && position <= 0x7fffffff; }
    void Read(void* buffer, int size)
    {
        if (size <= 0)
            return;
        long long newPosition = (long long)(unsigned int)(m_position + size);
        if (!IsValidPosition(newPosition))
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
    unsigned int ReadULong() { unsigned int v; Read(&v, sizeof(v)); return v; }
    unsigned short ReadUShort() { unsigned short v; Read(&v, sizeof(v)); return v; }
    unsigned char ReadUChar() { unsigned char v; Read(&v, sizeof(v)); return v; }
    bool ReadBool() { unsigned char v; Read(&v, sizeof(v)); return v != 0; }
};
struct CBaseBlock {
    unsigned int PrimitiveCount, StartIndex;
    bool IsStrip, DegenerateTriangles;
    unsigned char ChangeFlags;
    int m_baseCalls;
    void LoadBinary(CMemoryDataInputStream*) { ++m_baseCalls; }   // stand-in for 0x00A8E8A0
};
struct CAnimatedBlock : CBaseBlock {
    unsigned int   BoneConstantsFixupOffset;
    unsigned int   VertexCount;
    unsigned short BonesPerVertex;
    bool           PalettedFlag;
    unsigned char  GroupCount;
    unsigned char  Groups[18];
    void LoadBinary(CMemoryDataInputStream* stream)
    {
        CBaseBlock::LoadBinary(stream);
        VertexCount = stream->ReadULong();
        BonesPerVertex = stream->ReadUShort();
        PalettedFlag = stream->ReadBool();
        GroupCount = stream->ReadUChar();
        stream->Read(Groups, GroupCount);
    }
};
int main()
{
    // vertex count 300, bones 3, paletted 1, 4 groups {9,8,7,6}
    unsigned char a[] = { 0x2c, 0x01, 0, 0, 3, 0, 1, 4, 9, 8, 7, 6 };
    CMemoryDataInputStream s; std::memset(&s, 0, sizeof(s));
    s.m_pData = a; s.m_remaining = sizeof(a);
    CAnimatedBlock blk; std::memset(&blk, 0, sizeof(blk));
    blk.LoadBinary(&s);
    if (blk.m_baseCalls != 1 || blk.VertexCount != 300 || blk.BonesPerVertex != 3 || !blk.PalettedFlag
        || blk.GroupCount != 4 || blk.Groups[0] != 9 || blk.Groups[3] != 6 || blk.Groups[4] != 0 || s.m_position != 12) {
        std::printf("00a8eb10_TEST FAIL\n"); return 1;
    }
    // empty group list: nothing more is read
    unsigned char b[] = { 1, 0, 0, 0, 2, 0, 0, 0, 0xee };
    CMemoryDataInputStream t; std::memset(&t, 0, sizeof(t));
    t.m_pData = b; t.m_remaining = sizeof(b);
    CAnimatedBlock c; std::memset(&c, 0, sizeof(c));
    c.LoadBinary(&t);
    if (c.GroupCount != 0 || c.PalettedFlag || t.m_position != 8 || t.m_remaining != 1) {
        std::printf("00a8eb10_TEST FAIL\n"); return 1;
    }
    std::printf("00a8eb10_TEST PASS\n");
    return 0;
}