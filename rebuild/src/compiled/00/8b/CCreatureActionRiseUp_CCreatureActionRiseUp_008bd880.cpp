// CCreatureAction_RiseUp::CCreatureAction_RiseUp(CThingCreatureBase*) -- base CCombatActionBase(owner, 240, GetSoundPair(owner, null name)), animation "RISE_UP", no target
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008BD880, length 121 bytes (+ int3 padding). Vtable 0x01268C9C.
// Callees: CCharString::CCharString(const char*, long) @ 0x0099EBF0, GetSoundPair(CThingCreatureBase*, const CCharString&) @ 0x008561A0
// (`__fastcall`, CSoundPair returned through the hidden pointer), CCombatActionBase::CCombatActionBase(CThingCreatureBase*, long,
// const CSoundPair&) @ 0x00858030, CCharString::~CCharString @ 0x0099EAE0, CCombatActionBase::SetAnimationName @ 0x008574D0.
// Shape: the landed ScorpionChargeStrike shape (0x008C4F50) with a null sound name (`push -1 ; push 0`: the 0 converts implicitly
// through the defaulted length) and action code 240 (0xF0); the name is a scoped named local; the flag byte at +0x114 is a body
// assignment after the name. The two-argument overload (0x0074DAC0) sets it to 1.
struct CThingCreatureBase;

struct CCharString {                        // one pointer; the default length makes the argument convert implicitly
    void* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                         // 0x0099EAE0
};
struct CSoundPair {                         // a user constructor makes the 8-byte result return through the hidden pointer
    long FirstSound;
    long LastSound;
    CSoundPair() : FirstSound(0), LastSound(0) { }
};
CSoundPair __fastcall GetSoundPair(CThingCreatureBase* owner, const CCharString& name);   // 0x008561A0

struct CCombatActionBase {                  // retail sizeof 0x114
    virtual void v00();
    unsigned char _body[0x114 - 4];
    CCombatActionBase(CThingCreatureBase* owner, long actionCode, const CSoundPair& sound);   // 0x00858030
    void SetAnimationName(const CCharString& name);   // 0x008574D0
};
struct CCreatureAction_RiseUp : CCombatActionBase {   // vtable 0x01268C9C
    bool m_hasTarget;                       // +0x114 (descriptive: 1 from the (owner, thing) overload, 0 here)
    virtual void v00();
    CCreatureAction_RiseUp(CThingCreatureBase* owner);
};

CCreatureAction_RiseUp::CCreatureAction_RiseUp(CThingCreatureBase* owner)
    : CCombatActionBase(owner, 240, GetSoundPair(owner, 0))
{
    {
        CCharString name("RISE_UP", -1);
        SetAnimationName(name);
    }
    m_hasTarget = false;
}