// Behaviour test for CTCSwitchableNavigation::~CTCSwitchableNavigation @ 0x007876D0: unblocks the parent's 40-bit id nodes only when the map is active.
#include <cstdio>
struct CThing { unsigned long idLow, idHigh; };
struct CNavigatorManager {
    long active; long calls; long map; unsigned long low, high; bool blocked;
    bool IsNavigationMapActivated(long m) { return m == active; }
    void SetSwitchableNodesAsBlocked(long m, unsigned long lo, unsigned long hi, bool b) { ++calls; map = m; low = lo; high = hi; blocked = b; }
};
static CNavigatorManager g_nav;
struct CTCBase { CThing* ParentThing; ~CTCBase() {} };
struct CTCSwitchableNavigation : CTCBase {
    long MapIndex;
    ~CTCSwitchableNavigation()
    {
        CNavigatorManager* n = &g_nav;
        if (n->IsNavigationMapActivated(MapIndex)) n->SetSwitchableNodesAsBlocked(MapIndex, ParentThing->idLow, ParentThing->idHigh & 0xFF, false);
    }
};
int main()
{
    CThing th; th.idLow = 0x12345678; th.idHigh = 0xABCDEF9A;
    g_nav.active = 3; g_nav.calls = 0; g_nav.blocked = true;
    { CTCSwitchableNavigation s; s.ParentThing = &th; s.MapIndex = 2; }
    if (g_nav.calls != 0) { std::printf("007876d0_TEST FAIL\n"); return 1; }
    { CTCSwitchableNavigation s; s.ParentThing = &th; s.MapIndex = 3; }
    if (g_nav.calls != 1 || g_nav.map != 3 || g_nav.low != 0x12345678 || g_nav.high != 0x9A || g_nav.blocked) { std::printf("007876d0_TEST FAIL\n"); return 1; }
    std::printf("007876d0_TEST PASS\n");
    return 0;
}
