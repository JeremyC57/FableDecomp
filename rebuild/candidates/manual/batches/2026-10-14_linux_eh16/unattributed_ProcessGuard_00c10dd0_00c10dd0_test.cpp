#include <cstdio>
static int g_l = 0; struct G {}; static G g_g; struct L { void* t; long s; L(void* x) : t(x), s(0) { ++g_l; } ~L() { --g_l; } };
static void* g_h = 0; static int g_k = 0; static void Helper(void* a) { g_h = a; } struct Li { virtual void Poke() { ++g_k; } }; struct Ow { unsigned char p[0x64]; Li* li; void PL() { if (li) li->Poke(); } };
struct X { unsigned char p[4]; Ow* ow; unsigned char q[0x7c0]; unsigned long fl; long PG(void* a) { L l(&g_g); Helper(a); fl |= 6; ow->PL(); return 0; } };
static bool Check() { Li li; Ow ow; ow.li = 0; X* x = new X; x->ow = &ow; x->fl = 8; int a; long r = x->PG(&a); bool ok = r == 0 && g_h == &a && x->fl == 14 && g_k == 0; ow.li = &li; x->PG(&a); ok = ok && g_k == 1 && g_l == 0; delete x; return ok; }

int main()
{
    if (!Check()) { std::printf("00c10dd0_TEST FAIL\n"); return 1; }
    std::printf("00c10dd0_TEST PASS\n");
    return 0;
}
