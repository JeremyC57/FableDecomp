// NEntityEvents::CEventShownItem::~CEventShownItem() -- if the shown item is still alive and has its type-0x1a component, tell that component the showing has ended; then destroy both intelligent pointers
// Leaf: ??1CEventShownItem@@QAE@XZ
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: re-padded by-value maps +8.
// Retail: Fable.exe (TLC, VC7.1) @ 0x008116D0, 129 bytes (+ int3 padding).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (three times), the component map's LowerBound @ 0x0040F020
//   (inlined GetInterface, type 0x1a = bit 26 of the presence dword at +0x20), the component method @ 0x005D8E50 (manifest
//   "CChunkCollectionIndicator::OnPredicateFail" is propagated; named descriptively), CBaseIntelligentPointer::~CBaseIntelligentPointer
//   @ 0x00A01C10 (ItemShown, then ThingShowingItem). Vtables: own 0x012723D8, NEntityEvents::CEventBase 0x0124F2C4 (rtti-vtables.tsv).
// Layout (Ego_r.pdb CEventShownItem, 24 bytes): CEventBase vptr + one word, ThingShowingItem +0x08, ItemShown +0x10
//   (CIntelligentPointer<CThing>, 8 bytes: vptr + item).
// Shape: own vtable first; IS_THING_ALIVE (pointer set, dead bit +0x91 clear) then the forceinlined out-parameter
//   GetInterface (bit test, key temporary in the `push ecx` frame word, `it == end || 0x1a < it->type` falls back to end, result
//   used unchecked); members are destroyed in reverse order and the inlined CEventBase destructor stores its vtable last. A
//   non-virtual destructor in the polymorphic class keeps the `??_G` out of the TU.
class CTCBase;

struct CTCShowableItem {                         // the type-0x1a thing component
    void OnShowEnded();                          // 0x005D8E50
};

#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};

inline bool IS_THING_ALIVE(const CIntelligentPointer<CThing>& p)
{
    if (p.GetPItem() != 0) {
        if (!p.GetPItem()->IsDead())
            return true;
    }
    return false;
}

namespace NEntityEvents {
class CEventBase { public:                              // vtable 0x0124F2C4
    virtual void v00();
    unsigned long m_index;                       // +0x04
    ~CEventBase() {}
};
}

struct CEventShownItem : NEntityEvents::CEventBase {   // vtable 0x012723D8
    CIntelligentPointer<CThing> ThingShowingItem;    // +0x08
    CIntelligentPointer<CThing> ItemShown;           // +0x10
    virtual void v00();
    ~CEventShownItem();
};

CEventShownItem::~CEventShownItem()
{
    if (IS_THING_ALIVE(ItemShown)) {
        CTCShowableItem* item;
        if (ItemShown.GetPItem()->GetTC(TCI_INVENTORY_ITEM, &item))
            item->OnShowEnded();
    }
}