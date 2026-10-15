// Behaviour test for CTCTargetingBase::SetPScriptOverrideInterestingThing @ 0x007F5890: any thing but the parent itself is stored.
#include <cstdio>
struct CThing { int id; };
struct CIntelligentPointer {
    CThing* p; CIntelligentPointer() : p(0) {} CIntelligentPointer(CThing* t) : p(0) { SetPItem(t); }
    CThing* GetPItem() const { return p; } void SetPItem(CThing* t) { p = t; }
};
struct CTCTargetingBase {
    CThing* ParentThing; CIntelligentPointer PScriptOverrideInterestingThing;
    void SetPScriptOverrideInterestingThing(CThing* thing)
    { if (ParentThing != thing) { CIntelligentPointer ptr(thing); PScriptOverrideInterestingThing.SetPItem(ptr.GetPItem()); } }
};
int main()
{
    CThing self, other; CTCTargetingBase t; t.ParentThing = &self;
    t.SetPScriptOverrideInterestingThing(&other);
    if (t.PScriptOverrideInterestingThing.p != &other) { std::printf("007f5890_TEST FAIL\n"); return 1; }
    t.SetPScriptOverrideInterestingThing(&self);
    if (t.PScriptOverrideInterestingThing.p != &other) { std::printf("007f5890_TEST FAIL\n"); return 1; }
    t.SetPScriptOverrideInterestingThing(0);
    if (t.PScriptOverrideInterestingThing.p != 0) { std::printf("007f5890_TEST FAIL\n"); return 1; }
    std::printf("007f5890_TEST PASS\n");
    return 0;
}