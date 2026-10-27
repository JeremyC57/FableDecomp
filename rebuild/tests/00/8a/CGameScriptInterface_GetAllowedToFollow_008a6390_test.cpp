// behaviour test for 0x008A6390 CGameScriptInterface::GetAllowedToFollow(CScriptThing) (model; the scratchpad lookup is a stub)
#include <cstdio>
class CTCAIScratchpad { public: bool AllowedToFollow; };
class CThing { public: CTCAIScratchpad* tc; void GetTC(CTCAIScratchpad** out) { *out = tc; } };
class CScriptThing { public: CThing* thing; CThing* GetThing() const { return thing; } };
static bool GetAllowedToFollow(CScriptThing thing)
{
    if (!thing.GetThing())
        return true;
    CTCAIScratchpad* tc;
    thing.GetThing()->GetTC(&tc);
    return tc->AllowedToFollow;
}
int main()
{
    CTCAIScratchpad pad; pad.AllowedToFollow = false;
    CThing t; t.tc = &pad;
    CScriptThing none; none.thing = 0;
    CScriptThing some; some.thing = &t;
    int ok = GetAllowedToFollow(none) && !GetAllowedToFollow(some);
    pad.AllowedToFollow = true;
    ok = ok && GetAllowedToFollow(some);
    if (!ok) { std::printf("008a6390_TEST FAIL\n"); return 1; }
    std::printf("008a6390_TEST PASS\n");
    return 0;
}