#include <cstdio>
static int g_l = 0; struct G {}; static G g_g; struct L { void* t; long s; L(void* x) : t(x), s(0) { ++g_l; } ~L() { --g_l; } };
static int g_u = 0; struct Tg { void U() { ++g_u; } }; struct X { unsigned char p[8]; Tg* t; void F() { L l(&g_g); if (t) t->U(); } };
static bool Check() { Tg t; X x; x.t = 0; x.F(); x.t = &t; x.F(); return g_u == 1 && g_l == 0; }

int main()
{
    if (!Check()) { std::printf("00c15aa0_TEST FAIL\n"); return 1; }
    std::printf("00c15aa0_TEST PASS\n");
    return 0;
}