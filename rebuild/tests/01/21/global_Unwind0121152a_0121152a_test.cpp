#include <cstdio>
static int g_l = 0; static void* g_s = 0; static void* g_v = 0; struct G {}; static G g_g;
struct L { G* t; long s; L(G* x) : t(x), s(0) { ++g_l; } ~L() { --g_l; } };
struct Ob { virtual void N(void* s, void* v) { g_s = s; g_v = v; } }; static Ob g_ob;
struct O { long a; Ob* F(void* k) { return k ? &g_ob : 0; } };
struct X { unsigned char p[44]; O o; void F(void* k, void* v) { L l(&g_g); Ob* b = o.F(k); if (b) b->N(this, v); } };
int main()
{
    X* x = new X; int i; x->F(&i, &i); if (g_s != x || g_v != &i || g_l != 0) { std::printf("00c11430_TEST FAIL\n"); return 1; }
    delete x;

    std::printf("0121152a_TEST PASS\n");
    return 0;
}