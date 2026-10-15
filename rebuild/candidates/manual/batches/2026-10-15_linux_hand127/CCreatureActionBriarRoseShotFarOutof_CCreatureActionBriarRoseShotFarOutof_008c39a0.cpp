// CCreatureAction_BriarRoseShotFarOutof::CCreatureAction_BriarRoseShotFarOutof(CThingCreatureBase*, CThing*) -- base CCombatActionBase(owner, target, 250, GetSoundPair(owner, "SWORD_SWING")), own vtable (0x012807A4), PShot default-constructed, then SetAnimationName("STRIKE_SHOT_FAR_OUTOF")
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008C39A0, length 143 bytes (+ int3 padding).
// Callees: CCharString::CCharString(const char*, long) @ 0x0099EBF0 (twice), GetSoundPair @ 0x008561A0 (`__fastcall`: ecx hidden
// result, edx owner; the name on the stack), CCombatActionBase::CCombatActionBase(CThingCreatureBase*, CThing*, long, const CSoundPair&)
// @ 0x00857F30, CCharString::~CCharString @ 0x0099EAE0 (twice), CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x00A01B10,
// CCombatActionBase::SetAnimationName(const CCharString&) @ 0x008574D0.
// Shape: both names are implicit conversion temporaries re-addressed with `lea`; after the base constructor and the sound
// name's destructor come the vtable and the CIntelligentPointer<CThing> member at +0x114 (base ctor, vtable 0x01238C6C stored
// between the second temporary's pushes and its constructor call), then the body's SetAnimationName. `ret 8`.
// Layout: engine/CCreatureAction_BriarRoseShotFarOutof.h has PShot at the PDB's +0x12c; retail CCombatActionBase is 0x114
// bytes, so `this` is modelled locally with PShot at +0x114.
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

struct CBaseIntelligentPointer {
    virtual void v00();
    CBaseIntelligentPointer();              // 0x00A01B10
};

struct CIntelligentPointer : CBaseIntelligentPointer {   // CIntelligentPointer<CThing>, vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointer() : CBaseIntelligentPointer() { }
};

struct CCombatActionBase {                  // retail sizeof 0x114
    virtual void v00();
    unsigned char _body[0x114 - 4];
    CCombatActionBase(CThingCreatureBase* owner, CThing* target, long actionCode, const CSoundPair& sound);   // 0x00857F30
    void SetAnimationName(const CCharString& name);   // 0x008574D0
};

struct CCreatureAction_BriarRoseShotFarOutof : CCombatActionBase {   // vtable 0x012807A4
    CIntelligentPointer PShot;              // +0x114 (PDB +0x12c)
    virtual void v00();
    CCreatureAction_BriarRoseShotFarOutof(CThingCreatureBase* owner, CThing* target);
};

CCreatureAction_BriarRoseShotFarOutof::CCreatureAction_BriarRoseShotFarOutof(CThingCreatureBase* owner, CThing* target)
    : CCombatActionBase(owner, target, 250, GetSoundPair(owner, "SWORD_SWING"))
    , PShot()
{
    SetAnimationName("STRIKE_SHOT_FAR_OUTOF");
}
