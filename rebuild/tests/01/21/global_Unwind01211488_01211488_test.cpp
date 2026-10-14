#include <cstdio>
static int g_l = 0; struct G {}; static G g_g; struct L { void* t; long s; L(void* x) : t(x), s(0) { ++g_l; } ~L() { --g_l; } };
static int g_h = -1; struct Tg { void S(bool h) { g_h = h; } }; struct X { unsigned char p[0x7bc]; Tg* t; long S(void* v) { L l(&g_g); t->S(v == 0); return 0; } };
static bool Check() { Tg t; X* x = new X; x->t = &t; bool ok = x->S(0) == 0 && g_h == 1; int i; x->S(&i); ok = ok && g_h == 0 && g_l == 0; delete x; return ok; }

int main()
{
    if (!Check()) { std::printf("00c10d40_TEST FAIL\n"); return 1; }
    std::printf("00c10d40_TEST PASS\n");
    return 0;
}