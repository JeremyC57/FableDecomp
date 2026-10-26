// CTCBulletTime::~CTCBulletTime() -- end the slowdown and the looping sound, remove the screen filter (and, unless a cutscene cancelled it, play the def's end sound), kill the looping particle effect, then the members and the CTCBase base
// Leaf: ??1CTCBulletTime
// Retail 0x0079D110, 444 bytes (plain `ret`), vtable 0x0126C49C.
// Callees: CTCBase::GetWorld @ 0x00686A70, bullet-time manager RemoveBulletTimeSlowdown @ 0x004C6140 (world +0x68),
// LowerBound @ 0x0040F020 (inlined GetInterface, sound emitter 0x10), StopSample @ 0x0066ADD0, filter removal @ 0x006B83F0,
// CDefStringTable::GetString(long) @ 0x009D49B0 on g_DefStringTable 0x013CA828, CTCBulletTime::PlaySoundW @ 0x0079CE80
// (retail passes the CCharString's address), ~CCharString @ 0x0099EAE0, CBaseIntelligentPointer GetPItem 0x00A01B50 /
// ctor 0x00A01B10 / SetPItem 0x00A01B90 / dtor 0x00A01C10, CParticleEmitter::Kill @ 0x00AEE8C0, ~CBaseClass @ 0x0099A2E0,
// CTCBase::~CTCBase @ 0x00686830.
// Layout: engine/CTCBulletTime.h offsets (its opaque member arrays carry no destructors, so the members are re-typed here):
// DisplacementEffectPrimitive +0x18, PDef +0x20, m_tpLoopingEffect +0x30, LoopSound +0x38, PItemRemoved +0x3c,
// HitCreatureBlur +0x44, ScreenFilterKey +0x50, CutSceneCancel +0x56.
class CCharString { public:
    void* PStringData;
    ~CCharString();                             // 0x0099EAE0
};
struct CDefStringTable {
    CCharString GetString(long id);             // 0x009D49B0
};
extern CDefStringTable g_DefStringTable;        // 0x013CA828
struct CTCBaseInterface;
struct CInterfaceMap {
    struct SEntry { long type; CTCBaseInterface* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
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
};
namespace NParticleEngine {
class CParticleEmitter { public:
    unsigned char _pad00[0x40];
    unsigned short Flags40 : 5;
    unsigned short Dead : 1;                    // +0x40 bit 5
    unsigned short Flags40High : 10;
    bool IsDead() const { return Dead; }
    void Kill();                                // 0x00AEE8C0
};
}
class CThingRef;
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
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:
    CIntelligentPointer(T* item) : CBaseIntelligentPointer() { SetPItem(item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other)
    {
        T* item = other.GetPItem();
        SetPItem(item);
        return *this;
    }

public:
    T* GetPItem() const { return reinterpret_cast<T*>(CBaseIntelligentPointer::GetPItem()); }
public:
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem(reinterpret_cast<CBaseObject*>(item)); }
};
struct CEnvironmentSpellEffects {
    void RemoveFilter_006b83f0(long mode, void* key);   // 0x006B83F0 (descriptive)
};
class CEnvironment { public:
    unsigned char _pad00[0x1964];
    CEnvironmentSpellEffects* SpellEffects;     // +0x1964
};
class CBulletTimeManager { public:
    void RemoveBulletTimeSlowdown(unsigned long key);   // 0x004C6140
};
class CWorld { public:
    unsigned char _pad00[0x18];
    CEnvironment* PEnvironment;                 // +0x18
    unsigned char _pad1c[0x68 - 0x1c];
    CBulletTimeManager* PBulletTimeManager;     // +0x68
    void RemoveBulletTimeSlowdown(unsigned long key) { PBulletTimeManager->RemoveBulletTimeSlowdown(key); }
};
struct CTCSoundEmitter {
    void StopSample(unsigned long sampleID);    // 0x0066ADD0
};
class CEnginePrimitiveHandle;
class CEnginePersistentPrimitiveBase {
public:
    virtual void v000();
    virtual void Destroy();                     // slot 1
    virtual void v002();
    virtual void OnLastHandleReleased(CEnginePrimitiveHandle* handle);  // slot 3
    long RefCount;                              // +0x04
    void Release() { if (--RefCount == 0) Destroy(); }
};
class CIVCountedPointer {
public:
    CEnginePersistentPrimitiveBase* Object;
    CEnginePersistentPrimitiveBase* operator->() const { return Object; }
    bool IsUnique() const { return Object != 0 && Object->RefCount == 1; }
    __forceinline void ReleaseRef() { if (Object) { Object->Release(); Object = 0; } }
    __forceinline CIVCountedPointer& operator=(int) { ReleaseRef(); Object = 0; return *this; }
    __forceinline ~CIVCountedPointer() { ReleaseRef(); }
};
class CBaseClass {
public:
    virtual void v000();
    virtual ~CBaseClass();                              // 0x0099A2E0
};
class CEnginePrimitiveHandle : public CBaseClass {      // vtable 0x0122F598
public:
    CIVCountedPointer Data;                     // +0x04
    virtual void v001();
    __forceinline void Clear()
    {
        if (Data.IsUnique())
            Data->OnLastHandleReleased(this);
        Data = 0;
    }
    __forceinline ~CEnginePrimitiveHandle() { Clear(); }
};
class CSpecialAbilitiesBulletTimeDef { public:
    virtual void v00();
    virtual void Destroy();                     // slot 1
    long RefCount;                              // +0x04
    unsigned char _pad08[0xd0 - 0x08];
    long EndSoundStringID;                      // +0xd0
    unsigned char _padd4[0x188 - 0xd4];
    long FilterRemoveMode;                      // +0x188
    void Release() { if (--RefCount == 0) Destroy(); }
};
struct CDefPointer_BulletTimeDef {
    const CSpecialAbilitiesBulletTimeDef* Object;
    ~CDefPointer_BulletTimeDef()
    {
        if (Object) {
            const_cast<CSpecialAbilitiesBulletTimeDef*>(Object)->Release();
            Object = 0;
        }
    }
    const CSpecialAbilitiesBulletTimeDef* operator->() const { return Object; }
};
class CTCBase { public:
    virtual ~CTCBase();                         // 0x00686830
    CThing* ParentThing;                        // +0x04
    bool ToBeRemoved;                           // +0x08
    bool IsInGlobalUpdateTCs;                   // +0x09
protected:
    CWorld& GetWorld() const;   // 0x00686A70
};
class CTCBulletTime : public CTCBase { public:
    long BulletTimeRemainingTicks;              // +0x0c
    long BulletTimeAbilityLevel;                // +0x10
    void* DisplacementMask;                     // +0x14
    CEnginePrimitiveHandle DisplacementEffectPrimitive;     // +0x18
    CDefPointer_BulletTimeDef PDef;             // +0x20
    long m_nLevel;                              // +0x24
    long m_nTimeMultiplier;                     // +0x28
    float m_fDuration;                          // +0x2c
    CIntelligentPointer<NParticleEngine::CParticleEmitter> m_tpLoopingEffect;  // +0x30
    unsigned long LoopSound;                    // +0x38
    CIntelligentPointer<CThingRef> PItemRemoved;        // +0x3c
    CIntelligentPointer<CThingRef> HitCreatureBlur;     // +0x44
    void* RadialBlurKey;                        // +0x4c
    void* ScreenFilterKey;                      // +0x50
    bool Pausable;                              // +0x54
    bool Paused;                                // +0x55
    bool CutSceneCancel;                        // +0x56
    __forceinline void StopSound(unsigned long sound)
    {
        CTCSoundEmitter* emitter;
        if (ParentThing->GetInterface(&emitter, 0x10))
            emitter->StopSample(sound);
    }
    unsigned long PlaySoundW(const CCharString& sound, unsigned long flags);   // 0x0079CE80
    virtual ~CTCBulletTime();
};

CTCBulletTime::~CTCBulletTime()
{
    GetWorld().RemoveBulletTimeSlowdown((unsigned long)this);
    StopSound(LoopSound);
    if (CutSceneCancel) {
        CWorld* world = (&GetWorld());
        world->PEnvironment->SpellEffects->RemoveFilter_006b83f0(0, ScreenFilterKey);
    } else {
        CWorld* world = (&GetWorld());
        world->PEnvironment->SpellEffects->RemoveFilter_006b83f0(PDef->FilterRemoveMode, ScreenFilterKey);
        CCharString sound = g_DefStringTable.GetString(PDef->EndSoundStringID);
        PlaySoundW(sound, 1);
    }
    CIntelligentPointer<NParticleEngine::CParticleEmitter>& effect = m_tpLoopingEffect;
    if (effect.GetPItem() && !effect.GetPItem()->IsDead()) {
        effect.GetPItem()->Kill();
        effect = CIntelligentPointer<NParticleEngine::CParticleEmitter>(0);
    }
}