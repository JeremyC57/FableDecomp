// behaviour test for 0x008A7170 CGameScriptInterface::GetLookForFiniteTime(CScriptThing) (model; the scratchpad lookup is a stub)
#include <cstdio>
class CTCAIScratchpad { public: bool LookForFiniteTime; };
class CThing { public: CTCAIScratchpad* tc; void GetTC(CTCAIScratchpad** out) { *out = tc; } };
class CScriptThing { public: CThing* thing; CThing* GetThing() const { return thing; } };
static bool GetLookForFiniteTime(CScriptThing thing)
{
    if (!thing.GetThing())
        return true;
    CTCAIScratchpad* tc;
    thing.GetThing()->GetTC(&tc);
    return tc->LookForFiniteTime;
}
int main()
{
    CTCAIScratchpad pad; pad.LookForFiniteTime = false;
    CThing t; t.tc = &pad;
    CScriptThing none; none.thing = 0;
    CScriptThing some; some.thing = &t;
    int ok = GetLookForFiniteTime(none) && !GetLookForFiniteTime(some);
    pad.LookForFiniteTime = true;
    ok = ok && GetLookForFiniteTime(some);
    if (!ok) { std::printf("008a7170_TEST FAIL\n"); return 1; }
    std::printf("008a7170_TEST PASS\n");
    return 0;
}