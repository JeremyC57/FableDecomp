// CTCCarrying::IsCarryingThing(long defIndex) -- true when some carry slot holds a thing whose def index (+0x8c) matches
// Leaf: ?IsCarryingThing@CTCCarrying@@QAE_NJ@Z
//
// Retail 0x006A10A0, 124 bytes (`ret 4`). Callee: CBaseIntelligentPointer::GetPItem @ 0x00A01B50
// (twice per slot). This is the def-index overload; the CThing* overload is the landed 0x006A1060.
// Layout: CarrySlots at +0xc as in the landed 0x006A1060 / OnKill 0x006A0FF0 models; the thing's
// def index is the unsigned short at +0x8c (`movzx`).
// Shape: the OnKill 0x006A0FF0 index loop (`i < size()` recomputed, `*(begin() + i)` element,
// pointer address in ebp) with an early `return true` and the shared `xor al,al` after the loop.
#include "engine/CThing.h"

class CBaseObject;
class CBaseObjectPointer;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"


struct SCarrySlotEntry {                            // pair<long, CIntelligentPointer<CThing>>, 12 bytes
    long                    first;
    CIntelligentPointer<CThing> second;
};

struct CCarrySlotMap {                              // retail CVectorMap<long,CIntelligentPointer<CThing>>
    SCarrySlotEntry* m_pFirst;
    SCarrySlotEntry* m_pLast;
    SCarrySlotEntry* m_pEnd;
    bool             m_compare;
    bool             m_dirty;
    SCarrySlotEntry* begin() const { return m_pFirst; }
    unsigned long size() const { return m_pLast - m_pFirst; }
    SCarrySlotEntry& operator[](unsigned long n) const { return *(begin() + n); }
};

class CTCCarrying { public:
    virtual void v00();
    unsigned char _pad_0x04[0x0c - 0x04];           // +0x04 .. +0x0c: CTCBase state
    CCarrySlotMap CarrySlots;                       // +0x0c
    bool IsCarryingThing(long defIndex);
};

bool CTCCarrying::IsCarryingThing(long defIndex)
{
    for (unsigned long i = 0; i < CarrySlots.size(); ++i)
    {
        CIntelligentPointer<CThing>& carried = CarrySlots[i].second;
        if (carried.Get() && carried.Get()->GetDefGlobalIndex() == defIndex)
            return true;
    }
    return false;
}