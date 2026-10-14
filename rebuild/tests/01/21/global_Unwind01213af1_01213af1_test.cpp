#include <cstdio>
static int g_e = 0, g_m = 0; static long g_r = -1; struct E { unsigned char b[0x180]; ~E() { ++g_e; } }; struct M { unsigned char b[4]; ~M() { ++g_m; } }; struct X { unsigned char p[0x18]; E e[2]; unsigned char q[0xc]; M m; void R(long k) { g_r = k; } ~X() { R(0); } };
static bool Check() { { X x; } return g_e == 2 && g_m == 1 && g_r == 0; }

int main()
{
    if (!Check()) { std::printf("00c2d3b0_TEST FAIL\n"); return 1; }
    std::printf("00c2d3b0_TEST PASS\n");
    return 0;
}