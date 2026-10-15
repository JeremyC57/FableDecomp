#include <cstdio>
// behaviour test for CThingBuilding::SetIsAvailableForResidency (0x0082E090): the inlined interface lookup over a sorted map
struct CTCBase { int dummy; };
struct CTCBuyableHouse : CTCBase { bool IsResidential; };
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
struct CThing {
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
struct CThingBuilding : CThing {
    void SetIsAvailableForResidency(bool available);
};
void CThingBuilding::SetIsAvailableForResidency(bool available)
{
    CTCBuyableHouse* house;
    if (GetInterface(&house, 0xd5))
        house->IsResidential = available;
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
    b.SetIsAvailableForResidency(true);
    if (house.IsResidential) { std::printf("0082e090_TEST FAIL\n"); return 1; }
    b.m_presentBits[0xd5 >> 5] |= 1UL << (0xd5 & 31);
    b.SetIsAvailableForResidency(true);
    if (!house.IsResidential) { std::printf("0082e090_TEST FAIL\n"); return 1; }
    b.SetIsAvailableForResidency(false);
    if (house.IsResidential) { std::printf("0082e090_TEST FAIL\n"); return 1; }
    std::printf("0082e090_TEST PASS\n");
    return 0;
}
