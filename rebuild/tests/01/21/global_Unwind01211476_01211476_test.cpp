#include <cstdio>
static int g_l = 0; struct G {}; static G g_g; struct L { void* t; long s; L(void* x) : t(x), s(0) { ++g_l; } ~L() { --g_l; } };
struct Tg { void* F(void* k) { return k; } }; struct X { unsigned char p[0x7bc]; Tg* t; void* F(void* k) { L l(&g_g); return t->F(k); } };
static bool Check() { Tg t; X* x = new X; x->t = &t; int i; bool ok = x->F(&i) == &i && g_l == 0; delete x; return ok; }

int main()
{
    if (!Check()) { std::printf("00c10c10_TEST FAIL\n"); return 1; }
    std::printf("01211476_TEST PASS\n");
    return 0;
}