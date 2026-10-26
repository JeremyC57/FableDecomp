// CCreatureAction_ReleaseTimeSpell::CCreatureAction_ReleaseTimeSpell(long, CThingCreatureBase*, const CCharString&) -- the level at +0x114, base CCombatActionBase(owner, 145, zeroed CSoundPair), own vtable (0x012840F4), then the animation name is set
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008CDFF0, length 80 bytes.
// Callees: CCombatActionBase::CCombatActionBase(CThingCreatureBase*, long, const CSoundPair&) @ 0x00858030, CCombatActionBase::SetAnimationName @ 0x008574D0.
// Shape: `ret 0Ch`; the first argument is the level (stored after the vtable), the owner is the second, the name the third.
class CThingCreatureBase; class CThing; class CCharString;
class CSoundPair { public:                         // two sound ids, zeroed by the inline constructor (register-zeroed stack stores)
    long FirstSound;
    long LastSound;
    CSoundPair() : FirstSound(0), LastSound(0) { }
};
class CCombatActionBase { public:                  // retail sizeof 0x114
    virtual void v00();
    unsigned char _body[0x114 - 4];
    CCombatActionBase(long actionCode);                                                                         // 0x00857E40
    CCombatActionBase(CThingCreatureBase* owner, long actionCode, const CSoundPair& sound);                     // 0x00858030
    CCombatActionBase(CThingCreatureBase* owner, CThing* target, long actionCode, const CSoundPair& sound);     // 0x00857F30
    void SetAnimationName(const CCharString& name);   // 0x008574D0
};

class CCreatureAction_ReleaseTimeSpell : public CCombatActionBase { public:   // vtable 0x012840F4
    long m_level;                           // +0x114
    virtual void v00();
    CCreatureAction_ReleaseTimeSpell(long level, CThingCreatureBase* owner, const CCharString& name);
};

CCreatureAction_ReleaseTimeSpell::CCreatureAction_ReleaseTimeSpell(long level, CThingCreatureBase* owner, const CCharString& name)
    : CCombatActionBase(owner, 145, CSoundPair())
    , m_level(level)
{
    SetAnimationName(name);
}