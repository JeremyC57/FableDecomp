#ifndef FABLE_ENGINE_CWEATHERMASK_H
#define FABLE_ENGINE_CWEATHERMASK_H
// NEW (chunk 2008, row 0x00B54890 CEngineWeatherRenderer::RenderRain via SetCommonParticleShaderConstants 0x00B52160). Ego_r.pdb CWeatherMask
// (class, 0x5C bytes, no containers: retail = Ego_r): BSphere +0x00 (C3DBoundingSphere), BBox +0x10 (C3DBoundingBox), InvTransform +0x28
// (CMatrix3x4), Scale +0x58. SetCommonParticleShaderConstants uploads InvTransform transposed and the box corners (0x00B521A8..0x00B52236);
// ClipBoxToWeatherMaskSphere 0x00B52090 reads BSphere. Needs FABLE_RETAIL_C3DBOUNDINGBOX for the corner accessors.
#include "engine/C3DBoundingSphere.h"
#include "engine/C3DBoundingBox.h"
#include "engine/CMatrix3x4.h"
class CWeatherMask {
public:
    C3DBoundingSphere BSphere;                                           // +0x00
    C3DBoundingBox    BBox;                                              // +0x10
    CMatrix3x4        InvTransform;                                      // +0x28
    float             Scale;                                             // +0x58
};
FABLE_STATIC_ASSERT(sizeof(CWeatherMask) == 0x5c);
#endif
