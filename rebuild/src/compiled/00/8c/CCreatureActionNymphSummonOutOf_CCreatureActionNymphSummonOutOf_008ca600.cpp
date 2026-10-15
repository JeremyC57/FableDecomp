// CCreatureAction_NymphSummonOutOf::CCreatureAction_NymphSummonOutOf(CThingCreatureBase*) -- combat action with an unnamed sound pair (action code 150), animation "SUMMON_CREATURES_OUTOF"
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008CA600, length 116 bytes, `ret 4`. Vtable 0x01282354.
// Callees: CCharString::CCharString(const char*, long) @ 0x0099EBF0, GetSoundPair (__fastcall) @ 0x008561A0,
//          CCombatActionBase::CCombatActionBase(owner, actionCode, const CSoundPair&) @ 0x00858030,
//          CCharString::~CCharString @ 0x0099EAE0, CCombatActionBase::SetAnimationName(const CCharString&) @ 0x008574D0
//          (manifest label "CTCInGameMenu::SetPDef" is propagated; named as in the landed family 20).
// Shape: the sound name is a NULL pointer converted implicitly (`push -1 ; push 0`), the hidden-pointer CSoundPair
//   feeds the base constructor, the string dies after it; then a scoped named CCharString for the animation (its
//   constructor pushes are scheduled above the vtable store, GOTCHAS family wave) and the setter call.
// Identity: the stored vtable 0x01282354 is CCreatureAction_NymphSummonOutOf (rtti-vtables.tsv); the catalog label
//   "CCreatureAction_GoUnderground" is propagated over the identical body shape. No member of its own is touched.
struct CThingCreatureBase;

struct CCharString {                        // one pointer; the defaulted length makes a literal convert implicitly
    void* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};

struct CSoundPair {                         // user constructor: the 8-byte result comes back through the hidden pointer
    long FirstSound;
    long LastSound;
    CSoundPair() : FirstSound(0), LastSound(0) { }
};

CSoundPair __fastcall GetSoundPair(CThingCreatureBase* owner, const CCharString& name);   // 0x008561A0

struct CCombatActionBase {                  // retail sizeof 0x114
    virtual void v00();
    unsigned char _body[0x114 - 4];
    CCombatActionBase(CThingCreatureBase* owner, long actionCode, const CSoundPair& sound);   // 0x00858030
    void SetAnimationName(const CCharString& name);                                           // 0x008574D0
};

struct CCreatureAction_NymphSummonOutOf : CCombatActionBase {   // vtable 0x01282354
    virtual void v00();
    CCreatureAction_NymphSummonOutOf(CThingCreatureBase* owner);
};

CCreatureAction_NymphSummonOutOf::CCreatureAction_NymphSummonOutOf(CThingCreatureBase* owner)
    : CCombatActionBase(owner, 150, GetSoundPair(owner, (const char*)0))
{
    {
        CCharString name("SUMMON_CREATURES_OUTOF", -1);
        SetAnimationName(name);
    }
}