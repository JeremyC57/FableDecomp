// Behaviour test for 0x00E2A0F0 CScytheNearOracle::Init: the five script-interface calls in order, the
// by-value CScriptThing copy adding a reference.
#include <cstdio>
struct Info { long count; };
struct CScriptThing { void* Data; Info* InfoP; CScriptThing() : Data(0), InfoP(0) {} CScriptThing(const CScriptThing& o) : Data(o.Data), InfoP(o.InfoP) { if (InfoP) ++InfoP->count; } };
static int g_order[8]; static int g_n = 0; static long g_seenCount = 0;
struct GSI {
    void SetIsThingForcePushable(const CScriptThing&, bool b) { g_order[g_n++] = b ? -1 : 745; }
    void SetIsPushableByHero(CScriptThing t, bool b) { g_order[g_n++] = b ? -1 : 844; g_seenCount = t.InfoP->count; }
    void EntitySetAsKillable(const CScriptThing&, bool a, bool b) { g_order[g_n++] = (a || b) ? -1 : 517; }
    void EntitySetAsToAddToComboMultiplierWhenHit(const CScriptThing&, bool b) { g_order[g_n++] = b ? -1 : 526; }
    void SetThingHasInformation(const CScriptThing&, bool a, bool b, bool c) { g_order[g_n++] = (a || b || c) ? -1 : 360; }
};
struct CScytheNearOracle {
    GSI* gsi; CScriptThing Thing;
    void Init()
    {
        gsi->SetIsThingForcePushable(Thing, false);
        gsi->SetIsPushableByHero(Thing, false);
        gsi->EntitySetAsKillable(Thing, false, false);
        gsi->EntitySetAsToAddToComboMultiplierWhenHit(Thing, false);
        gsi->SetThingHasInformation(Thing, false, false, false);
    }
};
int main()
{
    Info info; info.count = 1; GSI g; CScytheNearOracle o; o.gsi = &g; o.Thing.InfoP = &info;
    o.Init();
    if (g_n != 5 || g_order[0] != 745 || g_order[1] != 844 || g_order[2] != 517 || g_order[3] != 526 || g_order[4] != 360 || g_seenCount != 2) {
        std::printf("00e2a0f0_TEST FAIL\n"); return 1;
    }
    std::printf("00e2a0f0_TEST PASS\n");
    return 0;
}
