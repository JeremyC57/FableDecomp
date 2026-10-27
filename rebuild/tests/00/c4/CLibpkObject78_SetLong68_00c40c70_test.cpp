// Test for 0x00C40C70 CLibpkObject78::SetLong68: a change stores, sets dirty 0x80000 and the global bit 0x1000000.
#include <cstdio>
static unsigned int g_flags = 0;
struct CObj { unsigned int Long68; unsigned int DirtyFlags;
    void SetLong68(unsigned int v) { if (Long68 != v) { Long68 = v; DirtyFlags |= 0x80000; g_flags |= 0x1000000; } } };
int main()
{
    CObj o; o.Long68 = 4; o.DirtyFlags = 1;
    o.SetLong68(4); bool ok = o.DirtyFlags == 1 && g_flags == 0;
    o.SetLong68(9); ok = ok && o.Long68 == 9 && o.DirtyFlags == 0x80001 && g_flags == 0x1000000;
    if (!ok) { std::printf("00c40c70_TEST FAIL\n"); return 1; }
    std::printf("00c40c70_TEST PASS\n");
    return 0;
}