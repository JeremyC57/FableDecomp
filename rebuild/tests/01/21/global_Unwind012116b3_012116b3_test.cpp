#include <cstdio>
#include <new>
static int g_l = 0; struct G {}; static G g_g; static G g_g2; struct L { void* t; long s; L(void* x) : t(x), s(0) { ++g_l; } ~L() { --g_l; } };
struct X; static int g_n = 0; struct En { unsigned char p[0xa4]; void* cr; void OnC(X* h) { ++g_n; } }; static En g_e[3]; struct Ar { long a; unsigned long c; En* Get(unsigned long i) { return &g_e[i]; } };
struct X { unsigned char p[0x5c]; Ar ar; void On(void* c) { L l(&g_g); for (unsigned long i = 0; i < ar.c; ++i) { En* e = ar.Get(i); if (e->cr == c) e->OnC(this); } } };
static bool Check() { int a, b; g_e[0].cr = &a; g_e[1].cr = &b; g_e[2].cr = &a; X* x = new X; x->ar.c = 3; x->On(&a); bool ok = g_n == 2 && g_l == 0; delete x; return ok; }

int main()
{
    if (!Check()) { std::printf("00c12270_TEST FAIL\n"); return 1; }
    std::printf("012116b3_TEST PASS\n");
    return 0;
}