#include <cstdio>
static int g_e = 0, g_l = 0, g_o = 0; struct CS; static void __stdcall E(CS*) { ++g_e; } static void __stdcall L(CS*) { ++g_l; } struct Lk { CS* p; Lk(CS* s) : p(s) { E(s); } ~Lk() { L(p); } }; struct X { unsigned char p[8]; unsigned char cs[0x5c]; long n; void D() { ++g_o; } long O() { Lk l((CS*)cs); n = 0; D(); return 0; } };
static bool Check() { X x; x.n = 5; return x.O() == 0 && x.n == 0 && g_e == 1 && g_l == 1 && g_o == 1; }

int main()
{
    if (!Check()) { std::printf("00caf890_TEST FAIL\n"); return 1; }
    std::printf("00caf890_TEST PASS\n");
    return 0;
}