#include <cstdio>
#include <new>
static int g_l = 0; struct G {}; static G g_g; struct L { void* t; long s; L(void* x) : t(x), s(0) { ++g_l; } ~L() { --g_l; } };
static int g_rel = 0, g_cr = 0; static bool g_make = true; struct Sc { virtual void v0() {} virtual void v1() {} virtual void v2() {} virtual void Release() { ++g_rel; } }; static Sc g_sc; struct Fa { virtual Sc* Create(void* k) { ++g_cr; return g_make ? &g_sc : 0; } }; struct Ow { unsigned char p[0x5c]; Fa* fa; };
struct X { long m0; Sc* sc; Ow* o; unsigned char p[0x3c]; long locked; bool Try(void* k) { L l(&g_g); if (locked) return false; if (sc) { sc->Release(); sc = 0; } sc = o->fa->Create(k); if (!sc) return false; return true; } };
static bool Check() { Fa fa; Ow o; o.fa = &fa; X x; x.sc = 0; x.o = &o; x.locked = 1; int k; if (x.Try(&k) || g_cr) return false; x.locked = 0; if (!x.Try(&k) || x.sc != &g_sc || g_rel) return false; g_make = false; return !x.Try(&k) && g_rel == 1 && x.sc == 0 && g_cr == 2 && g_l == 0; }

int main()
{
    if (!Check()) { std::printf("00c0e600_TEST FAIL\n"); return 1; }
    std::printf("01210fe0_TEST PASS\n");
    return 0;
}