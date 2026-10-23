#include <cstdio>
#include <new>
#include <cstdlib>
static int g_lk = 0, g_ev = 0, g_ra = 0, g_rb = 0; static bool g_fail = false; static void* g_key = 0; static void* g_src = 0; static void* g_d = 0; static unsigned char g_sec[8];
struct Pl { void* s; unsigned long t; Pl(void* x) : s(x), t(0) { ++g_lk; } ~Pl() { --g_lk; } }; struct Rk { int v; }; static Rk g_rk; struct Tg { Rk* Lookup(void* k) { g_key = k; return k ? &g_rk : 0; } };
struct X; struct Ev { unsigned char b[0x4c]; Ev(X* s, Rk* k, void* d) { ++g_ev; g_src = s; g_d = d; } static void* operator new(size_t n) throw() { return g_fail ? 0 : std::malloc(n); } static void operator delete(void* p) { std::free(p); } };
struct Da { void Reg(Ev* e) { ++g_ra; } }; struct Db { void Reg(Ev* e) { ++g_rb; } }; struct Wd { unsigned char p[8]; Da* a; Db* b; };
struct X { long m0; Wd* w; Ev* Create(Tg* t, void* k, void* d) { Pl l(g_sec); if (!t) return 0; Rk* r = t->Lookup(k); if (!r) return 0; Ev* e = new Ev(this, r, d); w->a->Reg(e); w->b->Reg(e); return e; } };
static bool Check() { Da a; Db b; Wd w; w.a = &a; w.b = &b; X x; x.w = &w; Tg t; int k, d; if (x.Create(0, &k, &d) != 0 || g_ev) return false; if (x.Create(&t, 0, &d) != 0 || g_ev) return false; Ev* e = x.Create(&t, &k, &d); if (!e || g_ev != 1 || g_src != &x || g_d != &d || g_ra != 1 || g_rb != 1) return false; delete e; g_fail = true; return x.Create(&t, &k, &d) == 0 && g_ra == 2 && g_lk == 0; }

int main()
{
    if (!Check()) { std::printf("00c2c7e0_TEST FAIL\n"); return 1; }
    std::printf("0121395a_TEST PASS\n");
    return 0;
}