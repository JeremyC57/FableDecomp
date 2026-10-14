// CCreatureAction_WaspQueenDie::CCreatureAction_WaspQueenDie(CThingCreatureBase*, long) -- base CCombatActionBase(owner, 250, zeroed CSoundPair), own vtable (0x01280D84), the death type at +0x114 from the argument
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008C5B30, length 64 bytes. The chunk label "CCreatureAction_UnsheatheWeapons" is propagated from the same-shaped body @ 0x008B85D0: the vtable 0x01280D84 is CCreatureAction_WaspQueenDie (rtti-vtables.tsv), the action code is 250.
// Callees: CCombatActionBase::CCombatActionBase(CThingCreatureBase*, long, const CSoundPair&) @ 0x00858030.
// Shape: the sound pair is zeroed by its inline constructor (register-zeroed stack stores); the long argument is read from its
// slot after the base call and stored after the vtable (as Level in the landed 0x008CE2F0). `ret 8`: two arguments.
struct CThingCreatureBase;
struct CSoundPair {                         // two sound ids, zeroed by the inline constructor (register-zeroed stack stores)
    long FirstSound;
    long LastSound;
    CSoundPair() : FirstSound(0), LastSound(0) { }
};
struct CCombatActionBase {                  // retail sizeof 0x114
    virtual void v00();
    unsigned char _body[0x114 - 4];
    CCombatActionBase(CThingCreatureBase* owner, long actionCode, const CSoundPair& sound);   // 0x00858030
};

struct CCreatureAction_WaspQueenDie : CCombatActionBase {   // vtable 0x01280D84
    long m_deathType;                   // +0x114
    virtual void v00();
    CCreatureAction_WaspQueenDie(CThingCreatureBase* owner, long deathType);
};

CCreatureAction_WaspQueenDie::CCreatureAction_WaspQueenDie(CThingCreatureBase* owner, long deathType)
    : CCombatActionBase(owner, 250, CSoundPair())
    , m_deathType(deathType)
{
}