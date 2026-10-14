#include <cstdio>
static int g_l = 0; struct G {}; static G g_g; struct L { void* t; long s; L(void* x) : t(x), s(0) { ++g_l; } ~L() { --g_l; } };
struct X; static int g_ref = 0; struct Ob { virtual void v0() {} virtual void Sel(X* s) { seen = s; } X* seen; Ob() : seen(0) {} }; static Ob g_ob; struct Obs { long a; Ob* F(void* k) { return k ? &g_ob : 0; } };
struct X { unsigned char p[0x2c]; Obs o; void Ref() { ++g_ref; } void P(void* k) { L l(&g_g); Ob* ob = o.F(k); if (ob) { ob->Sel(this); Ref(); } } };
static bool Check() { X x; x.P(0); if (g_ob.seen || g_ref) return false; x.P(&x); return g_ob.seen == &x && g_ref == 1 && g_l == 0; }

int main()
{
    if (!Check()) { std::printf("00c11330_TEST FAIL\n"); return 1; }
    std::printf("00c11330_TEST PASS\n");
    return 0;
}