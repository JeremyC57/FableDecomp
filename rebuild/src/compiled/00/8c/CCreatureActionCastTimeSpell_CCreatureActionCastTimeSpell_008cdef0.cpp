// CCreatureAction_CastTimeSpell::CCreatureAction_CastTimeSpell(CThingCreatureBase*, bool, const CCharString&) -- base CCombatActionBase(owner, 150, zeroed CSoundPair), Activate from the argument, own vtable, then the animation name handed to the base setter
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008CDEF0, length 80 bytes.
// Callees: CCombatActionBase::CCombatActionBase @ 0x00858030, CCombatActionBase::SetAnimationName @ 0x008574D0 (propagated
// manifest label "CTCInGameMenu::SetPDef"; every call site of the landed family passes an animation name).
// Vtable 0x01283E4C (rtti-vtables.tsv). Retail CCombatActionBase is 0x114 bytes (PDB 0x12c), so this class's
// members sit at +0x114.. (PDB +0x12c..) and `this` is modelled locally; member names are the Ego_r.pdb names.
// `ret 0Ch`: three arguments; the name reference is pushed from its slot after the base call.
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

class CCreatureAction_CastTimeSpell : public CCombatActionBase { public:   // vtable 0x01283E4C
    unsigned char Activate;                 // +0x114 (PDB +0x12c, bool)
    virtual void v00();
    CCreatureAction_CastTimeSpell(CThingCreatureBase* owner, bool activate, const CCharString& animName);
};

CCreatureAction_CastTimeSpell::CCreatureAction_CastTimeSpell(CThingCreatureBase* owner, bool activate, const CCharString& animName)
    : CCombatActionBase(owner, 150, CSoundPair())
    , Activate(activate)
{
    SetAnimationName(animName);
}