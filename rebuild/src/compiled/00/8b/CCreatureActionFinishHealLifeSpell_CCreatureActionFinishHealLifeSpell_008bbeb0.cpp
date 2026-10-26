// CCreatureAction_FinishHealLifeSpell::CCreatureAction_FinishHealLifeSpell(CThingCreatureBase*, bool) -- base CCombatActionBase(owner, 145, zeroed CSoundPair), own vtable (0x0127CD34), then SetAnimationName("ST_RELEASE_HEAL_SELF" when healing self, else "ST_RELEASE_HEAL")
// Label: the manifest row says CCreatureAction_BuildHealLifeSpell (propagated); the stored vtable 0x0127CD34 is CCreatureAction_FinishHealLifeSpell (rtti-vtables.tsv).
// Leaf: ??0CCreatureAction_FinishHealLifeSpell
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008BBEB0, length 110 bytes (+ int3 padding).
// Callees: CCombatActionBase::CCombatActionBase(CThingCreatureBase*, long, const CSoundPair&) @ 0x00858030,
// CCharString::CCharString(const char*, long) @ 0x0099EBF0, CCombatActionBase::SetAnimationName(const CCharString&) @ 0x008574D0,
// CCharString::~CCharString @ 0x0099EAE0.
// Shape: the sound pair is zeroed with immediates in the frame and passed by reference (arguments right to left: the pair,
// 0x91, the owner); the flag byte is tested before the vtable store, the two literal addresses are selected into eax
// (`mov eax,A ; jne ; mov eax,B`) and ONE implicit CCharString temporary is built from the chosen pointer, re-addressed with
// `lea` for SetAnimationName and destroyed after it. `ret 8`.
class CThingCreatureBase;

class CCharString { public:                        // one pointer; the defaulted length lets the literal convert implicitly
    void* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                         // 0x0099EAE0
};

class CSoundPair { public:                         // two sound ids, zeroed by the inline constructor (immediate frame stores)
    long FirstSound;
    long LastSound;
    CSoundPair() : FirstSound(0), LastSound(0) { }
};

class CCombatActionBase { public:                  // retail sizeof 0x114
    virtual void v00();
    unsigned char _body[0x114 - 4];
    CCombatActionBase(CThingCreatureBase* owner, long actionCode, const CSoundPair& sound);   // 0x00858030
    void SetAnimationName(const CCharString& name);   // 0x008574D0
};

class CCreatureAction_FinishHealLifeSpell : public CCombatActionBase { public:   // vtable 0x0127CD34; sizeof 0x114 (no members of its own)
    virtual void v00();
    CCreatureAction_FinishHealLifeSpell(CThingCreatureBase* owner, bool healSelf);
};

CCreatureAction_FinishHealLifeSpell::CCreatureAction_FinishHealLifeSpell(CThingCreatureBase* owner, bool healSelf)
    : CCombatActionBase(owner, 145, CSoundPair())
{
    SetAnimationName(healSelf ? "ST_RELEASE_HEAL_SELF" : "ST_RELEASE_HEAL");
}