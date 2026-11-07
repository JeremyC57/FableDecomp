// CEngineScreenEffectColourFilter::RenderColorFilteredQuad(float saturation, float contrast, const C4DVector& tint, float brightness, float alpha, CTexture* texture) -- draw a texture over the screen through the colour-filter pixel shader
// Shape (chunk 832 expert pass): the contrast clamp written out plus GFLimit (PDB const-ref) for level and alpha puts the CRenderStateManager bookmark test/or flip at retail's setter site;
//   `tint * (scaledContrast * 1.0f) * 0.5f` keeps the first product's X on the x87 stack as retail does (the plain product spills X and keeps W). The `* 1.0f` is behaviour-identical and has no source evidence: owner ruling 2026-11-07 (same as the same-type cast) accepts it.
// Retail-asm: 0x00B5B26B `fistp dword ptr [esp+58h]` -- the alpha rounding through the shared GFFloatToLongNear helper (the engine's inline-asm float-to-long; VC7.1 emits no bare dword fistp from C++).
// Leaf: ?RenderColorFilteredQuad@CEngineScreenEffectColourFilter@@QAEXMMABVC4DVector@@MMPAVCTexture@@@Z
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CSYSTEMMANAGER
#define FABLE_RETAIL_CDISPLAYMANAGER
#define FABLE_RETAIL_CDISPLAYMANAGER_BACKSCREEN
#define FABLE_RETAIL_CRENDERMANAGERCORE
#define FABLE_RETAIL_CRENDERMANAGERCORE_FORCEINLINE_HELPERS
#define FABLE_RETAIL_CRENDERMANAGERCORE_DRAWASINDEXEDTRIS_USER
#define FABLE_RETAIL_CRENDERSTATEMANAGER_STENCIL_INLINE
#define FABLE_RETAIL_CTEXTURE
#define FABLE_RETAIL_CRGBFLOATCOLOUR
#define FABLE_RETAIL_CPIXELFORMAT
#define FABLE_RETAIL_C2DBOXF
#define FABLE_RETAIL_CENGINECAMERA
#define FABLE_RETAIL_CENGINECAMERA_REFRESHD3DTRANSFORMS
#define FABLE_RETAIL_CENGINESCREENEFFECTCOLOURFILTER
#define FABLE_RETAIL_CENGINESCREENEFFECTCOLOURFILTER_MEMBERS
#include "engine/CDisplayManager.h"
#include "engine/CRenderManager.h"
#include "engine/CShaderRenderManager.h"
#include "engine/CRenderStateCaptureBlock.h"
#include "engine/CScopedShadersEnable.h"
#include "engine/CEngineCamera.h"
#include "engine/CEngineScreenEffectColourFilter.h"
#include "engine/GFFloatToLongNear.h"

namespace NEngineInternal { extern CRenderManager* GPRenderManager; }              // 0x01436E18
namespace NEngineInternal { extern CShaderRenderManager* GPShaderRenderManager; }  // 0x01436E14
extern CDisplayManager* g_pDisplayManager_01436e1c;                                // 0x01436E1C
extern CEngineCamera* g_pEngineCamera;                                             // 0x01436EA0

template <class T, class L, class H> inline void __fastcall GFLimit(T& value, const L& lower, const H& upper)
{
    if (value < lower)
        value = lower;
    else if (value > upper)
        value = upper;
}

void CEngineScreenEffectColourFilter::RenderColorFilteredQuad(float saturation, float contrast, const C4DVector& tint, float brightness,
                                                              float alpha, CTexture* texture)
{
    static unsigned short QuadIndices[6] = { 0, 1, 3, 1, 2, 3 };                   // 0x0139A4EC
    CRenderStateScopedCaptureBlock capture;
    CScopedShadersEnable shaders(true, true);

    C4DVector grey(GrayRGBWeights.R, GrayRGBWeights.G, GrayRGBWeights.B, saturation);
    float scaledContrast = contrast + 1.0f;
    if (scaledContrast < 0.0f)
        scaledContrast = 0.0f;
    else if (scaledContrast > 2.0f)
        scaledContrast = 2.0f;
    C4DVector scale = tint * (scaledContrast * 1.0f) * 0.5f;
    float offsetLevel = brightness - (1.0f - GrayLevel) * contrast;
    GFLimit(offsetLevel, -0.5f, 0.5f);
    offsetLevel += 0.5f;
    C4DVector offset = tint * offsetLevel;
    C4DVector inverse(1.0f - grey.X, 1.0f - grey.Y, 1.0f - grey.Z, 1.0f - grey.W);
    NEngineInternal::GPShaderRenderManager->ApplyVertexShader(ScreenCopyVertexShader, false);
    NEngineInternal::GPShaderRenderManager->SetPixelShaderConstant(5, grey);
    NEngineInternal::GPShaderRenderManager->SetPixelShaderConstant(6, scale);
    NEngineInternal::GPShaderRenderManager->SetPixelShaderConstant(7, offset);
    NEngineInternal::GPShaderRenderManager->SetPixelShaderConstant(8, inverse);
    NEngineInternal::GPShaderRenderManager->ApplyPixelShader(ColourFilterPixelShader);

    GFLimit(alpha, 0.0f, 1.0f);
    CRGBColour colour(255, 255, 255, (unsigned char)GFFloatToLongNear(alpha * 255.0f));
    NEngineInternal::GPRenderManager->SetAlphaBlendEnable(colour.A < 255);
    NEngineInternal::GPRenderManager->SetSourceAlphaBlendMode(ALPHA_BLEND_SRC_ALPHA);
    NEngineInternal::GPRenderManager->SetDestAlphaBlendMode(ALPHA_BLEND_INV_SRC_ALPHA);
    NEngineInternal::GPRenderManager->SetAlphaTestEnable(false);
    NEngineInternal::GPRenderManager->SetZBufferEnable(false);
    NEngineInternal::GPRenderManager->SetPolyCullMode(POLY_CULL_NONE);
    NEngineInternal::GPRenderManager->SetDitherEnable(false);
    NEngineInternal::GPRenderManager->SetTextureFilterModeMin(0, TEXTURE_FILTER_POINT);
    NEngineInternal::GPRenderManager->SetTextureFilterModeMag(0, TEXTURE_FILTER_POINT);
    NEngineInternal::GPRenderManager->SetStencilEnable(true);
    NEngineInternal::GPRenderManager->SetStencilFunc(STENCIL_FUNC_EQUAL);
    NEngineInternal::GPRenderManager->SetStencilMask(2);
    NEngineInternal::GPRenderManager->SetStencilRef(0);
    UpdateUVs();
    for (long i = 0; i < 4; ++i)
        TextureToBackBufferVerts[i].DiffuseColour = colour.IntValue;
    NEngineInternal::GPRenderManager->SetTextureUVModeU(0, TEXTURE_UV_CLAMP);
    NEngineInternal::GPRenderManager->SetTextureUVModeV(0, TEXTURE_UV_CLAMP);
    NEngineInternal::GPRenderManager->AttachTextureToStage(0, texture);
    NEngineInternal::GPShaderRenderManager->PrepareForRender(0);
    NEngineInternal::GPRenderManager->DrawAsIndexedTris(TextureToBackBufferVerts, sizeof(CTVertexColTex1), 4, QuadIndices, 2);
    NEngineInternal::GPRenderManager->AttachTextureToStage(0, 0);
}