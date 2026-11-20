// CShaderRenderManager::DisableVertexShaders() -- turn the vertex-shader path off: flag cleared, current shader cleared, the render manager told `false`
// through its XBox shader-constant stub, then its fixed-function pipeline flag set.
// Leaf: ?DisableVertexShaders@CShaderRenderManager@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009880E0.
// Port (NF133, q4727): the July CShaderRenderManagerDisableVertexView / CDisplayEngineVertexShaderView stand-ins replaced by the shared
//   engine/CShaderRenderManager.h (VertexShadersEnabled +0x390, CurrentVertexShader +0x1D8, RenderManager +0x1D0)
//   and engine/CRenderManagerCore.h (FixedFunctionPipelineEnabled +0x3CFD, EnableXBoxShaderConstants 0x00A0AA20,
//   Ego_r.pdb / FableWin.pdb ?EnableXBoxShaderConstants@CRenderManagerCore@@QAEX_N@Z). Own leaf now the Ego_r.pdb / FableWin.pdb spelling
//   the landed callers bind (0x009D8AD0 ...).
#define FABLE_RETAIL_CRENDERMANAGERCORE
#define FABLE_RETAIL_C2DBOXF
#include "fable_compiler.h"
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/C2DBoxF.h"
#include "engine/CShaderRenderManager.h"
#include "engine/CRenderManager.h"

void CShaderRenderManager::DisableVertexShaders()
{
    VertexShadersEnabled = false;
    CurrentVertexShader = 0;
    RenderManager->EnableXBoxShaderConstants(false);
    RenderManager->FixedFunctionPipelineEnabled = true;
}
