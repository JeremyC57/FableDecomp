#include <cstdio>
static int g_e = 0, g_l = 0, g_a = 0, g_r = 0; struct CS; static void __stdcall E(CS*) { ++g_e; } static void __stdcall Lv(CS*) { ++g_l; } struct Lk { CS* p; Lk(CS* s) : p(s) { E(s); } ~Lk() { Lv(p); } }; struct I { virtual long __stdcall Q() { return 0; } virtual unsigned long __stdcall A() { ++g_a; return 0; } virtual unsigned long __stdcall R() { ++g_r; return 0; } };
struct X { unsigned char p[0x6c]; I* f; unsigned char q[0x24]; unsigned char cs[4]; I* R() { Lk l((CS*)cs); if (f) f->A(); return f; } };
static bool Check() { I i; X x; x.f = &i; bool ok = x.R() == &i; x.f = 0; ok = ok && x.R() == 0; return ok && g_a == 1 && g_e == 2 && g_l == 2; }

int main()
{
    if (!Check()) { std::printf("00ca4b90_TEST FAIL\n"); return 1; }
    std::printf("00ca4b90_TEST PASS\n");
    return 0;
}