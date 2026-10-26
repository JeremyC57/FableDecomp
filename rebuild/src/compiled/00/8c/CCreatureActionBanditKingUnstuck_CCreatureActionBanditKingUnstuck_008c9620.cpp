// CCreatureAction_BanditKingUnstuck::CCreatureAction_BanditKingUnstuck(CThingCreatureBase*) -- base CCombatActionBase(owner, 150, zeroed CSoundPair), own vtable (0x01281D9C), the flag at +0x115 set
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008C9620, length 60 bytes.
// Callees: CCombatActionBase::CCombatActionBase(CThingCreatureBase*, long, const CSoundPair&) @ 0x00858030.
// Shape: the sound pair is built zeroed on the stack by its inline constructor (register-zeroed stores before the base call);
// the byte at +0x115 is stored after the vtable (a body assignment, or an init-list member after the base: same bytes).
// The chunk label "CCreatureAction_SheatheWeapons" is propagated: the vtable 0x01281D9C is CCreatureAction_BanditKingUnstuck
// (rtti-vtables.tsv), whose (owner, target) constructor @ 0x008C95B0 clears the same byte. `ret 4`: one argument.
class CThingCreatureBase;
class CSoundPair { public:                         // two sound ids, zeroed by the inline constructor (register-zeroed stack stores)
    long FirstSound;
    long LastSound;
    CSoundPair() : FirstSound(0), LastSound(0) { }
};
class CCombatActionBase { public:                  // retail sizeof 0x114
    virtual void v00();
    unsigned char _body[0x114 - 4];
    CCombatActionBase(CThingCreatureBase* owner, long actionCode, const CSoundPair& sound);   // 0x00858030
};

class CCreatureAction_BanditKingUnstuck : public CCombatActionBase { public:   // vtable 0x01281D9C
    unsigned char m_114;                    // +0x114 untouched here
    unsigned char m_done;                   // +0x115
    virtual void v00();
    CCreatureAction_BanditKingUnstuck(CThingCreatureBase* owner);
};

CCreatureAction_BanditKingUnstuck::CCreatureAction_BanditKingUnstuck(CThingCreatureBase* owner)
    : CCombatActionBase(owner, 150, CSoundPair())
    , m_done(1)
{
}