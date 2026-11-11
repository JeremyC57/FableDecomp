// CCreatureAction_ShowItemToThing::PerformAction() -- when both the thing being shown to and the item are alive (not being deleted) and the onlooker has an entity-events component (0x42), add a CEventShownItem(creature, item) to it
// Leaf: ?PerformAction@CCreatureAction_ShowItemToThing@@UAEXXZ
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: re-padded by-value maps +8.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00845270, length 224 bytes (plain `ret`, virtual).
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50, CVectorMap::LowerBound 0x0040F020 (inline GetInterface 0x42 =
// CTCEntityEvents, bit 2 of the present-bits byte +0x28), operator new 0x00BFEA1A (0x18 bytes),
// NEntityEvents::CEventShownItem::CEventShownItem(CThing*, CThing*) 0x0084AEE0 (vtable 0x012723D8 by RTTI; the manifest label
// "CEventDied" is a propagation), CTCEntityEvents::AddEvent(NEntityEvents::CEventBase*) 0x008113C0.
// Layout: owner PCreature +8, the onlooker PTarget +0xA8 (CCreatureActionBase), the shown item PItem +0xB0.
// Shape: the two liveness tests share the constant 1 in bl (`mov bl,1 ; test bl,cl`, then `test [eax+91h],bl`); the
// creature goes through the intelligent pointer's inline conversion (fetched before the item's plain GetPItem), the `new` null test selects 0 for AddEvent and the two
// AddEvent calls are not merged.
class CTCBase;
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CBaseObject;
struct CBaseIntelligentPointer {
    void*   __vftable;
    CThing* m_pItem;                            // +0x04
    CBaseObject* GetPItem() const;                   // 0x00A01B50
    operator CThing*() const { return ((CThing*)GetPItem()); }
};

namespace NEntityEvents {
class CEventBase { public: };
}
namespace NEntityEvents {
class CEventShownItem : public NEntityEvents::CEventBase { public:           // 0x18 bytes
    unsigned char _pad[0x18];
    CEventShownItem(CThing* shower, CThing* item);   // 0x0084AEE0
};
}
using NEntityEvents::CEventShownItem;
class CTCEntityEvents { public:
    void AddEvent(NEntityEvents::CEventBase* event);           // 0x008113C0
};

class CCreatureActionBase {
public:
    virtual void PerformAction();
    unsigned char           _pad04[4];
    CBaseIntelligentPointer PCreature;          // +0x08
    unsigned char           _pad10[0xa8 - 0x10];
    CBaseIntelligentPointer PTarget;            // +0xa8
};

class CCreatureAction_ShowItemToThing : public CCreatureActionBase {
public:
    CBaseIntelligentPointer PItem;              // +0xb0
    virtual void PerformAction();
};

void CCreatureAction_ShowItemToThing::PerformAction()
{
    if (PTarget.GetPItem() && !((CThing*)PTarget.GetPItem())->IsDead()
        && PItem.GetPItem() && !((CThing*)PItem.GetPItem())->IsDead()) {
        CTCEntityEvents* events;
        if (((CThing*)PTarget.GetPItem())->GetTC(TCI_ENTITY_EVENTS, &events)) {
            events->AddEvent(new CEventShownItem(PCreature, ((CThing*)PItem.GetPItem())));
        }
    }
}