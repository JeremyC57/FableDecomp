// CCreatureAction_GetUpFromSitting::CCreatureAction_GetUpFromSitting(CThingCreatureBase*, const CCharString&, long, bool) -- base CCombatActionBase(owner, 50, zeroed CSoundPair), SittingMode and RemoveOnPerformAction from the arguments, own vtable, then the animation name handed to the base setter
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00860150, length 96 bytes.
// Callees: CCombatActionBase::CCombatActionBase @ 0x00858030, CCombatActionBase::SetAnimationName @ 0x008574D0 (propagated
// manifest label "CTCInGameMenu::SetPDef"; every call site of the landed family passes an animation name).
// Vtable 0x01276C2C (rtti-vtables.tsv). Retail CCombatActionBase is 0x114 bytes (PDB 0x12c), so this class's
// members sit at +0x114.. (PDB +0x12c..) and `this` is modelled locally; member names are the Ego_r.pdb names.
// `ret 10h`: four arguments; the three trailing ones are loaded into registers together after the base call.
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
    void SetAnimationName(const CCharString& name);   // 0x008574D0 (propagated manifest label; the landed family names it so)
};

class CCreatureAction_GetUpFromSitting : public CCombatActionBase { public:   // vtable 0x01276C2C
    long          SittingMode;            // +0x114 (PDB +0x12c)
    unsigned char RemoveOnPerformAction;   // +0x118 (PDB +0x130, bool)
    virtual void v00();
    CCreatureAction_GetUpFromSitting(CThingCreatureBase* owner, const CCharString& animName, long sittingMode, bool removeOnPerformAction);
};

CCreatureAction_GetUpFromSitting::CCreatureAction_GetUpFromSitting(CThingCreatureBase* owner, const CCharString& animName, long sittingMode, bool removeOnPerformAction)
    : CCombatActionBase(owner, 50, CSoundPair())
    , SittingMode(sittingMode)
    , RemoveOnPerformAction(removeOnPerformAction)
{
    SetAnimationName(animName);
}