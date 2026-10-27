// Test for 0x00C40CC0 CLibpkObject78::SetFloat70: a change stores and sets the global bit 0x1000000.
#include <cstdio>
static unsigned int g_flags = 0;
struct CObj { float Float70; void SetFloat70(float v) { if (Float70 != v) { Float70 = v; g_flags |= 0x1000000; } } };
int main()
{
    CObj o; o.Float70 = 1.5f;
    o.SetFloat70(1.5f); bool ok = g_flags == 0;
    o.SetFloat70(2.0f); ok = ok && o.Float70 == 2.0f && g_flags == 0x1000000;
    if (!ok) { std::printf("00c40cc0_TEST FAIL\n"); return 1; }
    std::printf("00c40cc0_TEST PASS\n");
    return 0;
}