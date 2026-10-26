#include <cstdio>
class CTCBase { public: long m_frameCounter; };
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first; SEntry* last;
    SEntry* LowerBound(const long* type) { SEntry* p = first; while (p != last && p->type < *type) ++p; return p; }
};
class CThing { public:
    unsigned char m_thingType;
    unsigned long m_presentBits[9];
    CInterfaceMap m_interfaces;
    bool GetInterface(long type, CTCBase** out)
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31))) {
            long key = type;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&key);
            if (it == map->last || type < it->type)
                it = map->last;
            *out = it->component;
            return true;
        }
        return false;
    }
};
static long g_FramesPerSecond = 15;
class CThingCreatureBase { public: int id; };
class CCombatManager { public:
    CThingCreatureBase* m_last; int m_count;
    void DeregisterCombatant_006ea430(CThingCreatureBase* creature) { m_last = creature; ++m_count; }
};
class CTCCombat { public:
    CCombatManager* CombatManager; CThingCreatureBase* ParentCreature;
    void SetBlocked(CThing* blocker)
    {
        CTCBase* component;
        if (blocker->m_thingType == 2 && blocker->GetInterface(48, &component))
            component->m_frameCounter = (long)(g_FramesPerSecond * 2.0f);
        CombatManager->DeregisterCombatant_006ea430(ParentCreature);
    }
};
int main()
{
    CTCBase comp = { 0 }, endComp = { 0 };
    CInterfaceMap::SEntry entries[3] = { { 9, 0 }, { 48, &comp }, { 0, &endComp } };
    CThing thing; thing.m_thingType = 2;
    for (int i = 0; i < 9; ++i) thing.m_presentBits[i] = 0;
    thing.m_presentBits[1] = 0x10000;                      /* interface 48 present */
    thing.m_interfaces.first = entries; thing.m_interfaces.last = entries + 2;
    CThingCreatureBase me = { 1 };
    CCombatManager mgr = { 0, 0 };
    CTCCombat t; t.CombatManager = &mgr; t.ParentCreature = &me;
    t.SetBlocked(&thing);
    if (comp.m_frameCounter != 30 || mgr.m_last != &me || mgr.m_count != 1) { std::printf("00734050_TEST FAIL\n"); return 1; }
    comp.m_frameCounter = 0; thing.m_thingType = 1;
    t.SetBlocked(&thing);
    if (comp.m_frameCounter != 0 || mgr.m_count != 2) { std::printf("00734050_TEST FAIL\n"); return 1; }
    thing.m_thingType = 2; thing.m_presentBits[1] = 0;
    t.SetBlocked(&thing);
    if (comp.m_frameCounter != 0 || endComp.m_frameCounter != 0 || mgr.m_count != 3) { std::printf("00734050_TEST FAIL\n"); return 1; }
    std::printf("00734050_TEST PASS\n");
    return 0;
}