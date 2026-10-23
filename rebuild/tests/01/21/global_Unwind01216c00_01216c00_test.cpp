#include <cstdio>
static int g_r = 0, g_m = 0; struct I { virtual long __stdcall Q() { return 0; } virtual unsigned long __stdcall A() { return 0; } virtual unsigned long __stdcall R() { ++g_r; return 0; } }; struct O { unsigned char p[0xc]; I i; }; struct M { unsigned char b[4]; ~M() { ++g_m; } }; struct X { virtual void v() {} unsigned char p[8]; O* o; unsigned char q[8]; M m; ~X() { o->i.R(); } };
static bool Check() { O o; { X x; x.o = &o; } return g_r == 1 && g_m == 1; }

int main()
{
    if (!Check()) { std::printf("00ca7a20_TEST FAIL\n"); return 1; }
    std::printf("01216c00_TEST PASS\n");
    return 0;
}