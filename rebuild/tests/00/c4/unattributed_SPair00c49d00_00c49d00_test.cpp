#include <cstdio>
static int g_r = 0, g_f = 0; static void __cdecl Rel(void*) { ++g_r; } struct F { unsigned char b[0x1c]; ~F() { ++g_f; } }; struct H { void* p; ~H() { Rel(p); } }; struct P { F f; H h; ~P() {} };
static bool Check() { { P p; p.h.p = 0; } return g_r == 1 && g_f == 1; }

int main()
{
    if (!Check()) { std::printf("00c49d00_TEST FAIL\n"); return 1; }
    std::printf("00c49d00_TEST PASS\n");
    return 0;
}