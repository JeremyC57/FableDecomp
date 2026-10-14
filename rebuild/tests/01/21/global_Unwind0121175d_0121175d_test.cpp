#include <cstdio>
#include <new>
static int g_l = 0; struct G {}; static G g_g; struct L { void* t; long s; L(void* x) : t(x), s(0) { ++g_l; } ~L() { --g_l; } };
struct X; static int g_ap = 0, g_rf = 0, g_rb = 0, g_fin = 0; static unsigned long g_fl = 0; struct En { long bound; long IsBound() { return bound; } void Apply(X* i, unsigned long f) { ++g_ap; g_fl = f; } }; static En g_e[3]; struct Ar { long a; unsigned long c; En* Get(unsigned long i) { return &g_e[i]; } unsigned long GetCount() const { return c; } }; struct Ow { void Finish() { ++g_fin; } };
struct X { unsigned char p[0x5c]; Ar ar; unsigned char q[0x758]; Ow* o; unsigned char r[8]; unsigned long fl; void Refresh() { ++g_rf; } void Rebuild() { ++g_rb; } void Reset() { L l(&g_g); for (unsigned long i = 0; i < ar.GetCount(); ++i) { En* e = ar.Get(i); if (e->IsBound()) e->Apply(this, fl); } fl = 0; Refresh(); Rebuild(); o->Finish(); } };
static bool Check() { g_e[0].bound = 1; g_e[1].bound = 0; g_e[2].bound = 1; Ow o; X* x = new X; x->o = &o; x->ar.c = 3; x->fl = 9; x->Reset(); bool ok = g_ap == 2 && g_fl == 9 && x->fl == 0 && g_rf == 1 && g_rb == 1 && g_fin == 1 && g_l == 0; delete x; return ok; }

int main()
{
    if (!Check()) { std::printf("00c12880_TEST FAIL\n"); return 1; }
    std::printf("00c12880_TEST PASS\n");
    return 0;
}