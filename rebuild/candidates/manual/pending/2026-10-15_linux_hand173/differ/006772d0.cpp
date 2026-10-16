// CThingBaseDef::CThingBaseDef() -- default def constructor: CDefClassBase/CParentDefClassBase parts inline, own vtable, empty component set, default flag bits, zero scalars, no default script
// Grade: DIFFER(115v115) -- scheduling only: retail stores the +0x42 bit byte before the +0x44..+0x64 zero dwords and computes the +0x43 mask after them; ours stores +0x42 after the dwords. Next: write the +0x43 bit clears after the scalar assignments.
// Leaf: ??0CThingBaseDef
// Retail: Fable.exe (TLC, VC7.1) @ 0x006772D0, 128 bytes.
// Callees: CResource::CResource @ 0x009FBEC0.
// Identity: the only surviving vtable store is 0x0125B664 = CThingBaseDef (rtti-vtables.tsv; the CDefPointeeBase /
//   CDefClassBase / CParentDefClassBase stores are dead and dropped). Base models from the landed 0x0042D9E0 and
//   0x00430370 constructors.
// Layout (PDB names, retail offsets): the retail CParentDefClassBase is 0x39 bytes (byte-packed, its 16-byte
//   CVectorMap at +0x29), so every CThingBaseDef member sits 4 below the PDB: Components +0x39 (CThingComponentSet:
//   pointer +0x00, flag byte +0x04), flag bytes +0x41 (AvailableInEditor .. InMapSearches), +0x42 (IsLoadableGlobal ..
//   AlwaysIncludeInObjectStrikeScans), +0x43 (DisableOcclusionTesting .. GiveHeroStatChangesOnBeingHit), GroupDef +0x44,
//   RenderFadeDistance +0x48, PersistenceFlags +0x4c, Health +0x50, MinHealth +0x54, MeshHeightOffset +0x58,
//   MeshHeight +0x5c, MeshRadius +0x60, ApproxMaxMeshHeight +0x64, DefaultScriptName +0x68, DefaultScriptData +0x6c.
// Shape: all eight bits of +0x41 are assigned (IsDrawable = 1) so the byte is one store of 0x10; +0x42 clears all but
//   DrawProjectileWeaponTrails and +0x43 clears bits 2..4 (the other bits stay as the allocator left them).
struct CDefinitionManager;

struct CResource {
    virtual void v00();
    unsigned char _resource[0x18];          // +0x04 resource list links, size, last used frame
    CResource();                            // 0x009FBEC0
};

struct CDefPointeeBase : CResource {        // vtable 0x01230BA0
    virtual void v00();
    CDefPointeeBase() { }
};

#pragma pack(push, 1)                       // def classes are byte packed
struct CDefClassBase : CDefPointeeBase {    // vtable 0x01230BCC
    CDefinitionManager* PDefManager;        // +0x1c
    unsigned long       GlobalIndex;        // +0x20
    unsigned char       Setup : 1;          // +0x24 bit 0
    unsigned char       Template : 1;       // +0x24 bit 1
    unsigned char       DefaultValsApplied : 1;     // +0x24 bit 2
    virtual void v00();
    CDefClassBase()                         // out of line at 0x0042D9E0, inline here
        : PDefManager(0)
    {
        Setup = 0;
        Template = 0;
        DefaultValsApplied = 0;
    }
};

struct CDefString {                         // a def-name string-table position
    long TablePos;
    CDefString() { }
    explicit CDefString(long pos) : TablePos(pos) { }
};
#pragma pack(pop)

struct CSubDefInfoVectorMap {               // retail CVectorMap<unsigned long, CDefClassBase::CSubDefInfo> (16 bytes)
    void*         m_first;                  // +0x00 sorted vector of (key, info) pairs
    void*         m_last;                   // +0x04
    void*         m_end;                    // +0x08
    unsigned char Compare;                  // +0x0c CKeyPairCompareLess (empty)
    bool          Dirty;                    // +0x0d
    CSubDefInfoVectorMap() : m_first(0), m_last(0), m_end(0), Dirty(false) { }
};

struct CThingComponentSet {                 // 8 bytes (naturally aligned, outside the def pack)
    void* m_pComponents;                    // +0x00
    bool  m_flag;                           // +0x04
    CThingComponentSet() : m_pComponents(0), m_flag(false) { }
};

#pragma pack(push, 1)
struct CParentDefClassBase : CDefClassBase {    // vtable 0x01230CBC
    CDefString           InstantiationName; // +0x25
    CSubDefInfoVectorMap SubDefInfoMap;     // +0x29
    virtual void v00();
    CParentDefClassBase() : InstantiationName(-1) { }   // out of line at 0x00430370, inline here
};

struct CThingBaseDef : CParentDefClassBase {    // vtable 0x0125B664
    CThingComponentSet Components;          // +0x39
    bool AvailableInEditor : 1;             // +0x41
    bool AlwaysFrameUpdate : 1;
    bool StartKillLocked : 1;
    bool LevelLess : 1;
    bool IsDrawable : 1;
    bool IsDrawableInEditor : 1;
    bool ForceNoSerialise : 1;
    bool InMapSearches : 1;
    bool IsLoadableGlobal : 1;              // +0x42
    bool GameStatePersisted : 1;
    bool CanComeBetweenCameraAndObservedThing : 1;
    bool DrawWeaponTrails : 1;
    bool DrawProjectileWeaponTrails : 1;
    bool IsVulnerableToMelee : 1;
    bool Damageable : 1;
    bool AlwaysIncludeInObjectStrikeScans : 1;
    bool DisableOcclusionTesting : 1;       // +0x43
    bool DrawAfterWater : 1;
    bool ForceRenderedLastFrameOverride : 1;
    bool AddToComboMultiplierOnHit : 1;
    bool GiveHeroStatChangesOnBeingHit : 1;
    long GroupDef;                          // +0x44
    float RenderFadeDistance;               // +0x48
    long PersistenceFlags;                  // +0x4c
    float Health;                           // +0x50
    float MinHealth;                        // +0x54
    float MeshHeightOffset;                 // +0x58
    float MeshHeight;                       // +0x5c
    float MeshRadius;                       // +0x60
    float ApproxMaxMeshHeight;              // +0x64
    CDefString DefaultScriptName;           // +0x68
    CDefString DefaultScriptData;           // +0x6c
    virtual void v00();
    CThingBaseDef();
};
#pragma pack(pop)

CThingBaseDef::CThingBaseDef()
{
    AvailableInEditor = false;
    AlwaysFrameUpdate = false;
    StartKillLocked = false;
    LevelLess = false;
    IsDrawable = true;
    IsDrawableInEditor = false;
    ForceNoSerialise = false;
    InMapSearches = false;
    IsLoadableGlobal = false;
    GameStatePersisted = false;
    CanComeBetweenCameraAndObservedThing = false;
    DrawWeaponTrails = false;
    IsVulnerableToMelee = false;
    Damageable = false;
    AlwaysIncludeInObjectStrikeScans = false;
    ForceRenderedLastFrameOverride = false;
    AddToComboMultiplierOnHit = false;
    GiveHeroStatChangesOnBeingHit = false;
    GroupDef = 0;
    RenderFadeDistance = 0;
    PersistenceFlags = 0;
    Health = 0;
    MinHealth = 0;
    MeshHeightOffset = 0;
    MeshHeight = 0;
    MeshRadius = 0;
    ApproxMaxMeshHeight = 0;
    DefaultScriptName = CDefString(-1);
    DefaultScriptData = CDefString(-1);
}
