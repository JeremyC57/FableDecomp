// CCreatureAction_CastStrengthMultiStrike::CCreatureAction_CastStrengthMultiStrike(CThingCreatureBase*, CThing*, long) -- base CCombatActionBase(owner, target, 150, GetSoundPair(owner, "VITAL_STRIKE")), own vtable (0x0128764C), NumberOfStrikes from the argument
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008E6A70, length 97 bytes (+ int3 padding).
// Callees: CCharString::CCharString(const char*, long) @ 0x0099EBF0, GetSoundPair @ 0x008561A0 (`__fastcall`: ecx hidden result,
// edx owner; the name on the stack), CCombatActionBase::CCombatActionBase(CThingCreatureBase*, CThing*, long, const CSoundPair&)
// @ 0x00857F30, CCharString::~CCharString @ 0x0099EAE0.
// Shape: the sound name is an implicit conversion temporary (built first, re-addressed with `lea`); the 8-byte CSoundPair
// result lands in a frame temporary through the hidden pointer and goes by reference to the base; after the temporary dies
// the count argument is loaded before `pop edi` and stored after the vtable. `ret 0Ch`: three arguments.
// Layout: engine/CCreatureAction_CastStrengthMultiStrike.h has NumberOfStrikes at the PDB's +0x12c; retail CCombatActionBase
// is 0x114 bytes, so `this` is modelled locally with the member at +0x114.
struct CThingCreatureBase;
struct CThing;

struct CCharString {                        // one pointer; the defaulted length lets the literal convert implicitly
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
    CCombatActionBase(CThingCreatureBase* owner, CThing* target, long actionCode, const CSoundPair& sound);   // 0x00857F30
};

struct CCreatureAction_CastStrengthMultiStrike : CCombatActionBase {   // vtable 0x0128764C
    long NumberOfStrikes;                   // +0x114 (PDB +0x12c)
    virtual void v00();
    CCreatureAction_CastStrengthMultiStrike(CThingCreatureBase* owner, CThing* target, long numberOfStrikes);
};

CCreatureAction_CastStrengthMultiStrike::CCreatureAction_CastStrengthMultiStrike(CThingCreatureBase* owner, CThing* target, long numberOfStrikes)
    : CCombatActionBase(owner, target, 150, GetSoundPair(owner, "VITAL_STRIKE"))
    , NumberOfStrikes(numberOfStrikes)
{
}
