#include <cstdio>
struct CRACBuffer {
    unsigned char* m_pBase;
    unsigned int   m_size;
    unsigned int   m_offset;
    unsigned int   m_alignment;
    bool IsRangeAligned() const
    {
        return m_offset % m_alignment == 0 && m_size % m_alignment == 0;
    }
};
int main()
{
    CRACBuffer b;
    b.m_pBase = 0;
    b.m_alignment = 4;
    b.m_offset = 8; b.m_size = 16;
    if (!b.IsRangeAligned()) { std::printf("00c4c050_TEST FAIL\n"); return 1; }
    b.m_offset = 6;
    if (b.IsRangeAligned()) { std::printf("00c4c050_TEST FAIL\n"); return 1; }
    b.m_offset = 0; b.m_size = 18;
    if (b.IsRangeAligned()) { std::printf("00c4c050_TEST FAIL\n"); return 1; }
    b.m_alignment = 3; b.m_size = 9; b.m_offset = 3;
    if (!b.IsRangeAligned()) { std::printf("00c4c050_TEST FAIL\n"); return 1; }
    std::printf("00c4c050_TEST PASS\n");
    return 0;
}