#include <cstdio>
class CSoundPair { public: long a, b; CSoundPair() : a(0), b(0) {} };
static void* g_o = 0; static float g_a = 0, g_b = 0; static void* g_anim = 0; static long g_sp = 1, g_c = 1, g_d = 1; static long g_kind = -1; static void* g_lo = 0;
static void* GetTableAnimation(void* o, long k) { g_lo = o; g_kind = k; return (void*)0x1234; }
struct Base { virtual ~Base() {} unsigned char body[0xa4]; Base(void* o, float a, float b, void* an, const CSoundPair& s, long c, long d) { g_o = o; g_a = a; g_b = b; g_anim = an; g_sp = s.a + s.b; g_c = c; g_d = d; } };
struct Act : Base { Act(void* o, long k) : Base(o, -1.0f, 0.0f, GetTableAnimation(o, k), CSoundPair(), 0, 0) {} };
int main()
{
    int o; Act x(&o, 4);
    if (g_o != &o || g_lo != &o || g_kind != 4 || g_a != -1.0f || g_b != 0.0f || g_anim != (void*)0x1234 || g_sp || g_c || g_d) { std::printf("00915810_TEST FAIL\n"); return 1; }
    std::printf("00915810_TEST PASS\n");
    return 0;
}