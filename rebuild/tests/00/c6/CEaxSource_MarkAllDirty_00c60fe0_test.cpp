// Behaviour test for 00c60fe0: CEaxSource::MarkAllDirty -- sets the re-send bit, keeps the other bits, returns 0
#include <cstdio>
class CEaxSource { public: unsigned long m_dirty; long MarkAllDirty() { m_dirty |= 0x80000000; return 0; } };
int main()
{
    CEaxSource s; s.m_dirty = 0x15;
    int ok = s.MarkAllDirty() == 0 && s.m_dirty == 0x80000015 && s.MarkAllDirty() == 0 && s.m_dirty == 0x80000015;
    std::printf(ok ? "00c60fe0_TEST PASS\n" : "00c60fe0_TEST FAIL\n");
    return ok ? 0 : 1;
}
