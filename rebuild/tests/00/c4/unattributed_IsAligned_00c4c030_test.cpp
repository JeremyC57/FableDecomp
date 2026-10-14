#include <cstdio>
#include <cstdlib>
#include <cstring>
struct CAlignedRange {
    unsigned long m_base, m_unused, m_offset, m_stride;
    bool IsAligned() const { return ((m_base - m_offset % m_stride) & 3) == 0; }
};
static bool Check()
{
    CAlignedRange r; r.m_unused = 0;
    r.m_base = 0x1000; r.m_offset = 0x23; r.m_stride = 0x10; if (r.IsAligned()) return false;   // 0x1000 - 3 = ...fd
    r.m_offset = 0x24; if (!r.IsAligned()) return false;                                        // 0x1000 - 4
    r.m_base = 0x1001; r.m_offset = 0x11; if (!r.IsAligned()) return false;                     // 0x1001 - 1
    r.m_base = 0xFFFFFFFFul; r.m_offset = 7; r.m_stride = 4; return r.IsAligned();              // unsigned: 0xFFFFFFFF - 3
}
int main()
{
    if (!Check()) { std::printf("00c4c030_TEST FAIL\n"); return 1; }
    std::printf("00c4c030_TEST PASS\n");
    return 0;
}