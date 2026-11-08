// CCreatureAction_KrakenRoarInto::FinishAction() -- the creature-action base FinishAction (skipping CCombatActionBase's), then make the roar loop (loop 0) the owner's current action
// Leaf: ?FinishAction@
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: re-padded by-value maps +8.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00924B40, length 72 bytes (+ int3 padding), vtable slot 4 of 0x0129133C
// (RTTI `?A0x29922355::CCreatureAction_KrakenRoarInto`; FableWin.pdb ?FinishAction@CCreatureAction_KrakenRoarInto@?A0xe2035276@@UAEXXZ).
// Callees: CCreatureActionBase::FinishAction @ 0x00694430 (first; a qualified call to the grand-base), CBaseIntelligentPointer::GetPItem
// @ 0x00A01B50 (PCreature at +8, twice), CCreatureAction_KrakenRoarLoop::CCreatureAction_KrakenRoarLoop(CThingCreatureBase&, long)
// @ 0x00925920 (FableWin.pdb ??0...@QAE@AAVCThingCreatureBase@@J@Z), CThingCreatureBase::SetCurrentAction @ 0x006644F0,
// ~CCombatActionBase @ 0x0062E900.
// Shape: the 0x118-byte roar loop temporary sits at the frame base; the owner for the call's `this` is fetched first (edi).
#include "fable_compiler.h"
class CTCBase;
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CThingCreatureBase;
class CCombatActionBase;
class CThingCreatureBase : public CThing { public:
    bool SetCurrentAction(const CCombatActionBase& action);   // 0x006644F0
};
class CBaseObject;
class CBaseObjectPointer;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"


class CCreatureActionBase { public:                    // retail sizeof 0xa8 (Ego_r.pdb 0xb4)
    virtual void v00();
    virtual void FinishAction();                // 0x00694430 (slot 4)
    unsigned char           _pad04[4];          // +0x04
    CIntelligentPointer<CThing> PCreature;          // +0x08 CIntelligentPointer<CThingCreatureBase>
    unsigned char           _pad10[0xa8 - 0x10];
    FABLE_FORCEINLINE CThingCreatureBase& Creature() const { return *static_cast<CThingCreatureBase*>(PCreature.Get()); }
};
class CCombatActionBase : public CCreatureActionBase { public:   // retail sizeof 0x114
    CIntelligentPointer<CThing> PTarget;            // +0xa8 CIntelligentPointer<CThing>
    unsigned char           _padb0[0x114 - 0xb0];
    virtual ~CCombatActionBase();                       // 0x0062E900
};
struct CCreatureAction_KrakenRoarLoop : CCombatActionBase {   // vtable 0x01291494
    long LoopNum;                               // +0x114 (PDB +0x12c)
    CCreatureAction_KrakenRoarLoop(CThingCreatureBase& creature, long loopNum);   // 0x00925920
};
struct CCreatureAction_KrakenRoarInto : CCombatActionBase {   // vtable 0x0129133C
    virtual void FinishAction();
};

void CCreatureAction_KrakenRoarInto::FinishAction()
{
    CCreatureActionBase::FinishAction();
    Creature().SetCurrentAction(CCreatureAction_KrakenRoarLoop(Creature(), 0));
}