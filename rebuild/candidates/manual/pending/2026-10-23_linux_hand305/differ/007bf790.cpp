// Grade: DIFFER(752v762) -- retail moves each looked-up component through eax (`mov eax,[eax+4]; mov ecx,eax`) for the 0xbf and shooter-0x4d calls, caches 0x2000 in ebx / 0x4d in ebp (ours swapped), tail-merges the parent's two 0x4d lookups into one block and builds the AddTC zero arguments from `xor edi,edi` (the uninitialised component's register)
// CTCShotDefinitionBase::Init(CDefPointer<CThingShotDef>, CThing* shooter, CThing* weapon, const C3DVector& targetPos, float accuracy, float damageMultiplier, float buildupTime, const CDefPointer<CMeleeCombatAbilityDef>&) -- store the shot parameters, reset physics, run the derived graphics/movement inits, make the shot pass through its shooter, and stamp the start frame
// Leaf: Init
// Retail: Fable.exe (TLC, VC7.1) @ 0x007BF790, 768 bytes, `ret 0x20` (esp frame; the by-value def pointer is released at the end).
// Callees: CBaseIntelligentPointer ctor 0x00A01B10 / SetPItem 0x00A01B90 / GetPItem 0x00A01B50 / dtor 0x00A01C10,
//   CVectorMap::LowerBound 0x0040F020, CTCObjectAugmentations::InitAbilitySlot 0x00768210, CTCSolidToThing::
//   AddThingNotSolidTo 0x00770FA0, CThing::AddTC 0x004C9D60 ("CTCSolidToThing" @ 0x0123B47C), CCharString 0x0099EBF0 /
//   0x0099EAE0, CThing::GetPosition 0x004C73D0, CTCWeaponTrail::UpdateWeaponTrails 0x0076EFA0, CTCBase::GetWorld 0x00686A70,
//   CWorld::GetWorldFrame 0x0049D870; physics vslot +0xb0, own vslots +0x88 / +0x84.
// Layout: Ego_r.pdb CTCShotDefinitionBase minus 4 from HitThingsList on (12-byte retail vector): PShooter +0x28, PWeapon +0x30,
//   PShotDef +0x38, PShotAbilityDef +0x3c, AccuracyPercentage +0x40, DamageMultiplier +0x44, ShotBuildupTime +0x48, StartFrame
//   +0x4c, TargetPos +0x51 (packed), DieOnFrame +0x60, PassedTarget +0x64, KnockDown +0x66, Decapitate +0x67,
//   BestCollisionPos +0x70, BestCollisionType +0x90 (every store here proves its offset).
// Shape: source store order buildup/damage/accuracy gives retail's 0x40/0x48/0x44 store schedule; the HasTC
//   `unsigned long` mask form gives the `jns` bit-31 test.
struct C3DVector { float X, Y, Z; };
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
struct CBaseObject;
struct CBaseIntelligentPointer {
    virtual void v00();
    CBaseObject* PData;
    CBaseIntelligentPointer();                          // 0x00A01B10
    ~CBaseIntelligentPointer();                         // 0x00A01C10
    void SetPItem(CBaseObject* item);                   // 0x00A01B90
    CBaseObject* GetPItem() const;                      // 0x00A01B50
};
template <class T> struct CIntelligentPointer : CBaseIntelligentPointer {
    virtual void v00();                                 // vtable 0x01238C6C
    CIntelligentPointer(T* item) { SetPItem((CBaseObject*)item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other)
    {
        SetPItem(other.CBaseIntelligentPointer::GetPItem());
        return *this;
    }
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
};
struct CDefBase {
    virtual void v00();
    virtual void Destroy();
    long RefCount;
    __forceinline void Release() { if (--RefCount == 0) Destroy(); }
};
template <class T> struct CDefPointer {
    T* Data;
    __forceinline CDefPointer& operator=(const CDefPointer& other)
    {
        if (Data != other.Data) {
            if (Data) {
                Data->Release();
                Data = 0;
            }
            Data = other.Data;
            if (Data)
                ++Data->RefCount;
        }
        return *this;
    }
    ~CDefPointer()
    {
        if (Data)
            Data->Release();
    }
};
struct CThingShotDef : CDefBase { };
struct CMeleeCombatAbilityDef : CDefBase { };

struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);               // 0x0040F020
};
struct CTCParamsBase;
struct CTCPhysicsBase {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35();
    virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39(); virtual void v40(); virtual void v41();
    virtual void v42(); virtual void v43();
    virtual void Reset();                               // +0xb0
};
struct CTCInterface5B { unsigned char _pad[0x59]; unsigned char Flags; };
struct CThing {
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                     // +0x20
    CInterfaceMap m_interfaces;                         // +0x44
    unsigned char _pad4c[0x60 - 0x4c];
    CTCPhysicsBase* Physics;                            // +0x60
    CTCInterface5B* Comp64;                             // +0x64
    unsigned char _pad68[0x91 - 0x68];
    unsigned char Flags91;                              // +0x91
    const C3DVector& GetPosition() const;               // 0x004C73D0
    CTCBase* AddTC(const CCharString& name, bool flag, CTCParamsBase* params);   // 0x004C9D60
    __forceinline unsigned long HasTC(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    __forceinline CTCBase* GetTC(long type)
    {
        long key = type;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&key);
        return ((it == map->last || type < it->type) ? map->last : it)->component;
    }
    template <class T>
    __forceinline void LookupInterface(long type, T** out)
    {
        long key = type;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&key);
        *out = (T*)((it == map->last || type < it->type) ? map->last : it)->component;
    }
    template <class T>
    __forceinline void GetInterface(long type, T** out)
    {
        if (HasTC(type)) {
            long key = type;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&key);
            *out = (T*)((it == map->last || type < it->type) ? map->last : it)->component;
        }
    }
};
struct CTCObjectAugmentations { void InitAbilitySlot(CThing* weapon); };   // 0x00768210
struct CTCSolidToThing { void AddThingNotSolidTo(CThing* thing); };        // 0x00770FA0
struct CTCWeaponTrail { void UpdateWeaponTrails(); };                       // 0x0076EFA0
struct CWorld { static long GetWorldFrame(); };                             // 0x0049D870

#pragma pack(push, 1)
struct CTCShotDefinitionBase {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32();
    virtual void InitMovement();                        // +0x84
    virtual void InitGraphics();                        // +0x88
    CThing*        ParentThing;                         // +0x04
    unsigned char  _pad08[0x28 - 0x08];
    CIntelligentPointer<CThing> PShooter;               // +0x28
    CIntelligentPointer<CThing> PWeapon;                // +0x30
    CDefPointer<CThingShotDef> PShotDef;                // +0x38
    CDefPointer<CMeleeCombatAbilityDef> PShotAbilityDef;   // +0x3c
    float          AccuracyPercentage;                  // +0x40
    float          DamageMultiplier;                    // +0x44
    float          ShotBuildupTime;                     // +0x48
    long           StartFrame;                          // +0x4c
    bool           SubShotsTriggered;                   // +0x50
    C3DVector      TargetPos;                           // +0x51 (packed)
    unsigned char  _pad5d[3];
    long           DieOnFrame;                          // +0x60
    bool           PassedTarget;                        // +0x64
    bool           MadeSound;                           // +0x65
    bool           KnockDown;                           // +0x66
    bool           Decapitate;                          // +0x67
    unsigned char  _pad68[0x70 - 0x68];
    C3DVector      BestCollisionPos;                    // +0x70
    unsigned char  _pad7c[0x90 - 0x7c];
    long           BestCollisionType;                   // +0x90
    CWorld* GetWorld();                                 // 0x00686A70
    void Init(CDefPointer<CThingShotDef> shotDef, CThing* shooter, CThing* weapon, const C3DVector& targetPos,
              float accuracy, float damageMultiplier, float buildupTime, const CDefPointer<CMeleeCombatAbilityDef>& abilityDef);
};
#pragma pack(pop)

void CTCShotDefinitionBase::Init(CDefPointer<CThingShotDef> shotDef, CThing* shooter, CThing* weapon,
                                 const C3DVector& targetPos, float accuracy, float damageMultiplier, float buildupTime,
                                 const CDefPointer<CMeleeCombatAbilityDef>& abilityDef)
{
    PShotDef = shotDef;
    PShotAbilityDef = abilityDef;
    PShooter = CIntelligentPointer<CThing>(shooter);
    PWeapon = CIntelligentPointer<CThing>(weapon);
    ShotBuildupTime = buildupTime;
    DamageMultiplier = damageMultiplier;
    AccuracyPercentage = accuracy;
    DieOnFrame = -1;
    PassedTarget = false;
    KnockDown = false;
    Decapitate = false;
    if (PShotAbilityDef.Data)
        KnockDown = true;
    TargetPos = targetPos;
    ParentThing->Physics->Reset();
    InitGraphics();
    InitMovement();
    if (weapon && !(weapon->Flags91 & 1) && ParentThing->HasTC(0xbf)) {
        CTCObjectAugmentations* aug;
        ParentThing->LookupInterface(0xbf, &aug);
        aug->InitAbilitySlot(weapon);
    }
    if (PShooter.GetPItem() && !(PShooter.GetPItem()->Flags91 & 1)) {
        CThing* shooterThing = PShooter.GetPItem();
        if (shooterThing->HasTC(0x4d)) {
            CTCSolidToThing* solid;
            shooterThing->LookupInterface(0x4d, &solid);
            solid->AddThingNotSolidTo(ParentThing);
        }
        if (!ParentThing->HasTC(0x4d))
            ParentThing->AddTC("CTCSolidToThing", false, 0);
        CTCSolidToThing* solid = 0;
        ParentThing->GetInterface(0x4d, &solid);
        solid->AddThingNotSolidTo(PShooter.GetPItem());
    }
    ParentThing->Comp64->Flags |= 2;
    BestCollisionPos = ParentThing->GetPosition();
    BestCollisionType = 0;
    if (ParentThing->HasTC(0x4c)) {
        ((CTCWeaponTrail*)ParentThing->GetTC(0x4c))->UpdateWeaponTrails();
    }
    StartFrame = GetWorld()->GetWorldFrame();
}
