// CEngineLightingManager::SetupShaderSpotLight(const CMatrix3x4& transform) -- load the shadowed spot light's falloff (the transform times the shadow renderer's spot-light falloff matrix, the radius-ratio attenuation and the light colour) into the shader render manager
// Leaf: ?SetupShaderSpotLight@CEngineLightingManager@@QAEXABVCMatrix3x4@@@Z
// Retail: Fable.exe (TLC, VC7.1) 0x00B47390, 665 bytes, `ret 4`. Grade: RELOCATION_MATCH (e06 egod lane, first compiling grade).
// Names: Ego_r.pdb / FableWin.pdb ?SetupShaderSpotLight@CEngineLightingManager@@QAEXABVCMatrix3x4@@@Z. Ego_d has only the record of the
//   no-argument twin (0x00B47310: scene_data a const CShadowRendererSceneData&, floats light_attenuation_squared / light_attenuation_constant);
//   this overload uses the same names (borrowed from the twin: FITTED for this row). The scene data comes through the inline
//   PeekSceneData() (returns a const value: the reference binds the copy-constructed temporary, 0x00B473A8 call 0x00B38600). The two
//   `rep movsd` copies of the transform (0x00B473CB / 0x00B473D8) are the named falloff_matrix and PostMultiply's copy of *this
//   (shared CMatrix3x4 SetMultiply / PostMultiply bodies).
// Callees: CShadowRendererSceneData(const&) 0x00B38600, CShaderRenderManager::SetShadowSpotLightFallloffParameters(const CMatrix3x4&, float,
//   float, const CRGBFloatColour&) 0x009899B0 (Ego_r / FableWin spelling, "Fallloff"); globals NEngineInternal::GPShadowRenderer 0x01436E60,
//   NEngineInternal::GPShaderRenderManager 0x01436E14; 1.0f 0x0122DED8. Member CurrentShadowedSpotLightParams +0x4698 (shared header).
// Headers: existing opt-in FABLE_RETAIL_CENGINELIGHTINGMANAGER_SPOTLIGHT (the CMatrix3x4 overload declaration, landed row 0x00BB3E80) and
//   the other existing opt-ins defined below (GFSqr through C3DVECTOR_NORMALISE_SQUARE_HELPER). FITTED: the GFSqr spelling of the squared ratio (graded first and matched;
//   the in-place `x *= x` alternative was not graded).
#define FABLE_RETAIL_CENGINELIGHTINGMANAGER
#define FABLE_RETAIL_C3DVECTOR_NORMALISE_SQUARE_HELPER
#define FABLE_RETAIL_CENGINELIGHTINGMANAGER_MEMBERS
#define FABLE_RETAIL_CENGINELIGHTINGMANAGER_SPOTLIGHT
#define FABLE_RETAIL_CENGINESHADOWRENDERER
#define FABLE_RETAIL_CENGINESHADOWRENDERER_MEMBERS
#define FABLE_RETAIL_CENGINESHADOWRENDERER_PEEKSCENEDATA
#define FABLE_RETAIL_CSHADOWRENDERERSCENEDATA_COPY
#define FABLE_RETAIL_PASS22_SHADER_ORIGINAL_METHOD_ORDER
#define FABLE_RETAIL_CMATRIX3X4_SETMULTIPLY
#define FABLE_RETAIL_CMATRIX3X4_SETMULTIPLY_PREMULTIPLY
#define FABLE_RETAIL_CMATRIX3X4_SETMULTIPLY_POSTMULTIPLY
#include "engine/C3DVector.h"
#include "engine/CMatrix3x4.h"
#include "engine/CEngineLightingManager.h"
#include "engine/CEngineShadowRenderer.h"
#include "engine/CShaderRenderManager.h"
namespace NEngineInternal { extern CEngineShadowRenderer* GPShadowRenderer; }   // 0x01436E60
namespace NEngineInternal { extern CShaderRenderManager* GPShaderRenderManager; }   // 0x01436E14

void CEngineLightingManager::SetupShaderSpotLight(const CMatrix3x4& transform)
{
    const CShadowRendererSceneData& scene_data = NEngineInternal::GPShadowRenderer->PeekSceneData();
    float light_attenuation_squared = GFSqr(CurrentShadowedSpotLightParams.SpotRadius / CurrentShadowedSpotLightParams.Radius);
    float light_attenuation_constant = 1.0f / (1.0f / light_attenuation_squared - 1.0f);
    CMatrix3x4 falloff_matrix(transform);                                   // FITTED name (not in Ego_d)
    falloff_matrix.PostMultiply(scene_data.SpotLightFalloffMatrix);
    NEngineInternal::GPShaderRenderManager->SetShadowSpotLightFallloffParameters(falloff_matrix, light_attenuation_constant,
        -light_attenuation_constant, CurrentShadowedSpotLightParams.Colour);
}