// CEngineScreenEffectDisplacementRenderer::ShowDisplacementTexture() -- debug view (EnableDisplacementTextureView): draw the displacement texture over the back buffer inside a render-state capture block, with vertex and pixel shaders forced on and restored afterwards
// NF151 (2026-11-20, q4541 / q4669 / q11732): ported onto engine/CShaderRenderManager.h + engine/CScopedShadersEnable.h (the landed
//   0x00B5E700 / 0x00B5FAB0 form): the local CShaderEnableState stand-in (and its g_ShaderRenderManager global), the local CShaderRenderManager /
//   CVertexShaderConstantLayout / CArray / C4DVector models and the CShaderEnableSaver helper are gone. The row now binds the PDB spellings
//   ?Enable/DisableVertexShaders@CShaderRenderManager@@QAEXXZ (0x00987FE0 / 0x009880E0), ?ApplyPixelShader@CShaderRenderManager@@QAEXABVCPixelShader@@@Z
//   (0x00988140) and ?SingletonInstance@CShaderRenderManager@@0V1@A (0x013BC470, through the inline Get()).
// NF127 (2026-11-20): the row-local CShaderRenderManager declarations respelled in place to ?ApplyVertexShader@CShaderRenderManager@@QAEXABVCVertexShader@@_N@Z (0x00988020; Ego_r.pdb 0x00AE4680 + FableWin.pdb 0x03031510: const CVertexShader&, bool).
// Leaf: ShowDisplacementTexture
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B88BC0, length 784 bytes (body 0x308 + int3 padding), plain `ret`.
// Twin of ShowGlowTexture 0x00B61690 (same TU family): the same callees (EnableVertexShaders 0x00987FE0 / DisableVertexShaders
//   0x009880E0 / EnablePixelShaders 0x00988110 / DisablePixelShaders 0x00988190 on the static shader manager @ 0x013BC470,
//   ApplyVertexShader 0x00988020, ApplyPixelShader 0x00988140, the inlined PrepareForRender(0), DrawAsIndexedTris 0x00A0AEA0
//   with the quad indices @ 0x0139AB0C, RestoreCaptureBlock 0x00A05840) and the tracked states +0x28b8/+0x2868 (0) and +0x2890
//   (0x01396FB8) through a free `__forceinline SetState(value, state, core)`
//   (core passed last; the member `state.Set(v)` swaps core/value registers at the third state). The shader enables are saved in a two-bool frame object whose inline ctor/dtor force and restore them.
// Layout: engine/CEngineScreenEffectDisplacementRenderer.h (DisplacementTexture +0x10, ScreenCopyVertexShader +0x124,
//   ScreenCopyPixelShader +0x12c, DisplacementToBackBufferVerts +0x148, EnableDisplacementTextureView +0x1a8).
#include "fable_compiler.h"
// NF45 (round 7): the render core is NEngineInternal::GPRenderManager 0x01436E18 (Ego_r.pdb / FableWin.pdb
//   ?GPRenderManager@NEngineInternal@@3PAVCRenderManager@@A, engine/CRenderManager.h; CRenderManagerCore / CRenderStateManager at
//   offset 0): the three states are the shared inline setters AlphaBlendEnable (+0x28B8) / ZBufferEnable (+0x2868) false and
//   PolyCullMode (+0x2890) [POLY_CULL_NONE] through D3DPolyCullModeTable 0x01396FB0, and the quad is
//   ?DrawAsIndexedTris@CRenderManagerCore@@QAEXPAPBVCTexture@@KPBXJJPAXJ@Z 0x00A0AEA0 (repo opt-in DRAWASINDEXEDTRIS; const
//   CTexture* texture, the index table passed as the PDB's void*). The capture open / RestoreCaptureBlock pair on 0x013CB508 is a
//   CRenderStateScopedCaptureBlock (engine/CRenderStateCaptureBlock.h, CRenderStateCaptureBlock::StateManager). Was a local
//   CRenderManagerCoreView / CTrackedRenderState / SetState model, g_pRenderManagerCore / g_pRenderStateManager,
//   g_renderStateDefault_01396fb8 and a local struct CRenderStateCaptureBlock.
#define FABLE_RETAIL_CRENDERMANAGERCORE_DRAWASINDEXEDTRIS
#define FABLE_RETAIL_CRENDERSTATECAPTUREBLOCK_SCOPED_FORCEINLINE
#include "engine/CRenderManager.h"
#include "engine/CRenderStateCaptureBlock.h"
namespace NEngineInternal { extern CRenderManager* GPRenderManager; }   // 0x01436E18
#include "engine/CEngineScreenEffectDisplacementRenderer.h"

class CTexture;
#include "engine/CShaderRenderManager.h"
#include "engine/CScopedShadersEnable.h"
namespace NEngineInternal { extern CShaderRenderManager* GPShaderRenderManager; }    // 0x01436E14

extern const unsigned short g_QuadIndices_0139ab0c[6]; // 0x0139AB0C

struct CEngineScreenEffectDisplacementRenderer_Methods : CEngineScreenEffectDisplacementRenderer {
    void ShowDisplacementTexture();
};

void CEngineScreenEffectDisplacementRenderer_Methods::ShowDisplacementTexture()
{
    if (EnableDisplacementTextureView) {
        CRenderStateScopedCaptureBlock capture;
        CScopedShadersEnable shaders(true, true);
        NEngineInternal::GPShaderRenderManager->ApplyVertexShader(*(CVertexShader*)ScreenCopyVertexShader, false);
        NEngineInternal::GPShaderRenderManager->ApplyPixelShader(*(CPixelShader*)ScreenCopyPixelShader);
        NEngineInternal::GPRenderManager->SetAlphaBlendEnable(false);
        NEngineInternal::GPRenderManager->SetZBufferEnable(false);
        NEngineInternal::GPRenderManager->SetPolyCullMode(POLY_CULL_NONE);
        NEngineInternal::GPShaderRenderManager->PrepareForRender(0);
        const CTexture* texture = DisplacementTexture;
        NEngineInternal::GPRenderManager->DrawAsIndexedTris(&texture, 1, DisplacementToBackBufferVerts, 0x18, 4, const_cast<unsigned short*>(g_QuadIndices_0139ab0c), 2);
    }
}