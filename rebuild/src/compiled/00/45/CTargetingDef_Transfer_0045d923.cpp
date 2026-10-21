// CTargetingDef::Transfer(CPersistContext& context) -- persist every targeting field in declaration order: type, glow flag, arcs, camera flags, falloffs, the TargetingRanges map and the four preference weights
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0045D923, length 322 bytes (virtual, `?Transfer@CTargetingDef@@UAEXAAVCPersistContext@@@Z`, `ret 4`).
// Callees (the per-type CPersistContext transfers of the definition module; their catalog labels
// "CEngineLightingManager::UpdateShadowScene" are propagated, see labels_identity_corrections_2026-10-19.tsv):
//   unsigned long 0x00431020, bool 0x0043314A, float 0x00431061, and the map<ETargetingType,float> transfer 0x0045DA65
//   (no name argument: the callee pushes only the map address).
// Layout: the same local model as the landed CTargetingDef::operator= (0x0045D729): Ego_r.pdb CTargetingDef with the
//   retail 12-byte STLport map, so everything after TargetingRanges sits 4 bytes below the PDB (+0x80..+0x8c).
//   No engine header exists for CTargetingDef.
// Shape: `this` in esi, the context in edi, one `lea eax ; push eax ; mov ecx,edi ; call` per field; the last one
//   folds the address into esi (`add esi,8Ch`).
// Leaf: ?Transfer@CTargetingDef
#pragma optimize("s", on)
struct CTargetingRangeMap {                                               // retail map<ETargetingType,float> (12 bytes)
    void*         head;                                                   // +0x00
    unsigned long size;                                                   // +0x04
    unsigned long _alloc;                                                 // +0x08 (allocator proxy word)
};

struct CPersistContext {
    void Transfer(unsigned long& value);                                  // 0x00431020
    void Transfer(bool& value);                                           // 0x0043314A
    void Transfer(float& value);                                          // 0x00431061
    void Transfer(CTargetingRangeMap& value);                             // 0x0045DA65
};

struct CDefClassBase {
    virtual void Transfer(CPersistContext& context);
    unsigned char _body[0x24];                                            // +0x04..+0x28
};

struct CTargetingDef : CDefClassBase {                                    // vtable 0x01232D84
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
    CTargetingRangeMap TargetingRanges;                                   // +0x74
    float PreferPlayerWeight;                                             // +0x80
    float PreferNonCombatantsWeight;                                      // +0x84
    float PreferLastAttackerWeight;                                       // +0x88
    float PreferCurrentBestEnemyWeight;                                   // +0x8c
    virtual void Transfer(CPersistContext& context);
};

void CTargetingDef::Transfer(CPersistContext& context)
{
    context.Transfer(Type);
    context.Transfer(UseGlowTargeting);
    context.Transfer(TargetArc);
    context.Transfer(TargetArcPCThirdPersonAimingMode);
    context.Transfer(TargetingBasePosFromCamera);
    context.Transfer(TargetingFacingDirFromCamera);
    context.Transfer(TargetingPCFacingDirFromCamera);
    context.Transfer(RejectTargetsBehindIfRunning);
    context.Transfer(AcceptOnlyEnemiesBehind);
    context.Transfer(EnemyBehindTargetingRange);
    context.Transfer(FirstPersonTargetingAngleFalloff);
    context.Transfer(FirstPersonTargetingDistanceFalloff);
    context.Transfer(ThirdPersonTargetingAngleFalloff);
    context.Transfer(ThirdPersonTargetingDistanceFalloff);
    context.Transfer(ZLockTargetSelectAngleFalloff);
    context.Transfer(ZLockTargetSelectDistanceFalloff);
    context.Transfer(FirstPersonTargetingOverrideShortRangeFactor);
    context.Transfer(FirstPersonTargetingOverrideShortRangeFalloff);
    context.Transfer(ThirdPersonTargetingOverrideShortRangeFactor);
    context.Transfer(ThirdPersonTargetingOverrideShortRangeFalloff);
    context.Transfer(FirstPersonTargetingEnemyPreferenceFactor);
    context.Transfer(ThirdPersonTargetingEnemyPreferenceFactor);
    context.Transfer(TargetingRanges);
    context.Transfer(PreferPlayerWeight);
    context.Transfer(PreferNonCombatantsWeight);
    context.Transfer(PreferLastAttackerWeight);
    context.Transfer(PreferCurrentBestEnemyWeight);
}