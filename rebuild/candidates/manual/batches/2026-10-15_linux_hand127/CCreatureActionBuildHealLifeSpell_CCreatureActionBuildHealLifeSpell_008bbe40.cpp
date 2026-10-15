// CCreatureAction_BuildHealLifeSpell::CCreatureAction_BuildHealLifeSpell(CThingCreatureBase*, bool) -- base CCombatActionBase(owner, 140, zeroed CSoundPair), own vtable (0x0127CBC4), then SetAnimationName("ST_BUILD_UP_HEAL_SELF" when healing self, else "ST_BUILD_UP_HEAL")
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008BBE40, length 110 bytes (+ int3 padding).
// Callees: CCombatActionBase::CCombatActionBase(CThingCreatureBase*, long, const CSoundPair&) @ 0x00858030,
// CCharString::CCharString(const char*, long) @ 0x0099EBF0, CCombatActionBase::SetAnimationName(const CCharString&) @ 0x008574D0,
// CCharString::~CCharString @ 0x0099EAE0.
// Shape: the sound pair is zeroed with immediates in the frame and passed by reference (arguments right to left: the pair,
// 0x8c, the owner); the flag byte is tested before the vtable store, the two literal addresses are selected into eax
// (`mov eax,A ; jne ; mov eax,B`) and ONE implicit CCharString temporary is built from the chosen pointer, re-addressed with
// `lea` for SetAnimationName and destroyed after it. `ret 8`.
struct CThingCreatureBase;

struct CCharString {                        // one pointer; the defaulted length lets the literal convert implicitly
    void* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                         // 0x0099EAE0
};

struct CSoundPair {                         // two sound ids, zeroed by the inline constructor (immediate frame stores)
    long FirstSound;
    long LastSound;
    CSoundPair() : FirstSound(0), LastSound(0) { }
};

struct CCombatActionBase {                  // retail sizeof 0x114
    virtual void v00();
    unsigned char _body[0x114 - 4];
    CCombatActionBase(CThingCreatureBase* owner, long actionCode, const CSoundPair& sound);   // 0x00858030
    void SetAnimationName(const CCharString& name);   // 0x008574D0
};

struct CCreatureAction_BuildHealLifeSpell : CCombatActionBase {   // vtable 0x0127CBC4; sizeof 0x114 (no members of its own)
    virtual void v00();
    CCreatureAction_BuildHealLifeSpell(CThingCreatureBase* owner, bool healSelf);
};

CCreatureAction_BuildHealLifeSpell::CCreatureAction_BuildHealLifeSpell(CThingCreatureBase* owner, bool healSelf)
    : CCombatActionBase(owner, 140, CSoundPair())
{
    SetAnimationName(healSelf ? "ST_BUILD_UP_HEAL_SELF" : "ST_BUILD_UP_HEAL");
}
