#include <cstdio>
// Behaviour test for CAIStateGroup_HoverHornetQueen::KillBossHelpers (0x0095AA00).
class CTCBase { public: int killed; };
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type)
    {
        SEntry* it = first;
        while (it != last && it->type < *type) ++it;
        return it;
    }
};
class CThing { public:
    unsigned long m_presentBits[9];
    CInterfaceMap m_interfaces;
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
class CThingAICreature : public CThing { public:};
class CTCBoss : public CTCBase { public: void KillHelpers() { ++killed; } };
class CAIBrain { public: CThingAICreature* Creature; };
class CAIStateGroupBase { public: CAIBrain* PBrain; };
struct CAIStateGroup_HoverHornetQueen : CAIStateGroupBase { void KillBossHelpers(); };
void CAIStateGroup_HoverHornetQueen::KillBossHelpers()
{
    CTCBoss* boss;
    PBrain->Creature->GetInterface(&boss, 0xAD);
    boss->KillHelpers();
}
int main()
{
    CTCBoss other; other.killed = 0; CTCBoss boss; boss.killed = 0;
    CInterfaceMap::SEntry entries[3] = { { 0x10, &other }, { 0xAD, &boss }, { 0xB8, &other } };
    CThingAICreature creature;
    for (int i = 0; i < 9; ++i) creature.m_presentBits[i] = 0;
    creature.m_presentBits[0xAD >> 5] |= 1UL << (0xAD & 31);
    creature.m_interfaces.first = entries; creature.m_interfaces.last = entries + 3;
    CAIBrain brain; brain.Creature = &creature;
    CAIStateGroup_HoverHornetQueen g; g.PBrain = &brain;
    g.KillBossHelpers();
    g.KillBossHelpers();
    if (boss.killed != 2 || other.killed != 0) { std::printf("0095aa00_TEST FAIL\n"); return 1; }
    std::printf("0095aa00_TEST PASS\n");
    return 0;
}