#ifndef FABLE_ENGINE_CENGINELOCALLIGHTARRAY_H
#define FABLE_ENGINE_CENGINELOCALLIGHTARRAY_H
// CEngineLocalLightArray (chunk 723c): Ego_r.pdb class (V-keyed: ?SetupShaderPointLights@CEngineLightingManager@@QAEXABV
// CEngineLocalLightArray@@@Z), 0x1A4 bytes: LightCount +0 (read by RenderSingleSprite 0x00BC04F0 and RenderIceForeground 0x00B751F0),
// CEngineLocalLightElement Lights[8] +4 (opaque here). No container member (retail = Ego_r).
#include "rebuild_abi.h"
class CEngineLocalLightArray {
public:
    unsigned long LightCount;                                            // +0x00
    unsigned char Lights[0x1a0];                                         // +0x04 CEngineLocalLightElement[8]
};
FABLE_STATIC_ASSERT(sizeof(CEngineLocalLightArray) == 0x1a4);
#endif // FABLE_ENGINE_CENGINELOCALLIGHTARRAY_H
