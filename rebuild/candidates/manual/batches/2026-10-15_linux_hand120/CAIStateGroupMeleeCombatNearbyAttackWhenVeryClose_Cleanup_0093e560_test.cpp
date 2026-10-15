// Behaviour test for CAIStateGroup_MeleeCombatNearbyAttackWhenVeryClose::Cleanup (0x0093E560): a registered
// combatant is deregistered from its own combat manager; creature mode 0x19 is always removed.
#include <cstdio>

struct CTCBase { int dummy; };

// Sorted {type, component} vector map searched by LowerBound (0x0040F020 in retail).
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type)
    {
        SEntry* p = first;
        while (p != last && p->type < *type) ++p;
        return p;
    }
};

struct CThing {
    unsigned long m_presentBits[9];
    CInterfaceMap m_interfaces;
    unsigned char m_flags91;
    CThing() : m_flags91(0)
    {
        for (int i = 0; i < 9; ++i) m_presentBits[i] = 0;
        m_interfaces.first = m_interfaces.last = 0;
    }
    void SetBit(long t) { m_presentBits[t >> 5] |= 1UL << (t & 31); }
    template<class T> bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};

struct CAIBrain { CThing* PCreature; };

struct CCombatManager {
    CThing* m_removed;
    void DeregisterCombatant(CThing* c) { m_removed = c; }
};
static CCombatManager g_combat;
static bool g_registered = false;
struct CTCCreatureModeManager : CTCBase {
    unsigned long m_removedMode;
    void RemoveMode(unsigned long mode) { m_removedMode = mode; }
};
struct CCreature : CThing {
    bool IsRegisteredCombatant() { return g_registered; }
    CCombatManager* GetCombatManager() { return &g_combat; }
};
struct CBrain { CCreature* PCreature; };
struct CAIStateGroup_MeleeCombatNearbyAttackWhenVeryClose {
    CBrain* PBrain;
    void Cleanup()
    {
        if (PBrain->PCreature->IsRegisteredCombatant())
            PBrain->PCreature->GetCombatManager()->DeregisterCombatant(PBrain->PCreature);
        CTCCreatureModeManager* modes;
        PBrain->PCreature->GetInterface(&modes, 0x31);
        modes->RemoveMode(0x19);
    }
};

int main()
{
    CTCCreatureModeManager mgr; mgr.m_removedMode = 0;
    CInterfaceMap::SEntry e[1]; e[0].type = 0x31; e[0].component = &mgr;
    CCreature creature; creature.m_interfaces.first = e; creature.m_interfaces.last = e + 1; creature.SetBit(0x31);
    CBrain brain; brain.PCreature = &creature;
    CAIStateGroup_MeleeCombatNearbyAttackWhenVeryClose g; g.PBrain = &brain;
    g_combat.m_removed = 0;
    g.Cleanup();
    bool ok = g_combat.m_removed == 0 && mgr.m_removedMode == 0x19;
    mgr.m_removedMode = 0; g_registered = true; g.Cleanup();
    ok = ok && g_combat.m_removed == &creature && mgr.m_removedMode == 0x19;
    if (!ok) { std::printf("0093e560_TEST FAIL\n"); return 1; }
    std::printf("0093e560_TEST PASS\n");
    return 0;
}
