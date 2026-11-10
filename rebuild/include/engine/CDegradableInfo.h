#ifndef FABLE_ENGINE_CDEGRADABLEINFO_H
#define FABLE_ENGINE_CDEGRADABLEINFO_H
// Retail model (chunk 1018, row 0x0067E290 CDegradableDef::Transfer): Ego_r.pdb CDegradableInfo (28 bytes, struct_layouts_egor.tsv):
// HealthPercentage +0, the 14-byte packed CEngineGraphic +4 (its Type byte at +0x11: 0x0067E390 `mov [eax+11h],cl`),
// SmashParticleEmitter +0x14, BlocksNavigation +0x18; stride 0x1C (0x0067E393 `add eax,1Ch`). Ego_r.pdb ??0CDegradableInfo@@QAE@XZ
// (declaration only).
#include "rebuild_abi.h"
#include "engine/CEngineGraphic.h"
class CDegradableInfo {
public:
    CDegradableInfo();
    float          HealthPercentage;                       // +0x00
    CEngineGraphic EngineGraphic;                          // +0x04
    long           SmashParticleEmitter;                   // +0x14
    bool           BlocksNavigation;                       // +0x18
};
FABLE_STATIC_ASSERT(sizeof(CDegradableInfo) == 0x1C);
#endif
