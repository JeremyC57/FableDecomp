#ifndef FABLE_ENGINE_CREGIONDEF_H
#define FABLE_ENGINE_CREGIONDEF_H
// Retail model (chunk 2025, row 0x00605570 CTCInventoryMap::ConstructRegionDescription; new header, no repo copy existed).
// Ego_r.pdb CRegionDef (struct_layouts_egor.tsv, 0xD0, members from +0x40) with retail offsets: the landed CRegionDef::Transfer
// 0x004715AB reads EnvironmentTheme at +0x3C (the retail CParentDefClassBase prefix is 0x39 bytes, Ego_r 0x3D, so the first member
// sits 4 below its PDB offset) and the CSoundMap member is 0x20 bytes (PDB 0x28: STLport maps are 12 bytes in retail), so every later
// member sits 0x0C below its PDB offset; the landed CRegion::GetWorldMapOffset 0x006BBF40 reads WorldMapOffset at +0xAC (PDB 0xB8) and
// ConstructRegionDescription reads RegionDescription (PDB +0xAC, unsigned long, a text-bank id) at +0xA0 (0x006058EB / 0x0060595E).
// Base: CParentDefClassBase (Ego_r: 0x3D rounded to the first member +0x40; retail 0x39 rounded to +0x3C). Members whose types have
// no shared header stay opaque with their real type in a comment. Needs FABLE_RETAIL_DEF_CLASS_BASE.
#include "engine/CParentDefClassBase.h"
#include "engine/C2DVector.h"
class CRegionDef : public CParentDefClassBase {
public:
    long          EnvironmentTheme;                          // +0x3c
    long          InternalEnvironmentTheme;                  // +0x40
    long          InternalLightingChannelCount;              // +0x44
    long          CameraManagerSetIndex;                     // +0x48
    long          CameraManagerSetBuildingIndex;             // +0x4c
    unsigned char SoundMap[0x20];                            // +0x50 CSoundMap (retail 0x20)
    bool          PreventHeroWeaponUse;                      // +0x70
    bool          PreventHeroWillUse;                        // +0x71
    unsigned char WanderingPopulationScriptDefName[4];       // +0x74 CDefString
    long          Music;                                     // +0x78
    unsigned char ReverbEnvironmentType[4];                  // +0x7c EReverbEnvironmentType
    float         ReverbEnvironmentLevel;                    // +0x80
    float         InBuildingOcclusionLevel;                  // +0x84
    bool          UseVillageTime;                            // +0x88
    bool          AllowHenchmen;                             // +0x89
    long          AttachToCameraFX;                          // +0x8c
    bool          EnableSpotlightShadowMode;                 // +0x90
    bool          CheckAllThingsOnMapForCombatCollision;     // +0x91
    long          HeroBreathParticle;                        // +0x94
    long          MaxNumAttackersOnHero;                     // +0x98
    bool          WeaponOutCrime;                            // +0x9c
    bool          UseCrimes;                                 // +0x9d
    bool          DisplayCrimes;                             // +0x9e
    bool          PermitCrimes;                              // +0x9f
    unsigned long RegionDescription;                         // +0xa0
    bool          HasSea;                                    // +0xa4
    long          LockToFrameRate;                           // +0xa8
    C2DVector     WorldMapOffset;                            // +0xac
    C2DVector     NameGraphicOffset;                         // +0xb4
    unsigned char NameGraphic[4];                            // +0xbc EWorldMapNameGraphic
    float         FishWeightMult;                            // +0xc0
};
FABLE_STATIC_ASSERT(offsetof(CRegionDef, EnvironmentTheme) == 0x3c);
FABLE_STATIC_ASSERT(offsetof(CRegionDef, PreventHeroWeaponUse) == 0x70);
FABLE_STATIC_ASSERT(offsetof(CRegionDef, RegionDescription) == 0xa0);
FABLE_STATIC_ASSERT(offsetof(CRegionDef, WorldMapOffset) == 0xac);
FABLE_STATIC_ASSERT(sizeof(CRegionDef) == 0xc4);
#endif // FABLE_ENGINE_CREGIONDEF_H
