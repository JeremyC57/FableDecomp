// CEngineScreenEffectDisplacementRenderer::ApplyDisplacement() -- draw the back-buffer copy over the back buffer through the
// displacement texture: inside a render-state capture block with both shader kinds forced on, apply the displacement vertex /
// pixel shaders, set alpha blending (src alpha / inv src alpha, alpha test >= 1), the optional stencil test (equal 1), turn
// the Z buffer / culling / dither off, load the bump-environment matrix (DisplaceAmount on the diagonal) on stages 0..2, the two
// user vertex constants, linear filtering and wrap / wrap / clamp addressing on stages 0..2, then DrawAsIndexedTris the quad.
// Retail: Fable.exe @ 0x00B87870, length 4944 bytes (0x1350), plain `ret` (tail-duplicated epilogue).
// Callees: ApplyVertexShader 0x00988020, ApplyPixelShader 0x00988140, SetUserVertexShaderConstant 0x00989B00, DrawAsIndexedTris
//   0x00A0AEA0, RestoreCaptureBlock 0x00A05840, Enable/DisableVertexShaders 0x00987FE0 / 0x009880E0, Enable/DisablePixelShaders
//   0x00988110 / 0x00988190, inline PrepareForRender(0) (SetVSConstants 0x0098B930, UpdatePixelShader 0x00988A20, ...).
// Leaf: ?ApplyDisplacement@CEngineScreenEffectDisplacementRenderer@@AAEXXZ
#define FABLE_RETAIL_CRENDERMANAGERCORE
#define FABLE_RETAIL_CRENDERMANAGERCORE_DRAWASINDEXEDTRIS
#define FABLE_RETAIL_CTEXTURE
#define FABLE_RETAIL_CRENDERSTATEMANAGER_STENCIL_INLINE
#define FABLE_RETAIL_CRENDERSTATEMANAGER_OUTLINE_INLINE
#define FABLE_RETAIL_CRENDERSTATEMANAGER_DISPLACEMENT_INLINE
#define FABLE_RETAIL_CENGINESCREENEFFECTDISPLACEMENTRENDERER
#define FABLE_RETAIL_CENGINESCREENEFFECTDISPLACEMENTRENDERER_MEMBERS
#include "engine/CRenderManager.h"
#include "engine/CShaderRenderManager.h"
#include "engine/CRenderStateCaptureBlock.h"
#include "engine/CScopedShadersEnable.h"
#include "engine/CEngineScreenEffectDisplacementRenderer.h"

namespace NEngineInternal { extern CRenderManager* GPRenderManager; }              // 0x01436E18
namespace NEngineInternal { extern CShaderRenderManager* GPShaderRenderManager; }  // 0x01436E14

void CEngineScreenEffectDisplacementRenderer::ApplyDisplacement()
{
    static unsigned short QuadIndices[6] = { 0, 1, 3, 1, 2, 3 };                   // 0x0139AAFC
    static bool UseStencil = true;                                                 // 0x0139AB08 (descriptive name)
    CRenderStateScopedCaptureBlock capture;
    CScopedShadersEnable shaders(true, true);
    NEngineInternal::GPShaderRenderManager->ApplyVertexShader(DisplacementVertexShader, false);
    NEngineInternal::GPShaderRenderManager->ApplyPixelShader(DisplacementPixelShader);
    NEngineInternal::GPRenderManager->SetAlphaBlendEnable(true);
    NEngineInternal::GPRenderManager->SetAlphaTestEnable(true);
    NEngineInternal::GPRenderManager->SetAlphaTestFunc(ALPHA_TEST_GREATER_EQUAL);
    NEngineInternal::GPRenderManager->SetAlphaRef(1);
    NEngineInternal::GPRenderManager->SetSourceAlphaBlendMode(ALPHA_BLEND_SRC_ALPHA);
    NEngineInternal::GPRenderManager->SetDestAlphaBlendMode(ALPHA_BLEND_INV_SRC_ALPHA);
    if (UseStencil)
    {
        NEngineInternal::GPRenderManager->SetStencilEnable(true);
        NEngineInternal::GPRenderManager->SetStencilFunc(STENCIL_FUNC_EQUAL);
        NEngineInternal::GPRenderManager->SetStencilOpFail(STENCIL_OP_MODE_KEEP);
        NEngineInternal::GPRenderManager->SetStencilOpZFail(STENCIL_OP_MODE_KEEP);
        NEngineInternal::GPRenderManager->SetStencilOpPass(STENCIL_OP_MODE_ZERO);
        NEngineInternal::GPRenderManager->SetStencilRef(1);
        NEngineInternal::GPRenderManager->SetStencilMask(1);
    }
    NEngineInternal::GPRenderManager->SetZBufferEnable(false);
    NEngineInternal::GPRenderManager->SetPolyCullMode(POLY_CULL_NONE);
    NEngineInternal::GPRenderManager->SetDitherEnable(false);
    NEngineInternal::GPRenderManager->SetTextureBumpEnvMatrix(0, DisplaceAmount, 0.0f, 0.0f, DisplaceAmount);
    NEngineInternal::GPRenderManager->SetTextureBumpEnvMatrix(1, DisplaceAmount, 0.0f, 0.0f, DisplaceAmount);
    NEngineInternal::GPRenderManager->SetTextureBumpEnvMatrix(2, DisplaceAmount, 0.0f, 0.0f, DisplaceAmount);
    NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(0, DisplaceAmount, 0.0f, 0.0f, DisplaceAmount);
    float offset = DisplaceAmount * -0.5f - 0.001953125f;
    NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(1, offset, offset, 0.0f, 0.0f);
    NEngineInternal::GPRenderManager->SetTextureFilterModeMag(0, TEXTURE_FILTER_LINEAR);
    NEngineInternal::GPRenderManager->SetTextureFilterModeMin(0, TEXTURE_FILTER_LINEAR);
    NEngineInternal::GPRenderManager->SetTextureFilterModeMag(1, TEXTURE_FILTER_LINEAR);
    NEngineInternal::GPRenderManager->SetTextureFilterModeMin(1, TEXTURE_FILTER_LINEAR);
    NEngineInternal::GPRenderManager->SetTextureFilterModeMag(2, TEXTURE_FILTER_LINEAR);
    NEngineInternal::GPRenderManager->SetTextureFilterModeMin(2, TEXTURE_FILTER_LINEAR);
    NEngineInternal::GPRenderManager->SetTextureUVModeU(0, TEXTURE_UV_WRAP);
    NEngineInternal::GPRenderManager->SetTextureUVModeV(0, TEXTURE_UV_WRAP);
    NEngineInternal::GPRenderManager->SetTextureUVModeU(1, TEXTURE_UV_WRAP);
    NEngineInternal::GPRenderManager->SetTextureUVModeV(1, TEXTURE_UV_WRAP);
    NEngineInternal::GPRenderManager->SetTextureUVModeU(2, TEXTURE_UV_CLAMP);
    NEngineInternal::GPRenderManager->SetTextureUVModeV(2, TEXTURE_UV_CLAMP);
    NEngineInternal::GPShaderRenderManager->PrepareForRender(0);
    const CTexture* textures[3] = { DisplacementTexture, BackBufferCopyTexture, BackBufferCopyTexture };
    NEngineInternal::GPRenderManager->DrawAsIndexedTris(textures, 3, TextureToBackBufferVerts, sizeof(CTVertexColTex1), 4, QuadIndices, 2);
}