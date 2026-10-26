// Behaviour test for 0x0097E2D0 CAIStateGroupBase::SetProximityForDirectApproach: the float lands in the creature
// navigation component's (interface 7) +0x58 field.
#include <cstdio>
class CTCBase { public: int id; };
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
class CTCCreatureNavigation { public:
    unsigned char _pad0[0x58];
    float ProximityForDirectApproach;
    void SetProximityForDirectApproach(float proximity) { ProximityForDirectApproach = proximity; }
};
class CThingCreatureBase { public:
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
class CAIBrain { public:
    void* __vftable;
    unsigned char _pad04[0x14];
    CThingCreatureBase* Creature;
};
class CAIStateGroupBase {
public:
    void* __vftable;
    CAIBrain* PBrain;
    void SetProximityForDirectApproach(float proximity);
};
void CAIStateGroupBase::SetProximityForDirectApproach(float proximity)
{
    CTCCreatureNavigation* navigation;
    PBrain->Creature->GetInterface(&navigation, 7);
    navigation->SetProximityForDirectApproach(proximity);
}

int main()
{
    CTCCreatureNavigation nav, other;
    nav.ProximityForDirectApproach = 0.0f;
    other.ProximityForDirectApproach = 0.0f;
    CInterfaceMap::SEntry entries[3];
    entries[0].type = 2;  entries[0].component = (CTCBase*)&other;
    entries[1].type = 7;  entries[1].component = (CTCBase*)&nav;
    entries[2].type = 9;  entries[2].component = (CTCBase*)&other;
    CThingCreatureBase creature;
    for (int i = 0; i < 9; ++i) creature.m_presentBits[i] = 0;
    creature.m_presentBits[0] = (1UL << 2) | (1UL << 7) | (1UL << 9);
    creature.m_interfaces.first = entries;
    creature.m_interfaces.last = entries + 3;
    CAIBrain brain;
    brain.Creature = &creature;
    CAIStateGroupBase group;
    group.PBrain = &brain;
    group.SetProximityForDirectApproach(2.5f);
    if (nav.ProximityForDirectApproach != 2.5f || other.ProximityForDirectApproach != 0.0f) { std::printf("0097e2d0_TEST FAIL\n"); return 1; }
    group.SetProximityForDirectApproach(-0.25f);
    if (nav.ProximityForDirectApproach != -0.25f) { std::printf("0097e2d0_TEST FAIL\n"); return 1; }
    std::printf("0097e2d0_TEST PASS\n");
    return 0;
}