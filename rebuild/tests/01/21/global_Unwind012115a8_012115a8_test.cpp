#include <cstdio>
static int g_l = 0, g_a = 0; struct G {}; static G g_g;
struct L { G* t; long s; L(G* x) : t(x), s(0) { ++g_l; } ~L() { --g_l; } }; struct O { long a; void A(void*) { ++g_a; } };
struct X { unsigned char p[2008]; O o; void F(void* v) { L l(&g_g); o.A(v); } };
int main()
{
    X* x = new X; x->F(0); if (g_a != 1 || g_l != 0) { std::printf("00c117f0_TEST FAIL\n"); return 1; }
    delete x;

    std::printf("012115a8_TEST PASS\n");
    return 0;
}