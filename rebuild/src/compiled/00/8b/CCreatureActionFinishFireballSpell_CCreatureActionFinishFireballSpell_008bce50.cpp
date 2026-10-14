// CCreatureAction_FinishFireballSpell::CCreatureAction_FinishFireballSpell(CThingCreatureBase*, CThing*) -- base CCombatActionBase(owner, target, 145, zeroed CSoundPair), own vtable, then SetAnims()
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008BCE50, length 80 bytes.
// Callees: CCombatActionBase::CCombatActionBase(CThingCreatureBase*, CThing*, long, const CSoundPair&) @ 0x00857F30,
// CCreatureAction_FinishFireballSpell::SetAnims @ 0x008BAB70 (non-virtual member, called on `this` after the vtable store).
// Vtable 0x0127E1FC (rtti-vtables.tsv); the class adds no data. `ret 8`: two arguments (action code 0x91 = 145).
struct CThingCreatureBase;
struct CThing;

struct CSoundPair {                         // two sound ids, zeroed by the inline constructor (the register-zeroed
    long FirstSound;                        //   stack stores before the base call: an embedded object's inline ctor)
    long LastSound;
    CSoundPair() : FirstSound(0), LastSound(0) { }
};

struct CCombatActionBase {                  // retail sizeof 0x114 (PDB 0x12c)
    virtual void v00();
    unsigned char _body[0x114 - 4];
    CCombatActionBase(CThingCreatureBase* owner, long actionCode, const CSoundPair& sound);                     // 0x00858030
    CCombatActionBase(CThingCreatureBase* owner, CThing* target, long actionCode, const CSoundPair& sound);     // 0x00857F30
};

struct CCreatureAction_FinishFireballSpell : CCombatActionBase {   // vtable 0x0127E1FC; adds no data (sizeof 0x114, PDB 0x12c)
    virtual void v00();
    CCreatureAction_FinishFireballSpell(CThingCreatureBase* owner);
    CCreatureAction_FinishFireballSpell(CThingCreatureBase* owner, CThing* target);
    void SetAnims();                        // 0x008BAB70 (manifest, unique to this class)
};

CCreatureAction_FinishFireballSpell::CCreatureAction_FinishFireballSpell(CThingCreatureBase* owner, CThing* target)
    : CCombatActionBase(owner, target, 145, CSoundPair())
{
    SetAnims();
}