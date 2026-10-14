#include <cstdio>
static int g_r = 0, g_m = 0; struct I { virtual long __stdcall Q() { return 0; } virtual unsigned long __stdcall A() { return 0; } virtual unsigned long __stdcall R() { ++g_r; return 0; } }; struct M { unsigned char b[0x10]; ~M() { ++g_m; } }; struct X { unsigned char p[4]; M m; I* t; ~X() { if (t) { t->R(); t = 0; } } };
static bool Check() { I i; { X x; x.t = &i; } { X y; y.t = 0; } return g_r == 1 && g_m == 2; }

int main()
{
    if (!Check()) { std::printf("00caa790_TEST FAIL\n"); return 1; }
    std::printf("00caa790_TEST PASS\n");
    return 0;
}