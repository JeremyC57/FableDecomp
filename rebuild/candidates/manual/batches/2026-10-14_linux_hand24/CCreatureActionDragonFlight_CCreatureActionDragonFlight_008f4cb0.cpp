// CCreatureAction_DragonFlight::CCreatureAction_DragonFlight(CThingCreatureBase*, const CCharString&) -- base CCombatActionBase(owner, 50, zeroed CSoundPair), FaceTarget and TurnRate cleared (shared zero register ebx), own vtable, then the animation name handed to the base setter
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008F4CB0, length 80 bytes.
// Callees: CCombatActionBase::CCombatActionBase @ 0x00858030, CCombatActionBase::SetAnimationName @ 0x008574D0 (propagated
// manifest label "CTCInGameMenu::SetPDef"; every call site of the landed family passes an animation name).
// Vtable 0x01288544 (rtti-vtables.tsv; the manifest's class label was propagated from another body). Retail CCombatActionBase is 0x114 bytes (PDB 0x12c), so this class's
// members sit at +0x114.. (PDB +0x12c..) and `this` is modelled locally; member names are the Ego_r.pdb names.
// Label: the manifest says CCreatureAction_CastTimeSpell; the stored vtable 0x01288544 is CCreatureAction_DragonFlight's. `ret 8`: two arguments.
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
    CCreatureAction_DragonFlight(CThingCreatureBase* owner, const CCharString& animName);
};

CCreatureAction_DragonFlight::CCreatureAction_DragonFlight(CThingCreatureBase* owner, const CCharString& animName)
    : CCombatActionBase(owner, 50, CSoundPair())
    , FaceTarget(0)
    , TurnRate(0)
{
    SetAnimationName(animName);
}
