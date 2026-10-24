// Behaviour test for 0x006989D0 NCameraModes::GetPBuildingThingOccupying: a thing without component 0x75 gives 0;
// with it, the pointed-at thing comes back only when it exists, is not dead and is a building (type 3).
#include <cstdio>
#ifndef _MSC_VER
#define __fastcall
#define __forceinline inline
#endif

class CThing;
class CThingBuilding;
struct CTCBase { CThing* ParentThing; };
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    const SEntry* LowerBound(const long* type) const
    {
        const SEntry* it = first;
        while (it != last && it->type < *type) ++it;
        return it;
    }
};
class CBaseIntelligentPointer {
public:
    CThing* Item;
    CThing* GetPItem() const { return Item; }
};
struct CTCBuildingOccupant : CTCBase { CBaseIntelligentPointer Building; };

class CThing {
public:
    unsigned char ThingType;
    unsigned int  m_presentBits[9];
    CInterfaceMap m_interfaces;
    unsigned char Dead : 1;
    bool HasTC(long type) const
    {
        if (m_presentBits[type >> 5] & (1u << (type & 31)))
            return true;
        return false;
    }
    const CTCBase* GetTC(long type) const
    {
        long key = type;
        const CInterfaceMap* map = &m_interfaces;
        const CInterfaceMap::SEntry* it = map->LowerBound(&key);
        const CInterfaceMap::SEntry* end = map->last;
        return (it == end || type < it->type ? end : it)->component;
    }
};

namespace NCameraModes {
const CThingBuilding* __fastcall GetPBuildingThingOccupying(const CThing& thing)
{
    if (thing.HasTC(0x75)) {
        const CThing* building = ((const CTCBuildingOccupant*)thing.GetTC(0x75))->Building.GetPItem();
        if (building && !building->Dead && building->ThingType == 3)
            return (const CThingBuilding*)building;
    }
    return 0;
}
}

static void Init(CThing& t, unsigned char type)
{
    t.ThingType = type;
    for (int i = 0; i < 9; ++i) t.m_presentBits[i] = 0;
    t.m_interfaces.first = t.m_interfaces.last = 0;
    t.Dead = 0;
}

int main()
{
    CThing hero, house, cart;
    Init(hero, 2); Init(house, 3); Init(cart, 5);
    if (NCameraModes::GetPBuildingThingOccupying(hero) != 0) { std::printf("006989d0_TEST FAIL\n"); return 1; }

    CTCBuildingOccupant occupant;
    occupant.ParentThing = &hero;
    occupant.Building.Item = &house;
    CInterfaceMap::SEntry entries[3] = { { 0x23, 0 }, { 0x75, &occupant }, { 0x90, 0 } };
    hero.m_interfaces.first = entries;
    hero.m_interfaces.last = entries + 3;
    hero.m_presentBits[0x75 >> 5] |= 1u << (0x75 & 31);
    if (NCameraModes::GetPBuildingThingOccupying(hero) != (const CThingBuilding*)&house) { std::printf("006989d0_TEST FAIL\n"); return 1; }

    house.Dead = 1;
    if (NCameraModes::GetPBuildingThingOccupying(hero) != 0) { std::printf("006989d0_TEST FAIL\n"); return 1; }
    house.Dead = 0;
    occupant.Building.Item = &cart;
    if (NCameraModes::GetPBuildingThingOccupying(hero) != 0) { std::printf("006989d0_TEST FAIL\n"); return 1; }
    occupant.Building.Item = 0;
    if (NCameraModes::GetPBuildingThingOccupying(hero) != 0) { std::printf("006989d0_TEST FAIL\n"); return 1; }
    std::printf("006989d0_TEST PASS\n");
    return 0;
}