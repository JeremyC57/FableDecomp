#include <cstdio>
static int g_l = 0; struct G {}; static G g_g; struct L { void* t; long s; L(void* x) : t(x), s(0) { ++g_l; } ~L() { --g_l; } };
static void* g_a = 0; static void* g_b = 0; struct Tg { void T(void* a, void* b) { g_a = a; g_b = b; } }; struct X { unsigned char p[0x7d8]; Tg t; void T(void* a, void* b) { L l(&g_g); t.T(a, b); } };
static bool Check() { X* x = new X; int i, j; x->T(&i, &j); bool ok = g_a == &i && g_b == &j && g_l == 0; delete x; return ok; }

int main()
{
    if (!Check()) { std::printf("00c118e0_TEST FAIL\n"); return 1; }
    std::printf("00c118e0_TEST PASS\n");
    return 0;
}