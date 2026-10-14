#include <cstdio>
static int g_l = 0; struct G {}; static G g_g; struct L { void* t; long s; L(void* x) : t(x), s(0) { ++g_l; } ~L() { --g_l; } };
struct E { unsigned char p[0xc]; long v; }; static E g_e;
struct X { E* F(void* a, void* b, void* c, long f) { return (a && f == 0) ? &g_e : 0; } long G(void* a, void* b, void* c) { L l(&g_g); E* e = F(a, b, c, 0); long v = e ? e->v : -1; return v; } };
static bool Check() { g_e.v = 42; X x; int a; return x.G(&a, 0, 0) == 42 && x.G(0, 0, 0) == -1 && g_l == 0; }

int main()
{
    if (!Check()) { std::printf("00c13140_TEST FAIL\n"); return 1; }
    std::printf("00c13140_TEST PASS\n");
    return 0;
}