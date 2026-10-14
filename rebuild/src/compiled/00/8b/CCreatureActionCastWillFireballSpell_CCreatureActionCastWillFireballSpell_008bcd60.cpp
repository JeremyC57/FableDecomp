// CCreatureAction_CastWillFireballSpell::CCreatureAction_CastWillFireballSpell(CThingCreatureBase*, long, bool, long) -- base CCombatActionBase(owner, 150, zeroed CSoundPair), LevelToCast (hoisted above the vtable store) and ImmediateCast from the arguments, own vtable
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008BCD60, length 80 bytes.
// Callees: CCombatActionBase::CCombatActionBase @ 0x00858030, CCombatActionBase::SetAnimationName @ 0x008574D0 (propagated
// manifest label "CTCInGameMenu::SetPDef"; every call site of the landed family passes an animation name).
// Vtable 0x0127DF4C (rtti-vtables.tsv). Retail CCombatActionBase is 0x114 bytes (PDB 0x12c), so this class's
// members sit at +0x114.. (PDB +0x12c..) and `this` is modelled locally; member names are the Ego_r.pdb names.
// `ret 10h`: four arguments; the fourth is never read (OneAnimCast stays untouched), so it is a placeholder here. No SetAnimationName call in this body.
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

struct CCreatureAction_CastWillFireballSpell : CCombatActionBase {   // vtable 0x0127DF4C
    unsigned char ImmediateCast;            // +0x114 (PDB +0x12c, bool)
    unsigned char OneAnimCast;              // +0x115 (PDB +0x12d, bool; untouched)
    long          LevelToCast;              // +0x118 (PDB +0x130)
    virtual void v00();
    CCreatureAction_CastWillFireballSpell(CThingCreatureBase* owner, long levelToCast, bool immediateCast, long unused);
};

CCreatureAction_CastWillFireballSpell::CCreatureAction_CastWillFireballSpell(CThingCreatureBase* owner, long levelToCast, bool immediateCast, long unused)
    : CCombatActionBase(owner, 150, CSoundPair())
    , ImmediateCast(immediateCast)
    , LevelToCast(levelToCast)
{
}