// CCreatureAction_DragonFlight::CCreatureAction_DragonFlight(CThingCreatureBase*, CThing*, const CCharString&) -- base CCombatActionBase(owner, target, 150, zeroed CSoundPair), FaceTarget set, TurnRate cleared, own vtable, then the animation name handed to the base setter
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008F4D00, length 96 bytes.
// Callees: CCombatActionBase::CCombatActionBase @ 0x00857F30, CCombatActionBase::SetAnimationName @ 0x008574D0 (propagated
// manifest label "CTCInGameMenu::SetPDef"; every call site of the landed family passes an animation name).
// Vtable 0x01288544 (rtti-vtables.tsv; the manifest's class label was propagated from another body). Retail CCombatActionBase is 0x114 bytes (PDB 0x12c), so this class's
// members sit at +0x114.. (PDB +0x12c..) and `this` is modelled locally; member names are the Ego_r.pdb names.
// All zero stores are immediates here (no zero register: the compiler's cost model with one zero after the call). `ret 0Ch`: three arguments.
#include "fable_string.h"

struct CThingCreatureBase;
struct CThing;

struct CSoundPair {                         // two sound ids, zeroed by the inline constructor (the register-zeroed
    long FirstSound;                        //   stack stores before the base call: an embedded object's inline ctor)
    long LastSound;
    CSoundPair() : FirstSound(0), LastSound(0) { }
};

struct CCombatActionBase {                  // retail sizeof 0x114 (the PDB build is 0x12c: its containers are 4 bytes wider there)
    virtual void v00();
    unsigned char _body[0x114 - 4];
    CCombatActionBase(CThingCreatureBase* owner, long actionCode, const CSoundPair& sound);                     // 0x00858030
    CCombatActionBase(CThingCreatureBase* owner, CThing* target, long actionCode, const CSoundPair& sound);     // 0x00857F30
    void SetAnimationName(const CCharString& name);   // 0x008574D0 (propagated manifest label; the landed family names it so)
};

struct CCreatureAction_DragonFlight : CCombatActionBase {   // vtable 0x01288544
    unsigned char FaceTarget;               // +0x114 (PDB +0x12c, bool)
    float         TurnRate;                 // +0x118 (PDB +0x130)
    virtual void v00();
    CCreatureAction_DragonFlight(CThingCreatureBase* owner, CThing* target, const CCharString& animName);
};

CCreatureAction_DragonFlight::CCreatureAction_DragonFlight(CThingCreatureBase* owner, CThing* target, const CCharString& animName)
    : CCombatActionBase(owner, target, 150, CSoundPair())
    , FaceTarget(1)
    , TurnRate(0)
{
    SetAnimationName(animName);
}
