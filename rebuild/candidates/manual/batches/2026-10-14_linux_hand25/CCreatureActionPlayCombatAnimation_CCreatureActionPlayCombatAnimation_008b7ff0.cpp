// CCreatureAction_PlayCombatAnimation::CCreatureAction_PlayCombatAnimation(CThingCreatureBase*, const CCharString&, bool) -- base CCombatActionBase(owner, 50, zeroed CSoundPair), own vtable (0x0127A604), the three flags at +0x114..+0x116 (the middle one from the argument), then the animation name is set
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008B7FF0, length 96 bytes.
// Callees: CCombatActionBase::CCombatActionBase(CThingCreatureBase*, long, const CSoundPair&) @ 0x00858030, CCombatActionBase::SetAnimationName @ 0x008574D0.
// Shape: `ret 0Ch`; the zero register (ebx) that built the sound pair also clears the two outer flags, the argument byte lands between them.
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

struct CCreatureAction_PlayCombatAnimation : CCombatActionBase {   // vtable 0x0127A604
    unsigned char m_looping;                // +0x114
    unsigned char m_flag;                   // +0x115 (argument)
    unsigned char m_started;                // +0x116
    virtual void v00();
    CCreatureAction_PlayCombatAnimation(CThingCreatureBase* owner, const CCharString& name, bool flag);
};

CCreatureAction_PlayCombatAnimation::CCreatureAction_PlayCombatAnimation(CThingCreatureBase* owner, const CCharString& name, bool flag)
    : CCombatActionBase(owner, 50, CSoundPair())
    , m_looping(0)
    , m_flag(flag)
    , m_started(0)
{
    SetAnimationName(name);
}
