#ifndef FABLE_ENGINE_CENGINEINTERNALPRIMITIVEPARTICLESPRITE_H
#define FABLE_ENGINE_CENGINEINTERNALPRIMITIVEPARTICLESPRITE_H
// New overlay header (row 0x00BBF4A0 CEnginePrimitiveManagerParticleSpriteGroup::RenderSubPrimitive): CEngineInternalPrimitiveParticleSprite
// : CEngineInternalPrimitiveBase, own members with their Ego_r.pdb names and offsets (struct_layouts_egor.tsv, sizeof 0x68; no container,
// retail = Ego_r): WorldPos +0x48 (0x00BC00C3), Size +0x54 (0x00BBFFF7 / 0x00BC000B), Angle +0x5C (0x00BC00E0), Colour +0x60 (B,G,R,A bytes,
// 0x00BC003D..0x00BC004D), Frame +0x64 (0x00BC001F).
#include "rebuild_abi.h"
#include "engine/CEngineInternalPrimitiveBase.h"
#include "engine/C3DVector.h"
#include "engine/C2DVector.h"
#include "engine/CRGBColour.h"
class CEngineInternalPrimitiveParticleSprite : public CEngineInternalPrimitiveBase {
public:
    C3DVector     WorldPos;                                              // +0x48
    C2DVector     Size;                                                  // +0x54
    float         Angle;                                                 // +0x5c
    CRGBColour    Colour;                                                // +0x60
    unsigned long Frame;                                                 // +0x64
};
FABLE_STATIC_ASSERT(sizeof(CEngineInternalPrimitiveParticleSprite) == 0x68);
#endif
