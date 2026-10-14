#include <cstdio>
static int g_l = 0; struct G {}; static G g_g; struct L { void* t; long s; L(void* x) : t(x), s(0) { ++g_l; } ~L() { --g_l; } };
struct Tg { void* G() { return this; } }; struct X { unsigned char p[0x7bc]; Tg* t; void* G() { L l(&g_g); return t->G(); } };
static bool Check() { Tg t; X* x = new X; x->t = &t; bool ok = x->G() == &t && g_l == 0; delete x; return ok; }

int main()
{
    if (!Check()) { std::printf("00c10ba0_TEST FAIL\n"); return 1; }
    std::printf("00c10ba0_TEST PASS\n");
    return 0;
}