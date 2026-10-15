// CThingBuilding::SetIsAvailableForResidency(bool available) -- store the flag into the building's buyable-house component (when it has one)
// Retail: Fable.exe (TLC, VC7.1) @ 0x0082E090, 66 bytes (+ int3 padding).
// Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x0040F020 (the inlined interface lookup).
// Shape: the landed out-parameter GetInterface template (0x008386E0 / 0x007504E0): interface type 0xd5
//   (CTCBuyableHouse, as in the landed CGameScriptInterface::SetBuyableHouseAsScripted) = bit 21 of the bitset dword
//   at +0x38 (`test dword ptr [ecx+38h],200000h`), the map at +0x44 advanced once into esi, the component read from
//   the found entry; its IsResidential byte is at +0x49 (engine/CTCBuyableHouse.h).
#include "engine/CTCBuyableHouse.h"

struct CTCBase;

struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);      // 0x0040F020
};

struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
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
