#include <cstdio>
static int g_l = 0; struct G {}; static G g_g; struct L { void* t; long s; L(void* x) : t(x), s(0) { ++g_l; } ~L() { --g_l; } };
struct X; struct Ob { virtual void v0(){} virtual void v1(){} virtual void v2(){} virtual void v3(){} virtual void N(X* s) { seen = s; } X* seen; Ob() : seen(0) {} };
static Ob g_ob; struct Obs { long a; Ob* F(void* k) { return k ? &g_ob : 0; } };
struct X { unsigned char p[0x2c]; Obs o; void N(void* k) { L l(&g_g); Ob* ob = o.F(k); if (ob) ob->N(this); } };
static bool Check() { X x; x.N(0); if (g_ob.seen) return false; x.N(&x); return g_ob.seen == &x && g_l == 0; }

int main()
{
    if (!Check()) { std::printf("00c114b0_TEST FAIL\n"); return 1; }
    std::printf("00c114b0_TEST PASS\n");
    return 0;
}
