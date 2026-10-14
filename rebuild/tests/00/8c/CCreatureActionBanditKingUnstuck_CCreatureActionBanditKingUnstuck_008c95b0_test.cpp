#include <cstdio>
struct CSoundPair { long a, b; CSoundPair() : a(0), b(0) {} };
static void* g_owner = 0; static void* g_target = 0; static long g_code = 0; static long g_sa = 1;
struct Base { virtual ~Base() {} unsigned char body[0x110]; Base(void* o, void* t, long c, const CSoundPair& s) { g_owner = o; g_target = t; g_code = c; g_sa = s.a + s.b; } };
struct Act : Base { unsigned char m_114; unsigned char m_done; Act(void* o, void* t) : Base(o, t, 150, CSoundPair()), m_done(0) {} };
int main()
{
    int o, t; Act a(&o, &t);
    if (g_owner != &o || g_target != &t || g_code != 150 || g_sa != 0 || a.m_done != 0) { std::printf("008c95b0_TEST FAIL\n"); return 1; }
    std::printf("008c95b0_TEST PASS\n");
    return 0;
}