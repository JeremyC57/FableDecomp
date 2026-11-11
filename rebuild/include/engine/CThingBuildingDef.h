#ifndef FABLE_ENGINE_CTHINGBUILDINGDEF_RETAIL_ENTRY_H
#define FABLE_ENGINE_CTHINGBUILDINGDEF_RETAIL_ENTRY_H
#ifdef FABLE_RETAIL_CTHINGBUILDINGDEF
// Retail opt-in (agentNF9, the CThingBuilding definition rows 0x0082D2D0..0x0082D8C0): Ego_r.pdb CThingBuildingDef
// (struct_layouts_egor.tsv, 0xA8 bytes; class key V) on the retail CThingBaseDef (0x70, Ego_r 0x74): every member sits 4 bytes
// below its PDB offset. Retail reads SimBuildingDef +0x74 (0x0082D2D0), Graphic +0x78 (0x0082D6F0 `add edi,78h`, 14 bytes),
// WallMaterial +0x88 (0x0082D6B0 / 0x0082D5C0), FloorMaterial +0x8C (0x0082D7D0), the reverb pair +0x90/+0x94 (0x0082D8C0),
// CameraManagerSetIndex +0x9C (0x0082D3B0) and UseHighDetailQuadTree as bit 0 of +0xA0 (0x0082D870 `mov al,[..+0A0h] ; and al,1`).
// Natural alignment (not packed): Ego_r puts WallMaterial 16 bytes after the 14-byte Graphic, retail too.
#include "engine/CThingBaseDef.h"
#include "engine/CEngineGraphic.h"
// Ego_r.pdb enum EReverbEnvironmentType (the type of ReverbEnvironmentType and of GetReverbSettings' first parameter,
// ?GetReverbSettings@CThingBuilding@@QBEXAAW4EReverbEnvironmentType@@AAM@Z). Its enumerators are in no name set on disk, so
// none is declared here (the rows only copy the value).
enum EReverbEnvironmentType { };
class CThingBuildingDef : public CThingBaseDef {
public:
    long DieOffTimer;                                                    // +0x70 (Ego_r +0x74)
    unsigned long SimBuildingDef;                                        // +0x74
    CEngineGraphic Graphic;                                              // +0x78 (14 bytes)
    long WallMaterial;                                                   // +0x88
    long FloorMaterial;                                                  // +0x8C
    EReverbEnvironmentType ReverbEnvironmentType;                        // +0x90
    float ReverbEnvironmentLevel;                                        // +0x94
    float OutsideOcclusionLevel;                                         // +0x98
    long CameraManagerSetIndex;                                          // +0x9C
    bool UseHighDetailQuadTree:1, IsSelfIlluminating:1, IsPartOfHeroGuild:1, HasWeatherMask:1;   // +0xA0
};
FABLE_STATIC_ASSERT(sizeof(CThingBuildingDef)==0xa4);
#else
#include "engine/_quarantine/CThingBuildingDef.h"
#endif
#endif
