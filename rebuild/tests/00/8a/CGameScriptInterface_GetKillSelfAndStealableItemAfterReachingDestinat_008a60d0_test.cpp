// behaviour test for 0x008A60D0 CGameScriptInterface::GetKillSelfAndStealableItemAfterReachingDestination(CScriptThing) (model; the scratchpad lookup is a stub)
#include <cstdio>
class CTCAIScratchpad { public: bool KillSelfAndStealableItemAfterReachingDestination; };
class CThing { public: CTCAIScratchpad* tc; void GetTC(CTCAIScratchpad** out) { *out = tc; } };
class CScriptThing { public: CThing* thing; CThing* GetThing() const { return thing; } };
static bool GetKillSelfAndStealableItemAfterReachingDestination(CScriptThing thing)
{
    if (!thing.GetThing())
        return true;
    CTCAIScratchpad* tc;
    thing.GetThing()->GetTC(&tc);
    return tc->KillSelfAndStealableItemAfterReachingDestination;
}
int main()
{
    CTCAIScratchpad pad; pad.KillSelfAndStealableItemAfterReachingDestination = false;
    CThing t; t.tc = &pad;
    CScriptThing none; none.thing = 0;
    CScriptThing some; some.thing = &t;
    int ok = GetKillSelfAndStealableItemAfterReachingDestination(none) && !GetKillSelfAndStealableItemAfterReachingDestination(some);
    pad.KillSelfAndStealableItemAfterReachingDestination = true;
    ok = ok && GetKillSelfAndStealableItemAfterReachingDestination(some);
    if (!ok) { std::printf("008a60d0_TEST FAIL\n"); return 1; }
    std::printf("008a60d0_TEST PASS\n");
    return 0;
}