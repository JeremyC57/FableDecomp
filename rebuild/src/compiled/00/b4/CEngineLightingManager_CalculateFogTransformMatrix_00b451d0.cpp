// CEngineLightingManager::CalculateFogTransformMatrix(float fogStart, float fogEnd) -- the fog coordinate transform: the depth-to-fog matrix (z -> (z - start) / (end - start)) pre-multiplied by the camera's no-scale view transform, returned as that product's first column (the X row of the fog vertex-shader constant)
// Leaf: ?CalculateFogTransformMatrix@CEngineLightingManager@@AAE?AVC4DVector@@MM@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B451D0, 304 bytes (`ret 0Ch`: the hidden C4DVector result, fogStart, fogEnd). Ego_r.pdb
//   ?CalculateFogTransformMatrix@CEngineLightingManager@@AAE?AVC4DVector@@MM@Z.
// Globals: the engine camera @ 0x01436EA0 (NoScaleViewTransform +0x114), 1.0f @ 0x0122DED8.
// Shape: the landed same-module sibling SetupDefaultFogStates 0x00B47630 (which expands the same product; its bodies now sit in the
//   shared CMatrix3x4.h behind FABLE_RETAIL_CMATRIX3X4_PREMULTIPLY): the 12-float CMatrix3x4
//   constructor (??0CMatrix3x4@@QAE@MMMMMMMMMMMM@Z) and FableWin's ?PreMultiply@CMatrix3x4@@QAEXABV1@@Z (inline in retail: the copy of
//   *this is the `rep movsd`), then C4DVector(E11, E21, E31, E41).
#define FABLE_RETAIL_CENGINELIGHTINGMANAGER
#define FABLE_RETAIL_C4DVECTOR
#define FABLE_RETAIL_CENGINECOMPONENT
#define FABLE_RETAIL_CENGINELIGHTINGMANAGER_FOGTRANSFORM
#define FABLE_RETAIL_CMATRIX3X4
#define FABLE_RETAIL_CMATRIX3X4_PREMULTIPLY
#define FABLE_RETAIL_CENGINECAMERA
#define FABLE_RETAIL_CENGINECAMERA_NOSCALEVIEWTRANSFORM
#include "engine/C4DVector.h"
#include "engine/CMatrix3x4.h"
#include "engine/CEngineCamera.h"
#include "engine/CEngineLightingManager.h"

extern CEngineCamera* g_pEngineCamera;              // 0x01436EA0

C4DVector CEngineLightingManager::CalculateFogTransformMatrix(float fogStart, float fogEnd)
{
    float scale = 1.0f / (fogEnd - fogStart);
    CMatrix3x4 fog(0.0f, 0.0f, 0.0f,
                   0.0f, 0.0f, 0.0f,
                   scale, 0.0f, 0.0f,
                   -(scale * fogStart), 0.0f, 0.0f);
    fog.PreMultiply(g_pEngineCamera->NoScaleViewTransform);
    return C4DVector(fog.E11, fog.E21, fog.E31, fog.E41);
}