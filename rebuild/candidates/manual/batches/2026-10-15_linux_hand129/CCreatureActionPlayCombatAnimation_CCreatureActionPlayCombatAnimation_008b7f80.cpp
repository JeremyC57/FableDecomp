// CCreatureAction_PlayCombatAnimation::CCreatureAction_PlayCombatAnimation(CThingCreatureBase*, CThing*, const CCharString&, bool) -- base CCombatActionBase(owner, thing, 50, CSoundPair()), three flag bytes, then the animation name
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008B7F80, length 97 bytes (+ int3 padding). Vtable 0x0127A604.
// Model: rebuild/include/engine/CCreatureAction_PlayCombatAnimation.h is the flattened PDB layout and sits high against the bytes (CCombatActionBase is 0x12c bytes in the PDB, 0x114 in retail: retail containers are 12 bytes), so `this` is a local model with the retail offsets.
// Callees: CCombatActionBase::CCombatActionBase(CThingCreatureBase*, CThing*, long, const CSoundPair&) @ 0x00857F30,
// CCombatActionBase::SetAnimationName(const CCharString&) @ 0x008574D0.
// Shape: the zeroed sound pair's two words are written with immediates after the pushes (no free zero register here); the three
// bytes at +0x114..+0x116 are initialiser-list members stored after the vtable; the name reference is pushed before them.
struct CThingCreatureBase;
struct CThing;

struct CCharString {                        // one pointer; the default length makes a literal convert implicitly
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
    CCombatActionBase(CThingCreatureBase* owner, long actionCode, const CSoundPair& sound);                  // 0x00858030
    CCombatActionBase(CThingCreatureBase* owner, CThing* thing, long actionCode, const CSoundPair& sound);   // 0x00857F30
    void SetAnimationName(const CCharString& name);   // 0x008574D0
};
struct CCreatureAction_PlayCombatAnimation : CCombatActionBase {   // vtable 0x0127A604
    bool m_flag;                            // +0x114 (descriptive: the fourth argument)
    bool m_firstFrame;                      // +0x115 (descriptive: starts true)
    bool m_started;                         // +0x116 (descriptive: starts false)
    virtual void v00();
    CCreatureAction_PlayCombatAnimation(CThingCreatureBase* owner, CThing* thing, const CCharString& animationName, bool flag);
};

CCreatureAction_PlayCombatAnimation::CCreatureAction_PlayCombatAnimation(CThingCreatureBase* owner, CThing* thing, const CCharString& animationName, bool flag)
    : CCombatActionBase(owner, thing, 50, CSoundPair())
    , m_flag(flag)
    , m_firstFrame(true)
    , m_started(false)
{
    SetAnimationName(animationName);
}
