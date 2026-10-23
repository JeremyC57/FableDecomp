#include <cstdio>
static int g_b = 0, g_c = 0; struct B { virtual void v() {} unsigned char p[8]; long i; void* t; unsigned char q[0x40]; B() { ++g_b; } ~B() {} }; struct C { unsigned char b[0xa0]; C() { ++g_c; } ~C() {} }; struct X : B { C c; long f; X() : c() { i = -1; t = 0; f = 0; } };
static bool Check() { X x; return g_b == 1 && g_c == 1 && x.i == -1 && x.t == 0 && x.f == 0; }

int main()
{
    if (!Check()) { std::printf("00c42d60_TEST FAIL\n"); return 1; }
    std::printf("012152f0_TEST PASS\n");
    return 0;
}