#include <cstdio>
#include <new>
static int g_l = 0; struct G {}; static G g_g; static G g_g2; struct L { void* t; long s; L(void* x) : t(x), s(0) { ++g_l; } ~L() { --g_l; } };
static int g_upd = 0, g_det = 0, g_del = 0; static long g_eq = 0; static long g_slot = -1; struct Nd { unsigned char p[0x1c]; short eqp; }; struct Ow { void Update(Nd* n) { ++g_upd; } };
struct It { Nd* nd; unsigned char p[0x12]; signed char slot; long Equip(Ow* o, long s) { g_slot = s; return g_eq; } void Detach(Ow* o) { ++g_det; } ~It() { ++g_del; } };
struct X { unsigned char p[4]; Ow* o; void E(It* it) { L l(&g_g); long r = it->Equip(o, it->slot); Nd* n = it->nd; if (n) { n->eqp = r >= 0; o->Update(n); } if (r < 0) { it->Detach(o); delete it; } } };
static bool Check() { Ow o; X x; x.o = &o; Nd n; It a; a.nd = &n; a.slot = -2; g_eq = 3; x.E(&a); if (n.eqp != 1 || g_upd != 1 || g_slot != -2 || g_del) return false; It* b = new It; b->nd = 0; b->slot = 1; g_eq = -1; x.E(b); return g_det == 1 && g_del == 1 && g_upd == 1 && g_l == 0; }

int main()
{
    if (!Check()) { std::printf("00c26de0_TEST FAIL\n"); return 1; }
    std::printf("01213320_TEST PASS\n");
    return 0;
}