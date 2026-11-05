// CTCSpecialAbilities::FrameUpdate() -- cancel every ability while the thing's interface-0x29 component reports its flag at +0x0d clear
// Leaf: FrameUpdate
// Plan phase 2 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end().
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0072C370, length 74 bytes; vtable 0x0126683C (CTCSpecialAbilities) slot 9,
// the CTCBase FrameUpdate slot.
// Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x0040F020, CTCSpecialAbilities::CancelAllAbilities
// @ 0x0072BA40.
// Layout: the parent thing's interface bitset at +0x20 (type 0x29 -> word 1 = +0x24, bit 9: `test ch,2`) and its sorted
// interface map at +0x44 (the landed GetInterface shape of 0x007504E0); the component's flag byte is at +0x0d.
// Shape: inline out-parameter GetInterface (map pointer in esi), then the flag test.
class CTCBase;

#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

struct CTCInterface29 {                                     // the interface-0x29 component (only its flag is read)
    unsigned char _pad[0x0d];
    bool          m_flag0d;                                 // +0x0d
};

class CTCSpecialAbilities { public:
    void*   __vftable;                                      // +0x00
    CThing* ParentThing;                                    // +0x04
    void CancelAllAbilities();                              // 0x0072BA40
    void FrameUpdate();
};

void CTCSpecialAbilities::FrameUpdate()
{
    CTCInterface29* component;
    if (ParentThing->GetTC(TCI_HERO, &component)) {
        if (!component->m_flag0d)
            CancelAllAbilities();
    }
}