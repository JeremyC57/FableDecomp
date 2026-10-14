// CCreatureAction_TentacleStrikeLoop::CCreatureAction_TentacleStrikeLoop(CThingCreatureBase*, long) -- base CCombatActionBase(owner, 150, zeroed CSoundPair), StartTick from the argument, Interrupt and StartHealth cleared, own vtable (0x01284EF4)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008CF990, length 80 bytes.
// Callees: CCombatActionBase::CCombatActionBase(CThingCreatureBase*, long, const CSoundPair&) @ 0x00858030
// Label: the manifest says "CCreatureAction_CastBattleCharge" (propagated); the stored vtable 0x01284EF4 belongs to
// CCreatureAction_TentacleStrikeLoop (rtti-vtables.tsv). Members are the Ego_r.pdb names (PDB +0x12c/+0x130/+0x134; retail
// CCombatActionBase is 0x114 bytes, so they sit at +0x114/+0x118/+0x11c). The zero register (ebx) is shared by the sound
// pair and the two cleared members, so StartHealth is written as an integer zero (a float literal would be an immediate).
// `ret 8`: two arguments.
struct CThingCreatureBase;
struct CThing;
struct CCharString;

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

struct CCreatureAction_TentacleStrikeLoop : CCombatActionBase {   // vtable 0x01284EF4
    long          StartTick;                // +0x114 (PDB +0x12c)
    unsigned char Interrupt;                // +0x118 (PDB +0x130, bool)
    long          StartHealth;              // +0x11c (PDB +0x134: float; zeroed through the shared zero register here)
    virtual void v00();
    CCreatureAction_TentacleStrikeLoop(CThingCreatureBase* owner, long startTick);
};

CCreatureAction_TentacleStrikeLoop::CCreatureAction_TentacleStrikeLoop(CThingCreatureBase* owner, long startTick)
    : CCombatActionBase(owner, 150, CSoundPair())
    , StartTick(startTick)
    , Interrupt(0)
    , StartHealth(0)
{
}
