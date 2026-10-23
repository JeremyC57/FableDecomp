#include <cstdio>
#include <new>
static int g_l = 0; struct G {}; static G g_g; static G g_g2; struct L { void* t; long s; L(void* x) : t(x), s(0) { ++g_l; } ~L() { --g_l; } };
struct X; static int g_rf = 0, g_pr = 0, g_cr = 0; struct El { bool ok; bool Validate(X* c) { return ok; } void Process(X* c) { ++g_pr; } }; static El g_el[2]; struct En { long a; El* el; void Refresh() { ++g_rf; } }; static En g_en[3]; struct Ar { long a; unsigned long c; En* Get(unsigned long i) { return &g_en[i]; } };
struct X { unsigned char p[0x224]; Ar ar; void Refresh() { ++g_cr; } void P() { L l(&g_g); for (unsigned long i = 0; i < ar.c; ++i) { En* e = ar.Get(i); if (e->el) { e->Refresh(); El* el = e->el; if (!el->Validate(this)) el->Process(this); } } Refresh(); } };
static bool Check() { g_el[0].ok = true; g_el[1].ok = false; g_en[0].el = &g_el[0]; g_en[1].el = 0; g_en[2].el = &g_el[1]; X* x = new X; x->ar.c = 3; x->P(); bool ok = g_rf == 2 && g_pr == 1 && g_cr == 1 && g_l == 0; delete x; return ok; }

int main()
{
    if (!Check()) { std::printf("00c11f50_TEST FAIL\n"); return 1; }
    std::printf("0121167d_TEST PASS\n");
    return 0;
}