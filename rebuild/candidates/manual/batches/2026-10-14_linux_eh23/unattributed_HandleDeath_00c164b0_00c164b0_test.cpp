#include <cstdio>
#include <new>
static int g_l = 0; struct G {}; static G g_g; struct L { void* t; long s; L(void* x) : t(x), s(0) { ++g_l; } ~L() { --g_l; } };
struct Mw; struct En { long m0; Mw* mw; }; struct Ev { long m0; En* en; }; static int g_die = 0, g_clr = 0; static Ev* g_dev = 0; static En* g_cen = 0; struct Mw { void OnDie(Ev* e) { ++g_die; g_dev = e; } void ClearEntry(En* e) { ++g_clr; g_cen = e; } };
struct X { long m0, m4; Mw* mw; long H(Ev* e) { L l(&g_g); if (!e) return -1; En* n = e->en; if (n->mw != mw) return -1; mw->OnDie(e); mw->ClearEntry(n); return 0; } };
static bool Check() { Mw a, b; X x; x.mw = &a; En n; n.mw = &b; Ev e; e.en = &n; if (x.H(0) != -1 || x.H(&e) != -1 || g_die) return false; n.mw = &a; return x.H(&e) == 0 && g_die == 1 && g_clr == 1 && g_dev == &e && g_cen == &n && g_l == 0; }

int main()
{
    if (!Check()) { std::printf("00c164b0_TEST FAIL\n"); return 1; }
    std::printf("00c164b0_TEST PASS\n");
    return 0;
}
