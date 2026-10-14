// CCreatureAction_CastWillDrainLifeSpell::CCreatureAction_CastWillDrainLifeSpell(CThingCreatureBase*, CThing*) -- base CCombatActionBase(owner, 150, zeroed CSoundPair), Level and DrainDefIndex cleared, own vtable, then the target stored into the inherited PTarget
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008BBB40, length 96 bytes.
// Callees: CCombatActionBase::CCombatActionBase(CThingCreatureBase*, long, const CSoundPair&) @ 0x00858030,
// CBaseIntelligentPointer::SetPItem @ 0x00A01B90 (on PTarget at +0xa8).
// Shape: the two-argument base is used (not the target-taking one), so the target is set afterwards through the
// intelligent pointer. The zero register (edi) is shared by the sound pair and the two members; the member stores are
// scheduled after the vtable store, before the SetPItem call. Vtable 0x012645EC (rtti-vtables.tsv). Member names from
// the Ego_r.pdb layout (PDB +0x12c/+0x130; retail CCombatActionBase is 0x114 bytes, CCreatureActionBase 0xa8, so PTarget
// is at +0xa8). `ret 8`: two arguments.
struct CThingCreatureBase;
struct CThing;

struct CBaseObject;

struct CBaseIntelligentPointer {
    virtual void v00();
    CBaseObject* m_pItem;                   // +0x04
    void SetPItem(CBaseObject* item);       // 0x00A01B90
};

struct CSoundPair {                         // two sound ids, zeroed by the inline constructor (the register-zeroed
    long FirstSound;                        //   stack stores before the base call: an embedded object's inline ctor)
    long LastSound;
    CSoundPair() : FirstSound(0), LastSound(0) { }
};

struct CCombatActionBase {                  // retail sizeof 0x114 (PDB 0x12c)
    virtual void v00();
    unsigned char _pad04[0xa8 - 4];         // +0x04 (PCreature at +8, the rest of the 0xa8-byte CCreatureActionBase)
    CBaseIntelligentPointer PTarget;        // +0xa8 CIntelligentPointer<CThing> (PDB +0xb4)
    unsigned char _padb0[0x114 - 0xb0];     // +0xb0
    CCombatActionBase(CThingCreatureBase* owner, long actionCode, const CSoundPair& sound);   // 0x00858030
};

struct CCreatureAction_CastWillDrainLifeSpell : CCombatActionBase {   // vtable 0x012645EC; sizeof 0x11c
    long Level;                             // +0x114 (PDB +0x12c)
    long DrainDefIndex;                     // +0x118 (PDB +0x130)
    virtual void v00();
    CCreatureAction_CastWillDrainLifeSpell(CThingCreatureBase* owner, CThing* target);
    CCreatureAction_CastWillDrainLifeSpell(CThingCreatureBase* owner, CThing* target, long level, long drainDefIndex);
};

CCreatureAction_CastWillDrainLifeSpell::CCreatureAction_CastWillDrainLifeSpell(CThingCreatureBase* owner, CThing* target)
    : CCombatActionBase(owner, 150, CSoundPair())
    , Level(0)
    , DrainDefIndex(0)
{
    PTarget.SetPItem((CBaseObject*)target);
}