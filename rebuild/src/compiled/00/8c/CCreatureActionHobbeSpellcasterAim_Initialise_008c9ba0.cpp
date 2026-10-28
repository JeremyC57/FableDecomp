// CCreatureAction_HobbeSpellcasterAim::Initialise() -- base aim Initialise; kill and drop any effect still alive from an earlier aim, then attach a new "CHARGE_SPELL" effect to the hobbe's staff and keep it in PEffect
// Leaf: ?Initialise@CCreatureAction_HobbeSpellcasterAim@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008C9BA0, 220 bytes, thiscall, plain `ret` (virtual).
// Identity (proven): RTTI vtable 0x01263CF4 (.?AVCCreatureAction_HobbeSpellcasterAim@@) slot 12 = Initialise; Ego_r.pdb
//   ?Initialise@CCreatureAction_HobbeSpellcasterAim@@UAEXXZ (donor 0x00901260, right after the anonymous-namespace
//   AttachEffectToStaff 0x009010D0 = retail 0x008C9A40 and FinishAction 0x00901050 = retail 0x008C99C0, landed). Literal
//   0x01282048 "CHARGE_SPELL".
// Callees: CCreatureAction_GenericAimProjectileWeapon::Initialise 0x008BF740 (slot 12 of vtable 0x0127F614),
//   CBaseIntelligentPointer::GetPItem 0x00A01B50 / CBaseIntelligentPointer() 0x00A01B10 / SetPItem 0x00A01B90 /
//   ~CBaseIntelligentPointer 0x00A01C10 (CIntelligentPointer<CThing> vtable 0x01238C6C), CThing::Kill(bool) 0x004C9B80,
//   CCharString ctor/dtor 0x0099EBF0 / 0x0099EAE0, `anonymous namespace'::AttachEffectToStaff(CThingCreatureBase&,
//   const CCharString&) 0x008C9A40 (`__fastcall`; Ego_r ?AttachEffectToStaff@?A0x13e2aa0e@@YIPAVCThing@@AAVCThingCreatureBase@@ABVCCharString@@@Z).
// Shape: the landed FinishAction 0x008C99C0 (inline IS_THING_ALIVE, `PEffect = 0` through a CIntelligentPointer<CThing>
//   temporary); the new effect is one full expression, so the helper-name temporary lives until after the assignment.
// Layout: PEffect +0x114 (Ego_r +0x12C; retail CCombatActionBase is 0x114 bytes, the aim action adds nothing).
class CCharString {
public:
    CCharString(const char* text, long length = -1);         // 0x0099EBF0
    ~CCharString();                                          // 0x0099EAE0
    void* PStringData;
};

class CThing {
public:
    virtual ~CThing();
    unsigned char _pad_0x04[0x91 - 0x04];
    unsigned char Dead : 1;                                  // +0x91 bit 0
    void Kill(bool now);                                     // 0x004C9B80
};

class CThingCreatureBase : public CThing { };

namespace {
CThing* __fastcall AttachEffectToStaff(CThingCreatureBase& creature, const CCharString& effect_name);   // 0x008C9A40
}

class CBaseObject;
class CBaseIntelligentPointer {
public:
    CBaseIntelligentPointer();                               // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                      // 0x00A01C10
    CBaseObject* GetPItem() const;                           // 0x00A01B50
    void SetPItem(CBaseObject* item);                        // 0x00A01B90
    void* PData;
};

template <class T>
class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    CIntelligentPointer(T* item) : CBaseIntelligentPointer() { SetPItem((CBaseObject*)item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); return *this; }
    T* operator->() const { return (T*)GetPItem(); }
    T& operator*() const { return *(T*)GetPItem(); }
};

template <class T>
inline bool IS_THING_ALIVE(const CIntelligentPointer<T>& p)
{
    if (p.GetPItem() != 0) {
        if (!p->Dead)
            return true;
    }
    return false;
}

class CCreatureActionBase {
public:
    virtual ~CCreatureActionBase();
    unsigned char _pad_0x04[4];
    CIntelligentPointer<CThingCreatureBase> PCreature;       // +0x08
    unsigned char _pad_0x10[0x114 - 0x10];
};

class CCreatureAction_GenericAimProjectileWeapon : public CCreatureActionBase {
public:
    virtual void Initialise();                               // 0x008BF740
};

class CCreatureAction_HobbeSpellcasterAim : public CCreatureAction_GenericAimProjectileWeapon {
public:
    CIntelligentPointer<CThing> PEffect;                     // +0x114
    virtual void Initialise();
};

void CCreatureAction_HobbeSpellcasterAim::Initialise()
{
    CCreatureAction_GenericAimProjectileWeapon::Initialise();
    if (IS_THING_ALIVE(PEffect)) {
        PEffect->Kill(true);
        PEffect = 0;
    }
    PEffect = AttachEffectToStaff(*PCreature, "CHARGE_SPELL");
}