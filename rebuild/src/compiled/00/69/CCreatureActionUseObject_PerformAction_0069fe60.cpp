// CCreatureAction_UseObject::PerformAction() -- re-aim the move-to position at the creature, queue a CEventItemUsed(creature) on the used thing's entity events, then hand the use to the thing's scripted hook (CTCActionUseScriptedHook::OnActionUse) when it has one, else to its CTCActionUse component's virtual slot 28
// Leaf: ?PerformAction@CCreatureAction_UseObject@@QAEXXZ
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Plan phase 2 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end().
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0069FE60, length 366 bytes (+ int3 padding).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PCreature +8, PUsedThing +0xa8), CThing::GetPos @ 0x004C73D0,
// CCreatureActionBase::SetMoveToPos(const C3DVector&) @ 0x006929E0, CVectorMap::LowerBound @ 0x0040F020 (inlined lookups: 0x42
// CTCEntityEvents, bit 2 of +0x28, tested; 0xC2 CTCActionUseScriptedHook, bit 2 of +0x38, tested; 0xD CTCActionUse, bit 13 of +0x20,
// untested), operator new @ 0x00BFEA1A (0x10-byte event), CBaseIntelligentPointer ctor @ 0x00A01B10 / SetPItem @ 0x00A01B90 (the
// event's CIntelligentPointer<CThing>, vtable 0x01238C6C), CTCEntityEvents::AddEvent @ 0x008113C0, CTCActionUseScriptedHook::
// OnActionUse(CThing*) @ 0x00729B20, the CTCActionUse virtual slot 28 (+0x70, one CThing*). NEntityEvents::CEventItemUsed vtable
// 0x0125C820 (RTTI).
// Layout: retail CCreatureActionBase is 0xa8 bytes: PCreature +0x08, PUsedThing +0xa8.
// Shape: the event is {vptr, long 0, CIntelligentPointer<CThing>} with an inline ctor (the OpenChest 0x008448E0 model); &PCreature is
// spilled across the new/ctor block; the `new` null test selects 0 for AddEvent; the absent-events arm still evaluates a
// PUsedThing.GetPItem() whose result is dropped (a compiled-out assertion on the used thing); the scripted-hook branch returns early.
#include "fable_compiler.h"
class C3DVector;
class CTCBase;
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CBaseObject;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"
struct CIntelligentThingPointer : CBaseIntelligentPointer {   // vtable 0x01238C6C
    FABLE_FORCEINLINE CIntelligentThingPointer(CThing* item) : CBaseIntelligentPointer() { SetPItem((CBaseObject*)item); }
};
namespace NEntityEvents {
class __declspec(novtable) CEventBase { public:
    virtual void v00();
    long m_data;                                // +0x04
    CEventBase() : m_data(0) {}
};
}
namespace NEntityEvents {
class CEventItemUsed : public NEntityEvents::CEventBase { public:            // 0x10 bytes, vtable 0x0125C820
    CIntelligentThingPointer User;              // +0x08
    CEventItemUsed(CThing* user) : User(user) {}
};
}
class CTCEntityEvents { public:
    void AddEvent(NEntityEvents::CEventBase* event);           // 0x008113C0
};
class CTCActionUseScriptedHook { public:               // interface type 0xC2
    void OnActionUse(CThing* user);             // 0x00729B20
};
struct CTCActionUse {                           // interface type 0xD
#define VF(n) virtual void v##n();
#define VF10(p) VF(p##0) VF(p##1) VF(p##2) VF(p##3) VF(p##4) VF(p##5) VF(p##6) VF(p##7) VF(p##8) VF(p##9)
    VF10(0) VF10(1) VF(20) VF(21) VF(22) VF(23) VF(24) VF(25) VF(26) VF(27)
    virtual void OnUsed(CThing* user);          // slot 28 (+0x70)
};
class CCreatureActionBase { public:                    // retail sizeof 0xa8
    virtual void v00();
    unsigned char           _pad04[4];
    CBaseIntelligentPointer PCreature;          // +0x08
    unsigned char           _pad10[0xa8 - 0x10];
    void SetMoveToPos(const C3DVector& pos);    // 0x006929E0
};
class CCreatureAction_UseObject : public CCreatureActionBase { public:
    CBaseIntelligentPointer PUsedThing;         // +0xa8
    void PerformAction();
};

void CCreatureAction_UseObject::PerformAction()
{
    SetMoveToPos(((CThing*)PCreature.GetPItem())->GetPos());
    CThing* used = ((CThing*)PUsedThing.GetPItem());
    if (used->HasTC(TCI_ENTITY_EVENTS)) {
        CTCEntityEvents* events = ((CTCEntityEvents*)&used->GetTC(TCI_ENTITY_EVENTS));
        events->AddEvent(new NEntityEvents::CEventItemUsed(((CThing*)PCreature.GetPItem())));
    }
    else
        ((CThing*)PUsedThing.GetPItem());
    CThing* usedThing = ((CThing*)PUsedThing.GetPItem());
    if (usedThing->HasTC(TCI_ACTION_USE_SCRIPTED_HOOK)) {
        ((CTCActionUseScriptedHook*)&usedThing->GetTC(TCI_ACTION_USE_SCRIPTED_HOOK))->OnActionUse(((CThing*)PCreature.GetPItem()));
        return;
    }
    CTCActionUse* actionUse;
    ((CThing*)PUsedThing.GetPItem())->GetTC(TCI_ON_ACTION_USE, &actionUse);
    actionUse->OnUsed(((CThing*)PCreature.GetPItem()));
}