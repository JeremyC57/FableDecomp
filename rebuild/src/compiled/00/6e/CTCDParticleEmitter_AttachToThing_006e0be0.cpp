// CTCDParticleEmitter::AttachToThing(CThing& thing, long flags, const CCharString& posName, long posIndex, float offset) -- attach the emitter to a thing: clear the old attachment, record the flags/bone name/index/offset and the thing pointer, drop the emitter's thing from the thing's dependents, place the emitter, play the thing's "PARTICLE_<EMITTER NAME>" sound criteria when its entity sound def names one, and keep the emitter always updating when the thing ignores bullet time
// Leaf: ?AttachToThing@CTCDParticleEmitter@@QAEXAAVCThing@@JABVCCharString@@JM@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006E0BE0, 449 bytes, `ret 14h`.
// Callees: CTCDParticleEmitter::ClearAttachments 0x006E0AF0, CCharString::operator=(const CCharString&) 0x0099EFB0,
//   CBaseIntelligentPointer() 0x00A01B10 / GetPItem 0x00A01B50 / SetPItem 0x00A01B90 / ~CBaseIntelligentPointer 0x00A01C10
//   (CIntelligentPointer<CThing> vtable 0x01238C6C), CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound 0x0040F020,
//   CTCDependentThings::RemoveDependentThing(CThing*) 0x0071A940 (interface 0xd8; descriptive, AddDependentThing is
//   0x0071A8F0), CTCDParticleEmitter::UpdateEmitterThingAttachment(bool, float) 0x006DFFB0, CDefClassBase::GetSubDef
//   0x0065F604 (the CEntitySoundDef instantiation, as the landed 0x0066B1D0), CCharString::operator!=(const char*)
//   0x0099E960 ("" 0x0122D70E), CCharString::ToUpper 0x0099EE20, operator+(const char*, const CCharString&) 0x0099F690
//   (__fastcall, "PARTICLE_" 0x0125FFA4), ~CCharString 0x0099EAE0, CTCSound::PlaySound 0x0066BD00 (interface 0x10),
//   CThing::IsAffectedByBulletTime 0x004C9920.
// Layout: Ego_r.pdb CTCDParticleEmitter (PEmitter +0x14, PAttachedToThing +0x1c, AttachmentFlags +0x24,
//   AttachmentPosName +0x28, AttachmentPosIndex +0x2c, FloatOffset +0x30, AttachedToThing/AlwaysUpdate +0x45 bits 0/1);
//   CParticleEmitter (Ego_r - 0xc): AlwaysUpdate +0x44 bit 4, Name +0x140; CEntitySoundDef SoundIdentifier +0x48.
// Shape: the attachment is assigned through a CIntelligentPointer<CThing> temporary and the inline operator=; the
//   dependents lookup is the if-form GetInterface (as the landed ClearAttachments 0x006E0AF0) and the sound lookup the
//   one-ternary form, both over a same-TU LowerBound body; GetSubDef has a same-TU out-only stand-in (the def is cached in
//   ebp and reloaded from its slot); the emitter name is reached through a named emitter pointer; SetAlwaysUpdate(true)
//   is inline with one GetPItem.
struct CTCBase;
struct CThing;

struct CCharString {
    void* PStringData;
    ~CCharString();                                         // 0x0099EAE0
    CCharString& operator=(const CCharString& other);       // 0x0099EFB0
    bool operator!=(const char* text) const;                // 0x0099E960
    CCharString ToUpper() const;                            // 0x0099EE20
};
CCharString __fastcall operator+(const char* left, const CCharString& right);   // 0x0099F690

struct CDefBase {
    virtual void v00();
    virtual void Destroy();                                 // slot 1
    long RefCount;                                          // +0x04
};
template <class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) { }
    ~CDefPointer() { if (Object) { if (--Object->RefCount == 0) Object->Destroy(); } }
    T* operator->() const { return Object; }
};
struct CEntitySoundDef : CDefBase {
    unsigned char _pad08[0x48 - 0x08];
    CCharString SoundIdentifier;                            // +0x48
};
struct CDefClassBase {
    bool GetSubDef(CDefPointer<CEntitySoundDef>* out);      // 0x0065F604
};
CEntitySoundDef* FindRef_CDefClassBase_GetSubDef(CDefClassBase* defClass);   // stand-in callee of the stand-in body below
// Stand-in body (the real 0x0065F604 is an out-of-line template instantiation): it only writes the def through the out
// parameter, so VC7.1 knows the def pointer's address does not escape the call.
#pragma auto_inline(off)
bool CDefClassBase::GetSubDef(CDefPointer<CEntitySoundDef>* out)
{
    CEntitySoundDef* def = FindRef_CDefClassBase_GetSubDef(this);
    out->Object = def;
    return def != 0;
}
#pragma auto_inline(on)

struct CInterfaceMap {                                      // CVectorMap<ETCInterfaceType, CTCBase*>
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);                   // 0x0040F020
};
#pragma auto_inline(off)
// Same-TU body of the CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound instantiation: binary search for the first
// entry whose type is not below the key.
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
    unsigned long m_presentBits[9];                         // +0x20 interface-present bitset
    CInterfaceMap m_interfaces;                             // +0x44
    unsigned char _pad4c[0x70 - 0x4c];
    CDefClassBase* PDef;                                    // +0x70
    bool IsAffectedByBulletTime() const;                    // 0x004C9920
    template <class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
    // The same lookup with the one-ternary entry pick (retail loads the component straight into ecx at the sound call).
    template <class T> __forceinline bool GetTC(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
            return true;
        }
        return false;
    }
};
struct CTCDependentThings {                                 // interface 0xd8
    void RemoveDependentThing(CThing* thing);               // 0x0071A940
};
struct CTCSound {                                           // interface 0x10
    unsigned long PlaySound(const CCharString& name, long a, long b, long c, long d);   // 0x0066BD00
};

struct CBaseIntelligentPointer {
    virtual void v00();
    void* m_pItem;                                          // +0x04
    CBaseIntelligentPointer();                              // 0x00A01B10
    ~CBaseIntelligentPointer();                             // 0x00A01C10
    void* GetPItem() const;                                 // 0x00A01B50
    void SetPItem(void* item);                              // 0x00A01B90
};
template <class T> struct CIntelligentPointer : CBaseIntelligentPointer {   // CIntelligentPointer<CThing> vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointer(T* item) { SetPItem(item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); return *this; }
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
};

namespace NParticleEngine {
struct CParticleEmitter {
    unsigned char _pad00[0x44];
    bool PreWaterEffect : 1;                                // +0x44 bit 0
    bool WaterEffect : 1;
    bool ZBufferWriteable : 1;
    bool Visible : 1;
    bool AlwaysUpdate : 1;                                  // +0x44 bit 4
    bool DieIfOffsceen : 1;
    bool OffscreenUpdate : 1;
    bool ClipEffectToWeatherMask : 1;
    unsigned char _pad45[0x140 - 0x45];
    CCharString Name;                                       // +0x140
};
}

struct CTCDParticleEmitter {
    void*                                         __vftable;               // +0x00
    CThing*                                       ParentThing;             // +0x04 (CTCBase)
    unsigned char                                 _pad08[0x14 - 0x08];
    CIntelligentPointer<NParticleEngine::CParticleEmitter> PEmitter;      // +0x14
    CIntelligentPointer<CThing>                   PAttachedToThing;        // +0x1c
    unsigned long                                 AttachmentFlags;         // +0x24
    CCharString                                   AttachmentPosName;       // +0x28
    long                                          AttachmentPosIndex;      // +0x2c
    float                                         FloatOffset;             // +0x30
    unsigned char                                 _pad34[0x44 - 0x34];
    bool WaitingForParticleToLoad : 1;                      // +0x44 bit 0
    bool ForceParticleToLoad : 1;
    bool IndependantObject : 1;
    bool ParticleCreationDisabled : 1;
    bool ResetEmitterPosition : 1;
    bool ResetEmitterOrientation : 1;
    bool AttachedToCamera : 1;
    bool Moving : 1;
    bool AttachedToThing : 1;                               // +0x45 bit 0
    bool AlwaysUpdate : 1;                                  // +0x45 bit 1
    bool Asleep : 1;
    void ClearAttachments();                                // 0x006E0AF0
    void UpdateEmitterThingAttachment(bool force, float blend);   // 0x006DFFB0
    void SetAlwaysUpdate(bool update)
    {
        AlwaysUpdate = update;
        NParticleEngine::CParticleEmitter* emitter = PEmitter.GetPItem();
        if (emitter)
            emitter->AlwaysUpdate = update;
    }
    void AttachToThing(CThing& thing, long flags, const CCharString& posName, long posIndex, float offset);
};

void CTCDParticleEmitter::AttachToThing(CThing& thing, long flags, const CCharString& posName, long posIndex, float offset)
{
    ClearAttachments();
    AttachedToThing = true;
    AttachmentFlags = flags;
    AttachmentPosName = posName;
    AttachmentPosIndex = posIndex;
    PAttachedToThing = CIntelligentPointer<CThing>(&thing);
    FloatOffset = offset;
    CTCDependentThings* dependents;
    if (thing.GetInterface(&dependents, 0xd8))
        dependents->RemoveDependentThing(ParentThing);
    UpdateEmitterThingAttachment(true, 1.0f);
    CDefPointer<CEntitySoundDef> soundDef;
    if (thing.PDef->GetSubDef(&soundDef) && soundDef->SoundIdentifier != "") {
        if (PEmitter.GetPItem()) {
            NParticleEngine::CParticleEmitter* emitter = PEmitter.GetPItem();
            CCharString criteria = "PARTICLE_" + emitter->Name.ToUpper();
            CTCSound* sound;
            if (thing.GetTC(&sound, 0x10))
                sound->PlaySound(criteria, 1, 0, 0, -1);
        }
    }
    if (!thing.IsAffectedByBulletTime())
        SetAlwaysUpdate(true);
}