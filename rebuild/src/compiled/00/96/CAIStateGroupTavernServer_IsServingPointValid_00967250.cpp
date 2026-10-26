// CAIStateGroup_TavernServer::IsServingPointValid() -- is the reserved seat of the tavern table still valid
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00967250, length 108 bytes (+ int3 padding).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50, CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound
// @ 0x0040F020 (inlined GetInterface, type 0xB8 = bit 24 of the presence word at +0x34),
// CTCTavernTable::IsSeatValid(long) @ 0x007B8570 (manifest label "CTCInventoryBase::GetSelectedCategory" is
// propagated across a byte-identical body and rejected).
//
// Shape: the table thing comes from PTable (+0x24); its tavern-table component (interface 0xB8) is fetched with
// the landed out-parameter GetInterface template (the component lands in eax, the miss path reads the never-
// written frame slot that also held the key). The seat index (+0x94, SeatIndex) is checked on the component;
// an invalid seat is forgotten (-1). The false path returns the call's own al (no `xor al,al`): the result is a
// named bool local returned on both paths, the true path's `mov al,1` is the constant-folded `return ok`.
#include "engine/CAIStateGroup_TavernServer.h"

class CTCBase;

struct CInterfaceMap {                          // CVectorMap<ETCInterfaceType, CTCBase*> {first, last}
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};

class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
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

struct CBaseIntelligentPointer {
    void*   __vftable;                          // +0x00
    CThing* m_pItem;                            // +0x04
    CThing* GetPItem() const;                   // 0x00A01B50
};

class CTCTavernTable { public:                         // interface type 0xB8 component
    bool IsSeatValid(long seatIndex);           // 0x007B8570
};

struct CAIStateGroup_TavernServer_Methods : CAIStateGroup_TavernServer {
    CBaseIntelligentPointer& TablePtr() { return *(CBaseIntelligentPointer*)PTable; }
    bool IsServingPointValid();
};

bool CAIStateGroup_TavernServer_Methods::IsServingPointValid()
{
    CTCTavernTable* table;
    TablePtr().GetPItem()->GetInterface(&table, 0xB8);
    if (!table->IsSeatValid(SeatIndex)) {
        SeatIndex = -1;
        return false;
    }
    return true;
}