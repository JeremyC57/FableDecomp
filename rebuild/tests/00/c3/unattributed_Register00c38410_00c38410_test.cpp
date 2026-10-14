#include <cstdio>
static int g_l = 0; struct G {}; static G g_g; struct L { void* t; long s; L(void* x) : t(x), s(0) { ++g_l; } ~L() { --g_l; } };
static void* g_p = 0; struct X; struct Tg { void A(X* p) { g_p = p; } }; struct X { unsigned char p[4]; Tg* t; void R() { L l(&g_g); t->A(this); } };
static bool Check() { Tg t; X x; x.t = &t; x.R(); return g_p == &x && g_l == 0; }

int main()
{
    if (!Check()) { std::printf("00c38410_TEST FAIL\n"); return 1; }
    std::printf("00c38410_TEST PASS\n");
    return 0;
}