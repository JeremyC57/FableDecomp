#include <cstdio>
static int g_b = 0, g_m = 0; struct R { virtual void v() {} unsigned char b[0x2f4]; ~R() { ++g_b; } }; struct M { unsigned char b[4]; ~M() { ++g_m; } }; struct X : R { M m; ~X() {} };
static bool Check() { { X* x = new X; delete x; } return g_b == 1 && g_m == 1; }

int main()
{
    if (!Check()) { std::printf("00c54110_TEST FAIL\n"); return 1; }
    std::printf("01216120_TEST PASS\n");
    return 0;
}