// CCreatureAction_DragonRearUpFull::PerformAction() -- ground slam: look up JACK_DRAGON_GROUND_SLAM's CThingShotDef, create the
// Grade: RELOCATION_MATCH -- the last x87/push interleave (retail sinks the X*r store below the &ability/0.0f/1.0f Init
//   pushes) is a VC7.1 scheduling-window effect: the block is scheduled in fixed-size windows counted from its start, and
//   the Newton step written `0.5f * (r * (3.0f - lengthSquared * (r * r)))` carries two more (later-folded) x87 IR
//   instructions than `(3.0f - r * r * lengthSquared) * r * 0.5f`, which moves the window edge from fstp X up to fld X.
//   Same emitted FastInvSqrt bytes either way; `(r * r) * lengthSquared` matches too.
// shot thing at the dragon's position dropped to ground height + 0.25, keep it in PShot, add its shot TC (named by the def's
// TCName) and Init it toward position + flat facing * 10 with the dragon's accuracy, exclude the dragon from its hits, then
// spawn the GROUND_SLAM_EXPLOSION effect at the dragon and release the defs.
// Leaf: ?PerformAction@CCreatureAction_DragonRearUpFull
// Retail: Fable.exe (TLC, VC7.1) @ 0x008F3630, 880 bytes, plain `ret`.
// Callees: CGameDefinitionManager::Get 0x0044C6B0, GetDefGlobalIndexFromName 0x009AD410, CCreatureActionBase::GetPWorld
//   0x00692940, CDefinitionManager::GetDef<CThingShotDef> 0x0078B230, CWorldMap::GetGroundSizeZAt 0x0063A300, CThing::Create
//   0x00703210 (__fastcall), CIntelligentPointer ctor/GetPItem/SetPItem/dtor 0x00A01B10/B50/B90/C10 (vtable 0x01238C6C),
//   GetSubDef<CWeaponDef> 0x0042B1A6, GetShotAccuracyPercentage 0x00663B10, CDefStringTable::GetString 0x009D49B0,
//   CThing::AddTC 0x004C9D60, physics GetFacing (slot 70, +0x118), CTCShotDefinitionBase::Init 0x007BF790 / AddHitThing
//   0x007C0FB0, GetSubDef<CSpecialEffectsDef> 0x00669580, CSpecialEffectsDef::GetEffectFromString 0x00669D30,
//   CreateEffectThing 0x006E0880 (__fastcall).
// Shape: same-TU auto_inline(off) stand-ins for the out-param callees; the inline GetPhysics() accessor fixes the GetFacing
//   / FastInvSqrt / release scratch registers; a __forceinline GetDef wrapper evaluates GetPWorld before the pushes.
//   An inline CThing::GetDef() (PDef) accessor on the weapon sub-def path gives retail's callee-saved map (this/&PShot/shot
//   TC/effects = ebp, shotDef = edi); a plain `->PDef` read swaps edi/ebp.
//   The FastInvSqrt Newton step must keep its explicit grouping (see the grade line): the plain left-to-right form emits
//   the same instructions but stores X*r right after its fmul, ahead of the Init pushes.
// Layout: local model -- retail CCombatActionBase is 0x114 bytes (PShot at +0x114, PDB +0x12c).
extern unsigned long g_InvSqrtMantissaTable[128];   // 0x013CE558
__forceinline float FastInvSqrt(float lengthSquared)
{
    unsigned long bits = *(unsigned long*)&lengthSquared;
    unsigned long estimate = (((0x17c - ((bits >> 23) & 0xff)) >> 1) << 23) |
                             (g_InvSqrtMantissaTable[(bits >> 17) & 0x7f] << 15);
    float r = *(float*)&estimate;
    return 0.5f * (r * (3.0f - lengthSquared * (r * r)));
}
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    __forceinline void Normalise() { float r = FastInvSqrt(X * X + Y * Y + Z * Z); X *= r; Y *= r; Z *= r; }
    C3DVector operator*(float f) const { return C3DVector(X * f, Y * f, Z * f); }
    C3DVector operator+(const C3DVector& v) const { return C3DVector(X + v.X, Y + v.Y, Z + v.Z); }
};
struct CRightHandedSet;
struct CCharString {
    void* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
struct CDefStringTable {
    CCharString GetString(long index);                 // 0x009D49B0
};
extern CDefStringTable g_DefStringTable;               // 0x013CA828

struct CDefBase {
    virtual void v00();
    virtual void Destroy();                            // slot 1
    long RefCount;                                     // +0x04
    void Release() { if (--RefCount == 0) Destroy(); }
};
struct CWeaponDef : CDefBase { };
struct CSpecialEffectsDef : CDefBase {
    bool GetEffectFromString(const CCharString& name, long& type) const;   // 0x00669D30
};
long Lookup_GetEffectFromString(const void* def, const CCharString& name);
#pragma auto_inline(off)
__declspec(noinline) bool CSpecialEffectsDef::GetEffectFromString(const CCharString& name, long& type) const
{
    long found = Lookup_GetEffectFromString(this, name);
    if (found < 0)
        return false;
    type = found;
    return true;
}
#pragma auto_inline(on)
struct CThingShotDef : CDefBase {
    unsigned char _pad08[0x60 - 0x08];
    long TCName;                                       // +0x60
    unsigned char _pad64[0x7c - 0x64];
    long Thing;                                        // +0x7c
};
template <class T> struct CDefPointerByValue {
    T* m_p;
    CDefPointerByValue(T* p) : m_p(p) { ++p->RefCount; }
    CDefPointerByValue(const CDefPointerByValue& o) : m_p(o.m_p) { ++m_p->RefCount; }
    ~CDefPointerByValue();
};
struct CRefCounted {
    long m_count;
    void DecRef() { --m_count; }
    long GetRefCount() const { return m_count; }
};
struct CDefVirtual {
    virtual void v00();
    virtual void Destroy();
};
struct CMeleeCombatAbilityDef : CDefVirtual, CRefCounted {
    void ReleaseTemporary()
    {
        DecRef();
        if (GetRefCount() == 0)
            Destroy();
    }
};
template <class T> struct CDefPointer {
    T* m_p;
    CDefPointer() : m_p(0) { }
    ~CDefPointer()
    {
        if (m_p) {
            m_p->ReleaseTemporary();
            m_p = 0;
        }
    }
};
struct CDefClassBase {
    bool GetSubDef(CWeaponDef** out);                  // 0x0042B1A6
    bool GetSubDef(CSpecialEffectsDef** out);          // 0x00669580
};
void* FindSubDef_CWeaponDef(CDefClassBase* def);
void* FindSubDef_CSpecialEffectsDef(CDefClassBase* def);
#pragma auto_inline(off)
__declspec(noinline) bool CDefClassBase::GetSubDef(CWeaponDef** out)
{
    void* found = FindSubDef_CWeaponDef(this);
    if (!found)
        return false;
    *out = (CWeaponDef*)found;
    return true;
}
__declspec(noinline) bool CDefClassBase::GetSubDef(CSpecialEffectsDef** out)
{
    void* found = FindSubDef_CSpecialEffectsDef(this);
    if (!found)
        return false;
    *out = (CSpecialEffectsDef*)found;
    return true;
}
#pragma auto_inline(on)
struct CDefinitionManager {
    long GetDefGlobalIndexFromName(const CCharString& name);   // 0x009AD410
    bool GetDefShot(long index, CThingShotDef** out);  // 0x0078B230
    __forceinline bool GetDef(long index, CThingShotDef** out) { return GetDefShot(index, out); }
};
struct CGameDefinitionManager : CDefinitionManager {
    static CGameDefinitionManager& __fastcall Get();   // 0x0044C6B0
};
void* FindDef_CThingShotDef(CDefinitionManager* manager, long index);
#pragma auto_inline(off)
__declspec(noinline) bool CDefinitionManager::GetDefShot(long index, CThingShotDef** out)
{
    void* found = FindDef_CThingShotDef(this, index);
    if (!found)
        return false;
    *out = (CThingShotDef*)found;
    return true;
}
#pragma auto_inline(on)
struct CWorldMap {
    float GetGroundSizeZAt(const C3DVector& pos) const;   // 0x0063A300
};
struct CWorld {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual CWorldMap* GetWorldMap();                  // slot 12 (+0x30)
    unsigned char       _pad04[0x10 - 0x04];
    CDefinitionManager* PDefinitionManager;            // +0x10
};

struct CTCBase;
struct CTCParamsBase;
struct CTCPhysicsBase {
#define VF(n) virtual void v##n();
#define VF10(p) VF(p##0) VF(p##1) VF(p##2) VF(p##3) VF(p##4) VF(p##5) VF(p##6) VF(p##7) VF(p##8) VF(p##9)
    VF10(0) VF10(1) VF10(2) VF10(3) VF10(4) VF10(5) VF10(6)
    virtual C3DVector GetFacing();                     // slot 70 (+0x118)
#undef VF
#undef VF10
};
struct CThing {
    unsigned char         _pad0[0x60];
    CTCPhysicsBase*       PhysicsTC;                   // +0x60
    unsigned char         _pad64[0x70 - 0x64];
    CDefClassBase*        PDef;                        // +0x70
    unsigned char         _pad74[0x90 - 0x74];
    signed char           PlayerNumber;                // +0x90
    const C3DVector& GetPosition() const;              // 0x004C73D0
    __forceinline CTCPhysicsBase* GetPhysics() const { return PhysicsTC; }
    CDefClassBase* GetDef() const { return PDef; }
    long GetPlayerNumber() const { return PlayerNumber; }
    CTCBase* AddTC(const CCharString& name, bool init, CTCParamsBase* params);   // 0x004C9D60
};
struct CThingCreatureBase : CThing {
    long GetShotAccuracyPercentage();                  // 0x00663B10
};
CThing* __fastcall Create(long defIndex, const C3DVector* position, long player, const CRightHandedSet* orientation,
                          unsigned long flags, const CCharString& scriptName);   // 0x00703210
__forceinline CThing* CreateThingObject(const long& defIndex, const C3DVector& position, const CThing* creature, const CCharString& name)
{
    return Create(defIndex, &position, creature->PlayerNumber, 0, 0, name);
}
CThing* __fastcall CreateEffectThing(long type, const C3DVector* position, bool flag);   // 0x006E0880

struct CTCShotDefinitionBase {
    void Init(CDefPointerByValue<CThingShotDef> def, CThing* shooter, CThing* origin, const C3DVector* target, float accuracy,
              float a, float b, CDefPointer<CMeleeCombatAbilityDef>* ability);   // 0x007BF790
    void AddHitThing(CThing* thing);                   // 0x007C0FB0
};

struct CBaseIntelligentPointer {
    virtual void v00();
    CThingCreatureBase* m_pItem;
    CBaseIntelligentPointer();                         // 0x00A01B10
    ~CBaseIntelligentPointer();                        // 0x00A01C10
    CThingCreatureBase* GetPItem() const;              // 0x00A01B50
    void SetPItem(CThing* item);                       // 0x00A01B90
};
struct CIntelligentPointer : CBaseIntelligentPointer {   // CIntelligentPointer<CThing>, vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointer() { }
    CIntelligentPointer(CThing* item) { SetPItem(item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); return *this; }
};

struct CCombatActionBase {
    virtual void PerformAction();
    unsigned char           _pad04[4];
    CBaseIntelligentPointer PCreature;                 // +0x08
    unsigned char           _pad10[0x114 - 0x10];
    CWorld* GetPWorld() const;                         // 0x00692940
};

struct CCreatureAction_DragonRearUpFull : CCombatActionBase {
    CIntelligentPointer PShot;                         // +0x114 (PDB +0x12c)
    virtual void PerformAction();
};

void CCreatureAction_DragonRearUpFull::PerformAction()
{
    long index = CGameDefinitionManager::Get().GetDefGlobalIndexFromName("JACK_DRAGON_GROUND_SLAM");
    CThingShotDef* shotDef;
    {
        CThingShotDef* out = 0;
        GetPWorld()->PDefinitionManager->GetDef(index, &out);
        shotDef = out;
    }
    const C3DVector& creaturePos = PCreature.GetPItem()->GetPosition();
    C3DVector position(creaturePos.X, creaturePos.Y, creaturePos.Z + 10.0f);
    position.Z = GetPWorld()->GetWorldMap()->GetGroundSizeZAt(position) + 0.25f;
    CThing* thing = CreateThingObject(shotDef->Thing, position, PCreature.GetPItem(), "");
    if (thing) {
        PShot = CIntelligentPointer(thing);
        CWeaponDef* weaponDef = 0;
        CDefClassBase* creatureDef = PCreature.GetPItem()->GetDef();
        creatureDef->GetSubDef(&weaponDef);
        long accuracy = PCreature.GetPItem()->GetShotAccuracyPercentage();
        CTCShotDefinitionBase* shot;
        {
            CCharString name = g_DefStringTable.GetString(shotDef->TCName);
            shot = (CTCShotDefinitionBase*)PShot.GetPItem()->AddTC(name, true, 0);
        }
        C3DVector facing = PCreature.GetPItem()->GetPhysics()->GetFacing();
        facing.Z = 0.0f;
        facing.Normalise();
        C3DVector target = position + facing * 10.0f;
        {
            CDefPointer<CMeleeCombatAbilityDef> ability;
            shot->Init(shotDef, PCreature.GetPItem(), 0, &target, (float)accuracy, 1.0f, 0.0f, &ability);
        }
        shot->AddHitThing(PCreature.GetPItem());
        if (weaponDef)
            weaponDef->Release();
    }
    CSpecialEffectsDef* effects = 0;
    long effectType = 0;
    CThing* creature = PCreature.GetPItem();
    creature->GetDef()->GetSubDef(&effects);
    if (effects) {
        if (effects->GetEffectFromString("GROUND_SLAM_EXPLOSION", effectType))
            CreateEffectThing(effectType, &PCreature.GetPItem()->GetPosition(), false);
        effects->Release();
    }
    shotDef->Release();
}
