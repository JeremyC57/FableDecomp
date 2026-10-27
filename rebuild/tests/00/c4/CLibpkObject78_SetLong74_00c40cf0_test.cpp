// Test for 0x00C40CF0 CLibpkObject78::SetLong74: a change stores and sets the global bit 0x1000000.
#include <cstdio>
static unsigned int g_flags = 0;
struct CObj { unsigned int Long74; void SetLong74(unsigned int v) { if (Long74 != v) { Long74 = v; g_flags |= 0x1000000; } } };
int main()
{
    CObj o; o.Long74 = 3;
    o.SetLong74(3); bool ok = g_flags == 0;
    o.SetLong74(8); ok = ok && o.Long74 == 8 && g_flags == 0x1000000;
    if (!ok) { std::printf("00c40cf0_TEST FAIL\n"); return 1; }
    std::printf("00c40cf0_TEST PASS\n");
    return 0;
}