#include <cstdio>
static int g_r = 0, g_m = 0; struct B { virtual void v() {} ~B() {} }; struct M { unsigned char b[0x74]; ~M() { ++g_m; } }; struct Hd { unsigned char b[4]; void R() { ++g_r; } }; struct X : B { M m; void* d; unsigned char p[4]; Hd h; ~X() { if (d) h.R(); } };
static bool Check() { { X x; x.d = &x; } { X y; y.d = 0; } return g_r == 1 && g_m == 2; }

int main()
{
    if (!Check()) { std::printf("00c885c0_TEST FAIL\n"); return 1; }
    std::printf("00c885c0_TEST PASS\n");
    return 0;
}