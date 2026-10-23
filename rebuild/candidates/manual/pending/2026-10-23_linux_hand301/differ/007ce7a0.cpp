// Grade: DIFFER(847v863) -- register allocation and frame: retail reuses ebx (the zero) as the PEffect address and keeps the frame at 0x60 (ours keeps ebx=0, puts PEffect in edi and spills it, frame 0x64); retail adds HeightMedium first (fld [def+0x68]; fadd position.Z) and builds the DUMMY_CENTRE name between the top-point copy stores; flow and calls match
// CTCElectrocutedEffect::Reset() -- restarts the fade timers from the def and, when no live effect exists, creates the electrocution particle sized to the thing's height and attaches it
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007CE7A0, 864 bytes, `ret`.
// Callees: CThing::SetActive @ 0x004C7590, CThing::SetAsFrameUpdated @ 0x004C7C40, _ftol @ 0x00BFEA70,
//          CBaseIntelligentPointer GetPItem/ctor/SetPItem/dtor @ 0x00A01B50/0x00A01B10/0x00A01B90/0x00A01C10,
//          CThingPhysical::GetPosition() const @ 0x004C73D0, CTCGraphicAppearance::GetTopPoint(float) @ 0x004C1060,
//          CTCGraphicAppearance::GetDummyObject @ 0x004C08C0, CDefClassBase::GetSubDef<CParticleListDef> @ 0x00669580,
//          CParticleListDef::LookupParticle(const CCharString&, long*) @ 0x00669D30, CCharString(const char*, long)
//          @ 0x0099EBF0, CCharString::operator=(const char*) @ 0x0099EFE0, ~CCharString @ 0x0099EAE0,
//          CreateParticle(long, const C3DVector*, bool) @ 0x006E0880 (__fastcall), LowerBound @ 0x0040F020,
//          CTCDParticleEmitter::AttachToThing @ 0x006E0BE0. Global: frames per second (long) @ 0x01375550.
// Layout: CTCElectrocutedEffect (Ego_r.pdb): PDef +0x0C, CurTime +0x10, FadeInTime +0x14, FadeOutTimeStart +0x18,
//         FadeOutTime +0x1C, PEffect +0x20; retail CSpecialAbilitiesElectrocutedEffectDef (PDB - 8 past the vectors):
//         FadeInTimeSecs +0x3C, FadeOutTimeStartSecs +0x40, FadeOutTimeSecs +0x44, HeightSmall +0x64, HeightMedium +0x68.
class CCharString {
public:
    void* PStringData;
    CCharString(const char* s, long len = -1);      // 0x0099EBF0
    ~CCharString();                                 // 0x0099EAE0
    CCharString& operator=(const char* s);          // 0x0099EFE0
};
struct C3DVector { float X, Y, Z; };
struct CRightHandedSet { C3DVector Up; C3DVector Forward; };
extern long g_FramesPerSecond;                      // 0x01375550

class CDefClassBase {
public:
    virtual void v00();
    virtual void Destroy() const;                   // slot 1
    mutable long RefCount;                          // +0x04
};
template<class T> class CDefPointer {
public:
    T* Object;
    CDefPointer() : Object(0) {}
    CDefPointer(T* object) : Object(object)
    {
        if (Object)
            ++Object->RefCount;
    }
    ~CDefPointer()
    {
        if (Object && --Object->RefCount == 0)
            Object->Destroy();
    }
    T* operator->() const { return Object; }
};
class CParticleListDef : public CDefClassBase {
public:
    bool LookupParticle(const CCharString& name, long* index) const;   // 0x00669D30
};
class CDefWithParticles : public CDefClassBase {
public:
    bool GetSubDef(CDefPointer<const CParticleListDef>* out) const;   // 0x00669580
};
class CSpecialAbilitiesElectrocutedEffectDef : public CDefWithParticles {
public:
    unsigned char _pad08[0x3c - 0x08];
    float FadeInTimeSecs;                           // +0x3C
    float FadeOutTimeStartSecs;                     // +0x40
    float FadeOutTimeSecs;                          // +0x44
    unsigned char _pad48[0x64 - 0x48];
    float HeightSmall;                              // +0x64
    float HeightMedium;                             // +0x68
};

class CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);           // 0x0040F020
};
namespace NGraphicAppearance {
    class CTCGraphicAppearance {
    public:
        C3DVector GetTopPoint(float scale) const;   // 0x004C1060
        bool GetDummyObject(const CCharString& name, long index, unsigned long flags, C3DVector* position,
                            CRightHandedSet* orientation, float scale);   // 0x004C08C0
    };
}
class CThing {
public:
    virtual void v00();
    unsigned char _pad04[0x20 - 4];
    unsigned long PresentBits[9];                   // +0x20
    CInterfaceMap Interfaces;                       // +0x44
    unsigned char _pad4c[0x64 - 0x4c];
    NGraphicAppearance::CTCGraphicAppearance* PGraphicAppearance;   // +0x64
    unsigned char _pad68[0x70 - 0x68];
    CDefWithParticles* PDef;                        // +0x70
    unsigned char _pad74[0x91 - 0x74];
    unsigned char Flags91;                          // +0x91 (bit 0: being destroyed)
    void SetActive(bool active);                    // 0x004C7590
    void SetAsFrameUpdated();                       // 0x004C7C40
    const C3DVector& GetPosition() const;           // 0x004C73D0
    __forceinline bool HasTC(long type) const
    {
        if (PresentBits[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
    __forceinline CTCBase* GetTC(long type)
    {
        long key = type;
        CInterfaceMap* map = &Interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&key);
        CInterfaceMap::SEntry* end = map->last;
        return (it == end || type < it->type ? end : it)->component;
    }
};
CThing* __fastcall CreateParticle(long index, const C3DVector* position, bool flag);   // 0x006E0880

class CBaseIntelligentPointer {
public:
    virtual void v00();
    void* PData;
    CBaseIntelligentPointer();                      // 0x00A01B10
    ~CBaseIntelligentPointer();                     // 0x00A01C10
    CThing* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CThing* item);                    // 0x00A01B90
};
template<class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    CIntelligentPointer() {}
    CIntelligentPointer(T* item) { SetPItem(item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other)
    {
        SetPItem(other.GetPItem());
        return *this;
    }
};

class CTCDParticleEmitter {
public:
    void AttachToThing(CThing* thing, long mode, const CCharString& dummy, long index, float offset);   // 0x006E0BE0
};

class CTCElectrocutedEffect {
public:
    virtual void v00();
    CThing* Thing;                                  // +0x04
    unsigned char _pad08[4];
    const CSpecialAbilitiesElectrocutedEffectDef* PDef;   // +0x0C
    long CurTime;                                   // +0x10
    long FadeInTime;                                // +0x14
    long FadeOutTimeStart;                          // +0x18
    long FadeOutTime;                               // +0x1C
    CIntelligentPointer<CThing> PEffect;            // +0x20
    void Reset();
};

void CTCElectrocutedEffect::Reset()
{
    CThing* thing = Thing;
    thing->SetActive(true);
    thing->SetAsFrameUpdated();
    const CSpecialAbilitiesElectrocutedEffectDef* def = PDef;
    CurTime = 0;
    FadeInTime = (long)(def->FadeInTimeSecs * g_FramesPerSecond);
    FadeOutTimeStart = (long)(def->FadeOutTimeStartSecs * g_FramesPerSecond);
    FadeOutTime = (long)(def->FadeOutTimeSecs * g_FramesPerSecond);
    CDefPointer<const CParticleListDef> particleList;
    long particleIndex = -1;
    if (!PEffect.GetPItem() || (PEffect.GetPItem()->Flags91 & 1)) {
        bool foundDummy = false;
        C3DVector position = thing->GetPosition();
        CCharString effectName("FRAZZLED_EFFECT_MEDIUM");
        float baseZ = position.Z;
        C3DVector top;
        top.Z = PDef->HeightMedium + position.Z;
        NGraphicAppearance::CTCGraphicAppearance* appearance = thing->PGraphicAppearance;
        if (appearance) {
            top = appearance->GetTopPoint(1.0f);
            CRightHandedSet orientation;
            CCharString dummyName("DUMMY_CENTRE");
            foundDummy = appearance->GetDummyObject(dummyName, 0, 0, &position, &orientation, 1.0f);
        }
        CDefPointer<const CParticleListDef> thingParticles;
        {
            CDefPointer<const CDefWithParticles> thingDef(Thing->PDef);
            thingDef->GetSubDef(&thingParticles);
        }
        if (!thingParticles.Object || !thingParticles->LookupParticle(CCharString("ELECTROCUTED_EFFECT"), &particleIndex)) {
            float height = top.Z - baseZ;
            if (foundDummy)
                effectName = "FRAZZLED_EFFECT_POINT";
            else if (height < PDef->HeightSmall)
                effectName = "FRAZZLED_EFFECT_SMALL";
            else if (height > PDef->HeightMedium)
                effectName = "FRAZZLED_EFFECT_LARGE";
            if (PDef->GetSubDef(&particleList))
                particleList->LookupParticle(effectName, &particleIndex);
        }
        if (particleIndex != -1) {
            PEffect = CIntelligentPointer<CThing>(CreateParticle(particleIndex, &position, false));
            if (PEffect.GetPItem() && !(PEffect.GetPItem()->Flags91 & 1)) {
                CThing* effect = PEffect.GetPItem();
                if (effect->HasTC(0x4b)) {
                    CTCDParticleEmitter* emitter = (CTCDParticleEmitter*)effect->GetTC(0x4b);
                    if (emitter) {
                        if (foundDummy)
                            emitter->AttachToThing(thing, 10, CCharString("DUMMY_CENTRE"), 0, 0.0f);
                        else
                            emitter->AttachToThing(thing, 8, CCharString(""), 0, 0.0f);
                    }
                }
            }
        }
    }
}
