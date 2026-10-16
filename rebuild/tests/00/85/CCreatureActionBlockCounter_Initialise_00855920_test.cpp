// Behaviour test for 0x00855920 CCreatureAction_BlockCounter::Initialise.
#include <cstdio>
struct CTCCombat { int notified; int flourishCalls; bool flourish; void NotifyCounterStarted() { ++notified; } void SetMeleeFlourishValid(bool v) { ++flourishCalls; flourish = v; } };
struct CThing {
    unsigned char m_state, m_flags91; bool hasCombat; CTCCombat* combat;
    bool HasTC(long t) const { return t == 0x30 && hasCombat; }
    CTCCombat* FindEntryComponent(long) { return combat; }
    bool GetInterface(CTCCombat** pp, long t) { if (HasTC(t)) { *pp = combat; return true; } return false; }
};
struct CBaseIntelligentPointer { CThing* PItem; CThing* GetPItem() const { return PItem; } };
struct CCreatureAction_BlockCounter {
    CBaseIntelligentPointer Owner, Target; int baseInit;
    void Initialise()
    {
        CThing* owner = Owner.GetPItem();
        if (owner->HasTC(0x30)) owner->FindEntryComponent(0x30)->NotifyCounterStarted();
        CTCCombat* combat;
        if (Target.GetPItem() && !(Target.GetPItem()->m_flags91 & 1) && Target.GetPItem()->m_state == 2
            && Target.GetPItem()->GetInterface(&combat, 0x30))
            combat->SetMeleeFlourishValid(false);
        ++baseInit;
    }
};
int main()
{
    CTCCombat oc = {0, 0, true}, tc = {0, 0, true};
    CThing o = {0, 0, true, &oc}, t = {2, 0, true, &tc};
    CCreatureAction_BlockCounter a; a.Owner.PItem = &o; a.Target.PItem = &t; a.baseInit = 0;
    a.Initialise();
    bool ok = oc.notified == 1 && tc.flourishCalls == 1 && !tc.flourish && a.baseInit == 1;
    t.m_flags91 = 1; a.Initialise(); ok = ok && tc.flourishCalls == 1;
    t.m_flags91 = 0; t.m_state = 1; a.Initialise(); ok = ok && tc.flourishCalls == 1;
    a.Target.PItem = 0; o.hasCombat = false; a.Initialise(); ok = ok && oc.notified == 3 && a.baseInit == 4;
    if (!ok) { std::printf("00855920_TEST FAIL\n"); return 1; }
    std::printf("00855920_TEST PASS\n");
    return 0;
}