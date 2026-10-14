#include <cstdio>
#include <new>
static int g_l = 0; struct G {}; static G g_g; struct L { void* t; long s; L(void* x) : t(x), s(0) { ++g_l; } ~L() { --g_l; } };
static int g_mo = 0, g_nl = 0, g_bind = 0; static void* g_tgt = 0; static void* g_src = 0; static long g_z = 0; struct Mo { unsigned char b[0x1c]; Mo() { ++g_mo; } }; struct Nl { long a, b, c; Nl() : a(0), b(0), c(0) { ++g_nl; } };
struct Rq { Mo mo; long m1c, m20; float sx, sy; long m2c; Nl nl; long m3c; long m40; short m44; long m48, m4c; Rq() : m1c(0), m20(0), sx(1.0f), sy(1.0f), m2c(0), m40(0), m44(0), m48(0), m4c(0) {} void Bind(void* t) { ++g_bind; g_tgt = t; } };
struct X { long Load(void* s, Rq* r) { g_src = s; return r->sx == 1.0f && r->sy == 1.0f && r->m1c == 0 && r->m44 == 0 && r->nl.c == 0 ? 42 : -1; } long LoadLayer(void* s, void* t) { L l(&g_g); Rq r; r.Bind(t); return Load(s, &r); } };
static bool Check() { X x; int s, t; return x.LoadLayer(&s, &t) == 42 && g_mo == 1 && g_nl == 1 && g_bind == 1 && g_tgt == &t && g_src == &s && g_l == 0; }

int main()
{
    if (!Check()) { std::printf("00c13310_TEST FAIL\n"); return 1; }
    std::printf("00c13310_TEST PASS\n");
    return 0;
}