#include <cstdio>
#include <new>
static int g_l = 0; struct G {}; static G g_g; static G g_g2; struct L { void* t; long s; L(void* x) : t(x), s(0) { ++g_l; } ~L() { --g_l; } };
struct En { long b, s; long IsBound() { return b; } long IsSelected() { return s; } }; static En g_e[4]; struct Ar { long a; unsigned long c; En* Get(unsigned long i) { return &g_e[i]; } };
struct X { unsigned char p[0x5c]; Ar ar; unsigned long Count() { L l(&g_g); unsigned long n = 0; for (unsigned long i = 0; i < ar.c; ++i) { En* e = ar.Get(i); if (e->IsBound() && e->IsSelected()) ++n; } return n; } };
static bool Check() { g_e[0].b = 1; g_e[0].s = 1; g_e[1].b = 1; g_e[1].s = 0; g_e[2].b = 0; g_e[2].s = 1; g_e[3].b = 1; g_e[3].s = 1; X* x = new X; x->ar.c = 4; bool ok = x->Count() == 2 && g_l == 0; delete x; return ok; }

int main()
{
    if (!Check()) { std::printf("00c125e0_TEST FAIL\n"); return 1; }
    std::printf("00c125e0_TEST PASS\n");
    return 0;
}