// CCreatureAction_KrakenHighAttackInto::FinishAction() -- base FinishAction, then make the high attack loop (loop 0) the owner's current action while the target is alive, else the high attack "out of" action
// Leaf: ?FinishAction@
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: re-padded by-value maps +8.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00924A90, length 175 bytes (+ int3 padding), vtable slot 4 of 0x01290F1C
// (RTTI `?A0x29922355::CCreatureAction_KrakenHighAttackInto`; FableWin.pdb ?FinishAction@CCreatureAction_KrakenHighAttackInto@?A0xe2035276@@UAEXXZ).
// Callees: CCombatActionBase::FinishAction @ 0x00859A20 (first), CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PTarget at +0xa8
// in ebx, PCreature at +8), CCreatureAction_KrakenHighAttackLoop::CCreatureAction_KrakenHighAttackLoop(CThingCreatureBase&, CThing&,
// long, bool) @ 0x00925780 (landed; FableWin.pdb ??0...@QAE@AAVCThingCreatureBase@@AAVCThing@@J_N@Z),
// CCreatureAction_KrakenHighAttackOutOf::CCreatureAction_KrakenHighAttackOutOf(CThingCreatureBase&) @ 0x00925820 (landed),
// CThingCreatureBase::SetCurrentAction @ 0x006644F0, ~CCombatActionBase @ 0x0062E900 (manifest "~CTCLightningOrb" is propagated).
// Shape: an inline two-fetch alive test on PTarget (`IsValid`), each arm builds its 0x11c / 0x114-byte temporary at the frame base
// inside the SetCurrentAction call and destroys it through the base destructor.
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


class CCombatActionBase { public:                      // retail sizeof 0x114
    virtual void v00();
    protected: virtual void FinishAction(); public:                // 0x00859A20 (slot 4)
    unsigned char           _pad04[4];          // +0x04
    CIntelligentPointer<CThing> PCreature;          // +0x08 CIntelligentPointer<CThingCreatureBase>
    unsigned char           _pad10[0xa8 - 0x10];
    CIntelligentPointer<CThing> PTarget;            // +0xa8 CIntelligentPointer<CThing> (Ego_r.pdb +0xb4)
    unsigned char           _padb0[0x114 - 0xb0];
    virtual ~CCombatActionBase();                       // 0x0062E900
    __forceinline CThingCreatureBase& Creature() const { return *static_cast<CThingCreatureBase*>(PCreature.Get()); }
    __forceinline bool IsTargetAlive() const    // two fetches: present, then not dead
    {
        return PTarget.Get() && !PTarget.Get()->IsDead();
    }
};
struct CCreatureAction_KrakenHighAttackLoop : CCombatActionBase {    // vtable 0x0129107C
    bool Interrupt;                             // +0x114 (PDB +0x12c)
    long LoopNum;                               // +0x118 (PDB +0x130)
    CCreatureAction_KrakenHighAttackLoop(CThingCreatureBase& creature, CThing& target, long loopNum, bool interrupt);   // 0x00925780
};
struct CCreatureAction_KrakenHighAttackOutOf : CCombatActionBase {   // vtable 0x012911DC
    CCreatureAction_KrakenHighAttackOutOf(CThingCreatureBase& creature);   // 0x00925820
};
struct CCreatureAction_KrakenHighAttackInto : CCombatActionBase {    // vtable 0x01290F1C
    virtual void FinishAction();
};

void CCreatureAction_KrakenHighAttackInto::FinishAction()
{
    CCombatActionBase::FinishAction();
    if (IsTargetAlive())
        Creature().SetCurrentAction(CCreatureAction_KrakenHighAttackLoop(Creature(), *PTarget.Get(), 0, false));
    else
        Creature().SetCurrentAction(CCreatureAction_KrakenHighAttackOutOf(Creature()));
}