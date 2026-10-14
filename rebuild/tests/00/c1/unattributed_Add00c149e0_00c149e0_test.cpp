#include <cstdio>
static int g_l = 0; struct G {}; static G g_g; struct L { void* t; long s; L(void* x) : t(x), s(0) { ++g_l; } ~L() { --g_l; } };
static void* g_v = 0; struct Tg { void A(void* v) { g_v = v; } }; struct X { unsigned char p[8]; Tg* t; void A(void* v) { L l(&g_g); t->A(v); } };
static bool Check() { Tg t; X x; x.t = &t; int i; x.A(&i); return g_v == &i && g_l == 0; }

int main()
{
    if (!Check()) { std::printf("00c149e0_TEST FAIL\n"); return 1; }
    std::printf("00c149e0_TEST PASS\n");
    return 0;
}