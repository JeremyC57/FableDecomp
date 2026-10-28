// CCreatureAction_BriarRoseShotFarOutof::CCreatureAction_BriarRoseShotFarOutof(CThingCreatureBase*, CThing*) -- base CCombatActionBase(owner, target, 250, GetSoundPair(owner, "SWORD_SWING")), own vtable (0x012807A4), PShot default-constructed, then SetCombatAnimation("STRIKE_SHOT_FAR_OUTOF")
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008C39A0, length 143 bytes (+ int3 padding).
// Callees: CCharString::CCharString(const char*, long) @ 0x0099EBF0 (twice), GetSoundPair @ 0x008561A0 (`__fastcall`: ecx hidden
// result, edx owner; the name on the stack), CCombatActionBase::CCombatActionBase(CThingCreatureBase*, CThing*, long, const CSoundPair&)
// @ 0x00857F30, CCharString::~CCharString @ 0x0099EAE0 (twice), CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x00A01B10,
// CCombatActionBase::SetCombatAnimation(const CCharString&) @ 0x008574D0.
// Shape: both names are implicit conversion temporaries re-addressed with `lea`; after the base constructor and the sound
// name's destructor come the vtable and the CIntelligentPointer<CThing> member at +0x114 (base ctor, vtable 0x01238C6C stored
// between the second temporary's pushes and its constructor call), then the body's SetCombatAnimation. `ret 8`.
// Layout: engine/CCreatureAction_BriarRoseShotFarOutof.h has PShot at the PDB's +0x12c; retail CCombatActionBase is 0x114
// bytes, so `this` is modelled locally with PShot at +0x114.
class CThingCreatureBase;
class CThing;

class CCharString { public:                        // one pointer; the defaulted length lets the literal convert implicitly
    void* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                         // 0x0099EAE0
};

class CSoundPair { public:                         // a user constructor makes the 8-byte result return through the hidden pointer
    long FirstSound;
    long LastSound;
    CSoundPair() : FirstSound(0), LastSound(0) { }
};

CSoundPair __fastcall GetSoundPair(CThingCreatureBase* owner, const CCharString& name);   // 0x008561A0

class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};

template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {   // CIntelligentPointer<CThing>, vtable 0x01238C6C
public:
    CIntelligentPointer() : CBaseIntelligentPointer() { }

    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};

class CCombatActionBase { public:                  // retail sizeof 0x114
    virtual void v00();
    unsigned char _body[0x114 - 4];
    CCombatActionBase(CThingCreatureBase* owner, CThing* target, long actionCode, const CSoundPair& sound);   // 0x00857F30
    void SetCombatAnimation(const CCharString& name);   // 0x008574D0
};

class CCreatureAction_BriarRoseShotFarOutof : public CCombatActionBase { public:   // vtable 0x012807A4
    CIntelligentPointer<CThing> PShot;              // +0x114 (PDB +0x12c)
    virtual void v00();
    CCreatureAction_BriarRoseShotFarOutof(CThingCreatureBase* owner, CThing* target);
};

CCreatureAction_BriarRoseShotFarOutof::CCreatureAction_BriarRoseShotFarOutof(CThingCreatureBase* owner, CThing* target)
    : CCombatActionBase(owner, target, 250, GetSoundPair(owner, "SWORD_SWING"))
    , PShot()
{
    SetCombatAnimation("STRIKE_SHOT_FAR_OUTOF");
}