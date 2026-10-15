// CCreatureAction_CastPhysicalShieldSpell::CCreatureAction_CastPhysicalShieldSpell(CThingCreatureBase*) -- base CCombatActionBase(owner, 150, zeroed CSoundPair), own vtable (0x0127BDCC), ImmediateCast / OneAnimCast cleared, LevelToCast -1, TimeLimit 0, then SetAnimationName("ST_RELEASE_PHYSICAL_SHIELD")
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008BB670, length 120 bytes (+ int3 padding).
// Callees: CCombatActionBase::CCombatActionBase(CThingCreatureBase*, long, const CSoundPair&) @ 0x00858030,
// CCharString::CCharString(const char*, long) @ 0x0099EBF0, CCombatActionBase::SetAnimationName(const CCharString&) @ 0x008574D0,
// CCharString::~CCharString @ 0x0099EAE0.
// Shape: one zero register (ebx) zeroes the sound pair in the frame and later the two flag bytes and TimeLimit; base
// arguments right to left (the pair, 0x96, the owner); the init-list stores (declaration order, the -1 as an immediate) sink
// below the vtable store and between the animation name's pushes and its constructor call; the name is an implicit
// CCharString temporary re-addressed with `lea` for SetAnimationName and destroyed after it. `ret 4`.
// Layout: engine/CCreatureAction_CastPhysicalShieldSpell.h has the members at the PDB's +0x12c..; retail CCombatActionBase is
// 0x114 bytes, so `this` is modelled locally at +0x114 / +0x115 / +0x118 / +0x11c (ShieldHealth +0x120 untouched).
struct CThingCreatureBase;

struct CCharString {                        // one pointer; the defaulted length lets the literal convert implicitly
    void* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                         // 0x0099EAE0
};

struct CSoundPair {                         // two sound ids, zeroed by the inline constructor (register-zeroed frame stores)
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

struct CCreatureAction_CastPhysicalShieldSpell : CCombatActionBase {   // vtable 0x0127BDCC
    bool  ImmediateCast;                    // +0x114 (PDB +0x12c)
    bool  OneAnimCast;                      // +0x115 (PDB +0x12d)
    unsigned char _pad116[2];
    long  LevelToCast;                      // +0x118 (PDB +0x130)
    long  TimeLimit;                        // +0x11c (PDB +0x134)
    float ShieldHealth;                     // +0x120 (PDB +0x138), untouched here
    virtual void v00();
    CCreatureAction_CastPhysicalShieldSpell(CThingCreatureBase* owner);
};

CCreatureAction_CastPhysicalShieldSpell::CCreatureAction_CastPhysicalShieldSpell(CThingCreatureBase* owner)
    : CCombatActionBase(owner, 150, CSoundPair())
    , ImmediateCast(false)
    , OneAnimCast(false)
    , LevelToCast(-1)
    , TimeLimit(0)
{
    SetAnimationName("ST_RELEASE_PHYSICAL_SHIELD");
}