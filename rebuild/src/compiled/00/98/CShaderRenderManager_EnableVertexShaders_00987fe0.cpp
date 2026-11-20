// CShaderRenderManager::EnableVertexShaders() -- turn the vertex-shader path on: flag set, current shader cleared, software lighting off,
// the render manager's fixed-function pipeline flag cleared and its XBox shader-constant stub told `true`, every update flag raised.
// Leaf: ?EnableVertexShaders@CShaderRenderManager@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00987FE0.
// Port (NF133, q4727): the July CShaderRenderManagerEnableVertexView / CDisplayEngineVertexShaderView stand-ins replaced by the shared
//   engine/CShaderRenderManager.h (VertexShadersEnabled +0x390, CurrentVertexShader +0x1D8, SWLightingEnable +0x392, RenderManager +0x1D0,
//   UpdateFlags +0x3D8) and engine/CRenderManagerCore.h (FixedFunctionPipelineEnabled +0x3CFD, EnableXBoxShaderConstants 0x00A0AA20,
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

void CShaderRenderManager::EnableVertexShaders()
{
    VertexShadersEnabled = true;
    CurrentVertexShader = 0;
    SWLightingEnable = false;
    RenderManager->FixedFunctionPipelineEnabled = false;
    RenderManager->EnableXBoxShaderConstants(true);
    UpdateFlags = 0xFFFFFFFF;
}
