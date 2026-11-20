// CEngineScreenEffectRadialBlurRenderer::ShowBlurTexture() -- debug view: draw the blur texture picked by ShowDebug (1: the first 256 target, else the second) over the back buffer inside a render-state capture block, with vertex constant layout 2 and the vertex/pixel shaders forced on, all restored afterwards
// NF127 (2026-11-20): the row-local CShaderRenderManager declarations respelled in place to ?ApplyVertexShader@CShaderRenderManager@@QAEXABVCVertexShader@@_N@Z (0x00988020; Ego_r.pdb 0x00AE4680 + FableWin.pdb 0x03031510: const CVertexShader&, bool).
// NF134 (2026-11-20, q4650): ported onto engine/CShaderRenderManager.h with the shared CScopedVertexShaderConstantLayout /
//   CScopedShadersEnable (Ego_r.pdb + FableWin.pdb ??0CScopedVertexShaderConstantLayout@@QAE@J@Z / ??0CScopedShadersEnable@@QAE@_N0@Z, inline
//   here as in the landed twins 0x00B2B180 / 0x00B5E0F0): SetVertexShaderConstantLayout 0x0098B5E0 now binds
//   ?SetVertexShaderConstantLayout@CShaderRenderManager@@QAEXJ@Z (Ego_r.pdb 0x00AE7D60 + FableWin.pdb 0x03030910), the singleton 0x013BC470
//   ?SingletonInstance@CShaderRenderManager@@0V1@A, Enable/Disable{Vertex,Pixel}Shaders and ApplyPixelShader (const CPixelShader&) the PDB
//   spellings. Was the row-local CShaderEnableState / CShaderRenderManager / CVertexShaderConstantLayout / CArray models and g_ShaderRenderManager.
// Leaf: ShowBlurTexture
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B5E700, length 816 bytes (body 0x323 + int3 padding), plain `ret`.
// Twin of ShowGlowTexture 0x00B61690 plus a layout save: CShaderRenderManager::SetVertexShaderConstantLayout(long)
//   @ 0x0098B5E0 on the static shader manager @ 0x013BC470 (current layout index +0x8), restored at exit. Same callees
//   otherwise (Enable/DisableVertexShaders 0x00987FE0/0x009880E0, Enable/DisablePixelShaders 0x00988110/0x00988190,
//   ApplyVertexShader 0x00988020, ApplyPixelShader 0x00988140, inlined PrepareForRender(0), DrawAsIndexedTris 0x00A0AEA0
//   with the quad indices @ 0x0139A550, RestoreCaptureBlock 0x00A05840), tracked states +0x28b8/+0x2868 (0) and +0x2890
//   (0x01396FB8) through the value-first free SetState(value, state, core). The three saves are inline RAII frame objects
//   (capture block, layout, shader enables) destroyed in reverse order, as retail restores them.
// Layout: engine/CEngineScreenEffectRadialBlurRenderer.h (BlurTexture256_1 +0x0c, BlurTexture256_2 +0x10,
//   ScreenCopyVertexShader +0x20, TextureOnlyPixelShader +0x40, ShowDebug +0x1c4, BlurToBackBufferVerts +0x1cc).
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
#include "engine/CEngineScreenEffectRadialBlurRenderer.h"

class CTexture;
#include "engine/CShaderRenderManager.h"
#include "engine/CScopedShadersEnable.h"
#include "engine/CScopedVertexShaderConstantLayout.h"
namespace NEngineInternal { extern CShaderRenderManager* GPShaderRenderManager; }    // 0x01436E14

extern const unsigned short g_QuadIndices_0139a550[6]; // 0x0139A550

struct CEngineScreenEffectRadialBlurRenderer_Methods : CEngineScreenEffectRadialBlurRenderer {
    void ShowBlurTexture();
};

void CEngineScreenEffectRadialBlurRenderer_Methods::ShowBlurTexture()
{
    CRenderStateScopedCaptureBlock capture;
    CScopedVertexShaderConstantLayout layout(2);
    CScopedShadersEnable shaders(true, true);
    NEngineInternal::GPShaderRenderManager->ApplyVertexShader(*(CVertexShader*)ScreenCopyVertexShader, false);
    NEngineInternal::GPShaderRenderManager->ApplyPixelShader(*(CPixelShader*)TextureOnlyPixelShader);
    NEngineInternal::GPRenderManager->SetAlphaBlendEnable(false);
    NEngineInternal::GPRenderManager->SetZBufferEnable(false);
    NEngineInternal::GPRenderManager->SetPolyCullMode(POLY_CULL_NONE);
    NEngineInternal::GPShaderRenderManager->PrepareForRender(0);
    const CTexture* texture;
    switch (ShowDebug) {
    case 1:
        texture = BlurTexture256_1;
        break;
    default:
        texture = BlurTexture256_2;
        break;
    }
    NEngineInternal::GPRenderManager->DrawAsIndexedTris(&texture, 1, BlurToBackBufferVerts, 0x18, 4, const_cast<unsigned short*>(g_QuadIndices_0139a550), 2);
}