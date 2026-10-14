#include <cstdio>
static int g_n = 0; struct I { virtual long __stdcall M() = 0; }; struct J : I { long __stdcall M() { ++g_n; return 0; } }; struct X { unsigned char p[4]; I* i; void F() { i->M(); } };
int main()
{
    J j; X x; x.i = &j; x.F(); if (g_n != 1) { std::printf("00a63120_TEST FAIL\n"); return 1; }

    std::printf("00a63120_TEST PASS\n");
    return 0;
}