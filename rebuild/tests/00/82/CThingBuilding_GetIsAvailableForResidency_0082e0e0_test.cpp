#include <cstdio>
// behaviour test for CThingBuilding::GetIsAvailableForResidency (0x0082E0E0): the inlined interface lookup over a sorted map
class CTCBase { public: int dummy; };
class CTCBuyableHouse : public CTCBase { public: bool IsResidential; };
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
class CThingBuilding : public CThing { public:
    bool GetIsAvailableForResidency();
};
bool CThingBuilding::GetIsAvailableForResidency()
{
    CTCBuyableHouse* house;
    if (GetInterface(&house, 0xd5))
        return house->IsResidential;
    return false;
}
int main()
{
    static CTCBuyableHouse house; static CTCBase other;
    static CInterfaceMap::SEntry entries[3];
    entries[0].type = 0x10; entries[0].component = &other;
    entries[1].type = 0xd5; entries[1].component = &house;
    entries[2].type = 0, entries[2].component = 0;
    static CThingBuilding b;
    for (int i = 0; i < 9; ++i) b.m_presentBits[i] = 0;
    b.m_interfaces.first = entries; b.m_interfaces.last = entries + 2;
    house.IsResidential = false;
    house.IsResidential = true;
    if (b.GetIsAvailableForResidency()) { std::printf("0082e0e0_TEST FAIL\n"); return 1; }
    b.m_presentBits[0xd5 >> 5] |= 1UL << (0xd5 & 31);
    if (!b.GetIsAvailableForResidency()) { std::printf("0082e0e0_TEST FAIL\n"); return 1; }
    house.IsResidential = false;
    if (b.GetIsAvailableForResidency()) { std::printf("0082e0e0_TEST FAIL\n"); return 1; }
    std::printf("0082e0e0_TEST PASS\n");
    return 0;
}