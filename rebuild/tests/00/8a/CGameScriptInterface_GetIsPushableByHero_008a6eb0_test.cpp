// behaviour test for 0x008A6EB0 CGameScriptInterface::GetIsPushableByHero(CScriptThing) (model; the scratchpad lookup is a stub)
#include <cstdio>
class CTCAIScratchpad { public: bool IsPushableByHero; };
class CThing { public: CTCAIScratchpad* tc; void GetTC(CTCAIScratchpad** out) { *out = tc; } };
class CScriptThing { public: CThing* thing; CThing* GetThing() const { return thing; } };
static bool GetIsPushableByHero(CScriptThing thing)
{
    if (!thing.GetThing())
        return true;
    CTCAIScratchpad* tc;
    thing.GetThing()->GetTC(&tc);
    return tc->IsPushableByHero;
}
int main()
{
    CTCAIScratchpad pad; pad.IsPushableByHero = false;
    CThing t; t.tc = &pad;
    CScriptThing none; none.thing = 0;
    CScriptThing some; some.thing = &t;
    int ok = GetIsPushableByHero(none) && !GetIsPushableByHero(some);
    pad.IsPushableByHero = true;
    ok = ok && GetIsPushableByHero(some);
    if (!ok) { std::printf("008a6eb0_TEST FAIL\n"); return 1; }
    std::printf("008a6eb0_TEST PASS\n");
    return 0;
}