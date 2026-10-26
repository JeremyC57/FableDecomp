// CTargetingDef::operator=(const CTargetingDef&) -- memberwise def copy: CDefClassBase::operator=, the scalar targeting fields, the TargetingRanges map by its operator=, then the four preference weights; returns *this
// Leaf: ??4CTargetingDef
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0045D729, length 213 bytes.
// Identity: the manifest label "CThingBuildingDef::operator=" is propagated. The body sits behind the
//   5-byte `jmp` thunk at 0x0045D724 = slot 19 of vtable 0x01232D84 = CTargetingDef (rtti-vtables.tsv),
//   and the touched offsets are the PDB CTargetingDef fields (Type +0x28 .. PreferCurrentBestEnemyWeight
//   +0x8c; the retail STLport map is 12 bytes, so everything after TargetingRanges sits 4 bytes below Ego_r).
// Callees: CDefClassBase::operator= @ 0x00431F10, map<ETargetingType,float>::operator= @ 0x0045D7FE
//   (landed as CRetailTree_0045d871::operator=).
// Shape: compiler-generated memberwise assignment in declaration order; every copy pair goes through
//   eax alone (size-optimised def module: `#pragma optimize("s", on)`), the map through `lea`+push.
// No engine header exists for CTargetingDef: modelled locally from struct_layouts_egor.tsv.
#pragma optimize("s", on)

class CDefClassBase { public:
    virtual void v00();
    unsigned char _body[0x24];                                            // +0x04..+0x28
    CDefClassBase& operator=(const CDefClassBase& other);                 // 0x00431F10
};

struct CRetailTree_0045d871 {                                             // retail map<ETargetingType,float> (12 bytes)
    struct node;
    node*         head;                                                   // +0x00
    unsigned long size;                                                   // +0x04
    unsigned long _alloc;                                                 // +0x08 (allocator proxy word)
    CRetailTree_0045d871& operator=(const CRetailTree_0045d871& other);   // 0x0045D7FE
};

class CTargetingDef : public CDefClassBase { public:                                    // vtable 0x01232D84
    unsigned long Type;                                                   // +0x28
    bool  UseGlowTargeting;                                               // +0x2c
    float TargetArc;                                                      // +0x30
    float TargetArcPCThirdPersonAimingMode;                               // +0x34
    bool  TargetingBasePosFromCamera;                                     // +0x38
    bool  TargetingFacingDirFromCamera;                                   // +0x39
    bool  TargetingPCFacingDirFromCamera;                                 // +0x3a
    bool  RejectTargetsBehindIfRunning;                                   // +0x3b
    bool  AcceptOnlyEnemiesBehind;                                        // +0x3c
    float EnemyBehindTargetingRange;                                      // +0x40
    float FirstPersonTargetingAngleFalloff;                               // +0x44
    float FirstPersonTargetingDistanceFalloff;                            // +0x48
    float ThirdPersonTargetingAngleFalloff;                               // +0x4c
    float ThirdPersonTargetingDistanceFalloff;                            // +0x50
    float ZLockTargetSelectAngleFalloff;                                  // +0x54
    float ZLockTargetSelectDistanceFalloff;                               // +0x58
    float FirstPersonTargetingOverrideShortRangeFactor;                   // +0x5c
    float FirstPersonTargetingOverrideShortRangeFalloff;                  // +0x60
    float ThirdPersonTargetingOverrideShortRangeFactor;                   // +0x64
    float ThirdPersonTargetingOverrideShortRangeFalloff;                  // +0x68
    float FirstPersonTargetingEnemyPreferenceFactor;                      // +0x6c
    float ThirdPersonTargetingEnemyPreferenceFactor;                      // +0x70
    CRetailTree_0045d871 TargetingRanges;                                 // +0x74
    float PreferPlayerWeight;                                             // +0x80
    float PreferNonCombatantsWeight;                                      // +0x84
    float PreferLastAttackerWeight;                                       // +0x88
    float PreferCurrentBestEnemyWeight;                                   // +0x8c
    virtual void v00();
    CTargetingDef& operator=(const CTargetingDef& other);
};

CTargetingDef& CTargetingDef::operator=(const CTargetingDef& other)
{
    CDefClassBase::operator=(other);
    Type = other.Type;
    UseGlowTargeting = other.UseGlowTargeting;
    TargetArc = other.TargetArc;
    TargetArcPCThirdPersonAimingMode = other.TargetArcPCThirdPersonAimingMode;
    TargetingBasePosFromCamera = other.TargetingBasePosFromCamera;
    TargetingFacingDirFromCamera = other.TargetingFacingDirFromCamera;
    TargetingPCFacingDirFromCamera = other.TargetingPCFacingDirFromCamera;
    RejectTargetsBehindIfRunning = other.RejectTargetsBehindIfRunning;
    AcceptOnlyEnemiesBehind = other.AcceptOnlyEnemiesBehind;
    EnemyBehindTargetingRange = other.EnemyBehindTargetingRange;
    FirstPersonTargetingAngleFalloff = other.FirstPersonTargetingAngleFalloff;
    FirstPersonTargetingDistanceFalloff = other.FirstPersonTargetingDistanceFalloff;
    ThirdPersonTargetingAngleFalloff = other.ThirdPersonTargetingAngleFalloff;
    ThirdPersonTargetingDistanceFalloff = other.ThirdPersonTargetingDistanceFalloff;
    ZLockTargetSelectAngleFalloff = other.ZLockTargetSelectAngleFalloff;
    ZLockTargetSelectDistanceFalloff = other.ZLockTargetSelectDistanceFalloff;
    FirstPersonTargetingOverrideShortRangeFactor = other.FirstPersonTargetingOverrideShortRangeFactor;
    FirstPersonTargetingOverrideShortRangeFalloff = other.FirstPersonTargetingOverrideShortRangeFalloff;
    ThirdPersonTargetingOverrideShortRangeFactor = other.ThirdPersonTargetingOverrideShortRangeFactor;
    ThirdPersonTargetingOverrideShortRangeFalloff = other.ThirdPersonTargetingOverrideShortRangeFalloff;
    FirstPersonTargetingEnemyPreferenceFactor = other.FirstPersonTargetingEnemyPreferenceFactor;
    ThirdPersonTargetingEnemyPreferenceFactor = other.ThirdPersonTargetingEnemyPreferenceFactor;
    TargetingRanges = other.TargetingRanges;
    PreferPlayerWeight = other.PreferPlayerWeight;
    PreferNonCombatantsWeight = other.PreferNonCombatantsWeight;
    PreferLastAttackerWeight = other.PreferLastAttackerWeight;
    PreferCurrentBestEnemyWeight = other.PreferCurrentBestEnemyWeight;
    return *this;
}