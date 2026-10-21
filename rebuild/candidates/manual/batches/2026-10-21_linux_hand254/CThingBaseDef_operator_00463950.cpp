// CThingBaseDef::operator=(const CThingBaseDef& other) -- memberwise def assignment: the parent def's out-of-line operator=, the component set (self-checked), the 21 one-bit flags and the eleven trailing dwords
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00463950, length 337 bytes (`ret 4`, returns `this` in eax).
// Callees: the parent def class's operator= @ 0x00431ED0, CSmallVector<CThingComponentSet::CEntry,8>::Copy @ 0x00463AE0 (reached only when the two
//   vectors differ: the small vector's inline operator= tests `&other != this`).
// Layout: Ego_r.pdb CThingBaseDef (0x74 bytes, Components +0x3D, flag bytes +0x45/+0x46/+0x47, GroupDef +0x48 ..
//   DefaultScriptData +0x70) less 4 in retail (the definition base is 0x39 bytes in retail): Components +0x39 (8 bytes,
//   unaligned: the def module is packed), flags +0x41/+0x42/+0x43, GroupDef +0x44 .. DefaultScriptData +0x6C.
//   CDefString is a single-dword handle copied as a plain dword. No engine header exists for CThingBaseDef.
// Shape: the compiler-generated assignment: each `bool:1` field is a read-modify-write of its byte (`xor ; and mask ;
//   xor`), the last bit of a full byte via `and 7Fh`; the dwords alternate edx/eax/ecx.
// Leaf: ??4CThingBaseDef
#pragma pack(push, 1)
struct CParentDefClassBase {
    unsigned char _body[0x39];                                   // +0x00 (vptr, def index, name, parent ...)
    CParentDefClassBase& operator=(const CParentDefClassBase& other);   // 0x00431ED0
};

struct CThingComponentSetEntry;
struct CSmallVectorComponentEntries {                            // CSmallVector<CThingComponentSet::CEntry,8>
    CThingComponentSetEntry* Start;                              // +0x00
    signed char Size;                                            // +0x04
    unsigned char _pad05[3];
    void Copy(const CSmallVectorComponentEntries& other);        // 0x00463AE0
    CSmallVectorComponentEntries& operator=(const CSmallVectorComponentEntries& other)
    {
        if (&other != this)
            Copy(other);
        return *this;
    }
};

struct CThingComponentSet {
    CSmallVectorComponentEntries Entries;                        // +0x00
};

struct CDefString { long Index; };

struct CThingBaseDef : CParentDefClassBase {
    CThingComponentSet Components;                               // +0x39
    bool AvailableInEditor : 1;                                  // +0x41
    bool AlwaysFrameUpdate : 1;
    bool StartKillLocked : 1;
    bool LevelLess : 1;
    bool IsDrawable : 1;
    bool IsDrawableInEditor : 1;
    bool ForceNoSerialise : 1;
    bool InMapSearches : 1;
    bool IsLoadableGlobal : 1;                                   // +0x42
    bool GameStatePersisted : 1;
    bool CanComeBetweenCameraAndObservedThing : 1;
    bool DrawWeaponTrails : 1;
    bool DrawProjectileWeaponTrails : 1;
    bool IsVulnerableToMelee : 1;
    bool Damageable : 1;
    bool AlwaysIncludeInObjectStrikeScans : 1;
    bool DisableOcclusionTesting : 1;                            // +0x43
    bool DrawAfterWater : 1;
    bool ForceRenderedLastFrameOverride : 1;
    bool AddToComboMultiplierOnHit : 1;
    bool GiveHeroStatChangesOnBeingHit : 1;
    long  GroupDef;                                              // +0x44
    float RenderFadeDistance;                                    // +0x48
    long  PersistenceFlags;                                      // +0x4C
    float Health;                                                // +0x50
    float MinHealth;                                             // +0x54
    float MeshHeightOffset;                                      // +0x58
    float MeshHeight;                                            // +0x5C
    float MeshRadius;                                            // +0x60
    float ApproxMaxMeshHeight;                                   // +0x64
    CDefString DefaultScriptName;                                // +0x68
    CDefString DefaultScriptData;                                // +0x6C
};
#pragma pack(pop)

void AssignThingBaseDef(CThingBaseDef& to, const CThingBaseDef& from)
{
    to = from;
}
