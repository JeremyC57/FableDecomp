// CCreatureAction_Talk::PerformAction() -- when the talk target is an AI creature (thing class byte 1) with an entity-events component (0x42), add a CEventTalkedTo(owner) to it and clear/set its has-information flags
// Leaf: ?PerformAction@CCreatureAction_Talk@@UAEXXZ
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end().
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00846660, length 165 bytes (+ int3 padding), plain `ret` (virtual).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50, CVectorMap::LowerBound @ 0x0040F020 (inlined GetInterface 0x42 =
// CTCEntityEvents, bit 2 of +0x28), operator new @ 0x00BFEA1A (0x10 bytes), NEntityEvents::CEventTalkedTo::CEventTalkedTo(CThing*)
// @ 0x0084AF90 (vtable 0x01274228; manifest "CEventFiredShot" is a propagation), CTCEntityEvents::AddEvent @ 0x008113C0,
// CThingAICreature::SetHasInformation(bool, bool, bool, bool) @ 0x00830EB0.
// Shape: the target is tested for presence and its class byte (+0x10 == 1), then fetched into a creature local whose own
// null test precedes the checked out-parameter lookup; the `new` null test selects 0 for AddEvent.
// Layout: owner CIntelligentPointer +8, target +0xa8.
class CTCBase;
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CThingAICreature : public CThing { public:
    void SetHasInformation(bool a, bool b, bool c, bool d);   // 0x00830EB0
};

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

struct CEventBase { };
struct CEventTalkedTo : CEventBase {
    unsigned char _pad[0x10];
    CEventTalkedTo(CThing* talker);             // 0x0084AF90
};
class CTCEntityEvents { public:
    void AddEvent(CEventBase* event);           // 0x008113C0
};

class CCreatureActionBase {
public:
    virtual void PerformAction();
    unsigned char           _pad04[4];
    CIntelligentPointer<CThing> PCreature;          // +0x08
    unsigned char           _pad10[0xa8 - 0x10];
};

class CCreatureAction_Talk : public CCreatureActionBase {
public:
    CIntelligentPointer<CThing> PTarget;            // +0xa8
    virtual void PerformAction();
};

void CCreatureAction_Talk::PerformAction()
{
    if (PTarget.GetPItem() && PTarget.GetPItem()->GetType() == 1) {
        CThingAICreature* creature = (CThingAICreature*)PTarget.GetPItem();
        CTCEntityEvents* events;
        if (creature && creature->GetTC(TCI_ENTITY_EVENTS, &events)) {
            events->AddEvent(new CEventTalkedTo(PCreature.GetPItem()));
            creature->SetHasInformation(false, false, true, false);
        }
    }
}