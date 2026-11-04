// Behaviour test for CMiniMapDisplay::CMarkerInfo::IsReversedOnMap (0x0082A3E0): a model of the row over the four cases --
// no thing / dead thing (false), a region-exit driver (its flag), a scripted-hook component (its flag), neither (false).
#include <stdio.h>
struct CTCDRegionExit { bool ReversedOnMiniMap; };
struct CTCActionUseScriptedHook { bool ReversedOnMiniMap; };
struct CThing {
    bool Dead; bool HasRegionExit; bool HasHook; CTCDRegionExit Exit; CTCActionUseScriptedHook Hook;
    bool PeekTCDriver(const CTCDRegionExit** r) const { if (HasRegionExit) { *r = &Exit; return true; } return false; }
    bool PeekTC(const CTCActionUseScriptedHook** r) const { if (HasHook) { *r = &Hook; return true; } return false; }
};
struct CMarkerInfo {
    const CThing* PThing;
    bool IsReversedOnMap() const {
        if (PThing != 0 && !PThing->Dead) {
            const CTCDRegionExit* e; if (PThing->PeekTCDriver(&e)) return e->ReversedOnMiniMap;
            const CTCActionUseScriptedHook* h; if (PThing->PeekTC(&h)) return h->ReversedOnMiniMap;
        }
        return false;
    }
};
int main()
{
    int bad = 0;
    CMarkerInfo m; m.PThing = 0; if (m.IsReversedOnMap()) ++bad;
    CThing t; t.Dead = true; t.HasRegionExit = true; t.HasHook = false; t.Exit.ReversedOnMiniMap = true; t.Hook.ReversedOnMiniMap = true;
    m.PThing = &t; if (m.IsReversedOnMap()) ++bad;
    t.Dead = false; if (!m.IsReversedOnMap()) ++bad;
    t.Exit.ReversedOnMiniMap = false; if (m.IsReversedOnMap()) ++bad;
    t.HasRegionExit = false; t.HasHook = true; if (!m.IsReversedOnMap()) ++bad;
    t.HasHook = false; if (m.IsReversedOnMap()) ++bad;
    printf(bad ? "0082a3e0_TEST FAIL\n" : "0082a3e0_TEST PASS\n");
    return bad ? 1 : 0;
}