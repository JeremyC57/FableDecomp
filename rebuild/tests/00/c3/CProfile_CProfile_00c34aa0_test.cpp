#include <cstdio>
static int g_m = 0; struct __declspec(novtable) R { virtual void v() = 0; ~R() {} }; struct M { unsigned char b[4]; M() { ++g_m; } ~M() {} }; struct X : R { M m; void v() {} X() : m() {} };
static bool Check() { X x; return g_m == 1; }

int main()
{
    if (!Check()) { std::printf("00c34aa0_TEST FAIL\n"); return 1; }
    std::printf("00c34aa0_TEST PASS\n");
    return 0;
}