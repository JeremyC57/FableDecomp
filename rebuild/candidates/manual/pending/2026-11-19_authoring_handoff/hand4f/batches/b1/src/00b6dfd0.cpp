// CEngineWaterRenderer::GetIceForegroundColbuffShadowedVertexShader(long point_light_count) -- the colour-buffer shadowed ice foreground vertex shader for n point lights, clamped to the last one in the set
// Leaf: ?GetIceForegroundColbuffShadowedVertexShader@CEngineWaterRenderer@@AAEAAVCVertexShader@@J@Z
// Retail: Fable.exe @ 0x00B6DFD0, 54 bytes, `ret 4`. Identity: rebuild/oracles/reloc_bindings.tsv binds this symbol to 0x00B6DFD0 (caller
// RenderIceForeground 0x00B751F0); Ego_r.pdb Engine_Water_Renderer.obj #29 / FableWin.pdb #24, after
// GetIceForegroundShadowedVertexShader (the catalog's CEngineLandscapeRenderer label was a byte-shape propagation).
// Shape: FableWin /Od 0x02D4CEA0 -- one return of operator[] with a ternary clamp (`n < size() - 1 ? n : size() - 1`, unsigned compare);
// Ego_d parameter name point_light_count. Same text as the landed landscape twin 0x00BF5C60.
#include "fable_compiler.h"
#define FABLE_RETAIL_CENGINEWATERRENDERER
#define FABLE_RETAIL_CENGINEWATERRENDERER_FOREGROUNDHELPERS
#include "engine/CEngineWaterRenderer.h"

CVertexShader& CEngineWaterRenderer::GetIceForegroundColbuffShadowedVertexShader(long point_light_count)
{
    return IceForegroundColbuffShadowedVertexShader[(unsigned long)point_light_count < IceForegroundColbuffShadowedVertexShader.size() - 1 ? point_light_count : IceForegroundColbuffShadowedVertexShader.size() - 1];
}
