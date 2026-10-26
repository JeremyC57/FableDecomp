// Behaviour test for CAnimatedBlock::SaveBinary (0x00A8E770): the field order and widths written
// to the stream, ending with GroupCount raw group bytes.
#include <cstdio>
#include <cstring>

class CDataOutputStream { public:
    unsigned char buf[64]; int pos;
    virtual ~CDataOutputStream() {}
    virtual void Write(const void* data, long size) { std::memcpy(buf + pos, data, size); pos += size; }
    void WriteULong(unsigned int v) { Write(&v, 4); }
    void WriteBool(bool v) { unsigned char b = v ? 1 : 0; Write(&b, 1); }
    void WriteUChar(unsigned char v) { Write(&v, 1); }
    void WriteUShort(unsigned short v) { Write(&v, 2); }
};

struct CAnimatedBlock {
    unsigned int m_firstIndex, m_count; bool m_flagA, m_flagB; unsigned char m_type;
    unsigned int VertexCount; unsigned short BonesPerVertex; bool PalettedFlag; unsigned char GroupCount;
    unsigned char Groups[18];
    void SaveBinary(CDataOutputStream* stream) const
    {
        stream->WriteULong(m_firstIndex);
        stream->WriteULong(m_count);
        stream->WriteBool(m_flagA);
        stream->WriteUChar(m_type);
        stream->WriteBool(m_flagB);
        stream->WriteULong(VertexCount);
        stream->WriteUShort(BonesPerVertex);
        stream->WriteBool(PalettedFlag);
        stream->WriteUChar(GroupCount);
        stream->Write(Groups, GroupCount);
    }
};

int main()
{
    CAnimatedBlock b;
    b.m_firstIndex = 1; b.m_count = 2; b.m_flagA = true; b.m_flagB = false; b.m_type = 7;
    b.VertexCount = 300; b.BonesPerVertex = 3; b.PalettedFlag = true; b.GroupCount = 3;
    b.Groups[0] = 9; b.Groups[1] = 8; b.Groups[2] = 7;
    CDataOutputStream s; s.pos = 0;
    b.SaveBinary(&s);
    static const unsigned char expect[] = { 1,0,0,0, 2,0,0,0, 1, 7, 0, 44,1,0,0, 3,0, 1, 3, 9,8,7 };
    if (s.pos != (int)sizeof(expect) || std::memcmp(s.buf, expect, sizeof(expect)) != 0) {
        std::printf("00a8e770_TEST FAIL\n"); return 1;
    }
    std::printf("00a8e770_TEST PASS\n");
    return 0;
}