// CShaderRenderManager::ApplyVertexShader(const CVertexShader& shader, bool force) -- drop the cached shader when the vertex-processing mode
// the current pipeline wants (software fixed-function or software shaders) differs from the render manager's; then, when the shader or
// its declaration changed, cache both, clear the render manager's fixed-function flag and select them on the device
// Leaf: ?ApplyVertexShader@CShaderRenderManager@@QAEXABVCVertexShader@@_N@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00988020, 176 bytes, `ret 8` (`force` is never read; the SetVertexDeclaration call is a tail jump).
// Identity: Ego_r.pdb 0x00AE4680 / FableWin.pdb 0x03031510 ?ApplyVertexShader@CShaderRenderManager@@QAEXABVCVertexShader@@_N@Z; every landed
//   binder already binds this spelling (nf127 q4540).
// Port (NF151, q11717): the July CShaderRenderManagerApplyVertexShaderView / CDisplayEngineApplyVertexShaderView / CVertexShaderApplyView
//   stand-ins replaced by the shared headers: engine/CShaderRenderManager.h (SoftwareVertexShaderEnable +0x393, SoftwareFixedFunctionEnable
//   +0x394, FixedFunctionVertexMode +0x395, RenderManager +0x1D0, D3DDevice +0x1D4, CurrentVertexShader +0x1D8,
//   CurrentVertexShaderVertexDeclairation +0x1DC), engine/CVertexShader.h (CVertexShaderData VertexDeclaration +4 / VertexShader +8),
//   engine/CRenderManagerCore.h (FixedFunctionPipelineEnabled +0x3CFD) and engine/CRenderStateManager.h: FableWin.pdb's inline
//   ?IsSoftwareProcessingEnabled@CRenderStateManager@@QAE_NXZ reads the SoftwareVertexProcessing state at +0x2908 (opt-in
//   FABLE_RETAIL_CRENDERSTATEMANAGER_ISSOFTWAREPROCESSINGENABLED_INLINE). IDirect3DDevice9 SetVertexShader slot 92 (+0x170),
//   SetVertexDeclaration slot 87 (+0x15C).
#define FABLE_RETAIL_CRENDERMANAGERCORE
#define FABLE_RETAIL_C2DBOXF
#define FABLE_RETAIL_CVERTEXSHADER
#define FABLE_RETAIL_CRENDERSTATEMANAGER_ISSOFTWAREPROCESSINGENABLED_INLINE
#include "fable_compiler.h"
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/C2DBoxF.h"
#include "engine/CShaderRenderManager.h"
#include "engine/CVertexShader.h"
#include "engine/CRenderManager.h"

void CShaderRenderManager::ApplyVertexShader(const CVertexShader& shader, bool force)
{
    bool software_processing = FixedFunctionVertexMode ? SoftwareFixedFunctionEnable : SoftwareVertexShaderEnable;
    if (software_processing != RenderManager->IsSoftwareProcessingEnabled())
        CurrentVertexShader = 0;
    if (CurrentVertexShader != shader.Data->VertexShader || CurrentVertexShaderVertexDeclairation != shader.Data->VertexDeclaration) {
        CurrentVertexShader = shader.Data->VertexShader;
        CurrentVertexShaderVertexDeclairation = shader.Data->VertexDeclaration;
        RenderManager->FixedFunctionPipelineEnabled = false;
        D3DDevice->SetVertexShader(CurrentVertexShader);
        D3DDevice->SetVertexDeclaration(CurrentVertexShaderVertexDeclairation);
    }
}
