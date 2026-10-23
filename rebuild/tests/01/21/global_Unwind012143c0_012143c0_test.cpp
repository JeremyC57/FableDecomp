#include <cstdio>
static int g_c = 0, g_d = 0; static long g_s = 0; static void* g_h = 0; static void __stdcall St(long s) { g_s = s; } static void __stdcall Rh(void* h) { g_h = h; } struct M { unsigned char b[4]; void C() { ++g_c; } ~M() { ++g_d; } }; struct X { unsigned char p[4]; void* h; M m; ~X() { m.C(); St(0x14); Rh(h); h = 0; } };
static bool Check() { int i; { X x; x.h = &i; } return g_c == 1 && g_d == 1 && g_s == 0x14 && g_h == &i; }

int main()
{
    if (!Check()) { std::printf("00c354a0_TEST FAIL\n"); return 1; }
    std::printf("012143c0_TEST PASS\n");
    return 0;
}