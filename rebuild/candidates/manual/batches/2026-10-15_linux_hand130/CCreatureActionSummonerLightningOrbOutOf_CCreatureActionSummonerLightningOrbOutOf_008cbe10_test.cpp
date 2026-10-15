// Behaviour test for 0x008CBE10 CCreatureAction_SummonerLightningOrbOutOf(owner, orb): PTarget set only for a live orb.
#include <cstdio>
struct CThing { unsigned char m_flags91; };
struct CSoundPair { int m_first, m_second; CSoundPair() : m_first(0), m_second(0) { } };
struct CBaseIntelligentPointer {
    CThing* m_pItem;
    CBaseIntelligentPointer() : m_pItem(0) { }
    CThing* GetPItem() const { return m_pItem; }
    void SetPItem(CThing* p) { m_pItem = p; }
};
struct CIntelligentPointer : CBaseIntelligentPointer { CIntelligentPointer(CThing* item) { SetPItem(item); } };
struct CCombatActionBase {
    CThing* m_owner; int m_code; CBaseIntelligentPointer PTarget;
    CCombatActionBase(CThing* owner, int code, const CSoundPair&) : m_owner(owner), m_code(code) { }
};
struct CAction : CCombatActionBase {
    CAction(CThing* owner, CThing* orb) : CCombatActionBase(owner, 150, CSoundPair())
    {
        if (orb != 0 && !(orb->m_flags91 & 1)) {
            CIntelligentPointer target(orb);
            PTarget.SetPItem(target.GetPItem());
        }
    }
};
int main()
{
    CThing owner, orb; orb.m_flags91 = 0;
    CAction a(&owner, &orb);
    CAction b(&owner, 0);
    orb.m_flags91 = 1; CAction c(&owner, &orb);
    if (a.PTarget.GetPItem() != &orb || b.PTarget.GetPItem() != 0 || c.PTarget.GetPItem() != 0 || a.m_code != 150 || a.m_owner != &owner)
    { std::printf("008cbe10_TEST FAIL\n"); return 1; }
    std::printf("008cbe10_TEST PASS\n");
    return 0;
}
