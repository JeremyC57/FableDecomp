#include <cstdio>
static int g_c = 0, g_m = 0, g_b = 0; struct B { virtual void v() {} ~B() { ++g_b; } }; struct M { unsigned char b[4]; ~M() { ++g_m; } }; struct X : B { unsigned char p[0x50]; M m; void C() { ++g_c; } ~X() { C(); } };
static bool Check() { { X x; } return g_c == 1 && g_m == 1 && g_b == 1; }

int main()
{
    if (!Check()) { std::printf("00c3cf40_TEST FAIL\n"); return 1; }
    std::printf("01214f80_TEST PASS\n");
    return 0;
}