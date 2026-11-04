#ifndef FABLE_ENGINE_CTHINGBASEDEF_H
#define FABLE_ENGINE_CTHINGBASEDEF_H
#include "engine/CParentDefClassBase.h"
// Original Ego_r CThingBaseDef derives CParentDefClassBase; native STLport sizeof70.
// Complete original Components through DefaultScriptData range retained opaque.
#ifdef FABLE_RETAIL_CTHINGBASEDEF
// Retail opt-in (chunk 771, row 0x006A5D90): the Ego_r.pdb CThingBaseDef members at their retail offsets (PDB offset - 4, the
// smaller retail CParentDefClassBase 0x39). Retail reads Health at +0x50 (`fld dword ptr [eax+50h]`) and Damageable as bit 6
// of +0x42 (`shr dl,6 ; and dl,1`), 0x006A5DC0 / 0x006A609C. CThingComponentSet / CDefString stay opaque (no shared header).
class CThingBaseDef : public CParentDefClassBase {
public:
    unsigned char Components[8];                                 // +0x39 CThingComponentSet
    bool AvailableInEditor:1, AlwaysFrameUpdate:1, StartKillLocked:1, LevelLess:1, IsDrawable:1, IsDrawableInEditor:1,
        ForceNoSerialise:1, InMapSearches:1;                     // +0x41
    bool IsLoadableGlobal:1, GameStatePersisted:1, CanComeBetweenCameraAndObservedThing:1, DrawWeaponTrails:1,
        DrawProjectileWeaponTrails:1, IsVulnerableToMelee:1, Damageable:1, AlwaysIncludeInObjectStrikeScans:1;   // +0x42
    bool DisableOcclusionTesting:1, DrawAfterWater:1, ForceRenderedLastFrameOverride:1, AddToComboMultiplierOnHit:1,
        GiveHeroStatChangesOnBeingHit:1;                         // +0x43
    long GroupDef;                                               // +0x44
    float RenderFadeDistance;                                    // +0x48
    long PersistenceFlags;                                       // +0x4C
    float Health;                                                // +0x50
    float MinHealth;                                             // +0x54
    float MeshHeightOffset;                                      // +0x58
    float MeshHeight;                                            // +0x5C
    float MeshRadius;                                            // +0x60
    float ApproxMaxMeshHeight;                                   // +0x64
    unsigned char DefaultScriptName[4];                          // +0x68 CDefString
    unsigned char DefaultScriptData[4];                          // +0x6C CDefString
};
#else
class CThingBaseDef : public CParentDefClassBase {
    unsigned char _pad_0x39[0x37];
};
#endif
FABLE_STATIC_ASSERT(sizeof(CThingBaseDef)==0x70);
#ifdef FABLE_RETAIL_CTHING
// Retail opt-in (2026-11-03, row 0x005EB9E0): the body of CThing::PeekBaseDef (declared in engine/CThing.h; Ego_r.pdb
// ?PeekBaseDef@CThing@@QBEABVCThingBaseDef@@XZ, inline in retail: CTCTavernGame::FrameUpdate reads PBaseDef +0x70 in place,
// `mov edx,[esi+4]; mov ecx,[edx+70h]`, as the landed 0x007CF7E0 local model does). It needs the complete CThingBaseDef.
#include "engine/CThing.h"
inline const CThingBaseDef& CThing::PeekBaseDef() const { return *PBaseDef; }
#endif

#endif
