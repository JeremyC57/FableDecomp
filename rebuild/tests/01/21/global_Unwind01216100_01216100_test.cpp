#include <cstdio>
static int g_r = 0, g_m = 0; struct R { virtual void v() {} unsigned char b[0x2f4]; R() { ++g_r; } ~R() {} }; struct M { unsigned char b[4]; M() { ++g_m; } ~M() {} }; struct X : R { M m; X() : m() {} };
static bool Check() { X* x = new X; bool ok = g_r == 1 && g_m == 1; delete x; return ok; }

int main()
{
    if (!Check()) { std::printf("00c54090_TEST FAIL\n"); return 1; }
    std::printf("01216100_TEST PASS\n");
    return 0;
}