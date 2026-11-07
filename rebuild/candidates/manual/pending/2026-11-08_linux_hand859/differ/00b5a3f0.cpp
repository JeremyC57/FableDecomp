// CEngineScreenEffectColourFilter::ApplyColourFilter() -- grey / contrast / brightness / tint filter of the whole back buffer through the colour-filter pixel shader
// Grade: DIFFER(2923v2916) -- 15 lines: the third SetPixelShaderConstant's `UpdateFlags |= 2` is a load/or/store with the next value's load scheduled in where retail writes `or [eax+3D8h],ecx` (0x00B5A68C), the loop-alignment nop that follows from the 7-byte gap, and 8 relocation-equivalent table+8 operands. The CRenderStateManager bookmark test/or flip is fixed with the same clamp mix as 0x00B5AF80 (contrast clamp written out, GFLimit const-ref for the brightness and alpha clamps); chunk 859: 1 regrade (26 total): the RMW/count-position effect is documented (chunk 832)
// Next idea: the RMW is the same count/position effect as the flip but with its own threshold (chunk 832: grey built from a C3DVector gives retail's RMW while moving the flip one site late; the all-GFLimit / by-value / inline clamp mixes move the flip without the RMW); try a grey / inverse construction that costs one unit less without changing bytes (Initialise forms were neutral, the CRGBFloatColour ctor and operator- forms move the flip the wrong way)
// Leaf: ?ApplyColourFilter@CEngineScreenEffectColourFilter@@AAEXXZ
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

void CEngineScreenEffectColourFilter::ApplyColourFilter()
{
    static unsigned short QuadIndices[6] = { 0, 1, 3, 1, 2, 3 };                   // 0x0139A4E0
    CTexture& backBuffer = g_pDisplayManager_01436e1c->CopyBackBufferToTexture();
    CRenderStateScopedCaptureBlock capture;
    CScopedShadersEnable shaders(true, true);
    g_pDisplayManager_01436e1c->SetRenderTargetAsBackScreen();
    g_pEngineCamera->RefreshD3DTransforms();

    C4DVector grey(GrayRGBWeights.R, GrayRGBWeights.G, GrayRGBWeights.B, Settings.Saturation);
    float contrast = Settings.Contrast + 1.0f;
    if (contrast < 0.0f)
        contrast = 0.0f;
    else if (contrast > 2.0f)
        contrast = 2.0f;
    C4DVector scale = C4DVector(Settings.Tint * contrast * 0.5f);
    float brightness = Settings.Brightness - (1.0f - GrayLevel) * Settings.Contrast;
    GFLimit(brightness, -0.5f, 0.5f);
    brightness += 0.5f;
    C4DVector offset = C4DVector(Settings.Tint * brightness);
    C4DVector inverse(1.0f - grey.X, 1.0f - grey.Y, 1.0f - grey.Z, 1.0f - grey.W);
    NEngineInternal::GPShaderRenderManager->ApplyVertexShader(ScreenCopyVertexShader, false);
    NEngineInternal::GPShaderRenderManager->SetPixelShaderConstant(5, grey);
    NEngineInternal::GPShaderRenderManager->SetPixelShaderConstant(6, scale);
    NEngineInternal::GPShaderRenderManager->SetPixelShaderConstant(7, offset);
    NEngineInternal::GPShaderRenderManager->SetPixelShaderConstant(8, inverse);
    NEngineInternal::GPShaderRenderManager->ApplyPixelShader(ColourFilterPixelShader);

    float alpha = Settings.Alpha;
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
    NEngineInternal::GPRenderManager->AttachTextureToStage(0, &backBuffer);
    NEngineInternal::GPShaderRenderManager->PrepareForRender(0);
    NEngineInternal::GPRenderManager->DrawAsIndexedTris(TextureToBackBufferVerts, sizeof(CTVertexColTex1), 4, QuadIndices, 2);
    NEngineInternal::GPRenderManager->AttachTextureToStage(0, 0);
}
