#ifndef FABLE_ENGINE_CENGINECOMPONENTRENDERPARAMS_H
#define FABLE_ENGINE_CENGINECOMPONENTRENDERPARAMS_H
// NEW (chunk 2008, row 0x00B665E0). Ego_r.pdb CEngineComponentRenderParams (class, 12 bytes: Params +0, RenderLayer +4 (EEngineRenderLayer),
// RenderLayerFlags +8 (ERenderRenderLayerFlags)) and CEngineComponentParams (class, 0x80 bytes: FrameInterpolation +0, InterpolationInfo
// +4 (CEngineInterpolationInfo, 0x70), FrameID +0x74, SceneFilterFlags +0x78, OffWorldTickProcessFlag +0x7C). The interpolation info
// and the two enums are kept opaque / as long until a row needs their members (0x00B667C2 reads Params->FrameInterpolation).
#include "rebuild_abi.h"
class CEngineComponentParams {
public:
    float         FrameInterpolation;                                    // +0x00
    unsigned char InterpolationInfo[0x70];                               // +0x04 CEngineInterpolationInfo
    unsigned long FrameID;                                               // +0x74
    unsigned long SceneFilterFlags;                                      // +0x78
    bool          OffWorldTickProcessFlag;                               // +0x7c
};
FABLE_STATIC_ASSERT(sizeof(CEngineComponentParams) == 0x80);
class CEngineComponentRenderParams {
public:
    CEngineComponentParams* Params;                                      // +0x00
    long                    RenderLayer;                                 // +0x04 EEngineRenderLayer
    unsigned long           RenderLayerFlags;                            // +0x08 ERenderRenderLayerFlags
};
FABLE_STATIC_ASSERT(sizeof(CEngineComponentRenderParams) == 0xc);
#endif
