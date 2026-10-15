// Behaviour test for 0075d940 CTCCameraPointBuilding::GetOwnerBuilding: through the interface-51 component's owner pointer, only a building (flag 0x40) is handed out.
#include <cstdio>

struct CTCBase { long tag; };

struct CInterfaceMap
{
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

struct CThing
{
    unsigned long m_presentBits[9];
    CInterfaceMap m_interfaces;
    unsigned long ThingTypeFlags;

    template <class T> bool GetInterface(long type, T** out)
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31)))
        {
            long key = type;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&key);
            if (it == map->last || type < it->type)
                it = map->last;
            *out = (T*)it->component;
            return true;
        }
        return false;
    }
};

struct CBaseIntelligentPointer
{
    CThing* m_pItem;
    CThing* GetPItem() const { return m_pItem; }
};

template <class T>
struct CIntelligentPointer : CBaseIntelligentPointer
{
    T* Get() const { return (T*)GetPItem(); }
};

struct CTCBuildingOwned : CTCBase
{
    CIntelligentPointer<CThing> POwner;
};

struct CTCCameraPointBuilding
{
    CThing* ParentThing;
    bool GetOwnerBuilding(CThing** out) const
    {
        CTCBuildingOwned* owned;
        if (ParentThing->GetInterface(51, &owned))
        {
            CThing* building = owned->POwner.Get();
            if (building->ThingTypeFlags & 0x40)
            {
                *out = building;
                return true;
            }
        }
        return false;
    }
};

int main()
{
    CThing building; building.ThingTypeFlags = 0x40;
    CTCBuildingOwned owned; owned.tag = 51; owned.POwner.m_pItem = &building;
    CInterfaceMap::SEntry entries[2];
    entries[0].type = 51; entries[0].component = &owned;
    entries[1].type = 60; entries[1].component = 0;
    CThing thing; thing.ThingTypeFlags = 0;
    for (int i = 0; i < 9; ++i) thing.m_presentBits[i] = 0;
    thing.m_interfaces.first = entries; thing.m_interfaces.last = entries + 2;
    CTCCameraPointBuilding point; point.ParentThing = &thing;
    CThing* out = 0;
    if (point.GetOwnerBuilding(&out) || out != 0) { std::printf("0075d940_TEST FAIL\n"); return 1; }
    thing.m_presentBits[1] = 0x80000;               // interface 51 = word 1, bit 19
    if (!point.GetOwnerBuilding(&out) || out != &building) { std::printf("0075d940_TEST FAIL\n"); return 1; }
    building.ThingTypeFlags = 0; out = 0;
    if (point.GetOwnerBuilding(&out) || out != 0) { std::printf("0075d940_TEST FAIL\n"); return 1; }
    std::printf("0075d940_TEST PASS\n");
    return 0;
}