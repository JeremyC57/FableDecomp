// Behaviour test for CTCTargetingBase::GetPNearestInterestingThing @ 0x007F5860: none when ignoring, else override, else nearest.
#include <cstdio>
class CThing { public: int id; };
struct CIntelligentPointer { CThing* p; CThing* Get() const { return p; } };
class CTCTargetingBase { public:
    bool ScriptOverrideIgnoreEverything; CIntelligentPointer PNearestInterestingThing, PScriptOverrideInterestingThing;
    CThing* GetPNearestInterestingThing()
    {
        if (ScriptOverrideIgnoreEverything) return 0;
        if (PScriptOverrideInterestingThing.Get()) return PScriptOverrideInterestingThing.Get();
        return PNearestInterestingThing.Get();
    }
};
int main()
{
    CThing a, b; CTCTargetingBase t; t.ScriptOverrideIgnoreEverything = false; t.PNearestInterestingThing.p = &a; t.PScriptOverrideInterestingThing.p = 0;
    if (t.GetPNearestInterestingThing() != &a) { std::printf("007f5860_TEST FAIL\n"); return 1; }
    t.PScriptOverrideInterestingThing.p = &b;
    if (t.GetPNearestInterestingThing() != &b) { std::printf("007f5860_TEST FAIL\n"); return 1; }
    t.ScriptOverrideIgnoreEverything = true;
    if (t.GetPNearestInterestingThing() != 0) { std::printf("007f5860_TEST FAIL\n"); return 1; }
    std::printf("007f5860_TEST PASS\n");
    return 0;
}