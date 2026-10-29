// CTCKraken::UpdateBeamExplosion() -- while the target and the beam effect live and the kraken's beam-explosion interval has passed, spawns KRAKEN_BEAM_EXPLOSION along the beam from the mouth dummy (or the effect's own facing) and, when the beam has no line of sight to its end, a BEAM_HIT effect at the hit point
// Retail: Fable.exe (TLC, VC7.1) @ 0x007FBE50, 955 bytes (+ int3 padding), plain `ret`, speed TU (no pragmas, no frame pointer).
// Callees: CWorld::GetFrame 0x0049D870, CBaseIntelligentPointer::GetPItem 0x00A01B50, CThing::GetPos 0x004C73D0,
//   CCharString 0x0099EBF0 / ~CCharString 0x0099EAE0, CTCGraphicAppearance::GetDummyObject 0x004C08C0, physics slot 70 (+0x118)
//   GetFacing, CTCBase GetDefinitionManager 0x006869D0, CDefinitionManager::GetDefGlobalIndexFromName 0x009AD410, CreateThing
//   0x0080C500 (`__fastcall`), CExplosionDriver::AddExcludedThing 0x0080CBF0, CTCKraken::AddStrikeExplosion 0x007FBA20,
//   GetSubDef 0x00669580 / GetEffectFromString 0x00669D30 (same-TU noinline stand-ins, as StartBeamEffect 0x007FB6A0),
//   CTCBase GetDisplayEngine 0x00686A70 (engine+0x34 slot 10 hands back the camera manager), CTools::IsLineOfSightFiltered
//   0x00877C40, CreateEffectThing 0x006E0880 (`__fastcall`). Global 0x013BAF10 = frame of the last beam explosion.
// Strings: "MOUTH_DUMMY" 0x0126FA88, "KRAKEN_BEAM_EXPLOSION" 0x01272098, "BEAM_HIT" 0x0127208C; -1.0f at 0x0122DEE0.
// Layout (local view): retail CTCKraken PKrakenDef +0x0C, PTarget +0x20, PBeamWeaponEffect +0x34, Mode +0x64 (PDB +0x74: the
//   two STLport lists are 4 bytes in retail, 12 in the PDB); kraken def +0x54 extra beam length, +0x60 / +0x64 beam range
//   (mode 4 / otherwise), +0x68 beam-explosion interval (frames). CThing PTCDriver +0x5C, physics +0x60, appearance +0x64,
//   def +0x70, dead bit +0x91; CGameCameraManager CamTools +0x78.
// Shape: C3DVector needs a USER copy constructor (without it the beam-end sum keeps the product's Y in st(1) and loads start.Y
//   first, +4 bytes); the physics body and the graphic appearance go through inline CThing accessors (a direct member chain
//   loads them into eax / after the pushes); the beam direction * length product is written twice (the Create end argument
//   and the line-of-sight end) and VC keeps it in the frame; the driver is the uninitialised GetTCDriver out-pointer.
class C3DVector { public:
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) {}
    C3DVector operator*(float s) const { return C3DVector(X * s, Y * s, Z * s); }
    C3DVector operator+(const C3DVector& o) const { return C3DVector(X + o.X, Y + o.Y, Z + o.Z); }
};
class CRightHandedSet { public: C3DVector Up; C3DVector Forward; };
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);  // 0x0099EBF0
    ~CCharString();                                   // 0x0099EAE0
};
struct CDefVirtual {
    virtual ~CDefVirtual();
    virtual void Destroy();
};
struct CDefBase : CDefVirtual {
    long m_refCount;
};
class CSpecialEffectsDef : public CDefBase { public:
    bool GetEffectFromString(const CCharString& name, long& type) const;   // 0x00669D30
};
long Lookup_CSpecialEffectsDef_GetEffectFromString(const void* def, const CCharString& name);
#pragma auto_inline(off)
__declspec(noinline) bool CSpecialEffectsDef::GetEffectFromString(const CCharString& name, long& type) const
{
    long found = Lookup_CSpecialEffectsDef_GetEffectFromString(this, name);
    if (found < 0)
        return false;
    type = found;
    return true;
}
#pragma auto_inline(on)
template <class T> class CDefPointer { public:
    T* m_p;
    CDefPointer() : m_p(0) { }
    ~CDefPointer() { if (m_p && --m_p->m_refCount == 0) m_p->Destroy(); }
    T* operator->() const { return m_p; }
};
class CDefClassBase : public CDefBase { public:
    template <class T> bool GetSubDef(CDefPointer<T>& out);   // 0x00669580
};
void* Find_CDefClassBase_GetSubDef(const void* owner);
#pragma auto_inline(off)
template <class T> __declspec(noinline) bool CDefClassBase::GetSubDef(CDefPointer<T>& out)
{
    void* def = Find_CDefClassBase_GetSubDef(this);
    if (!def)
        return false;
    out.m_p = (T*)def;
    return true;
}
#pragma auto_inline(on)
struct CDefClassReference {
    CDefClassBase* m_p;
    CDefClassReference(CDefClassBase* p) : m_p(p) { if (m_p) ++m_p->m_refCount; }
    ~CDefClassReference() { if (m_p && --m_p->m_refCount == 0) m_p->Destroy(); }
    CDefClassBase* operator->() const { return m_p; }
};
struct CTCGraphicAppearance {
    bool GetDummyObject(const CCharString& name, long index, unsigned long flags, C3DVector* position,
                        CRightHandedSet* orientation, float scale);   // 0x004C08C0
};
struct CPhysicsBody {
#define VF(n) virtual void v##n();
#define VF10(p) VF(p##0) VF(p##1) VF(p##2) VF(p##3) VF(p##4) VF(p##5) VF(p##6) VF(p##7) VF(p##8) VF(p##9)
    VF10(0) VF10(1) VF10(2) VF10(3) VF10(4) VF10(5) VF10(6)
    virtual C3DVector GetFacing() const;              // slot 70 (+0x118)
};
class CThing;
struct CExplosionDriver {
    void AddExcludedThing(CThing* thing);             // 0x0080CBF0
};
class CThing {
public:
    unsigned char  _pad00[0x20];
    unsigned long  m_presentBits[9];                  // +0x20
    unsigned char  _pad44[0x5c - 0x44];
    CExplosionDriver* PTCDriver;                      // +0x5c
    CPhysicsBody*  PhysicsBody;                       // +0x60
    CTCGraphicAppearance* PGraphicAppearance;         // +0x64
    unsigned char  _pad68[0x70 - 0x68];
    CDefClassBase* m_pDef;                            // +0x70
    unsigned char  _pad74[0x91 - 0x74];
    unsigned char  m_flags91;                         // +0x91
    CPhysicsBody* GetPhysics() const { return PhysicsBody; }
    CTCGraphicAppearance* GetGraphicAppearance() const { return PGraphicAppearance; }
    bool IsDead() const { return (m_flags91 & 1) != 0; }
    const C3DVector& GetPos() const;             // 0x004C73D0
    bool GetTCDriver(CExplosionDriver** out)
    {
        if ((m_presentBits[2] & 2) != 0) {
            *out = PTCDriver;
            return true;
        }
        return false;
    }
};
struct CTools {
    bool IsLineOfSightFiltered(const C3DVector* from, const C3DVector* to, C3DVector* hitPos, C3DVector* hitNormal,
                               long* hitIndex, const CThing*& hitThing, long filter, bool flag);   // 0x00877C40
};
class CGameCameraManager { public:
    unsigned char _pad00[0x78];
    CTools CamTools;                                  // +0x78
};
struct CCameraSource {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void GetCameraManager(CGameCameraManager** out);   // slot 10 (+0x28)
};
class CDisplayEngine { public:
    unsigned char _pad00[0x34];
    CCameraSource* PCameraSource;                     // +0x34
    CCameraSource* GetCameraSource() const { return PCameraSource; }
};
class CDefinitionManager { public:
    long GetDefGlobalIndexFromName(const CCharString& name);   // 0x009AD410
};
CThing* __fastcall CreateThing(long defIndex, const C3DVector* position, const C3DVector* lookAt);   // 0x0080C500
CThing* __fastcall CreateEffectThing(long type, const C3DVector* position, bool flag);              // 0x006E0880
class CWorld { public: static long __fastcall GetFrame(); };                             // 0x0049D870
extern long g_LastBeamExplosionFrame;                 // 0x013BAF10

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
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};
inline bool IS_THING_ALIVE(const CIntelligentPointer<CThing>& p)
{
    if (p.GetPItem() != 0) {
        if (!p.GetPItem()->IsDead())
            return true;
    }
    return false;
}
class CKrakenDef { public:
    unsigned char _pad00[0x54];
    float BeamExtraLength;                            // +0x54
    unsigned char _pad58[0x60 - 0x58];
    float BeamRangeMode4;                             // +0x60
    float BeamRange;                                  // +0x64
    long  BeamExplosionInterval;                      // +0x68
};
class CTCKraken { public:
    void*                   __vftable;                // +0x00
    CThing*                 ParentThing;              // +0x04
    unsigned char           _pad08[0x0c - 0x08];
    CKrakenDef*             PKrakenDef;               // +0x0c
    unsigned char           _pad10[0x20 - 0x10];
    CIntelligentPointer<CThing> PTarget;                  // +0x20
    unsigned char           _pad28[0x34 - 0x28];
    CIntelligentPointer<CThing> PBeamWeaponEffect;        // +0x34
    unsigned char           _pad3c[0x64 - 0x3c];
    long                    Mode;                     // +0x64
    CDefinitionManager* GetDefinitionManager();       // 0x006869D0
    CDisplayEngine* GetDisplayEngine();               // 0x00686A70
    void AddStrikeExplosion(CThing* explosion);       // 0x007FBA20
    void UpdateBeamExplosion();
};

void CTCKraken::UpdateBeamExplosion()
{
    long now = CWorld::GetFrame();
    if (!IS_THING_ALIVE(PTarget))
        return;
    if (now - g_LastBeamExplosionFrame <= PKrakenDef->BeamExplosionInterval)
        return;
    if (!IS_THING_ALIVE(PBeamWeaponEffect))
        return;
    float range;
    if (Mode == 4)
        range = PKrakenDef->BeamRangeMode4;
    else
        range = PKrakenDef->BeamRange;
    C3DVector start = PBeamWeaponEffect.GetPItem()->GetPos();
    C3DVector dir;
    CRightHandedSet orientation;
    if (!ParentThing->GetGraphicAppearance()->GetDummyObject("MOUTH_DUMMY", 0, 0, &start, &orientation, 1.0f)) {
        dir = PBeamWeaponEffect.GetPItem()->GetPhysics()->GetFacing();
        start = PBeamWeaponEffect.GetPItem()->GetPos();
    } else {
        dir = orientation.Up * -1.0f;
    }
    float length = range + PKrakenDef->BeamExtraLength;
    CThing* explosion = CreateThing(GetDefinitionManager()->GetDefGlobalIndexFromName("KRAKEN_BEAM_EXPLOSION"),
                                    &(start + dir * range), &(start + dir * length));
    if (explosion) {
        CExplosionDriver* driver;
        explosion->GetTCDriver(&driver);
        driver->AddExcludedThing(ParentThing);
        AddStrikeExplosion(explosion);
        g_LastBeamExplosionFrame = now;
    }
    CDefPointer<CSpecialEffectsDef> effects;
    long effectType = 0;
    if (CDefClassReference(ParentThing->m_pDef)->GetSubDef(effects) &&
        effects->GetEffectFromString("BEAM_HIT", effectType)) {
        CGameCameraManager* cameras;
        GetDisplayEngine()->GetCameraSource()->GetCameraManager(&cameras);
        C3DVector hitPos;
        C3DVector hitNormal;
        long hitIndex;
        const CThing* hitThing;
        if (!cameras->CamTools.IsLineOfSightFiltered(&start, &(start + dir * length), &hitPos, &hitNormal, &hitIndex,
                                                     hitThing, 15, false))
            CreateEffectThing(effectType, &hitPos, false);
    }
}