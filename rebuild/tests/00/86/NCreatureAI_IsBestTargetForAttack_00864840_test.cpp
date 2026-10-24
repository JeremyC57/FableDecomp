// Behaviour test for 0x00864840: IsBestTargetForAttack needs a live target, a targeting AI (type 8) on the creature,
// and that AI's best enemy to be the target.
#include <cstdio>

class CTCBase;
class CThingCreatureBase;

struct CInterfaceMap
{
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long& type) const
    {
        SEntry* it = first;
        while (it != last && it->type < type) ++it;
        return it;
    }
};

class CTCTargetingAI
{
public:
    CThingCreatureBase* m_best;
    CTCTargetingAI() : m_best(0) { }
    virtual ~CTCTargetingAI() { }
    virtual CThingCreatureBase* GetPBestEnemyCreatureForAttack() const { return m_best; }
};

class CThing
{
public:
    unsigned char _pad0[0x20];
    unsigned int  m_presentBits[9];
    CInterfaceMap m_interfaces;
    unsigned char Dead : 1;
    unsigned char CreatedByOwnerThingCreation : 1;

    bool HasTC(long type) const
    {
        if (m_presentBits[type >> 5] & (1U << (type & 31)))
            return true;
        return false;
    }
    CInterfaceMap::SEntry* FindEntry(long type) const
    {
        long key = type;
        const CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(key);
        CInterfaceMap::SEntry* end = map->last;
        CInterfaceMap::SEntry* sel;
        if (it == end)
            sel = end;
        else if (it->type <= type)
            sel = it;
        else
            sel = end;
        return sel;
    }
};

class CThingCreatureBase : public CThing { };
class CThingAICreature : public CThingCreatureBase { };

bool IsBestTargetForAttack(const CThingAICreature& creature, const CThingCreatureBase* target)
{
    if (target && !target->Dead)
    {
        if (creature.HasTC(8) && ((const CTCTargetingAI*)creature.FindEntry(8)->component)->GetPBestEnemyCreatureForAttack() == target)
            return true;
    }
    return false;
}

static void Clear(CThing& t)
{
    for (int i = 0; i < 9; ++i) t.m_presentBits[i] = 0;
    t.m_interfaces.first = 0;
    t.m_interfaces.last = 0;
    t.Dead = 0;
    t.CreatedByOwnerThingCreation = 0;
}

int main()
{
    CTCTargetingAI ai;
    CInterfaceMap::SEntry entries[1];
    entries[0].type = 8; entries[0].component = (CTCBase*)&ai;
    CThingAICreature creature; Clear(creature);
    creature.m_interfaces.first = entries; creature.m_interfaces.last = entries + 1;
    CThingCreatureBase target; Clear(target);
    CThingCreatureBase other; Clear(other);
    ai.m_best = &target;
    int fails = 0;
    if (IsBestTargetForAttack(creature, &target)) ++fails;      // no type-8 bit
    creature.m_presentBits[0] = 0x100;
    if (!IsBestTargetForAttack(creature, &target)) ++fails;
    if (IsBestTargetForAttack(creature, &other)) ++fails;        // not the best enemy
    if (IsBestTargetForAttack(creature, 0)) ++fails;             // null target
    target.Dead = 1;
    if (IsBestTargetForAttack(creature, &target)) ++fails;      // dead target
    if (fails) { std::printf("00864840_TEST FAIL\n"); return 1; }
    std::printf("00864840_TEST PASS\n");
    return 0;
}