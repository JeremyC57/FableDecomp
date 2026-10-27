// behaviour test for 0x008A7430 CGameScriptInterface::GetAvoidRegionExits(CScriptThing) (model; the scratchpad lookup is a stub)
#include <cstdio>
class CTCAIScratchpad { public: bool AvoidRegionExits; };
class CThing { public: CTCAIScratchpad* tc; void GetTC(CTCAIScratchpad** out) { *out = tc; } };
class CScriptThing { public: CThing* thing; CThing* GetThing() const { return thing; } };
static bool GetAvoidRegionExits(CScriptThing thing)
{
    if (!thing.GetThing())
        return true;
    CTCAIScratchpad* tc;
    thing.GetThing()->GetTC(&tc);
    return tc->AvoidRegionExits;
}
int main()
{
    CTCAIScratchpad pad; pad.AvoidRegionExits = false;
    CThing t; t.tc = &pad;
    CScriptThing none; none.thing = 0;
    CScriptThing some; some.thing = &t;
    int ok = GetAvoidRegionExits(none) && !GetAvoidRegionExits(some);
    pad.AvoidRegionExits = true;
    ok = ok && GetAvoidRegionExits(some);
    if (!ok) { std::printf("008a7430_TEST FAIL\n"); return 1; }
    std::printf("008a7430_TEST PASS\n");
    return 0;
}