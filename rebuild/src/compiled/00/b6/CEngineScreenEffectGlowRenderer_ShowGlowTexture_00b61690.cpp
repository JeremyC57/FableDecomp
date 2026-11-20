// CEngineScreenEffectGlowRenderer::ShowGlowTexture() -- debug view: draw the glow texture picked by ShowDebug (1: 256, 2: 64, else 16) over the back buffer inside a render-state capture block, with vertex and pixel shaders forced on and restored afterwards
// NF151 (2026-11-20, q4541 / q4669 / q11731): ported onto engine/CShaderRenderManager.h + engine/CScopedShadersEnable.h (the landed
//   0x00B5E700 / 0x00B5FAB0 form): the local CShaderEnableState stand-in (and its g_ShaderRenderManager global), the local CShaderRenderManager /
//   CVertexShaderConstantLayout / CArray / C4DVector models and the CShaderEnableSaver helper are gone. The row now binds the PDB spellings
//   ?Enable/DisableVertexShaders@CShaderRenderManager@@QAEXXZ (0x00987FE0 / 0x009880E0), ?ApplyPixelShader@CShaderRenderManager@@QAEXABVCPixelShader@@@Z
//   (0x00988140) and ?SingletonInstance@CShaderRenderManager@@0V1@A (0x013BC470, through the inline Get()).
// NF127 (2026-11-20): the row-local CShaderRenderManager declarations respelled in place to ?ApplyVertexShader@CShaderRenderManager@@QAEXABVCVertexShader@@_N@Z (0x00988020; Ego_r.pdb 0x00AE4680 + FableWin.pdb 0x03031510: const CVertexShader&, bool).
// Leaf: ShowGlowTexture
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B61690, length 800 bytes (body 0x31b + int3 padding), plain `ret`.
// Callees: CShaderRenderManager::EnableVertexShaders @ 0x00987FE0 / DisableVertexShaders @ 0x009880E0 / EnablePixelShaders
//   @ 0x00988110 / DisablePixelShaders @ 0x00988190 on the static shader manager object @ 0x013BC470 (VertexShadersEnabled
//   +0x390, PixelShadersEnabled +0x391), ApplyVertexShader @ 0x00988020, ApplyPixelShader @ 0x00988140, the inlined
//   PrepareForRender(0) (landed model of 0x00B54100), CRenderManagerCore::DrawAsIndexedTris @ 0x00A0AEA0 (textures,
//   count, vertices, stride, vertex count, indices @ 0x0139A6D0, triangles), CRenderStateManager::RestoreCaptureBlock
//   @ 0x00A05840.
// Render core: tracked states +0x28b8 / +0x2868 (0) and +0x2890 (global default 0x01396FB8) through a free
//   `__forceinline SetState(value, state, core)` -- VALUE FIRST, core last: the member Set or a (state, value, core)
//   order swaps the core/value registers at the third state's join (retail re-reads the core for the mask); the capture block opens with `m_captured[m_capturedCount++]` (landed 0x00B63010 form).
// Layout: engine/CEngineScreenEffectGlowRenderer.h (GlowTexture256 +0x10, GlowTexture64 +0x14, GlowTexture16 +0x1c,
//   VertexShaderShowDebug +0x6c, PixelShaderShowDebug +0x74, ShowDebug +0x218, GlowToBackBufferVerts +0x220).
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
#include "engine/CEngineScreenEffectGlowRenderer.h"

class CTexture;
#include "engine/CShaderRenderManager.h"
#include "engine/CScopedShadersEnable.h"
namespace NEngineInternal { extern CShaderRenderManager* GPShaderRenderManager; }    // 0x01436E14

extern const unsigned short g_QuadIndices_0139a6d0[6]; // 0x0139A6D0

struct CEngineScreenEffectGlowRenderer_Methods : CEngineScreenEffectGlowRenderer {
    void ShowGlowTexture();
};

void CEngineScreenEffectGlowRenderer_Methods::ShowGlowTexture()
{
    CRenderStateScopedCaptureBlock capture;
    CScopedShadersEnable shaders(true, true);
    NEngineInternal::GPShaderRenderManager->ApplyVertexShader(*(CVertexShader*)VertexShaderShowDebug, false);
    NEngineInternal::GPShaderRenderManager->ApplyPixelShader(*(CPixelShader*)PixelShaderShowDebug);
    NEngineInternal::GPRenderManager->SetAlphaBlendEnable(false);
    NEngineInternal::GPRenderManager->SetZBufferEnable(false);
    NEngineInternal::GPRenderManager->SetPolyCullMode(POLY_CULL_NONE);
    NEngineInternal::GPShaderRenderManager->PrepareForRender(0);
    const CTexture* texture;
    switch (ShowDebug) {
    case 1:
        texture = GlowTexture256;
        break;
    case 2:
        texture = GlowTexture64;
        break;
    default:
        texture = GlowTexture16;
        break;
    }
    NEngineInternal::GPRenderManager->DrawAsIndexedTris(&texture, 1, GlowToBackBufferVerts, 0x18, 4, const_cast<unsigned short*>(g_QuadIndices_0139a6d0), 2);
}