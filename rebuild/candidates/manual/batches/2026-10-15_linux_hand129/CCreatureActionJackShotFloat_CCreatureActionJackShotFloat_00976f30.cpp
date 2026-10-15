// CCreatureAction_JackShotFloat::CCreatureAction_JackShotFloat(CThingCreatureBase*, CThing*, const CCharString&) -- base CCombatActionBase(owner, thing, 250, GetSoundPair(owner, "SWORD_SWING")), then the given animation name
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00976F30, length 99 bytes (+ int3 padding). Vtable 0x012978A4.
// Model: rebuild/include/engine/CCreatureAction_JackShotFloat.h is the flattened PDB layout and sits high against the bytes (CCombatActionBase is 0x12c bytes in the PDB, 0x114 in retail: retail containers are 12 bytes), so `this` is a local model with the retail offsets.
// Callees: CCharString::CCharString(const char*, long) @ 0x0099EBF0, GetSoundPair @ 0x008561A0 (`__fastcall`, hidden-pointer CSoundPair),
// CCombatActionBase::CCombatActionBase(CThingCreatureBase*, CThing*, long, const CSoundPair&) @ 0x00857F30, CCharString::~CCharString
// @ 0x0099EAE0, CCombatActionBase::SetAnimationName(const CCharString&) @ 0x008574D0.
// Shape: the landed two-argument combat-action family (0x008C4F50): the literal converts implicitly (re-addressed with `lea`), the
// string temporary dies after the base constructor, then the vtable store and the name reference forwarded to the setter (`ret 0Ch`).
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
struct CCreatureAction_JackShotFloat : CCombatActionBase {   // vtable 0x012978A4
    virtual void v00();
    CCreatureAction_JackShotFloat(CThingCreatureBase* owner, CThing* thing, const CCharString& animationName);
};

CCreatureAction_JackShotFloat::CCreatureAction_JackShotFloat(CThingCreatureBase* owner, CThing* thing, const CCharString& animationName)
    : CCombatActionBase(owner, thing, 250, GetSoundPair(owner, "SWORD_SWING"))
{
    SetAnimationName(animationName);
}
