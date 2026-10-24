// Behaviour test for 0x009215E0 CAIStateGroup_MinionFollowThing::SetThingToNotAvoid: forwards the thing to the
// creature's navigation component when the creature has one (interface 7), otherwise does nothing.
#include <cstdio>

struct CTCBase { };
struct CThing;

struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type)
    {
        SEntry* it = first;
        while (it != last && it->type < *type)
            ++it;
        return it;
    }
};

struct CThing {
    unsigned int m_presentBits[9];
    CInterfaceMap m_interfaces;
    template<class T> bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1U << (interfaceType & 31))) {
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

struct CTCCreatureNavigation : CTCBase {
    const CThing* ignored;
    int calls;
    void SetThingToIgnore(const CThing* thing) { ignored = thing; ++calls; }
};

struct CAIStateGroup_MinionFollowThing {
    CAIBrain* PBrain;
    void SetThingToNotAvoid(const CThing* thing);
};

void CAIStateGroup_MinionFollowThing::SetThingToNotAvoid(const CThing* thing)
{
    CTCCreatureNavigation* navigation;
    if (PBrain->PCreature->GetInterface(&navigation, 7))
        navigation->SetThingToIgnore(thing);
}

int main()
{
    int ok = 1;
    CTCCreatureNavigation nav;
    nav.ignored = 0;
    nav.calls = 0;
    CInterfaceMap::SEntry entries[2] = { { 3, 0 }, { 7, &nav } };
    CThing creature;
    for (int i = 0; i < 9; ++i) creature.m_presentBits[i] = 0;
    creature.m_interfaces.first = entries;
    creature.m_interfaces.last = entries + 2;
    CThing other;
    CAIBrain brain;
    brain.PCreature = &creature;
    CAIStateGroup_MinionFollowThing group;
    group.PBrain = &brain;
    group.SetThingToNotAvoid(&other);
    if (nav.calls != 0) ok = 0;                                  // no navigation component
    creature.m_presentBits[0] |= 1U << 7;
    group.SetThingToNotAvoid(&other);
    if (nav.calls != 1 || nav.ignored != &other) ok = 0;
    group.SetThingToNotAvoid(0);
    if (nav.calls != 2 || nav.ignored != 0) ok = 0;
    if (!ok) { std::printf("009215e0_TEST FAIL\n"); return 1; }
    std::printf("009215e0_TEST PASS\n");
    return 0;
}