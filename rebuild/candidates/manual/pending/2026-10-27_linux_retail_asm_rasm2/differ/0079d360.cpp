// CTCBulletTime::CTCBulletTime(CThing&) -- build the bullet-time TC: members, the def from "SPECIAL_ABILITIES_BULLET_TIME_DEF", the hero's ability level (its stats TC 0x6F), duration/multiplier from the def, the noise, start and loop sounds, the "CAST" particle effect, the slowdown, the remaining ticks (rounded), the screen filter and the carried-item hide
// Leaf: ??0CTCBulletTime
// Retail-asm: 0x0079D65F..0x0079D67F `fld [ebp - 8] ; fadd [ebp - 0xc] ; fistp [ebp + 8] ; fild [ebp + 8] ; fsub [ebp - 0x10] ; fstp [ebp - 0x14] ; mov eax, [ebp - 8] ; cmp eax, [ebp - 0x14] ; jne 0x79d67c ; dec [ebp + 8] ; mov eax, [ebp + 8] ; mov [esi + 0xc], eax` -- the engine's inline-assembly ceiling helper expanded in place (0.5f / 1.0f stored as frame constants, a bare dword fistp into a named long, the bit compare and `jne ; dec dword ptr` in the same 10-instruction block: rasm1 family (b)). VC7.1 emits no bare dword fistp from C++ (`(long)x` calls __ftol2, /QIfist gives `fistp qword`, /arch:SSE2 cvttss2si: rasm1 probe family (a)); the result is read back by C++ after the block.
// Retail: Fable.exe (TLC, VC7.1) @ 0x0079D360; reconstructed from the parked reference body (rebuild/candidates/manual/pending), the C++ rounding stand-in replaced by the inline-assembly helper. The remaining ticks are the ceiling block.
#pragma optimize("y", off)
// the engine's inline-assembly FPU ceiling helper (0x004FA9A0 is its standalone copy): VC7.1 expands the __forceinline body in
// place; the by-value parameter and the four locals become frame words, which is why the function carries an ebp frame.
__forceinline long CeilToLong(float value)
{
    long  result;
    float one;
    float half;
    float back;
    half = 0.5f;
    one = 1.0f;
    __asm {
        fld   value
        fadd  half
        fistp result
        fild  result
        fsub  one
        fstp  back
        mov   eax, value
        cmp   eax, back
        jne   done
        dec   result
    done:
    }
    return result;
}
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
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
struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    void PlaySoundCriteria(const CCharString& name);    // 0x00662110
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
struct CParticleEmitter {
    unsigned char _pad00[0x40];
    unsigned short Flags40 : 5;
    unsigned short Dead : 1;                    // +0x40 bit 5
    unsigned short Flags40High : 10;
    bool IsDead() const { return Dead; }
    void Kill();                                // 0x00AEE8C0
};
}
class CThingRef;
struct CBaseIntelligentPointer {
    void* m_pItem;
    CBaseIntelligentPointer();                  // 0x00A01B10
    virtual ~CBaseIntelligentPointer();         // 0x00A01C10
    NParticleEngine::CParticleEmitter* GetPItem() const;   // 0x00A01B50
    void SetPItem(void* item);                  // 0x00A01B90
};
template <class T> struct CIntelligentPointer : CBaseIntelligentPointer {
    CIntelligentPointer(T* item) : CBaseIntelligentPointer() { SetPItem(item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other)
    {
        T* item = other.GetPItem();
        SetPItem(item);
        return *this;
    }
};
struct CEnvironmentSpellEffects {
    void* AddFilter(long mode, long a, long b, long c, long d, const float* colour, const void* list);   // 0x006B85D0
    void RemoveFilter_006b83f0(long mode, void* key);   // 0x006B83F0 (descriptive)
};
struct CEnvironment {
    unsigned char _pad00[0x1964];
    CEnvironmentSpellEffects* SpellEffects;     // +0x1964
};
struct CBulletTimeManager {
    void AddBulletTimeSlowdown(long multiplier, void* key, float amount);   // 0x004C6170
    void RemoveBulletTimeSlowdown(unsigned long key);   // 0x004C6140
};
struct CWorld {
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
    CBaseClass();                               // 0x0099A2D0
    virtual void v000();
    ~CBaseClass();                              // 0x0099A2E0
};
class CEnginePrimitiveHandle : public CBaseClass {      // vtable 0x0122F598
public:
    CIVCountedPointer Data;                     // +0x04
    virtual void v001();
    CEnginePrimitiveHandle() { Data.Object = 0; }
    __forceinline void Clear()
    {
        if (Data.IsUnique())
            Data->OnLastHandleReleased(this);
        Data = 0;
    }
    __forceinline ~CEnginePrimitiveHandle() { Clear(); }
};
struct CSpecialAbilitiesBulletTimeDef {
    virtual void v00();
    virtual void Destroy();                     // slot 1
    long RefCount;                              // +0x04
    unsigned char _pad08[0x3c - 0x08];
    float Durations[4];                         // +0x3c
    long TimeMultipliers[4];                    // +0x4c
    unsigned char _pad5c[0xc8 - 0x5c];
    long StartSoundStringID;                    // +0xc8
    long LoopSoundStringID;                     // +0xcc
    long EndSoundStringID;                      // +0xd0
    unsigned char _padd4[0x130 - 0xd4];
    long* FilterA;                              // +0x130
    unsigned char _pad134[0x13c - 0x134];
    long* FilterB;                              // +0x13c
    unsigned char _pad140[0x148 - 0x140];
    long* FilterC;                              // +0x148
    unsigned char _pad14c[0x154 - 0x14c];
    long* FilterD;                              // +0x154
    unsigned char _pad158[0x160 - 0x158];
    float* ColourR;                             // +0x160
    unsigned char _pad164[0x16c - 0x164];
    float* ColourG;                             // +0x16c
    unsigned char _pad170[0x178 - 0x170];
    float* ColourB;                             // +0x178
    unsigned char _pad17c[0x184 - 0x17c];
    long FilterMode;                            // +0x184
    long FilterRemoveMode;                      // +0x188
    void Release() { if (--RefCount == 0) Destroy(); }
};
struct CDefPointer_BulletTimeDef {
    const CSpecialAbilitiesBulletTimeDef* Object;
    CDefPointer_BulletTimeDef() : Object(0) { }
    ~CDefPointer_BulletTimeDef()
    {
        if (Object) {
            const_cast<CSpecialAbilitiesBulletTimeDef*>(Object)->Release();
            Object = 0;
        }
    }
    const CSpecialAbilitiesBulletTimeDef* operator->() const { return Object; }
};
struct CDefinitionManager { long GetDefGlobalIndex(const CCharString& name, bool warn); };   // 0x009AD390
CDefinitionManager* __fastcall PeekDefinitionManagerFor(const CCharString& name, bool warn);    // 0x0044C6B0 (stand-in)
struct CSpecialAbilitiesDefLoader { void Load(CDefPointer_BulletTimeDef* out); };            // 0x0079DBB0
CSpecialAbilitiesDefLoader* __fastcall GetLoader(long index, CDefPointer_BulletTimeDef* out); // 0x0044C6B0
struct CSpecialEffectsDef {
    virtual void v00(); virtual void Destroy(); long RefCount;
    bool GetEffectFromString(const CCharString& name, long* effect);   // 0x00669D30
};
struct CDefPointer_EffectsDef {
    CSpecialEffectsDef* Object;
    CDefPointer_EffectsDef() : Object(0) { }
    ~CDefPointer_EffectsDef() { if (Object && --Object->RefCount == 0) Object->Destroy(); }
};
bool GetSubDef(const CSpecialAbilitiesBulletTimeDef* def, CDefPointer_EffectsDef* out);   // 0x00669580
struct C3DVector { float X, Y, Z; };
struct SEffectPlacement { C3DVector Up; C3DVector Forward; };
struct C2DCoord { float X, Y; };
float SinOf(float angle);                               // 0x009E1E40
float CosOf(float angle);                               // 0x009E1E10
struct CParticleEngine { NParticleEngine::CParticleEmitter* GetEmitter(); };   // 0x00AEB790
CParticleEngine* CreateParticle(long effect, const C2DCoord& position, long kind, bool attached, const SEffectPlacement& placement);   // 0x00AEAAF0
extern double g_SinAngle;                              // 0x013961F0
extern double g_CosAngle;                              // 0x013961E8
extern long g_FramesPerSecond;                         // 0x01375550
struct CTCCarrying { void SetFilterKey(void* key); };  // 0x006A2520
struct CThingPtrVector {
    CBaseIntelligentPointer* First; CBaseIntelligentPointer* Last; CBaseIntelligentPointer* End;
    CThingPtrVector() : First(0), Last(0), End(0) { }
    void push_back(const CBaseIntelligentPointer& p);  // 0x006A3C70
};
struct CTCBase {
    CTCBase(CThing& thing);                     // 0x00686800
    virtual ~CTCBase();                         // 0x00686830
    CThing* ParentThing;                        // +0x04
    bool ToBeRemoved;                           // +0x08
    bool IsInGlobalUpdateTCs;                   // +0x09
    CWorld* GetWorld();                         // 0x00686A70
};
struct CTCBulletTime : CTCBase {
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
    CTCBulletTime(CThing& thing);
};

CTCBulletTime::CTCBulletTime(CThing& thing) : CTCBase(thing), BulletTimeAbilityLevel(0), m_nLevel(0),
    m_tpLoopingEffect(0), PItemRemoved(0), HitCreatureBlur(0), RadialBlurKey(0), ScreenFilterKey(0), Pausable(true), Paused(false), CutSceneCancel(false)
{
    CThing* parent = ParentThing;
    long index = PeekDefinitionManagerFor(CCharString("SPECIAL_ABILITIES_BULLET_TIME_DEF"), true)->GetDefGlobalIndex(CCharString("SPECIAL_ABILITIES_BULLET_TIME_DEF"), true);
    GetLoader(index, &PDef)->Load(&PDef);
    m_fDuration = PDef->Durations[m_nLevel];
    m_nTimeMultiplier = PDef->TimeMultipliers[m_nLevel];
    m_tpLoopingEffect = CIntelligentPointer<NParticleEngine::CParticleEmitter>(0);
    if (parent->m_presentBits[3] & 0x8000)
        ;
    m_nTimeMultiplier = PDef->TimeMultipliers[m_nLevel];
    m_fDuration = PDef->Durations[m_nLevel];
    parent->PlaySoundCriteria("SIMPLE_NOISE");
    PlaySoundW(g_DefStringTable.GetString(PDef->StartSoundStringID), 1);
    LoopSound = PlaySoundW(g_DefStringTable.GetString(PDef->LoopSoundStringID), 1);
    CDefPointer_EffectsDef effects;
    long effect = 0;
    if (GetSubDef(PDef.Object, &effects) && effects.Object->GetEffectFromString("CAST", &effect)) {
        SEffectPlacement placement = { { 0.0f, 0.0f, 1.0f }, { 0.0f, 1.0f, 0.0f } };
        C2DCoord position;
        position.Y = SinOf((float)g_SinAngle) * 0.5f;
        position.X = CosOf((float)g_CosAngle) * 0.5f;
        NParticleEngine::CParticleEmitter* emitter = CreateParticle(effect, position, 0x52, true, placement)->GetEmitter();
        if (emitter && !emitter->IsDead())
            *((unsigned char*)emitter + 0x44) |= 0x10;
    }
    GetWorld()->PBulletTimeManager->AddBulletTimeSlowdown(m_nTimeMultiplier, this, 0.5f);
    BulletTimeRemainingTicks = CeilToLong(g_FramesPerSecond * m_fDuration);
    CThingPtrVector things;
    things.push_back(CIntelligentPointer<NParticleEngine::CParticleEmitter>((NParticleEngine::CParticleEmitter*)parent));
    CEnvironmentSpellEffects* spell = GetWorld()->PEnvironment->SpellEffects;
    float colour[4];
    colour[0] = PDef->ColourR[m_nLevel];
    colour[1] = PDef->ColourG[m_nLevel];
    colour[2] = PDef->ColourB[m_nLevel];
    colour[3] = 1.0f;
    void* key = spell->AddFilter(PDef->FilterMode, 2, PDef->FilterA[m_nLevel], PDef->FilterB[m_nLevel], PDef->FilterC[m_nLevel], colour, &things);
    ScreenFilterKey = key;
    CTCCarrying* carrying;
    if (parent->GetInterface(&carrying, 0x46))
        carrying->SetFilterKey(key);
}
