#include <cstdio>
#include <new>
static int g_l = 0; struct G {}; static G g_g; static G g_g2; struct L { void* t; long s; L(void* x) : t(x), s(0) { ++g_l; } ~L() { --g_l; } };
struct X; static int g_p = 0; struct En { unsigned char p[0x18]; void* k; long bound; long IsBound() { return bound; } void Process(X* i) { ++g_p; } }; static En g_e[3]; struct Ar { long a; unsigned long c; En* Get(unsigned long i) { return &g_e[i]; } };
struct X { unsigned char p[0x5c]; Ar ar; void P(void* k) { L l(&g_g); for (unsigned long i = 0; i < ar.c; ++i) { En* e = ar.Get(i); if (e->IsBound() && e->k == k) e->Process(this); } } };
static bool Check() { int a; g_e[0].k = &a; g_e[0].bound = 1; g_e[1].k = &a; g_e[1].bound = 0; g_e[2].k = 0; g_e[2].bound = 1; X* x = new X; x->ar.c = 3; x->P(&a); bool ok = g_p == 1 && g_l == 0; delete x; return ok; }

int main()
{
    if (!Check()) { std::printf("00c120a0_TEST FAIL\n"); return 1; }
    std::printf("00c120a0_TEST PASS\n");
    return 0;
}