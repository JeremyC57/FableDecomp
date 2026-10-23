#include <cstdio>
static int g_b = 0, g_m = 0; struct B { virtual void v() {} unsigned char b[8]; B() {} B(const B&) { ++g_b; } ~B() {} }; struct M { unsigned char b[4]; M() {} M(const M&) { ++g_m; } ~M() {} }; struct X : B { M m; X() {} X(const X& o) : B(o), m(o.m) {} };
static bool Check() { X a; X b(a); return g_b == 1 && g_m == 1; }

int main()
{
    if (!Check()) { std::printf("00c13a00_TEST FAIL\n"); return 1; }
    std::printf("012118c0_TEST PASS\n");
    return 0;
}