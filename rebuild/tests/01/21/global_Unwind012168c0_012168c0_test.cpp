#include <cstdio>
static int g_e = 0, g_l = 0, g_o = 0; struct CS; static void __stdcall E(CS*) { ++g_e; } static void __stdcall Lv(CS*) { ++g_l; } struct Lk { CS* p; Lk(CS* s) : p(s) { E(s); } ~Lk() { Lv(p); } }; struct X { virtual void O() { ++g_o; } unsigned char q[0x90]; unsigned char cs[0x2c]; void* pd; void C() { Lk l((CS*)cs); if (pd) { pd = 0; O(); } } };
static bool Check() { X x; x.pd = &x; x.C(); x.C(); return g_o == 1 && x.pd == 0 && g_e == 2 && g_l == 2; }

int main()
{
    if (!Check()) { std::printf("00ca4c60_TEST FAIL\n"); return 1; }
    std::printf("012168c0_TEST PASS\n");
    return 0;
}