#include <cstdio>
static int g_l = 0; struct G {}; static G g_g; struct L { void* t; long s; L(void* x) : t(x), s(0) { ++g_l; } ~L() { --g_l; } };
struct X; static int g_n = 3, g_u = 0; struct El { void U(X* c) { ++g_u; } }; static El g_el; struct Rg { long a; El* F(void* k) { return (k && g_n-- > 0) ? &g_el : 0; } };
struct X { unsigned char p[0x2c]; Rg r; void UA(void* k) { L l(&g_g); El* e = r.F(k); while (e) { e->U(this); e = r.F(k); } } };
static bool Check() { X x; x.UA(0); if (g_u) return false; x.UA(&x); return g_u == 3 && g_l == 0; }

int main()
{
    if (!Check()) { std::printf("00c11950_TEST FAIL\n"); return 1; }
    std::printf("00c11950_TEST PASS\n");
    return 0;
}