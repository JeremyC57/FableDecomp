#include <cstdio>
static int g_e = 0, g_v = 0; struct CS; static void __stdcall E(CS*) { ++g_e; } static void __stdcall Lv(CS*) { ++g_v; } struct Lk { CS* p; Lk(CS* s) : p(s) { E(s); } ~Lk() { Lv(p); } };
static int g_r = 0; struct I { virtual long __stdcall Q() { return 0; } virtual unsigned long __stdcall A() { return 0; } virtual unsigned long __stdcall R() { ++g_r; return 0; } };
struct X { unsigned char p[0x6c]; I* f; unsigned char q[0x24]; unsigned char cs[4]; long R() { Lk l((CS*)cs); if (f) { f->R(); f = 0; } return 0; } };
static bool Check() { I i; X x; x.f = &i; return x.R() == 0 && x.f == 0 && g_r == 1 && g_e == 1 && g_v == 1; }

int main()
{
    if (!Check()) { std::printf("00ca4bf0_TEST FAIL\n"); return 1; }
    std::printf("00ca4bf0_TEST PASS\n");
    return 0;
}