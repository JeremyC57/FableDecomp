// CEngineScreenEffectGlowRenderer::ApplyGlow() -- blends the glow textures onto the back buffer.
// Leaf: ?ApplyGlow@CEngineScreenEffectGlowRenderer@@AAEXXZ
// Flags: default
#define FABLE_RETAIL_CRENDERMANAGERCORE_DRAWASINDEXEDTRIS
#define FABLE_RETAIL_CRENDERSTATECAPTUREBLOCK_SCOPED_FORCEINLINE
#define FABLE_RETAIL_CENGINESCREENEFFECTGLOWRENDERER
#define FABLE_RETAIL_CENGINESCREENEFFECTGLOWRENDERER_MEMBERS
#define FABLE_RETAIL_CENGINESCREENEFFECTGLOWRENDERER_APPLYGLOW
#include "engine/CEngineScreenEffectGlowRenderer.h"
#include "engine/CRenderManager.h"
#include "engine/CRenderStateCaptureBlock.h"
#include "engine/CShaderRenderManager.h"
#include "engine/CScopedShadersEnable.h"

namespace NEngineInternal {
extern CRenderManager* GPRenderManager;
extern CShaderRenderManager* GPShaderRenderManager;
}

void CEngineScreenEffectGlowRenderer::ApplyGlow()
{
    CRenderStateScopedCaptureBlock block;
    CScopedShadersEnable shaders_enable(true, true);
    NEngineInternal::GPShaderRenderManager->ApplyVertexShader(VertexShaderApplyGlow, false);
    NEngineInternal::GPShaderRenderManager->ApplyPixelShader(PixelShaderApplyGlow);
    NEngineInternal::GPShaderRenderManager->SetPixelShaderConstant(5, C4DVector(IntensityR * 8.0f, IntensityG * 8.0f, IntensityB * 8.0f, 1.0f));
    NEngineInternal::GPRenderManager->SetAlphaBlendEnable(true);
    NEngineInternal::GPRenderManager->SetAlphaTestEnable(false);
    NEngineInternal::GPRenderManager->SetSourceAlphaBlendMode(ALPHA_BLEND_ONE);
    NEngineInternal::GPRenderManager->SetDestAlphaBlendMode(ALPHA_BLEND_ONE);
    if (ThresholdR < 1.0f)
        NEngineInternal::GPRenderManager->SetBlendOp(BLEND_OP_REVSUBTRACT);
    NEngineInternal::GPRenderManager->SetZBufferEnable(false);
    NEngineInternal::GPRenderManager->SetPolyCullMode(POLY_CULL_NONE);
    NEngineInternal::GPRenderManager->SetTextureFilterModeMag(0, TEXTURE_FILTER_LINEAR);
    NEngineInternal::GPRenderManager->SetTextureFilterModeMin(0, TEXTURE_FILTER_LINEAR);
    NEngineInternal::GPRenderManager->SetTextureFilterModeMag(1, TEXTURE_FILTER_LINEAR);
    NEngineInternal::GPRenderManager->SetTextureFilterModeMin(1, TEXTURE_FILTER_LINEAR);
    NEngineInternal::GPShaderRenderManager->PrepareForRender(0);
    const CTexture* textures[2];
    textures[0] = GlowTexture64;
    textures[1] = GlowTexture16;
    static unsigned short ids[] = {0, 1, 3, 1, 2, 3};
    NEngineInternal::GPRenderManager->DrawAsIndexedTris(textures, 2, TextureToBackBufferVerts, 24, 4, ids, 2);
}