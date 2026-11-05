// CTCBuyableHouse::CountMoneyBags() const -- count the live things in the house's interface-0x21 thing list that carry the money-bag interface (0xCC)
// Leaf: ?CountMoneyBags@CTCBuyableHouse@@QBEJXZ
// Plan phase 2 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). By-value members re-padded to the header layout (re-padded by-value maps +8).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006C04D0, length 164 bytes (+ int3 padding).
// Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x0040F020 (inlined GetInterface for type 0x21 = bit 1 of
// CThing+0x24), CBaseIntelligentPointer::GetPItem() const @ 0x00A01B50 (three times per element: null test, the dead bit
// +0x91 & 1, the interface-0xCC bit = word 6 (+0x38) & 0x1000).
// Shape: the component-side twin of the landed thing-side CountMoneyBags (0x0082DD90): `this` is the CTCBuyableHouse
// component (ParentThing at +4); the component owns a list<CIntelligentPointer<CThing>> whose header node pointer is at +0x0C
// (end re-read every pass); the interface mask 0x1000 is hoisted into ebx and the count lives in the frame.
class CTCBase;
class CBaseObject;

struct CBaseIntelligentPointer
{
    virtual void v00();
    CBaseObject* m_pItem;                           // +0x04
    CBaseObject* GetPItem() const;                  // 0x00A01B50
};

class CThing;

template <class T>
class CIntelligentPointer : public CBaseIntelligentPointer
{ public:
    T* Get() const { return (T*)GetPItem(); }
};

#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

struct SThingNode {                                 // list<CIntelligentPointer<CThing>> node
    SThingNode* next;
    SThingNode* prev;
    CIntelligentPointer<CThing> thing;              // +0x08
};

struct CTCThingList_0x21 {                          // the interface-0x21 component
    unsigned char _pad_0x00[0xc];
    SThingNode*   m_head;                           // +0x0c list header node
};

inline bool IsThingAlive(const CIntelligentPointer<CThing>& p)
{
    return p.Get() && !p.Get()->IsDead();
}

class CTCBuyableHouse {
public:
    virtual void v00();
    CThing* ParentThing;                            // +0x04
    long CountMoneyBags() const;
};

long CTCBuyableHouse::CountMoneyBags() const
{
    long count = 0;
    CTCThingList_0x21* list;
    if (ParentThing->GetTC(TCI_THING_OWNER, &list)) {
        for (SThingNode* node = list->m_head->next; node != list->m_head; node = node->next)
            if (IsThingAlive(node->thing) && node->thing.Get()->HasTC(TCI_GOLD))
                ++count;
    }
    return count;
}