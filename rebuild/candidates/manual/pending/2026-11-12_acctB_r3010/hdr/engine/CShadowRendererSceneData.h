#ifndef FABLE_ENGINE_CSHADOWRENDERERSCENEDATA_H
#define FABLE_ENGINE_CSHADOWRENDERERSCENEDATA_H
// CShadowRendererSceneData (chunk 727): Ego_r.pdb class, 0x240 bytes, no container member (retail = Ego_r; CEngineShadowRenderer
// +0x0C SceneData: 0x00B4BCCA passes +0x0C as WorldToShadowBufferMatrix, 0x00B4BFAx reads ShadowFadeStart +0xFC and
// ShadowLightVector +0x104, the volumes sit at +0x11C / +0x1B4).
#include <stddef.h>
#include "rebuild_abi.h"
#include "engine/C3DVector.h"
#include "engine/CMatrix3x4.h"
#include "engine/CMatrix4x4.h"
#include "engine/CShadowClipVolume.h"
class CShadowRendererSceneData {
public:
    CMatrix4x4        WorldToShadowBufferMatrix;                         // +0x000
    CMatrix4x4        WorldToShadowBufferMatrixHomogenousScreenSpace;    // +0x040
    CMatrix4x4        WorldToShadowBufferMatrixTextureSpace;             // +0x080
    CMatrix3x4        SpotLightFalloffMatrix;                            // +0x0c0
    float             ShadowFadeStart;                                   // +0x0f0
    float             ShadowFadeEnd;                                     // +0x0f4
    C3DVector         ShadowLightVector;                                 // +0x0f8
    C3DVector         CameraPos;                                         // +0x104
    CShadowClipVolume CasterClipVolume;                                  // +0x110
    CShadowClipVolume RecieverClipVolume;                                // +0x1a8
};
FABLE_STATIC_ASSERT(sizeof(CShadowRendererSceneData) == 0x240);
FABLE_STATIC_ASSERT(offsetof(CShadowRendererSceneData, ShadowFadeStart) == 0xf0);
FABLE_STATIC_ASSERT(offsetof(CShadowRendererSceneData, CasterClipVolume) == 0x110);
FABLE_STATIC_ASSERT(offsetof(CShadowRendererSceneData, RecieverClipVolume) == 0x1a8);
#endif // FABLE_ENGINE_CSHADOWRENDERERSCENEDATA_H
