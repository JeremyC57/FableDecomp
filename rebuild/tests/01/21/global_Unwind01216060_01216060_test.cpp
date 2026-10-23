#include <cstdio>
static int g_n = 0; struct __declspec(novtable) R { virtual void v() = 0; ~R() {} }; struct Sl { unsigned char b[0x14]; Sl() { ++g_n; } ~Sl() {} }; struct X : R { Sl a; Sl b; Sl c; Sl d; void v() {} X() : a(), b(), c(), d() {} };
static bool Check() { X x; return g_n == 4; }

int main()
{
    if (!Check()) { std::printf("00c4d000_TEST FAIL\n"); return 1; }
    std::printf("01216060_TEST PASS\n");
    return 0;
}