// Grade: DIFFER(551v551) -- scheduling only: retail loads AddTC's this (mov ecx,[esi+4]) before the three pushes after the CCharString temp; ours loads it after (lea ecx/push ecx then mov ecx). Best probe/daa/a.cpp; 14 variants tried.
// NCreatureMode::CModeThunderLightningStorm::CreateSkyBeam()
struct C3DVector { float X, Y, Z; };
struct CTCBase;
struct CTCParamsBase;
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);  // 0x0099EBF0
    ~CCharString();                                   // 0x0099EAE0
};
struct CSpecialEffectsDef {
    virtual void v00();
    virtual void Destroy();                           // slot 1
    long RefCount;                                    // +0x04
    bool CalcAndLookup(const CCharString& name, long* value);   // 0x00669D30
};
template<class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) { }
    ~CDefPointer() { if (Object) { if (--Object->RefCount == 0) Object->Destroy(); } }
    T* operator->() const { return Object; }
};
struct CDefClassBase {
    virtual void v00(); virtual void Destroy();
    long RefCount;                                    // +0x04
    bool GetSubDef(CDefPointer<CSpecialEffectsDef>* out);   // 0x00669580
};
void* Find_CDefClassBase_GetSubDef(const void* owner);
#pragma auto_inline(off)
__declspec(noinline) bool CDefClassBase::GetSubDef(CDefPointer<CSpecialEffectsDef>* out)
{
    void* def = Find_CDefClassBase_GetSubDef(this);
    if (!def)
        return false;
    out->Object = (CSpecialEffectsDef*)def;
    return true;
}
#pragma auto_inline(on)
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
#pragma auto_inline(off)
__declspec(noinline) CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{
    SEntry* it = first;
    long count = last - first;
    while (count > 0) {
        long half = count / 2;
        if (it[half].type < *type) {
            it += half + 1;
            count -= half + 1;
        } else
            count = half;
    }
    return it;
}
#pragma auto_inline(on)
struct CThing {
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                   // +0x20
    CInterfaceMap m_interfaces;                       // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char m_flags91;                          // +0x91 bit 0: dead
    const C3DVector& GetPosition() const;             // 0x004C73D0
    CTCBase* AddTC(const CCharString& name, bool activate, CTCParamsBase* params);   // 0x004C9D60
    __forceinline bool HasTC(long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)))
            return true;
        return false;
    }
    template<class T> __forceinline T* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
    }
    template<class T> __forceinline T* GetTCIfPresent(long interfaceType)
    {
        T* component = 0;
        if (HasTC(interfaceType))
            component = GetTC<T>(interfaceType);
        return component;
    }
};
inline bool IS_THING_ALIVE(const CThing* thing)
{
    if (thing && !(thing->m_flags91 & 1))
        return true;
    return false;
}
CThing* __fastcall CreateSpecialEffect(long effect, const C3DVector& position, bool attached);   // 0x006E0880
struct CTCDParticleEmitter {
    void AttachToThing(CThing* thing, long boneIndex, const CCharString& boneName, long a, float b);   // 0x006E0BE0
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                                  // +4
    CBaseIntelligentPointer();                        // 0x00A01B10
    ~CBaseIntelligentPointer();                       // 0x00A01C10
    CThing* GetPItem() const;                         // 0x00A01B50
    void SetPItem(CThing* item);                      // 0x00A01B90
};
struct CIntelligentPointerThing : CBaseIntelligentPointer {   // vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointerThing(CThing* item) { SetPItem(item); }
    CIntelligentPointerThing& operator=(const CIntelligentPointerThing& other) { CThing* item = other.GetPItem(); SetPItem(item); return *this; }
};
struct CVectorIP { void* a; void* b; void* c; };
namespace NCreatureMode {
struct CModeBase {
    virtual ~CModeBase();
    CThing*       m_pThing;                           // +0x04
    unsigned char _pad08[0x14];
};
struct CModeStandBase : CModeBase {
    unsigned char _pad1c[0x4c];
};
struct CModeThunderLightningStorm : CModeStandBase {
    unsigned char            Filter[0xc];             // +0x68
    CVectorIP                LightningBeamParticleEmitters;     // +0x74
    CVectorIP                LightningBeamEndParticleEmitters;  // +0x80
    CIntelligentPointerThing PSkyBeam;                // +0x8c
    CIntelligentPointerThing PCreatorEffect;          // +0x94
    CIntelligentPointerThing PHandGlow;               // +0x9c
    CDefClassBase*           PLightningStormDef;      // +0xa4
    void CreateSkyBeam();
};

void CModeThunderLightningStorm::CreateSkyBeam()
{
    CDefPointer<CSpecialEffectsDef> effects;
    long effect = 0;
    PLightningStormDef->GetSubDef(&effects);
    if (effects->CalcAndLookup("LIGHTNING_SKY_BEAM_EFFECT", &effect)) {
        C3DVector pos = m_pThing->GetPosition();
        CThing* beam = CreateSpecialEffect(effect, pos, false);
        if (IS_THING_ALIVE(beam))
            PSkyBeam = CIntelligentPointerThing(beam);
    }
    effect = 0;
    if (effects->CalcAndLookup("LIGHTNING_CREATOR_EFFECT", &effect)) {
        CThing* creator = CreateSpecialEffect(effect, m_pThing->GetPosition(), false);
        if (IS_THING_ALIVE(creator)) {
            PCreatorEffect = CIntelligentPointerThing(creator);
            CTCDParticleEmitter* emitter = creator->GetTCIfPresent<CTCDParticleEmitter>(0x4b);
            if (emitter)
                emitter->AttachToThing(m_pThing, 12, "", 2, 0.0f);
        }
    }
    if (!m_pThing->HasTC(0xee))
        m_pThing->AddTC("CTCElectrocuteOnTouch", false, 0);
}
}
