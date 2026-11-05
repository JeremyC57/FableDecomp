// CTCCarrying::UpdateCarriedObjects() -- per carried thing, update its CTCCarryable component (attachment) without forcing
// Leaf: ?UpdateCarriedObjects@CTCCarrying@@QAEXXZ
// Plan phase 2 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end().
//
// Retail 0x006A1D60, 158 bytes (plain `ret`). Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50,
// CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x0040F020 (this module's copy),
// CTCCarryable::UpdateCarriedPosition(bool) @ 0x00705B60 (passed false).
// Layout: CarrySlots at +0xc as in the landed 0x006A1060 / OnKill 0x006A0FF0 models.
// Shape: the OnKill index loop; the CTCCarryable out pointer (interface 0x47: bitset word 2 bit 7,
// `mov cl,[eax+28h] ; test cl,cl ; jns`) is declared OUTSIDE the loop and never initialised, so a
// thing without the interface reuses the previous value (edi, first loaded from the frame word
// that also homes the lookup key).
class CTCBase;

#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CTCCarryable { public:
    void UpdateCarriedPosition(bool force);                      // 0x00705B60
};

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
    void UpdateCarriedObjects();
};

void CTCCarrying::UpdateCarriedObjects()
{
    CTCCarryable* carryable;
    for (unsigned long i = 0; i < CarrySlots.size(); ++i)
    {
        CThing* thing = CarrySlots[i].second.Get();
        if (thing)
        {
            thing->GetTC(TCI_CARRYABLE, &carryable);          // TC_CARRYABLE
            carryable->UpdateCarriedPosition(false);
        }
    }
}