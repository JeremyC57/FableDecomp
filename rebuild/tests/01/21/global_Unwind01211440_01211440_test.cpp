#include <cstdio>
static int g_l = 0; struct G {}; static G g_g; struct L { void* t; long s; L(void* x) : t(x), s(0) { ++g_l; } ~L() { --g_l; } };
static int g_u = 0; struct Tg { void U() { ++g_u; } }; struct X { unsigned char p[0x7bc]; Tg* t; unsigned char q[0xc5]; bool m; void S(bool b) { L l(&g_g); m = b; t->U(); } };
static bool Check() { Tg t; X* x = new X; x->t = &t; x->S(true); bool ok = x->m && g_u == 1 && g_l == 0; delete x; return ok; }

int main()
{
    if (!Check()) { std::printf("00c10a90_TEST FAIL\n"); return 1; }
    std::printf("00c10a90_TEST PASS\n");
    return 0;
}