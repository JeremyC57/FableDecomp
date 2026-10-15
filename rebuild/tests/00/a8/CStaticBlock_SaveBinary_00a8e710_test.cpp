#include <cstdio>
// Behaviour test for 00a8e710: the block writes its two base words, flag A, the type byte, flag B and the material
// index, in that order and with those widths.
static long g_values[8]; static int g_kinds[8]; static int g_n = 0;
class CDataOutputStream {
public:
    void WriteULong(unsigned long value) { g_values[g_n] = (long)value; g_kinds[g_n++] = 4; }
    void WriteBool(bool value) { g_values[g_n] = value ? 1 : 0; g_kinds[g_n++] = 0; }
    void WriteUChar(unsigned char value) { g_values[g_n] = value; g_kinds[g_n++] = 1; }
    void WriteLong(long value) { g_values[g_n] = value; g_kinds[g_n++] = 5; }
};
struct CTexture;
struct CStaticBlock {
    unsigned long m_firstIndex;
    unsigned long m_count;
    bool          m_flagA;
    bool          m_flagB;
    unsigned char m_type;
    unsigned long DiffuseTextureFixupOffset;
    unsigned long BumpTextureFixupOffset;
    CTexture*     DiffuseTexture;
    CTexture*     BumpTexture;
    long          MaterialIndex;
    void SaveBinary(CDataOutputStream* stream) const;
};
void CStaticBlock::SaveBinary(CDataOutputStream* stream) const
{
    stream->WriteULong(m_firstIndex);
    stream->WriteULong(m_count);
    stream->WriteBool(m_flagA);
    stream->WriteUChar(m_type);
    stream->WriteBool(m_flagB);
    stream->WriteLong(MaterialIndex);
}
int main()
{
    CStaticBlock b; b.m_firstIndex = 100; b.m_count = 12; b.m_flagA = true; b.m_flagB = false; b.m_type = 3; b.MaterialIndex = -7;
    CDataOutputStream s;
    b.SaveBinary(&s);
    static const long ev[6] = { 100, 12, 1, 3, 0, -7 };
    static const int ek[6] = { 4, 4, 0, 1, 0, 5 };
    if (g_n != 6) { std::printf("00a8e710_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 6; ++i)
        if (g_values[i] != ev[i] || g_kinds[i] != ek[i]) { std::printf("00a8e710_TEST FAIL\n"); return 1; }
    std::printf("00a8e710_TEST PASS\n");
    return 0;
}