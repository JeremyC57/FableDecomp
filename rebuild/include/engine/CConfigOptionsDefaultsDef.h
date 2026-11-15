#ifndef FABLE_PASS22_CONFIGOPTIONSDEFAULTSDEF_H
#define FABLE_PASS22_CONFIGOPTIONSDEFAULTSDEF_H
#ifdef FABLE_RETAIL_CCONFIGOPTIONSDEFAULTSDEF_PASS22
#include "engine/CParentDefClassBase.h"
class CConfigOptionsDefaultsDef : public CParentDefClassBase {      // Ego_r.pdb members at retail offsets (Ego_r - 4), sizeof 0x74
public:
    unsigned long Antialiasing;                                      // +0x3c
    long          ResolutionWidth;                                   // +0x40
    long          ResolutionHeight;                                  // +0x44
    long          BitDepth;                                          // +0x48
    long          MinResolutionWidth;                                // +0x4c
    long          MinResolutionHeight;                               // +0x50
    float         TextureDetail;                                     // +0x54
    float         MaxTextureDetail;                                  // +0x58
    float         ShadowDetail;                                      // +0x5c
    float         MaxShadowDetail;                                   // +0x60
    float         MeshDetail;                                        // +0x64
    float         MaxMeshDetail;                                     // +0x68
    float         EffectsDetail;                                     // +0x6c
    float         MaxEffectsDetail;                                  // +0x70
};

#else
#include "engine/_quarantine/CConfigOptionsDefaultsDef.h"
#endif
#endif
