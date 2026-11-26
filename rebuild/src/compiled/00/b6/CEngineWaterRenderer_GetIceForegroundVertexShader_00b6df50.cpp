// CEngineWaterRenderer::GetIceForegroundVertexShader(long point_light_count) -- the ice foreground vertex shader for n point lights, clamped to the last one in the set
// Leaf: ?GetIceForegroundVertexShader@CEngineWaterRenderer@@AAEAAVCVertexShader@@J@Z
// Retail: Fable.exe @ 0x00B6DF50, 54 bytes, `ret 4`. Identity: Ego_r.pdb 0x00A1B5B0 and FableWin.pdb 0x02D4CDC0, both
//   ?GetIceForegroundVertexShader@CEngineWaterRenderer@@AAEAAVCVertexShader@@J@Z. The manifest label
//   CEngineLandscapeRenderer::GetForegroundSpotLightVertexShader is the landed row 0x00BF5C60 (array at +0x4E8).
//   This body reads IceForegroundVertexShader at +0x12C. Same clamp as the landed twins 0x00B6DF90 and 0x00BF5C60.
#include "fable_compiler.h"
#define FABLE_RETAIL_CENGINEWATERRENDERER
#define FABLE_RETAIL_CENGINEWATERRENDERER_FOREGROUNDHELPERS
#include "engine/CEngineWaterRenderer.h"

CVertexShader& CEngineWaterRenderer::GetIceForegroundVertexShader(long point_light_count)
{
    return IceForegroundVertexShader[(unsigned long)point_light_count < IceForegroundVertexShader.size() - 1 ? point_light_count : IceForegroundVertexShader.size() - 1];
}