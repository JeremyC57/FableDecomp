// CCreatureAction_CastWillFireballSpell::CCreatureAction_CastWillFireballSpell(CThingCreatureBase*) -- base CCombatActionBase(owner, 150, zeroed CSoundPair), ImmediateCast/OneAnimCast cleared, own vtable (0x0127DF4C), LevelToCast = -1
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008BCD10, length 80 bytes.
// Callees: CCombatActionBase::CCombatActionBase(CThingCreatureBase*, long, const CSoundPair&) @ 0x00858030
// Shape: the two byte stores (+0x114, +0x115, from the zeroed ebx that also built the sound pair) come BEFORE this
// class's vtable store and the -1 store after it: the bytes belong to an inlined `__declspec(novtable)` intermediate
// base (a spell-cast base that stores no vtable of its own, GOTCHAS wave 23), the long is this class's own init-list
// member. Names from engine/CCreatureAction_CastWillFireballSpell.h (PDB +0x12c/+0x12d/+0x130; retail
// CCombatActionBase is 0x114 bytes, so +0x114/+0x115/+0x118 here).
class CThingCreatureBase;
class CCharString;

class CSoundPair { public:                         // two sound ids, zeroed by the inline constructor (the register-zeroed
    long FirstSound;                        //   stack stores before the base call: an embedded object's inline ctor)
    long LastSound;
    CSoundPair() : FirstSound(0), LastSound(0) { }
};

class CCombatActionBase { public:                  // retail sizeof 0x114 (the PDB build is 0x12c: three containers are 4 bytes wider there)
    virtual void v00();
    unsigned char _body[0x114 - 4];
    CCombatActionBase(CThingCreatureBase* owner, long actionCode, const CSoundPair& sound);   // 0x00858030
    void SetCombatAnimation(const CCharString& name);                                           // 0x008574D0
};

struct __declspec(novtable) CCreatureAction_CastSpellBase : CCombatActionBase {   // intermediate: flags only, no vtable store of its own
    unsigned char ImmediateCast;            // +0x114
    unsigned char OneAnimCast;              // +0x115
    CCreatureAction_CastSpellBase(CThingCreatureBase* owner, long actionCode)
        : CCombatActionBase(owner, actionCode, CSoundPair()), ImmediateCast(0), OneAnimCast(0) { }
};

class CCreatureAction_CastWillFireballSpell : public CCreatureAction_CastSpellBase { public:   // vtable 0x0127DF4C
    long LevelToCast;                       // +0x118 (the base ends at +0x116, aligned up)
    virtual void v00();
    CCreatureAction_CastWillFireballSpell(CThingCreatureBase* owner);
};

CCreatureAction_CastWillFireballSpell::CCreatureAction_CastWillFireballSpell(CThingCreatureBase* owner)
    : CCreatureAction_CastSpellBase(owner, 150)
    , LevelToCast(-1)
{
}