#include <cstdio>
static int g_b = 0, g_m = 0; struct B { virtual void v() {} unsigned char b[8]; ~B() { ++g_b; } }; struct M { unsigned char b[4]; ~M() { ++g_m; } }; struct X : B { M m; ~X() {} };
static bool Check() { { X x; } return g_b == 1 && g_m == 1; }

int main()
{
    if (!Check()) { std::printf("00c10940_TEST FAIL\n"); return 1; }
    std::printf("01211420_TEST PASS\n");
    return 0;
}