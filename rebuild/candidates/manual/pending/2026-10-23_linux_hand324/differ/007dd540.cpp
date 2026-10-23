// CTCCreatureFlyInCircles::UpdateSwoop(C3DVector& position, C3DVector& velocity) -- one step of the dragon's swoop: advance along the swoop spline (velocity = normalised step), and while the target lives keep the spline's mid point on it, drop DRAGON_FIRE_TRAIL explosions under the fire line every other frame and (re)spawn the BURNINATE effect on the mouth dummy; past the burn window kill the effect; at the end of the spline (or without a target) return to circling
// Grade: DIFFER(1074v1099) -- frame 0x40 v 0x4c: retail keeps the 12-byte step/fire-position temp at +0x2c, clear of the scalar words +0x14..+0x1f (ours overlays it on effectType at +0x14); then register-only diffs in the GetEffectFromString call and the emitter lookup (ours keeps the effect thing in edi)
// Leaf: UpdateSwoop
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007DD540, 1104 bytes, `ret 8`.
// Callees: CCardinalSpline::GetSplinePosition 0x00A567A0, C3DVector::Normalise 0x00A14440, CBaseIntelligentPointer GetPItem
// 0x00A01B50 / ctor 0x00A01B10 / SetPItem 0x00A01B90 / dtor 0x00A01C10, CThing::GetPosition 0x004C73D0, GFGetCurrentFrame 0x0049D870,
// CGameDefinitionManager::Get 0x0044C6B0, GetDefGlobalIndexFromName 0x009AD410, CTCBase::PeekWorldMap 0x00686D20 / GetWorldMap
// 0x00686D30, CWorldMap::IsPosInMap 0x004FC600 / GetHeightZBelowPos 0x0063E480, CreateThing 0x0080C460 (__fastcall), LowerBound
// 0x0040F020 (inlined GetTC 0x41 / 0x4B), CTCDExplosion::AddExcludedThing 0x0080CBF0, IS_THING_ALIVE 0x004CC340,
// CTCGraphicAppearance::GetDummyObject 0x004C08C0, GetSubDef 0x00669580, GetEffectFromString 0x00669D30, CreateEffectThing
// 0x006E0880, CIntelligentPointer<CThing>(CThing*) 0x004A96F0 / operator= 0x004A9760, AttachToThing 0x006E0BE0, CThing::Kill 0x004C9B80.
// Layout: the landed CTCCreatureFlyInCircles ctor 0x007DDF90 model (packed: PBurninationEffect +0x3b, PSwoopTarget +0x44,
// SwoopSpline +0x4c, SwoopTargOffset +0x60, SwoopProgress +0x6c, SwoopDelta +0x70, LastExplodeTick +0x74).
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    __forceinline C3DVector operator+(const C3DVector& o) const { return C3DVector(X + o.X, Y + o.Y, Z + o.Z); }
    __forceinline C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    __forceinline C3DVector operator*(float s) const { return C3DVector(X * s, Y * s, Z * s); }
    void Normalise();                                        // 0x00A14440
};
struct CRightHandedSet { C3DVector Up, Forward; };
struct CCharString {
    void* m_pData;
    CCharString(const char* text, long length = -1);         // 0x0099EBF0
    ~CCharString();                                          // 0x0099EAE0
};
struct CDefVirtual {
    virtual void v00();
    virtual void Destroy();                                  // slot 1
};
struct CDefBase : CDefVirtual {
    long m_refCount;                                         // +0x04
};
long Find_CSpecialEffectsDef_GetEffectFromString(const void* def, const CCharString& name);
struct CSpecialEffectsDef : CDefBase {
    bool GetEffectFromString(const CCharString& name, long& type) const;   // 0x00669D30
};
#pragma auto_inline(off)
__declspec(noinline) bool CSpecialEffectsDef::GetEffectFromString(const CCharString& name, long& type) const
{
    long found = Find_CSpecialEffectsDef_GetEffectFromString(this, name);
    if (found < 0)
        return false;
    type = found;
    return true;
}
#pragma auto_inline(on)
template <class T> struct CDefPointer {
    T* m_p;
    CDefPointer() : m_p(0) { }
};
template <class T> __forceinline void ReleaseDef(T* def)
{
    if (def && --def->m_refCount == 0)
        def->Destroy();
}
void* FindSubDef_CDefClassBase_GetSubDef(void* def);
struct CDefClassBase : CDefBase {
    template <class T> bool GetSubDef(CDefPointer<T>& out);   // 0x00669580
};
#pragma auto_inline(off)
template <class T> __declspec(noinline) bool CDefClassBase::GetSubDef(CDefPointer<T>& out)
{
    void* def = FindSubDef_CDefClassBase_GetSubDef(this);
    if (!def)
        return false;
    out.m_p = (T*)def;
    return true;
}
#pragma auto_inline(on)
struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);                    // 0x0040F020
};
struct CTCGraphicAppearance {
    bool GetDummyObject(const CCharString& name, long index, unsigned long flags, C3DVector* pos, CRightHandedSet* set, float scale);   // 0x004C08C0
};
struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                          // +0x20
    CInterfaceMap m_interfaces;                              // +0x44
    unsigned char _pad4c[0x64 - 0x4c];
    CTCGraphicAppearance* m_pAppearance;                     // +0x64
    unsigned char _pad68[0x70 - 0x68];
    CDefClassBase* m_pDef;                                   // +0x70
    unsigned char _pad74[0x91 - 0x74];
    unsigned char m_flags91;                                 // +0x91 bit 0: dead
    const C3DVector& GetPosition() const;                    // 0x004C73D0
    void Kill(bool immediately);                             // 0x004C9B80
    template<class T> __forceinline T* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
    }
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
    __forceinline unsigned long HasTC(long interfaceType) const
    {
        return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31));
    }
};
CThing* __fastcall CreateEffectThing(long type, const C3DVector* position, bool flag);   // 0x006E0880
CThing* __fastcall CreateThing(long defIndex, const C3DVector& position);              // 0x0080C460
long __fastcall GFGetCurrentFrame();                         // 0x0049D870
struct CDefinitionManager {
    long GetDefGlobalIndexFromName(const CCharString& name); // 0x009AD410
};
struct CGameDefinitionManager : CDefinitionManager {
    static CGameDefinitionManager& __fastcall Get();         // 0x0044C6B0
};
struct CWorldMap {
    bool IsPosInMap(const C3DVector& pos) const;             // 0x004FC600
    float GetHeightZBelowPos(const C3DVector& pos);          // 0x0063E480
};
struct CTCDParticleEmitter {
    void AttachToThing(CThing* thing, long attachType, const CCharString& helperName, long helperIndex, float offset);   // 0x006E0BE0
};
struct CTCDExplosion {
    void AddExcludedThing(CThing* thing);                    // 0x0080CBF0
};
struct CThingCreatureBase : CThing { };

struct CVector3D {
    C3DVector* _M_start;
    C3DVector* _M_finish;
    C3DVector* _M_end_of_storage;
};
struct CCardinalSpline {                                     // retail 0x14 bytes
    CVector3D ControlPoints;                                 // +0x00
    float     Tension;                                       // +0x0c
    bool      EndPointsConnected;                            // +0x10
    C3DVector GetSplinePosition(float progress) const;       // 0x00A567A0
};
#pragma pack(push, 1)
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                                         // +0x04
    CBaseIntelligentPointer();                               // 0x00A01B10
    ~CBaseIntelligentPointer();                              // 0x00A01C10
    CThing* GetPItem() const;                                // 0x00A01B50
    void SetPItem(CThing* item);                             // 0x00A01B90
};
struct CIntelligentPointerThing : CBaseIntelligentPointer {  // CIntelligentPointer<CThing>, vtable 0x01238C6C
    virtual void v01();
    CIntelligentPointerThing() { SetPItem(0); }
    CIntelligentPointerThing(CThing* item);                  // 0x004A96F0
    CIntelligentPointerThing& operator=(const CIntelligentPointerThing& other);   // 0x004A9760
};
struct CIntelligentPointerConstCreature : CBaseIntelligentPointer {   // vtable 0x0125AAEC
    virtual void v01();
    CIntelligentPointerConstCreature(CThing* item) { SetPItem(item); }
    CIntelligentPointerConstCreature& operator=(const CIntelligentPointerConstCreature& other) { SetPItem(other.GetPItem()); return *this; }
};
template <class T> bool __fastcall IS_THING_ALIVE(const T& thing);   // 0x004CC340
struct CTCCreatureFlyInCircles {
    virtual void v00();
    CThing* ParentThing;                                     // +0x04
    bool  ToBeRemoved;                                       // +0x08
    bool  IsInGlobalUpdateTCs;                               // +0x09
    unsigned char _pad0a[2];                                 // +0x0a
    CThingCreatureBase* Creature;                            // +0x0c
    C3DVector Centre;                                        // +0x10
    float Speed;                                             // +0x1c
    float DeltaSpeed;                                        // +0x20
    long  SpeedChangeTicks;                                  // +0x24
    float CurrentRadius;                                     // +0x28
    float DeltaRadius;                                       // +0x2c
    long  RadiusChangeTicks;                                 // +0x30
    long  Mode;                                              // +0x34 EFlyMode
    bool  Transitioning;                                     // +0x38
    bool  ModeChangeQueued;                                  // +0x39
    bool  Active;                                            // +0x3a
    CIntelligentPointerThing PBurninationEffect;             // +0x3b
    bool  HasBeenHit;                                        // +0x43
    CIntelligentPointerConstCreature PSwoopTarget;           // +0x44
    CCardinalSpline SwoopSpline;                             // +0x4c
    C3DVector SwoopTargOffset;                               // +0x60
    float SwoopProgress;                                     // +0x6c
    float SwoopDelta;                                        // +0x70
    long  LastExplodeTick;                                   // +0x74
    const CWorldMap& PeekWorldMap() const;                   // 0x00686D20 (CTCBase)
    CWorldMap* GetWorldMap();                                // 0x00686D30 (CTCBase)
    void UpdateSwoop(C3DVector& position, C3DVector& velocity);
};
#pragma pack(pop)

void CTCCreatureFlyInCircles::UpdateSwoop(C3DVector& position, C3DVector& velocity)
{
    long effectType;
    SwoopProgress += SwoopDelta;
    C3DVector splinePos = SwoopSpline.GetSplinePosition(SwoopProgress);
    velocity = splinePos - position;
    velocity.Normalise();
    position = splinePos;
    if (SwoopProgress >= 1.0f || !PSwoopTarget.GetPItem() || (PSwoopTarget.GetPItem()->m_flags91 & 1)) {
        Mode = 1;
        PSwoopTarget = CIntelligentPointerConstCreature(0);
        return;
    }
    if (SwoopProgress < 0.45f) {
        if (SwoopProgress < 0.3f)
            SwoopSpline.ControlPoints._M_start[1] = PSwoopTarget.GetPItem()->GetPosition() + SwoopTargOffset;
        if (GFGetCurrentFrame() - LastExplodeTick > 1) {
            long defIndex = CGameDefinitionManager::Get().GetDefGlobalIndexFromName("DRAGON_FIRE_TRAIL");
            C3DVector firePos = velocity * 60.0f + splinePos;
            if (PeekWorldMap().IsPosInMap(firePos)) {
                firePos.Z = 10000.0f;
                firePos.Z = GetWorldMap()->GetHeightZBelowPos(firePos);
                CThing* explosion = CreateThing(defIndex, firePos);
                if (explosion) {
                    CTCDExplosion* explosionTC;
                    if (explosion->GetInterface(&explosionTC, 0x41))
                        explosionTC->AddExcludedThing(Creature);
                    LastExplodeTick = GFGetCurrentFrame();
                }
            }
        }
        if (!IS_THING_ALIVE(PBurninationEffect)) {
            CDefPointer<CSpecialEffectsDef> effectsSlot;
            effectType = 0;
            C3DVector mouthPos;
            CRightHandedSet mouthSet;
            Creature->m_pAppearance->GetDummyObject("MOUTH_DUMMY", 0, 0, &mouthPos, &mouthSet, 1.0f);
            bool found = Creature->m_pDef->GetSubDef(effectsSlot);
            if (found && effectsSlot.m_p->GetEffectFromString("BURNINATE", effectType)) {
                PBurninationEffect = CreateEffectThing(effectType, &mouthPos, false);
                if (IS_THING_ALIVE(PBurninationEffect)) {
                    CThing* effect = PBurninationEffect.GetPItem();
                    if (effect->HasTC(0x4b))
                        effect->GetTC<CTCDParticleEmitter>(0x4b)->AttachToThing(Creature, 0x2a, "MOUTH_DUMMY", 0, 0.0f);
                }
            }
            ReleaseDef(effectsSlot.m_p);
        }
    } else if (PBurninationEffect.GetPItem() && !(PBurninationEffect.GetPItem()->m_flags91 & 1)) {
        PBurninationEffect.GetPItem()->Kill(true);
        PBurninationEffect.SetPItem(CIntelligentPointerThing().GetPItem());
    }
}
