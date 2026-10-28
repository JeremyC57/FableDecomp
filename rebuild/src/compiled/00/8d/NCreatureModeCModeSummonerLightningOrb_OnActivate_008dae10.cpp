// NCreatureMode::CModeSummonerLightningOrb::OnActivate() -- stand-mode activation; suspend a player's animation; spawn the orb-charge effect (attached to the summoner's LIGHTNING_ORB_FOCUS bone through its particle emitter) and the charge effect; give the summoner an electrocute-on-touch component; arm the timer
// Leaf: ?OnActivate@CModeSummonerLightningOrb
// Retail: Fable.exe (TLC, VC7.1) @ 0x008DAE10, 541 bytes (`ret`).
// Callees: CModeStandBase::OnActivate @ 0x008621A0, CThingAnimatable::SetSuspendAnimation @ 0x00833A10,
//   CDefClassBase::GetSubDef<CSpecialEffectsDef> @ 0x00669580, CCRC::CalcAndLookup @ 0x00669D30, CCharString(const char*, long)
//   @ 0x0099EBF0 / ~CCharString @ 0x0099EAE0, CThing::GetPos @ 0x004C73D0, the special-effect Create(long, const C3DVector&,
//   bool) @ 0x006E0880 (__fastcall), CBaseIntelligentPointer() @ 0x00A01B10 / GetPItem @ 0x00A01B50 / SetPItem @ 0x00A01B90 /
//   ~CBaseIntelligentPointer @ 0x00A01C10, CTCDParticleEmitter::AttachToThing @ 0x006E0BE0, CThing::AddTC @ 0x004C9D60, __ftol2.
// Data: "LIGHTNING_ORB_CHARGE" 0x01285B2C, "LIGHTNING_ORB_FOCUS" 0x01282C4C, "LIGHTNING_CHARGE" 0x01285B18,
//   "CTCElectrocuteOnTouch" 0x0123DD48, the timer seconds (long) @ 0x01375550, 3.0f @ 0x0122DED4 (frames per ... scale).
// Layout: as the landed ctor 0x008CC180 / OnDeactivate 0x008D79F0 (Ego_r.pdb): Filter +0x68, PTarget +0x74, PChargeEffect +0x7c,
//   POrbChargeEffect +0x84, Timer +0x8c; CThing type byte +0x10, def +0x70, the particle-emitter driver +0x5c (interface 0x4b).
// Shape: as the landed CCreatureAction_JackDeathInto::Initialise 0x008C0EA0: the thing's def is a by-value CDefPointer copy
//   (addref, GetSubDef, release in the full expression) and GetSubDef has the same-TU stand-in body, so the sub-def stays in ebp
//   and the effect id reuses its frame word; the emitter is an uninitialised local filled under HasTC(0x4b).
class C3DVector { public: float X, Y, Z; };
class CTCBase;
class CTCParamsBase;
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);  // 0x0099EBF0
    ~CCharString();                                   // 0x0099EAE0
};
static __forceinline bool DecRefToZero(long& count) { return --count == 0; }
class CSpecialEffectsDef { public:
    virtual void v00();
    virtual void Destroy();                           // slot 1
    long RefCount;                                    // +0x04
    bool CalcAndLookup(const CCharString& name, long* value);   // 0x00669D30
};
template<class T> class CDefPointer { public:
    T* Object;
    CDefPointer() : Object(0) { }
    CDefPointer(T* object) : Object(object) { if (Object) ++Object->RefCount; }
    CDefPointer(const CDefPointer& other) : Object(other.Object) { if (Object) ++Object->RefCount; }
    ~CDefPointer() { if (Object) { if (DecRefToZero(Object->RefCount)) Object->Destroy(); } }
    T* operator->() const { return Object; }
    CDefPointer& operator=(const CDefPointer& other)
    {
        if (Object != other.Object) {
            if (Object) {
                if (--Object->RefCount == 0)
                    Object->Destroy();
                Object = 0;
            }
            Object = other.Object;
            if (Object)
                ++Object->RefCount;
        }
        return *this;
    }
};
class CDefinitionManager { public:
    CDefPointer<CSpecialEffectsDef> GetPDefFromGlobalIndex(long index);   // 0x009ADA10
};
class CDefClassBase { public:
    virtual void v00(); virtual void Destroy(); virtual void v08(); virtual void v0c(); virtual void v10();
    virtual void v14(); virtual void v18(); virtual void v1c(); virtual void v20(); virtual void v24();
    virtual void v28(); virtual void v2c(); virtual void v30(); virtual void v34();
    virtual long* GetSubDefIndex(const CCharString& name);   // slot 14 (+0x38)
    long RefCount;                                    // +0x04
    unsigned char _pad08[0x1c - 8];
    CDefinitionManager* m_pManager;                   // +0x1c
    bool GetSubDef(CDefPointer<CSpecialEffectsDef>* out);   // 0x00669580
};
#pragma auto_inline(off)
// Same-TU stand-in for GetSubDef<CSpecialEffectsDef> 0x00669580 (its landed body; see 0x008C0EA0).
bool CDefClassBase::GetSubDef(CDefPointer<CSpecialEffectsDef>* out)
{
    long* pIndex;
    {
        CCharString name("CSpecialEffectsDef");
        pIndex = GetSubDefIndex(name);
    }
    if (pIndex) {
        CDefPointer<CSpecialEffectsDef> result = m_pManager->GetPDefFromGlobalIndex(*pIndex);
        *out = result;
        return true;
    }
    return false;
}
#pragma auto_inline(on)

namespace NCreatureMode { struct CModeBase; }
class CThing { public:
    unsigned char _pad00[0x10];
    unsigned char m_type;                             // +0x10 (1 = the player creature)
    unsigned char _pad11[0x20 - 0x11];
    unsigned long m_presentBits[9];                   // +0x20
    unsigned char _pad44[0x5c - 0x44];
    CTCBase*      PTCDriver;                          // +0x5c
    unsigned char _pad60[0x70 - 0x60];
    CDefClassBase* m_pDef;                            // +0x70
    unsigned char _pad74[0x91 - 0x74];
    unsigned char m_flags91;                          // +0x91 bit 0: dead
    const C3DVector& GetPos() const;             // 0x004C73D0
    void SetSuspendAnimation(bool suspend, NCreatureMode::CModeBase* mode);   // 0x00833A10
    CTCBase* AddTC(const CCharString& name, bool activate, CTCParamsBase* params);   // 0x004C9D60
    unsigned long HasTC(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    CDefPointer<CDefClassBase> GetDef() const { return CDefPointer<CDefClassBase>(m_pDef); }
};
CThing* __fastcall CreateSpecialEffect(long effect, const C3DVector& position, bool attached);   // 0x006E0880

class CTCDParticleEmitter { public:
    void AttachToThing(CThing* thing, long boneIndex, const CCharString& boneName, long a, float b);   // 0x006E0BE0
};

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
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {   // vtable 0x01238C6C
public:
    CIntelligentPointer(T* item) { SetPItem(item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other) { T* item = other.GetPItem(); SetPItem(item); return *this; }

public:
    T* GetPItem() const { return reinterpret_cast<T*>(CBaseIntelligentPointer::GetPItem()); }
public:
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem(reinterpret_cast<CBaseObject*>(item)); }
};
inline bool IS_THING_ALIVE(const CIntelligentPointer<CThing>& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}

extern long g_LightningOrbChargeSeconds;              // 0x01375550
extern const float g_FramesPerSecond_0122ded4;        // 0x0122DED4 (3.0f)

namespace NCreatureMode {
struct CModeBase {                                    // vtable 0x012673CC
    virtual ~CModeBase();
    CThing*       m_pThing;                           // +0x04
    unsigned char _pad08[0x14];
};
struct CModeStandBase : CModeBase {
    unsigned char _pad1c[0x4c];                       // +0x1c .. +0x68
    void OnActivate();                                // 0x008621A0
};
struct CModeSummonerLightningOrb : CModeStandBase {
    unsigned char            _filter[0xc];            // +0x68 CAnimFilterOfGroup Filter
    CIntelligentPointer<CThing> PTarget;                 // +0x74
    CIntelligentPointer<CThing> PChargeEffect;           // +0x7c
    CIntelligentPointer<CThing> POrbChargeEffect;        // +0x84
    long                     Timer;                   // +0x8c
    virtual void OnActivate();
};

void CModeSummonerLightningOrb::OnActivate()
{
    CModeStandBase::OnActivate();
    if (m_pThing->m_type == 1)
        m_pThing->SetSuspendAnimation(true, this);
    CDefPointer<CSpecialEffectsDef> effects;
    if (m_pThing->GetDef()->GetSubDef(&effects)) {
        long effect;
        if (effects->CalcAndLookup("LIGHTNING_ORB_CHARGE", &effect)) {
            POrbChargeEffect = CIntelligentPointer<CThing>(CreateSpecialEffect(effect, m_pThing->GetPos(), false));
            if (IS_THING_ALIVE(POrbChargeEffect)) {
                CTCDParticleEmitter* emitter;
                CThing* orb = POrbChargeEffect.GetPItem();
                if (orb->HasTC(0x4b))
                    emitter = (CTCDParticleEmitter*)orb->PTCDriver;
                emitter->AttachToThing(m_pThing, 10, "LIGHTNING_ORB_FOCUS", 0, 0.0f);
            }
        }
        if (effects->CalcAndLookup("LIGHTNING_CHARGE", &effect))
            PChargeEffect = CIntelligentPointer<CThing>(CreateSpecialEffect(effect, m_pThing->GetPos(), false));
    }
    m_pThing->AddTC("CTCElectrocuteOnTouch", true, 0);
    Timer = (long)(g_LightningOrbChargeSeconds * g_FramesPerSecond_0122ded4);
}
}