// Behaviour test for 0x0097E270 CAIStateGroupBase::SetThingToNotAvoid: the creature's navigation component (interface 7,
// found through the sorted interface map) receives the thing.
#include <cstdio>
struct CTCBase { int id; };
class CThing { public: int id; };

struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);
};
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{
    SEntry* it = first;
    while (it != last && it->type < *type) ++it;
    return it;
}
static const void* g_navThis = 0;
static const CThing* g_navThing = 0;
struct CTCCreatureNavigation {
    CTCBase base;
    void SetThingToIgnore(const CThing* thing);
};
void CTCCreatureNavigation::SetThingToIgnore(const CThing* thing) { g_navThis = this; g_navThing = thing; }
struct CThingCreatureBase {
    unsigned char _pad0[0x20];
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
struct CAIBrain {
    void* __vftable;
    unsigned char _pad04[0x14];
    CThingCreatureBase* Creature;
};
class CAIStateGroupBase {
public:
    void* __vftable;
    CAIBrain* PBrain;
    void SetThingToNotAvoid(const CThing* thing);
};
void CAIStateGroupBase::SetThingToNotAvoid(const CThing* thing)
{
    CTCCreatureNavigation* navigation;
    PBrain->Creature->GetInterface(&navigation, 7);
    navigation->SetThingToIgnore(thing);
}

int main()
{
    CTCCreatureNavigation nav;
    CTCCreatureNavigation other;
    CInterfaceMap::SEntry entries[3];
    entries[0].type = 3;  entries[0].component = &other.base;
    entries[1].type = 7;  entries[1].component = &nav.base;
    entries[2].type = 0x31; entries[2].component = &other.base;
    CThingCreatureBase creature;
    for (int i = 0; i < 9; ++i) creature.m_presentBits[i] = 0;
    creature.m_presentBits[0] = (1UL << 3) | (1UL << 7);
    creature.m_presentBits[1] = 1UL << (0x31 - 32);
    creature.m_interfaces.first = entries;
    creature.m_interfaces.last = entries + 3;
    CAIBrain brain;
    brain.Creature = &creature;
    CAIStateGroupBase group;
    group.PBrain = &brain;
    CThing target;
    group.SetThingToNotAvoid(&target);
    if (g_navThis != &nav || g_navThing != &target) { std::printf("0097e270_TEST FAIL\n"); return 1; }
    group.SetThingToNotAvoid(0);
    if (g_navThis != &nav || g_navThing != 0) { std::printf("0097e270_TEST FAIL\n"); return 1; }
    std::printf("0097e270_TEST PASS\n");
    return 0;
}