// CCreatureAction_DragonRetreat::CCreatureAction_DragonRetreat(CThingCreatureBase*, const CCharString&) -- base CCombatActionBase(owner, 250, zeroed CSoundPair), own vtable, then the animation name handed to the base setter; the class adds no data
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008F4EF0, length 80 bytes.
// Callees: CCombatActionBase::CCombatActionBase @ 0x00858030, CCombatActionBase::SetCombatAnimation @ 0x008574D0 (propagated
// manifest label "CTCInGameMenu::SetPDef"; every call site of the landed family passes an animation name).
// Vtable 0x0128868C (rtti-vtables.tsv). Retail CCombatActionBase is 0x114 bytes (PDB 0x12c), so this class's
// members sit at +0x114.. (PDB +0x12c..) and `this` is modelled locally; member names are the Ego_r.pdb names.
// `ret 8`: two arguments.
#include "fable_string.h"

class CThingCreatureBase;
class CThing;

class CSoundPair { public:                         // two sound ids, zeroed by the inline constructor (the register-zeroed
    long FirstSound;                        //   stack stores before the base call: an embedded object's inline ctor)
    long LastSound;
    CSoundPair() : FirstSound(0), LastSound(0) { }
};

class CCombatActionBase { public:                  // retail sizeof 0x114 (the PDB build is 0x12c: its containers are 4 bytes wider there)
    virtual void v00();
    unsigned char _body[0x114 - 4];
    CCombatActionBase(CThingCreatureBase* owner, long actionCode, const CSoundPair& sound);                     // 0x00858030
    CCombatActionBase(CThingCreatureBase* owner, CThing* target, long actionCode, const CSoundPair& sound);     // 0x00857F30
public:
    void SetCombatAnimation(const CCharString&);   // 0x008574D0
};

class CCreatureAction_DragonRetreat : public CCombatActionBase { public:   // vtable 0x0128868C
    virtual void v00();
    CCreatureAction_DragonRetreat(CThingCreatureBase* owner, const CCharString& animName);
};

CCreatureAction_DragonRetreat::CCreatureAction_DragonRetreat(CThingCreatureBase* owner, const CCharString& animName)
    : CCombatActionBase(owner, 250, CSoundPair())
{
    SetCombatAnimation(animName);
}