// CEngineScreenEffectRadialBlurRenderer::CaptureBackBuffer() -- radial blur set-up: take the two 256 scratch textures, copy the thresholded back buffer into BlurTexture256_1 (screen-copy shaders, PS constants 5..7, VS constants 0..1), then three radial-blur ping-pong passes 1->2, 2->1, 1->2 (four taps each, steps 1 / 4 / 16: VS constants 1..4 = (cos, sin) * length and the centre offsets, PS constants 5..8 = IntensityR^k), each into a CRenderTarget on the destination's top surface, and restore the back screen
// Grade: DIFFER(5724v5748) -- 38 hunks: pass B tap-4 fsin position; pow->SetPixelShaderConstant temp-store scheduling in passes B/C/D (index 7 `mov/or/mov` form missing = the 24 bytes); SetAlphaBlendEnable/UVMode setter mask/captured register swaps @0xB5D0F3/0xB5D625; all are c2 81-tuple scheduling-region cuts (see NOTES.md)
// Leaf: ?CaptureBackBuffer@CEngineScreenEffectRadialBlurRenderer@@AAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B5CA70, 5748 bytes, plain `ret`, frame 0x88, no SEH.
// Callees: CBaseClass 0x0099A2D0/0x0099A2E0, CTexture::GetSurfaceLevel 0x009F9E00, CRenderTarget::AttachTarget 0x00A10E60, CSurface::Uninitialise
//   0x009F2E20, CDisplayManager::SetRenderTarget 0x009BF160 / CopyBackBufferToTexture 0x009BF5E0 / SetRenderTargetAsBackScreen 0x009BF220,
//   CGPUScratchBufferTextureManager::StartTextureUse 0x00F36EE0, CShaderRenderManager ApplyVertexShader 0x00988020 / ApplyPixelShader 0x00988140 /
//   SetUserVertexShaderConstant 0x00989B00 / SetVertexShaderConstantLayout 0x0098B5E0 / Enable-Disable shaders 0x00987FE0.. / PrepareForRender callees,
//   CRenderManagerCore::DrawAsIndexedTris 0x00A0AF40 / 0x00A0AEA0, CRenderStateManager::RestoreCaptureBlock 0x00A05840, _CIpow 0x00BFEB40.
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CSYSTEMMANAGER
#define FABLE_RETAIL_CDISPLAYMANAGER
#define FABLE_RETAIL_CDISPLAYMANAGER_BACKSCREEN
#define FABLE_RETAIL_CDISPLAYMANAGER_SETRENDERTARGET
#define FABLE_RETAIL_CRENDERMANAGERCORE
#define FABLE_RETAIL_CRENDERMANAGERCORE_FORCEINLINE_HELPERS
#define FABLE_RETAIL_CRENDERMANAGERCORE_DRAWASINDEXEDTRIS
#define FABLE_RETAIL_CRENDERMANAGERCORE_DRAWASINDEXEDTRIS_USER
#define FABLE_RETAIL_CTEXTURE
#define FABLE_RETAIL_CPIXELFORMAT
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_CSHADERRENDERMANAGER_SETPSCONSTANT_SUBSCRIPT
#define FABLE_RETAIL_CTEXTURE_SIZED
#define FABLE_RETAIL_CSURFACE
#define FABLE_RETAIL_CSURFACE_COPYFROMMEMORY
#define FABLE_RETAIL_CGPUSCRATCHBUFFERTEXTUREMANAGER_TEXTUREUSE
#define FABLE_RETAIL_CENGINERESOURCEMANAGER
#define FABLE_RETAIL_CENGINESCREENEFFECTRADIALBLURRENDERER
#define FABLE_RETAIL_CENGINESCREENEFFECTRADIALBLURRENDERER_MEMBERS
#define FABLE_RETAIL_CENGINESCREENEFFECTRADIALBLURRENDERER_CAPTUREBACKBUFFER
#include <math.h>
#include "engine/CPixelFormat.h"
#include "engine/CRenderManager.h"
#include "engine/CDisplayManager.h"
#include "engine/CBaseClass.h"
#include "engine/CShaderRenderManager.h"
#include "engine/CRenderStateCaptureBlock.h"
#include "engine/CScopedShadersEnable.h"
#include "engine/CScopedVertexShaderConstantLayout.h"
#include "engine/CGPUScratchBufferTextureManager.h"
#include "engine/CEngineResourceManager.h"
#include "engine/CEngineScreenEffectRadialBlurRenderer.h"

// No shared header for CRenderTarget (Ego_r ??0CRenderTarget@@QAE@AAVCSurface@@0@Z): modelled as the landed BeginDisplacementRendering.
class CRenderTarget : public CBaseClass {                 // vtable 0x0129C800, 0x24 bytes
public:
    CSurface RenderToSurface;                             // +0x04
    CSurface ZBufferSurface;                              // +0x14
    CRenderTarget() {}
    void AttachTarget(const CSurface& target);            // 0x00A10E60 ?AttachTarget@CRenderTarget@@QAEXABVCSurface@@@Z
};

namespace NEngineInternal { extern CRenderManager* GPRenderManager; }              // 0x01436E18
namespace NEngineInternal { extern CShaderRenderManager* GPShaderRenderManager; }  // 0x01436E14
namespace NEngineInternal { extern CDisplayManager* GPDisplayManager; }            // 0x01436E1C
namespace NEngineInternal { extern CEngineResourceManager* GPResourceManager; }    // 0x01436E98

void CEngineScreenEffectRadialBlurRenderer::CaptureBackBuffer()
{
    static unsigned short QuadIndices[6] = { 0, 1, 2, 0, 2, 3 };                   // 0x0139A538
    CRenderStateScopedCaptureBlock capture;
    CScopedVertexShaderConstantLayout layout(2);
    CGPUScratchBufferTextureManager& pool = NEngineInternal::GPResourceManager->GetGPUScratchBufferTextureManager();
    BlurTexture256_1 = pool.StartTextureUse(BlurTextureHandle256_1);
    BlurTexture256_2 = pool.StartTextureUse(BlurTextureHandle256_2);
    {
        CRenderTarget target;
        target.AttachTarget(BlurTexture256_1->GetSurfaceLevel(0));
        NEngineInternal::GPDisplayManager->SetRenderTarget(target);
        CScopedShadersEnable shaders(true, true);
        NEngineInternal::GPShaderRenderManager->ApplyVertexShader(ScreenCopyVertexShader, false);
        NEngineInternal::GPShaderRenderManager->ApplyPixelShader(ScreenCopyPixelShader);
        NEngineInternal::GPShaderRenderManager->SetPixelShaderConstant(5, C4DVector(CurrentParameters.ThresholdR, CurrentParameters.ThresholdG, CurrentParameters.ThresholdB, 0.3f));
        NEngineInternal::GPShaderRenderManager->SetPixelShaderConstant(6, C4DVector(1.0f, 1.0f, 1.0f, 1.0f));
        NEngineInternal::GPShaderRenderManager->SetPixelShaderConstant(7, C4DVector(0.0f, 0.0f, 0.0f, 0.0f));
        float invRange = 1.0f / CurrentParameters.Range;
        NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(0, AreaCenter.X * 2.0f - 1.0f, 1.0f - AreaCenter.Y * 2.0f, invRange, invRange);
        NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(1, 0.5f, 0.0f, 0.0f, 0.0f);
        NEngineInternal::GPRenderManager->SetZBufferEnable(false);
        NEngineInternal::GPRenderManager->SetPolyCullMode(POLY_CULL_NONE);
        NEngineInternal::GPRenderManager->SetAlphaBlendEnable(false);
        CTexture& backBuffer = NEngineInternal::GPDisplayManager->CopyBackBufferToTexture();
        NEngineInternal::GPRenderManager->SetTextureUVModeU(0, TEXTURE_UV_CLAMP);
        NEngineInternal::GPRenderManager->SetTextureUVModeV(0, TEXTURE_UV_CLAMP);
        NEngineInternal::GPRenderManager->AttachTextureToStage(0, &backBuffer);
        NEngineInternal::GPShaderRenderManager->PrepareForRender(0);
        NEngineInternal::GPRenderManager->DrawAsIndexedTris(BackBufferToTextureVerts, sizeof(CTVertexColTex1), 4, QuadIndices, 2);
        NEngineInternal::GPRenderManager->AttachTextureToStage(0, 0);
        NEngineInternal::GPDisplayManager->SetRenderTargetAsBackScreen();
    }
    float twist = CurrentParameters.Twist;
    NEngineInternal::GPRenderManager->SetAlphaBlendEnable(false);
    {
        CRenderTarget target;
        target.AttachTarget(BlurTexture256_2->GetSurfaceLevel(0));
        NEngineInternal::GPDisplayManager->SetRenderTarget(target);
        NEngineInternal::GPShaderRenderManager->ApplyVertexShader(RadialBlurVertexShader, false);
        NEngineInternal::GPShaderRenderManager->ApplyPixelShader(RadialBlurPixelShader);
        NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(0, 0.5f, 0.5f, 0.0f, 0.0f);
        float length1 = 1.0f - CurrentParameters.Length * 0.00390625f;
        float length2 = 1.0f - CurrentParameters.Length * 0.0078125f;
        float length3 = 1.0f - CurrentParameters.Length * 0.01171875f;
        float offsetX1 = (BlurCenter.X - 0.5f) * CurrentParameters.Length * 0.00390625;
        float offsetX2 = (BlurCenter.X - 0.5f) * CurrentParameters.Length * 0.0078125;
        float offsetX3 = (BlurCenter.X - 0.5f) * CurrentParameters.Length * 0.01171875;
        float offsetY1 = (BlurCenter.Y - 0.5f) * CurrentParameters.Length * 0.00390625;
        float offsetY2 = (BlurCenter.Y - 0.5f) * CurrentParameters.Length * 0.0078125;
        float offsetY3 = (BlurCenter.Y - 0.5f) * CurrentParameters.Length * 0.01171875;
        float angle0 = twist * 0.0f;
        float angle1 = twist * 1.0f;
        float angle2 = twist * 2.0f;
        float angle3 = twist * 3.0f;
        NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(0, 0.5f, 0.5f, 0.5f, 0.5f);
        NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(1, (float)cos((double)angle0), (float)sin((double)angle0), 0.0f, 0.0f);
        NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(2, (float)cos((double)angle1) * length1, (float)sin((double)angle1) * length1, offsetX1, offsetY1);
        NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(3, (float)cos((double)angle2) * length2, (float)sin((double)angle2) * length2, offsetX2, offsetY2);
        NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(4, (float)cos((double)angle3) * length3, (float)sin((double)angle3) * length3, offsetX3, offsetY3);
        float intensity0 = (float)pow((double)CurrentParameters.IntensityR, 0.0);
        float intensity1 = (float)pow((double)CurrentParameters.IntensityR, 1.0);
        float intensity2 = (float)pow((double)CurrentParameters.IntensityR, 2.0);
        float intensity3 = (float)pow((double)CurrentParameters.IntensityR, 3.0);
        NEngineInternal::GPShaderRenderManager->SetPixelShaderConstant(5, C4DVector(intensity0, intensity0, intensity0, intensity0));
        NEngineInternal::GPShaderRenderManager->SetPixelShaderConstant(6, C4DVector(intensity1, intensity1, intensity1, intensity1));
        NEngineInternal::GPShaderRenderManager->SetPixelShaderConstant(7, C4DVector(intensity2, intensity2, intensity2, intensity2));
        NEngineInternal::GPShaderRenderManager->SetPixelShaderConstant(8, C4DVector(intensity3, intensity3, intensity3, intensity3));
        const CTexture* textures[4] = { BlurTexture256_1, BlurTexture256_1, BlurTexture256_1, BlurTexture256_1 };
        NEngineInternal::GPShaderRenderManager->PrepareForRender(0);
        NEngineInternal::GPRenderManager->DrawAsIndexedTris(textures, 4, Blur256ToBlur256Verts, sizeof(CTVertexColTex1), 4, QuadIndices, 2);
        NEngineInternal::GPDisplayManager->SetRenderTargetAsBackScreen();
    }
    NEngineInternal::GPRenderManager->SetAlphaBlendEnable(false);
    {
        CRenderTarget target;
        target.AttachTarget(BlurTexture256_1->GetSurfaceLevel(0));
        NEngineInternal::GPDisplayManager->SetRenderTarget(target);
        NEngineInternal::GPShaderRenderManager->ApplyVertexShader(RadialBlurVertexShader, false);
        NEngineInternal::GPShaderRenderManager->ApplyPixelShader(RadialBlurPixelShader);
        NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(0, 0.498046875f, 0.498046875f, 0.0f, 0.0f);
        NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(0, 0.5f, 0.5f, 0.0f, 0.0f);
        float length1 = 1.0f - CurrentParameters.Length * 0.015625f;
        float length2 = 1.0f - CurrentParameters.Length * 0.03125f;
        float length3 = 1.0f - CurrentParameters.Length * 0.046875f;
        float offsetX1 = (BlurCenter.X - 0.5f) * CurrentParameters.Length * 0.015625;
        float offsetX2 = (BlurCenter.X - 0.5f) * CurrentParameters.Length * 0.03125;
        float offsetX3 = (BlurCenter.X - 0.5f) * CurrentParameters.Length * 0.046875;
        float offsetY1 = (BlurCenter.Y - 0.5f) * CurrentParameters.Length * 0.015625;
        float offsetY2 = (BlurCenter.Y - 0.5f) * CurrentParameters.Length * 0.03125;
        float offsetY3 = (BlurCenter.Y - 0.5f) * CurrentParameters.Length * 0.046875;
        float angle0 = twist * 0.0f;
        float angle1 = twist * 4.0f;
        float angle2 = twist * 8.0f;
        float angle3 = twist * 12.0f;
        NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(0, 0.5f, 0.5f, 0.5f, 0.5f);
        NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(1, (float)cos((double)angle0), (float)sin((double)angle0), 0.0f, 0.0f);
        NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(2, (float)cos((double)angle1) * length1, (float)sin((double)angle1) * length1, offsetX1, offsetY1);
        NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(3, (float)cos((double)angle2) * length2, (float)sin((double)angle2) * length2, offsetX2, offsetY2);
        NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(4, (float)cos((double)angle3) * length3, (float)sin((double)angle3) * length3, offsetX3, offsetY3);
        float intensity0 = (float)pow((double)CurrentParameters.IntensityR, 0.0);
        float intensity1 = (float)pow((double)CurrentParameters.IntensityR, 4.0);
        float intensity2 = (float)pow((double)CurrentParameters.IntensityR, 8.0);
        float intensity3 = (float)pow((double)CurrentParameters.IntensityR, 12.0);
        NEngineInternal::GPShaderRenderManager->SetPixelShaderConstant(5, C4DVector(intensity0, intensity0, intensity0, intensity0));
        NEngineInternal::GPShaderRenderManager->SetPixelShaderConstant(6, C4DVector(intensity1, intensity1, intensity1, intensity1));
        NEngineInternal::GPShaderRenderManager->SetPixelShaderConstant(7, C4DVector(intensity2, intensity2, intensity2, intensity2));
        NEngineInternal::GPShaderRenderManager->SetPixelShaderConstant(8, C4DVector(intensity3, intensity3, intensity3, intensity3));
        const CTexture* textures[4] = { BlurTexture256_2, BlurTexture256_2, BlurTexture256_2, BlurTexture256_2 };
        NEngineInternal::GPShaderRenderManager->PrepareForRender(0);
        NEngineInternal::GPRenderManager->DrawAsIndexedTris(textures, 4, Blur256ToBlur256Verts, sizeof(CTVertexColTex1), 4, QuadIndices, 2);
        NEngineInternal::GPDisplayManager->SetRenderTargetAsBackScreen();
    }
    NEngineInternal::GPRenderManager->SetAlphaBlendEnable(false);
    {
        CRenderTarget target;
        target.AttachTarget(BlurTexture256_2->GetSurfaceLevel(0));
        NEngineInternal::GPDisplayManager->SetRenderTarget(target);
        NEngineInternal::GPShaderRenderManager->ApplyVertexShader(RadialBlurVertexShader, false);
        NEngineInternal::GPShaderRenderManager->ApplyPixelShader(RadialBlurPixelShader);
        NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(0, 0.498046875f, 0.498046875f, 0.0f, 0.0f);
        NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(0, 0.5f, 0.5f, 0.0f, 0.0f);
        float length1 = 1.0f - CurrentParameters.Length * 0.0625f;
        float length2 = 1.0f - CurrentParameters.Length * 0.125f;
        float length3 = 1.0f - CurrentParameters.Length * 0.1875f;
        float offsetX1 = (BlurCenter.X - 0.5f) * CurrentParameters.Length * 0.0625;
        float offsetX2 = (BlurCenter.X - 0.5f) * CurrentParameters.Length * 0.125;
        float offsetX3 = (BlurCenter.X - 0.5f) * CurrentParameters.Length * 0.1875;
        float offsetY1 = (BlurCenter.Y - 0.5f) * CurrentParameters.Length * 0.0625;
        float offsetY2 = (BlurCenter.Y - 0.5f) * CurrentParameters.Length * 0.125;
        float offsetY3 = (BlurCenter.Y - 0.5f) * CurrentParameters.Length * 0.1875;
        float angle0 = twist * 0.0f;
        float angle1 = twist * 16.0f;
        float angle2 = twist * 32.0f;
        float angle3 = twist * 48.0f;
        NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(0, 0.5f, 0.5f, 0.5f, 0.5f);
        NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(1, (float)cos((double)angle0), (float)sin((double)angle0), 0.0f, 0.0f);
        NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(2, (float)cos((double)angle1) * length1, (float)sin((double)angle1) * length1, offsetX1, offsetY1);
        NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(3, (float)cos((double)angle2) * length2, (float)sin((double)angle2) * length2, offsetX2, offsetY2);
        NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(4, (float)cos((double)angle3) * length3, (float)sin((double)angle3) * length3, offsetX3, offsetY3);
        float intensity0 = (float)pow((double)CurrentParameters.IntensityR, 0.0);
        float intensity1 = (float)pow((double)CurrentParameters.IntensityR, 16.0);
        float intensity2 = (float)pow((double)CurrentParameters.IntensityR, 32.0);
        float intensity3 = (float)pow((double)CurrentParameters.IntensityR, 48.0);
        NEngineInternal::GPShaderRenderManager->SetPixelShaderConstant(5, C4DVector(intensity0, intensity0, intensity0, intensity0));
        NEngineInternal::GPShaderRenderManager->SetPixelShaderConstant(6, C4DVector(intensity1, intensity1, intensity1, intensity1));
        NEngineInternal::GPShaderRenderManager->SetPixelShaderConstant(7, C4DVector(intensity2, intensity2, intensity2, intensity2));
        NEngineInternal::GPShaderRenderManager->SetPixelShaderConstant(8, C4DVector(intensity3, intensity3, intensity3, intensity3));
        const CTexture* textures[4] = { BlurTexture256_1, BlurTexture256_1, BlurTexture256_1, BlurTexture256_1 };
        NEngineInternal::GPShaderRenderManager->PrepareForRender(0);
        NEngineInternal::GPRenderManager->DrawAsIndexedTris(textures, 4, Blur256ToBlur256Verts, sizeof(CTVertexColTex1), 4, QuadIndices, 2);
        NEngineInternal::GPDisplayManager->SetRenderTargetAsBackScreen();
    }
    NEngineInternal::GPDisplayManager->SetRenderTargetAsBackScreen();
}
