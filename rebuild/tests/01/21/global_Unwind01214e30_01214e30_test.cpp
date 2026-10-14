#include <cstdio>
static int g_r = 0, g_m = 0; static void __cdecl Rel(void*) { ++g_r; } struct M { unsigned char b[4]; ~M() { ++g_m; } }; struct H { void* p; ~H() { if (p) { Rel(p); p = 0; } } }; struct X { M m; H h; ~X() {} };
static bool Check() { { X x; x.h.p = &x; } { X y; y.h.p = 0; } return g_r == 1 && g_m == 2; }

int main()
{
    if (!Check()) { std::printf("00c3b6b0_TEST FAIL\n"); return 1; }
    std::printf("00c3b6b0_TEST PASS\n");
    return 0;
}