#include <cstdio>
static int g_r = 0, g_s = 0, g_x = 0; static void __stdcall Sh() { ++g_s; } struct I { virtual long __stdcall Q() { return 0; } virtual unsigned long __stdcall A() { return 0; } virtual unsigned long __stdcall R() { ++g_r; return 0; } };
struct B { virtual void v() {} ~B() {} }; struct X : B { virtual void Rr() { ++g_x; } __forceinline void D() { Rr(); } unsigned char p[4]; I* d; ~X() { D(); if (d) { d->R(); Sh(); d = 0; } } };
static bool Check() { I i; { X x; x.d = &i; } { X y; y.d = 0; } return g_r == 1 && g_s == 1 && g_x == 2; }

int main()
{
    if (!Check()) { std::printf("00c57310_TEST FAIL\n"); return 1; }
    std::printf("00c57310_TEST PASS\n");
    return 0;
}