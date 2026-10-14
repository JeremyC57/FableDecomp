#include <cstdio>
#include <new>
static int g_hd = 0, g_pt = 0, g_dt = 0, g_gr = 0; static bool g_ok = true, g_res = true; static void* g_fa = 0; static void* g_fe = 0;
struct Hd { unsigned char b[0x10]; Hd() { ++g_hd; } }; struct Rq; struct Pt { unsigned char b[0x20]; Pt(Hd* o, long f) { ++g_pt; } void Res(bool* r) { ++g_gr; *r = g_res; } }; struct Rq : Hd { Pt pt; Rq() : Hd(), pt(this, 1) {} ~Rq() { ++g_dt; } };
struct Dp { virtual void v0() {} Pt* p; void* t; Dp(Pt* P, void* T) : p(P), t(T) {} }; struct Tg { virtual void v0() {} virtual bool Disp(void* a, void* b, void* c, void* d, void* e, Dp* f) { g_fa = a; g_fe = e; return g_ok; } };
struct X { Tg* t; bool Go(void* a, void* b, void* c, void* d) { Rq r; Dp dp(&r.pt, t); if (!t->Disp(a, b, c, d, 0, &dp)) return false; bool res; r.pt.Res(&res); return res; } };
static bool Check() { Tg t; X x; x.t = &t; int a; if (!x.Go(&a, 0, 0, 0) || g_hd != 1 || g_pt != 1 || g_dt != 1 || g_gr != 1 || g_fa != &a || g_fe != 0) return false; g_res = false; if (x.Go(&a, 0, 0, 0) || g_dt != 2) return false; g_ok = false; return !x.Go(&a, 0, 0, 0) && g_gr == 2 && g_dt == 3; }

int main()
{
    if (!Check()) { std::printf("00c1df70_TEST FAIL\n"); return 1; }
    std::printf("00c1df70_TEST PASS\n");
    return 0;
}
