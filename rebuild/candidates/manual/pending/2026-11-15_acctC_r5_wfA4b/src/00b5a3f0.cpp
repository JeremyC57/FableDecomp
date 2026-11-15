// CEngineScreenEffectColourFilter::ApplyColourFilter() -- grey / contrast / brightness / tint filter of the whole back buffer through the colour-filter pixel shader
// Retail: 0x00B5A3F0, 2,916 bytes (last instruction `jmp 0xa05840`). RELOCATION_MATCH under VC7.1 RTM.
// Spellings: EVIDENCED by retail bytes -- the brightness offset folded into the Tint product (`Tint * (brightness + 0.5f)`, fadd at 0x00B5A56A feeds the four fmuls); C4DVector(const CRGBFloatColour&) through InitialiseFromColour (FableWin 12i = one call); clamps through the PDB GFLimit. No device, no flags.
// Leaf: ?ApplyColourFilter@CEngineScreenEffectColourFilter@@AAEXXZ
// Retail-asm: 0x00B5A70F `fld dword ptr [esp+10h]` + 0x00B5A713 `fistp dword ptr [esp+18h]` -- the alpha byte through the engine's inline-assembly GFFloatToLongNear (engine/GFFloatToLongNear.h, the scaled value homed in the helper's by-value parameter and reloaded for a bare dword fistp); the row itself holds no asm.
#define FABLE_RETAIL_CSHADERRENDERMANAGER_PSCONST_INDEX
#define FABLE_RETAIL_C4DVECTOR_INITIALISEFROMCOLOUR
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
#include "engine/GFLimit.h"

namespace NEngineInternal { extern CRenderManager* GPRenderManager; }              // 0x01436E18
namespace NEngineInternal { extern CShaderRenderManager* GPShaderRenderManager; }  // 0x01436E14
namespace NEngineInternal { extern CDisplayManager* GPDisplayManager; }          // 0x01436E1C ?GPDisplayManager@NEngineInternal@@3PAVCDisplayManager@@A
namespace NEngineInternal { extern CEngineCamera* GPCamera; }                    // 0x01436EA0

void CEngineScreenEffectColourFilter::ApplyColourFilter()
{
    static unsigned short QuadIndices[6] = { 0, 1, 3, 1, 2, 3 };                   // 0x0139A4E0
    CTexture& backBuffer = NEngineInternal::GPDisplayManager->CopyBackBufferToTexture();
    CRenderStateScopedCaptureBlock capture;
    CScopedShadersEnable shaders(true, true);
    NEngineInternal::GPDisplayManager->SetRenderTargetAsBackScreen();
    NEngineInternal::GPCamera->RefreshD3DTransforms();

    C4DVector grey(GrayRGBWeights.R, GrayRGBWeights.G, GrayRGBWeights.B, Settings.Saturation);
    float contrast = Settings.Contrast + 1.0f;
    if (contrast < 0.0f)
        contrast = 0.0f;
    else if (contrast > 2.0f)
        contrast = 2.0f;
    C4DVector scale = C4DVector(Settings.Tint * contrast * 0.5f);
    float brightness = Settings.Brightness - (1.0f - GrayLevel) * Settings.Contrast;
    GFLimit(brightness, -0.5f, 0.5f);
    C4DVector offset = C4DVector(Settings.Tint * (brightness + 0.5f));
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
