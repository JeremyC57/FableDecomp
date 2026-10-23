#include <cstdio>
static int g_a = 0, g_r = 0, g_b = 0; struct I { virtual long __stdcall Q() { return 0; } virtual unsigned long __stdcall A() { ++g_a; return 0; } virtual unsigned long __stdcall R() { ++g_r; return 0; } };
struct B { unsigned char b[0x38]; ~B() { ++g_b; } }; struct X : B { unsigned char p[0x64]; I* t; ~X() { if (t) { t->R(); t = 0; } } };
static bool Check() { I i; { X x; x.t = &i; } { X y; y.t = 0; } return g_r == 1 && g_b == 2; }

int main()
{
    if (!Check()) { std::printf("00ca8940_TEST FAIL\n"); return 1; }
    std::printf("01216c80_TEST PASS\n");
    return 0;
}