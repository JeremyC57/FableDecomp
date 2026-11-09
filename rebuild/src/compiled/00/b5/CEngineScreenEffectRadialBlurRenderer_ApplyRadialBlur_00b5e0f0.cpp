// CEngineScreenEffectRadialBlurRenderer::ApplyRadialBlur() -- fade the four texture-to-back-buffer quad vertices to the current parameters' Fade (grey level, opaque alpha), then draw the second blur texture over the back buffer with the texture-diffuse pixel shader inside a render-state capture block, vertex constant layout 2 and the shaders forced on, all restored afterwards
// Leaf: ?ApplyRadialBlur@CEngineScreenEffectRadialBlurRenderer@@AAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B5E0F0, length 1552 bytes (body 0x60b + int3 padding), plain `ret`.
// Twin of the landed ShowBlurTexture 0x00B5E700 (same scoped capture block / layout / shader enables, inlined PrepareForRender(0)):
//   the fade is __ftol2 @ 0x00BFEA70 of Fade * 255.0f (0x01230014) clamped to 0..255 and spread to 0xFFgggggg; shaders
//   ScreenCopyVertexShader +0x20 / TextureDiffusePixelShader +0x48; the inline CRenderStateManager setters on GPRenderManager
//   (AlphaBlendEnable +0x28B8, AlphaTestEnable +0x28CC, Source/DestAlphaBlendMode +0x291C/+0x2930 <- D3DAlphaBlendModeTable[ONE]
//   0x01396F6C, ZBufferEnable +0x2868, PolyCullMode +0x2890 <- D3DPolyCullModeTable[NONE] 0x01396FB8, DitherEnable +0x2A0C,
//   TextureFilterModeMag/Min[0] +0x3164/+0x30C4 <- D3DTextureFilterModeTable[LINEAR] 0x01396E5C); DrawAsIndexedTris 0x00A0AEA0 of
//   BlurTexture256_2 with TextureToBackBufferVerts and the function's quad indices @ 0x0139A544.
#define FABLE_RETAIL_CRENDERMANAGERCORE
#define FABLE_RETAIL_CRENDERMANAGERCORE_DRAWASINDEXEDTRIS
#define FABLE_RETAIL_CTEXTURE
#define FABLE_RETAIL_CENGINESCREENEFFECTRADIALBLURRENDERER
#define FABLE_RETAIL_CENGINESCREENEFFECTRADIALBLURRENDERER_MEMBERS
#include "engine/CRenderManager.h"
#include "engine/CShaderRenderManager.h"
#include "engine/CRenderStateCaptureBlock.h"
#include "engine/CScopedShadersEnable.h"
#include "engine/CScopedVertexShaderConstantLayout.h"
#include "engine/CEngineScreenEffectRadialBlurRenderer.h"

namespace NEngineInternal { extern CRenderManager* GPRenderManager; }              // 0x01436E18
namespace NEngineInternal { extern CShaderRenderManager* GPShaderRenderManager; }  // 0x01436E14

void CEngineScreenEffectRadialBlurRenderer::ApplyRadialBlur()
{
    static unsigned short QuadIndices[6] = { 0, 1, 3, 1, 2, 3 };                   // 0x0139A544
    long fade = (long)(CurrentParameters.Fade * 255.0f);
    if (fade < 0)
        fade = 0;
    else if (fade > 255)
        fade = 255;
    for (long i = 0; i < 4; ++i)
        TextureToBackBufferVerts[i].DiffuseColour = fade * 0x10101 + 0xff000000;
    CRenderStateScopedCaptureBlock capture;
    CScopedVertexShaderConstantLayout layout(2);
    CScopedShadersEnable shaders(true, true);
    NEngineInternal::GPShaderRenderManager->ApplyVertexShader(ScreenCopyVertexShader, false);
    NEngineInternal::GPShaderRenderManager->ApplyPixelShader(TextureDiffusePixelShader);
    NEngineInternal::GPRenderManager->SetAlphaBlendEnable(true);
    NEngineInternal::GPRenderManager->SetAlphaTestEnable(false);
    NEngineInternal::GPRenderManager->SetSourceAlphaBlendMode(ALPHA_BLEND_ONE);
    NEngineInternal::GPRenderManager->SetDestAlphaBlendMode(ALPHA_BLEND_ONE);
    NEngineInternal::GPRenderManager->SetZBufferEnable(false);
    NEngineInternal::GPRenderManager->SetPolyCullMode(POLY_CULL_NONE);
    NEngineInternal::GPRenderManager->SetDitherEnable(false);
    NEngineInternal::GPRenderManager->SetTextureFilterModeMag(0, TEXTURE_FILTER_LINEAR);
    NEngineInternal::GPRenderManager->SetTextureFilterModeMin(0, TEXTURE_FILTER_LINEAR);
    NEngineInternal::GPShaderRenderManager->PrepareForRender(0);
    const CTexture* texture = BlurTexture256_2;
    NEngineInternal::GPRenderManager->DrawAsIndexedTris(&texture, 1, TextureToBackBufferVerts, sizeof(CTVertexColTex1), 4, QuadIndices, 2);
}