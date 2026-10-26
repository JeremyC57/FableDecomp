// Behaviour test for 0x008CD730 CCreatureAction_CastTimeSpell::Initialise: add the CTCBulletTime component, set its target when alive.
#include <cstdio>
#include <cstring>
class CCharString { public: const char* t; CCharString(const char* s, long = -1) : t(s) {} };
class CThing;
struct CBaseIntelligentPointer {
    CThing* PItem;
    CBaseIntelligentPointer() : PItem(0) {}
    CThing* GetPItem() const { return PItem; }
    void SetPItem(CThing* i) { PItem = i; }
};
struct CIntelligentPointerThing : CBaseIntelligentPointer {
    CIntelligentPointerThing() {}
    CIntelligentPointerThing(const CIntelligentPointerThing& o) : CBaseIntelligentPointer() { SetPItem(o.GetPItem()); }
    CIntelligentPointerThing& operator=(CIntelligentPointerThing o) { SetPItem(o.GetPItem()); return *this; }
};
struct TC {
    CIntelligentPointerThing PTarget; long AICastLevel; int healing;
    TC() : AICastLevel(-1), healing(0) {}
    void SetAICast(long l) { AICastLevel = l; }
    void StartHealingEffect() { ++healing; }
};
class CThing { public:
    unsigned char m_state, m_flags91; bool has; TC tc; const char* added;
    CThing() : m_state(0), m_flags91(0), has(false), added(0) {}
    unsigned long HasTC(long) const { return has; }
    TC* AddTC(const CCharString& n, bool, void*) { added = n.t; has = true; return &tc; }
};
class CCombatActionBase { public:
    CBaseIntelligentPointer PCreature; CIntelligentPointerThing PItemRemoved; int baseInit;
    void Initialise() { ++baseInit; }
};
struct CAction : CCombatActionBase { long Level; void Run() {
    CCombatActionBase::Initialise();
    if (!PCreature.GetPItem()->HasTC(0x92)) {
        TC* tc = PCreature.GetPItem()->AddTC("CTCBulletTime", true, 0);
        CIntelligentPointerThing target = PItemRemoved;
        if (target.GetPItem() && !(target.GetPItem()->m_flags91 & 1))
            tc->PTarget = target;
    }
} };
int main()
{
    CThing owner, target; owner.m_state = 1;
    CAction a; a.PCreature.PItem = &owner; a.PItemRemoved.PItem = &target; a.Level = 3; a.baseInit = 0;
    a.Run();
    bool ok = owner.added && !std::strcmp(owner.added, "CTCBulletTime") && owner.tc.PTarget.GetPItem() == &target && a.baseInit == 1;
    CThing owner2, dead; dead.m_flags91 = 1;
    CAction b; b.PCreature.PItem = &owner2; b.PItemRemoved.PItem = &dead; b.Level = 2; b.baseInit = 0;
    b.Run();
    ok = ok && owner2.tc.PTarget.GetPItem() == 0 && owner2.tc.AICastLevel == -1;
    if (!ok) { std::printf("008cd730_TEST FAIL\n"); return 1; }
    std::printf("008cd730_TEST PASS\n");
    return 0;
}