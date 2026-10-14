#include <cstdio>
static int g_l = 0; struct G {}; static G g_g; struct L { void* t; long s; L(void* x) : t(x), s(0) { ++g_l; } ~L() { --g_l; } };
static void* g_v = 0; struct Tg { void A(void* v) { g_v = v; } }; struct X { unsigned char p[0x7bc]; Tg* t; void A(void* v) { L l(&g_g); t->A(v); } };
static bool Check() { Tg t; X* x = new X; x->t = &t; int i; x->A(&i); bool ok = g_v == &i && g_l == 0; delete x; return ok; }

int main()
{
    if (!Check()) { std::printf("00c10b30_TEST FAIL\n"); return 1; }
    std::printf("00c10b30_TEST PASS\n");
    return 0;
}