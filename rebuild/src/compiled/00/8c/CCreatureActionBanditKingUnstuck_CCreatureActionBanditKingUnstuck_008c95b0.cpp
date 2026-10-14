// CCreatureAction_BanditKingUnstuck::CCreatureAction_BanditKingUnstuck(CThingCreatureBase*, CThing*) -- base CCombatActionBase(owner, target, 150, zeroed CSoundPair), own vtable (0x01281D9C, rtti-vtables.tsv: the chunk label "CCreatureAction_LeapLong" is propagated), the flag at +0x115 cleared
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008C95B0, length 80 bytes.
// Callees: CCombatActionBase::CCombatActionBase(CThingCreatureBase*, CThing*, long, const CSoundPair&) @ 0x00857F30.
// Shape: `ret 8` (owner, target); the byte at +0x114 is untouched by this constructor, the one at +0x115 is zeroed after the vtable store.
struct CThingCreatureBase; struct CThing; struct CCharString;
struct CSoundPair {                         // two sound ids, zeroed by the inline constructor (register-zeroed stack stores)
    long FirstSound;
    long LastSound;
    CSoundPair() : FirstSound(0), LastSound(0) { }
};
struct CCombatActionBase {                  // retail sizeof 0x114
    virtual void v00();
    unsigned char _body[0x114 - 4];
    CCombatActionBase(long actionCode);                                                                         // 0x00857E40
    CCombatActionBase(CThingCreatureBase* owner, long actionCode, const CSoundPair& sound);                     // 0x00858030
    CCombatActionBase(CThingCreatureBase* owner, CThing* target, long actionCode, const CSoundPair& sound);     // 0x00857F30
    void SetAnimationName(const CCharString& name);   // 0x008574D0
};

struct CCreatureAction_BanditKingUnstuck : CCombatActionBase {   // vtable 0x01281D9C
    unsigned char m_114;                    // untouched here
    unsigned char m_done;                   // +0x115
    virtual void v00();
    CCreatureAction_BanditKingUnstuck(CThingCreatureBase* owner, CThing* target);
};

CCreatureAction_BanditKingUnstuck::CCreatureAction_BanditKingUnstuck(CThingCreatureBase* owner, CThing* target)
    : CCombatActionBase(owner, target, 150, CSoundPair())
    , m_done(0)
{
}