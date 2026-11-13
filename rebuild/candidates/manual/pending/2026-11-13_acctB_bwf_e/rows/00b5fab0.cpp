// CEngineScreenEffectGlowRenderer::CaptureBackBuffer() -- capture the back buffer into the glow textures: inside a render-state capture block, take the four scratch glow textures; pass 1 thresholds the back buffer into GlowTexture256 (glow / negative-glow pixel shader picked by ThresholdR < 1, optional motion-blur alpha blend); pass 2 blurs it down into GlowTexture64 (1/128 taps); NoBlurPasses times ping-pong GlowTexture64 <-> GlowTexture64Alt with growing tap offsets (x BlurOffsetScale); last pass blurs GlowTexture64 into GlowTexture16 with the intensity colour (1/32 taps); back screen restored and the camera's D3D transforms refreshed.
// Grade: RELOCATION_MATCH (5302 bytes).
// Leaf: ?CaptureBackBuffer@CEngineScreenEffectGlowRenderer@@AAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B5FAB0, 5302 bytes, __thiscall, plain `ret`, frame 0xC0, no EH (no /EHsc), no flags.
// Identity: Ego_r.pdb / FableWin.pdb ?CaptureBackBuffer@CEngineScreenEffectGlowRenderer@@AAEXXZ (Ego_r order and size match the retail
//   neighbours); sole caller Render 0x00B619B0.
// Callees: StartTextureUse 0x00F36EE0 x4, CBaseClass() 0x0099A2D0 / ~CBaseClass 0x0099A2E0 (inline CRenderTarget), GetSurfaceLevel
//   0x009F9E00, CRenderTarget::AttachTarget 0x00A10E60, CSurface::Uninitialise 0x009F2E20, CDisplayManager::SetRenderTarget 0x009BF160 /
//   SetRenderTargetAsBackScreen 0x009BF220 / CopyBackBufferToTexture 0x009BF5E0, CShaderRenderManager ApplyVertexShader 0x00988020 /
//   ApplyPixelShader 0x00988140 / SetUserVertexShaderConstant 0x00989B00 (+ inline PrepareForRender callees), DrawAsIndexedTris
//   0x00A0AF40 / 0x00A0AEA0, CEngineCamera::RefreshD3DTransforms 0x00B2FC50, RestoreCaptureBlock 0x00A05840.
// Shape: the threshold constant is a C4DVector temporary per if/else arm (a named `threshold` filled member by member moves every
//   frame slot by 4 and flips the x87 scale*offset order); the two loop blocks name their destination / source textures and the 0.33
//   weights (symbol records that fix the mid-loop setter's load order). CRenderTarget has no shared header: local model (Ego_r: CBaseClass
//   base, RenderToSurface +4, ZBufferSurface +0x14, implicit destructor -- an empty user one stores the vtable, retail does not).
//   Function statics BlurOffsetScale / NoBlurPasses / QuadIndices are descriptive names (no PDB record).
//   BlurOffsetScale / NoBlurPasses are `static volatile` tweakables (see the Retail-volatile line; coordinator decision pending: no
//   PDB LF_MODIFIER or vendor source backs it, the evidence is the loop-end schedule only).
// Retail-volatile: 0x00B60C79 fld [0x0139A6B0] / 0x00B60C83..0x00B60C88 mov eax,[i]; inc eax; mov [i],eax / 0x00B60C90 fld [0x0139A6B0]
//   then 0x00B60C96 mov ecx,[0x0139A6B4]; cmp eax,ecx -- retail loads NoBlurPasses AFTER the second BlurOffsetScale load and after the
//   i store. Without volatile on both statics VC7.1 hoists `mov ecx,[0x0139A6B4]` up to right after `mov eax,[i]` and stores i after
//   the cmp (r/nv1.cpp: DIFFER, 3 hunks, all at 0x00B60C87..0x00B60C9C; volatile on only one of the two: same 3 hunks). The c2 DAG
//   (prior v2/sch_base.json) has no edge fld scale -> load N without volatile (load-N prio 38912 > inc 7168); the volatile
//   load->load order edge is the only text found that gives retail's order (no read is dropped either way).
//   FITTED origin locals: the two loop blocks name the -1/128 origin tap (`const float origin`, no code of its own) only to add one
//   symbol record per block: without them the mid-loop inline SetAlphaBlendEnable(false) in front of loop block B loads in the wrong
//   order (retail 0x00B6094A fld [0x0139A6B0]; fmul [esp+28]; 0x00B60954 mov ecx,[GPRenderManager]; mov edx,[ecx+0x28BC]; cmp edx,ebx;
//   fstp; lea eax,[ecx+0x28B8]; 0x00B6096C fld ...; 0x00B6097C mov edi,[ecx+0x2814]; 0x00B60982 mov edx,[eax+8]; test edi,edx).
//   FITTED record count (the lane's bookmark test/or register tie): the expanded SetRenderState / AddStateToUpdateList carry three named
//   locals that leave no code (hdr opt-ins SETRENDERSTATE_DESIRED `desired_state`, SETRENDERSTATE_BOOKMARK `bookmark_mask`,
//   ADDSTATE_DIRTY `dirty`; 12 expansions = 36 symbols, a dead-local ruler put the needed count at 36..44, matched here with 0..3
//   diagnostic units to spare). Retail bytes that need them: the loop-top inline SetAlphaBlendEnable(false) at 0x00B60665
//   mov edi,[eax+0x2814] (CurrentBookmarkMask -> edi) / 0x00B6066B mov edx,[ecx+8] (state BookmarkMask -> edx) / test edi,edx; without
//   bookmark_mask / dirty the two registers come out swapped (edx / edi).
#define FABLE_RETAIL_CRENDERSTATEMANAGER_SETRENDERSTATE_DESIRED
#define FABLE_RETAIL_CRENDERSTATEMANAGER_SETRENDERSTATE_BOOKMARK
#define FABLE_RETAIL_CRENDERSTATEMANAGER_ADDSTATE_DIRTY
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CSYSTEMMANAGER
#define FABLE_RETAIL_CRGBFLOATCOLOUR
#define FABLE_RETAIL_CPIXELFORMAT
#define FABLE_RETAIL_C2DBOXF
#define FABLE_RETAIL_CSURFACE
#define FABLE_RETAIL_CSURFACE_COPYFROMMEMORY
#define FABLE_RETAIL_CSURFACE_FORCEINLINE_DTOR
#define FABLE_RETAIL_CTEXTURE
#define FABLE_RETAIL_CTEXTURE_SIZED
#define FABLE_RETAIL_CDISPLAYMANAGER
#define FABLE_RETAIL_CDISPLAYMANAGER_BACKSCREEN
#define FABLE_RETAIL_CDISPLAYMANAGER_SETRENDERTARGET
#define FABLE_RETAIL_CRENDERMANAGERCORE
#define FABLE_RETAIL_CRENDERMANAGERCORE_FORCEINLINE_HELPERS
#define FABLE_RETAIL_CRENDERMANAGERCORE_DRAWASINDEXEDTRIS
#define FABLE_RETAIL_CRENDERMANAGERCORE_DRAWASINDEXEDTRIS_USER
#define FABLE_RETAIL_CENGINECAMERA
#define FABLE_RETAIL_CENGINECAMERA_REFRESHD3DTRANSFORMS
#define FABLE_RETAIL_CENGINERESOURCEMANAGER
#define FABLE_RETAIL_CGPUSCRATCHBUFFERTEXTUREMANAGER_STARTTEXTUREUSE
#define FABLE_RETAIL_CPIXELSHADERDATA
#define FABLE_RETAIL_CPIXELSHADER_COPYCTOR
#define FABLE_RETAIL_CENGINESCREENEFFECTGLOWRENDERER
#define FABLE_RETAIL_CENGINESCREENEFFECTGLOWRENDERER_MEMBERS
#define FABLE_RETAIL_CENGINESCREENEFFECTGLOWRENDERER_CAPTUREBACKBUFFER
#include "engine/CDisplayManager.h"
#include "engine/CSurface.h"
#include "engine/CTexture.h"
#include "engine/CBaseClass.h"
#include "engine/CDisplayManager.h"
#include "engine/CRenderManager.h"
#include "engine/CShaderRenderManager.h"
#include "engine/CRenderStateCaptureBlock.h"
#include "engine/CScopedShadersEnable.h"
#include "engine/CEngineCamera.h"
#include "engine/CEngineResourceManager.h"
#include "engine/CGPUScratchBufferTextureManager.h"
#include "engine/CEngineScreenEffectGlowRenderer.h"

class CRenderTarget : public CBaseClass {
public:
    CRenderTarget() { }
    void AttachTarget(const CSurface& surface);         // 0x00A10E60
    CSurface RenderToSurface;                           // +0x04
    CSurface ZBufferSurface;                            // +0x14
};

namespace NEngineInternal { extern CRenderManager* GPRenderManager; }              // 0x01436E18
namespace NEngineInternal { extern CShaderRenderManager* GPShaderRenderManager; }  // 0x01436E14
namespace NEngineInternal { extern CDisplayManager* GPDisplayManager; }            // 0x01436E1C
extern CEngineResourceManager* g_pResourceManager;                                 // 0x01436E98
extern CEngineCamera* g_pEngineCamera;                                             // 0x01436EA0

void CEngineScreenEffectGlowRenderer::CaptureBackBuffer()
{
    static volatile float BlurOffsetScale = 1.5f;                                           // 0x0139A6B0
    static volatile long NoBlurPasses = 2;                                                  // 0x0139A6B4
    static unsigned short QuadIndices[6] = { 0, 1, 2, 0, 2, 3 };                   // 0x0139A6B8
    CRenderStateScopedCaptureBlock capture;
    CGPUScratchBufferTextureManager& scratch = g_pResourceManager->GetGPUScratchBufferTextureManager();
    GlowTexture256 = scratch.StartTextureUse(GlowTexture256Handle);
    GlowTexture64 = scratch.StartTextureUse(GlowTexture64Handle);
    GlowTexture64Alt = scratch.StartTextureUse(GlowTexture64AltHandle);
    GlowTexture16 = scratch.StartTextureUse(GlowTexture16Handle);
    {
        CRenderTarget target;
        target.AttachTarget(GlowTexture256->GetSurfaceLevel(0));
        NEngineInternal::GPDisplayManager->SetRenderTarget(target);
        CScopedShadersEnable shaders(true, true);
        NEngineInternal::GPShaderRenderManager->ApplyVertexShader(VertexShaderCopy, false);
        CPixelShader glowShader(ThresholdR < 1.0f ? PixelShaderGlowNeg : PixelShaderGlow);
        NEngineInternal::GPShaderRenderManager->ApplyPixelShader(glowShader);
        if (ThresholdR < 1.0f)
            NEngineInternal::GPShaderRenderManager->SetPixelShaderConstant(5, C4DVector(ThresholdR, ThresholdG, ThresholdB, 0.3f));
        else
            NEngineInternal::GPShaderRenderManager->SetPixelShaderConstant(5, C4DVector(ThresholdR - 1.0f, ThresholdG - 1.0f, ThresholdB - 1.0f, 0.3f));
        NEngineInternal::GPShaderRenderManager->SetPixelShaderConstant(8, C4DVector(MotionBlur, MotionBlur, MotionBlur, MotionBlur));
        NEngineInternal::GPRenderManager->SetZBufferEnable(false);
        NEngineInternal::GPRenderManager->SetPolyCullMode(POLY_CULL_NONE);
        if (MotionBlur > 0.0f && MotionBlur < 1.0f) {
            NEngineInternal::GPRenderManager->SetAlphaBlendEnable(true);
            NEngineInternal::GPRenderManager->SetAlphaTestEnable(false);
            NEngineInternal::GPRenderManager->SetSourceAlphaBlendMode(ALPHA_BLEND_SRC_ALPHA);
            NEngineInternal::GPRenderManager->SetDestAlphaBlendMode(ALPHA_BLEND_INV_SRC_ALPHA);
        } else
            NEngineInternal::GPRenderManager->SetAlphaBlendEnable(false);
        CTexture& backBuffer = NEngineInternal::GPDisplayManager->CopyBackBufferToTexture();
        BackBufferToTextureVerts[0].U1 = 0.0f;
        BackBufferToTextureVerts[0].V1 = 1.0f;
        BackBufferToTextureVerts[1].U1 = 1.0f;
        BackBufferToTextureVerts[1].V1 = 1.0f;
        BackBufferToTextureVerts[2].U1 = 1.0f;
        BackBufferToTextureVerts[2].V1 = 0.0f;
        BackBufferToTextureVerts[3].U1 = 0.0f;
        BackBufferToTextureVerts[3].V1 = 0.0f;
        NEngineInternal::GPRenderManager->SetTextureUVModeU(0, TEXTURE_UV_CLAMP);
        NEngineInternal::GPRenderManager->SetTextureUVModeV(0, TEXTURE_UV_CLAMP);
        NEngineInternal::GPRenderManager->AttachTextureToStage(0, &backBuffer);
        NEngineInternal::GPShaderRenderManager->PrepareForRender(0);
        NEngineInternal::GPRenderManager->DrawAsIndexedTris(BackBufferToTextureVerts, sizeof(CTVertexColTex1), 4, QuadIndices, 2);
        NEngineInternal::GPRenderManager->AttachTextureToStage(0, 0);
        NEngineInternal::GPDisplayManager->SetRenderTargetAsBackScreen();
    }
    NEngineInternal::GPRenderManager->SetAlphaBlendEnable(false);
    {
        CRenderTarget target;
        target.AttachTarget(GlowTexture64->GetSurfaceLevel(0));
        NEngineInternal::GPDisplayManager->SetRenderTarget(target);
        NEngineInternal::GPShaderRenderManager->ApplyVertexShader(VertexShaderBlur, false);
        NEngineInternal::GPShaderRenderManager->ApplyPixelShader(PixelShaderBlur);
        NEngineInternal::GPShaderRenderManager->SetPixelShaderConstant(5, C4DVector(0.33f, 0.33f, 0.33f, 0.33f));
        NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(0, 0.0f, 0.0f, 1.0f, 1.0f);
        NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(1, 0.0f, 1.0f / 128.0f, 1.0f, 1.0f);
        NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(2, 1.0f / 128.0f, 0.0f, 1.0f, 1.0f);
        NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(3, 1.0f / 128.0f, 1.0f / 128.0f, 1.0f, 1.0f);
        const CTexture* textures[4] = { GlowTexture256, GlowTexture256, GlowTexture256, GlowTexture256 };
        NEngineInternal::GPShaderRenderManager->PrepareForRender(0);
        NEngineInternal::GPRenderManager->DrawAsIndexedTris(textures, 4, Glow256ToGlow64Verts, sizeof(CTVertexColTex1), 4, QuadIndices, 2);
        NEngineInternal::GPDisplayManager->SetRenderTargetAsBackScreen();
    }
    float offsetX = 1.0f / 128.0f;
    float offsetY = 1.0f / 128.0f;
    for (long i = 0; i < NoBlurPasses; ++i) {
        NEngineInternal::GPRenderManager->SetAlphaBlendEnable(false);
        {
            CRenderTarget target;
            CTexture* destination = GlowTexture64Alt;
            target.AttachTarget(destination->GetSurfaceLevel(0));
            NEngineInternal::GPDisplayManager->SetRenderTarget(target);
            NEngineInternal::GPShaderRenderManager->ApplyVertexShader(VertexShaderBlur, false);
            NEngineInternal::GPShaderRenderManager->ApplyPixelShader(PixelShaderBlur);
            const float origin = -1.0f / 128.0f;
            float left = origin - offsetX;
            float right = offsetX - 1.0f / 128.0f;
            float top = origin - offsetY;
            float bottom = offsetY - 1.0f / 128.0f;
            C4DVector weights(0.33f, 0.33f, 0.33f, 0.33f);
            NEngineInternal::GPShaderRenderManager->SetPixelShaderConstant(5, weights);
            NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(0, origin, origin, 1.0f, 1.0f);
            NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(1, left, top, 1.0f, 1.0f);
            NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(2, right, top, 1.0f, 1.0f);
            NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(3, origin, bottom, 1.0f, 1.0f);
            const CTexture* source = GlowTexture64;
            const CTexture* textures[4] = { source, source, source, source };
            NEngineInternal::GPShaderRenderManager->PrepareForRender(0);
            NEngineInternal::GPRenderManager->DrawAsIndexedTris(textures, 4, Glow64ToGlow16Verts, sizeof(CTVertexColTex1), 4, QuadIndices, 2);
            NEngineInternal::GPDisplayManager->SetRenderTargetAsBackScreen();
        }
        offsetX = BlurOffsetScale * offsetX;
        offsetY = BlurOffsetScale * offsetY;
        NEngineInternal::GPRenderManager->SetAlphaBlendEnable(false);
        {
            CRenderTarget target;
            CTexture* destination = GlowTexture64;
            target.AttachTarget(destination->GetSurfaceLevel(0));
            NEngineInternal::GPDisplayManager->SetRenderTarget(target);
            NEngineInternal::GPShaderRenderManager->ApplyVertexShader(VertexShaderBlur, false);
            NEngineInternal::GPShaderRenderManager->ApplyPixelShader(PixelShaderBlur);
            const float origin = -1.0f / 128.0f;
            float left = origin - offsetX;
            float right = offsetX - 1.0f / 128.0f;
            float top = origin - offsetY;
            float bottom = offsetY - 1.0f / 128.0f;
            C4DVector weights(0.33f, 0.33f, 0.33f, 0.33f);
            NEngineInternal::GPShaderRenderManager->SetPixelShaderConstant(5, weights);
            NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(0, origin, origin, 1.0f, 1.0f);
            NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(1, origin, top, 1.0f, 1.0f);
            NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(2, left, bottom, 1.0f, 1.0f);
            NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(3, right, bottom, 1.0f, 1.0f);
            const CTexture* source = GlowTexture64Alt;
            const CTexture* textures[4] = { source, source, source, source };
            NEngineInternal::GPShaderRenderManager->PrepareForRender(0);
            NEngineInternal::GPRenderManager->DrawAsIndexedTris(textures, 4, Glow64ToGlow16Verts, sizeof(CTVertexColTex1), 4, QuadIndices, 2);
            NEngineInternal::GPDisplayManager->SetRenderTargetAsBackScreen();
        }
        offsetX = BlurOffsetScale * offsetX;
        offsetY = BlurOffsetScale * offsetY;
    }
    {
        CRenderTarget target;
        target.AttachTarget(GlowTexture16->GetSurfaceLevel(0));
        NEngineInternal::GPDisplayManager->SetRenderTarget(target);
        NEngineInternal::GPShaderRenderManager->ApplyVertexShader(VertexShaderBlur, false);
        NEngineInternal::GPShaderRenderManager->ApplyPixelShader(PixelShaderBlur);
        NEngineInternal::GPShaderRenderManager->SetPixelShaderConstant(5, C4DVector(IntensityR, IntensityG, IntensityB, 0.3f));
        NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(0, 0.0f, 0.0f, 1.0f, 1.0f);
        NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(1, 0.0f, 1.0f / 32.0f, 1.0f, 1.0f);
        NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(2, 1.0f / 32.0f, 0.0f, 1.0f, 1.0f);
        NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(3, 1.0f / 32.0f, 1.0f / 32.0f, 1.0f, 1.0f);
        const CTexture* textures[4] = { GlowTexture64, GlowTexture64, GlowTexture64, GlowTexture64 };
        NEngineInternal::GPShaderRenderManager->PrepareForRender(0);
        NEngineInternal::GPRenderManager->DrawAsIndexedTris(textures, 4, Glow64ToGlow16Verts, sizeof(CTVertexColTex1), 4, QuadIndices, 2);
    }
    NEngineInternal::GPDisplayManager->SetRenderTargetAsBackScreen();
    g_pEngineCamera->RefreshD3DTransforms();
}
