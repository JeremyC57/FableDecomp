// CEngineScreenEffectOutlineGlow::ApplyOutline() -- blur the outline texture and composite it over the back buffer through the stencil mask
// FITTED (owner acceptance device d): one inferred dead release assert, see Inferred; no unused locals, no flags, no header change.
// Blur the outline texture into the back buffer: inside a render-state capture block with both shader kinds forced on, save the display's
// render target (OldRenderTarget) and viewport, set point filtering / clamp addressing on stages 0..3 with blending, alpha test, Z, culling,
// dither and stencil off; pass 0 renders OutlineTexture into Blur1Texture with the OutlineToBlur shaders; then BlurStepCount times ping-pong
// Blur1 -> Blur2 -> Blur1 with the BlurToBlur shaders (vertex-constant offsets +-BlurOffsetU/V); finally switch back to the back screen, refresh
// the camera transforms, release OldRenderTarget and composite Blur1Texture with linear filtering, src-alpha blending, alpha test >= 1 and the
// stencil test (equal 4, pass -> zero).
// Retail: Fable.exe @ 0x00B89C30, length 6431 bytes (0x191F), plain `ret` (two epilogue copies), no frame pointer, no SEH.
// Callees: ApplyVertexShader 0x00988020, ApplyPixelShader 0x00988140, SetUserVertexShaderConstant 0x00989B00, Enable/DisableVertexShaders
//   0x00987FE0 / 0x009880E0, Enable/DisablePixelShaders 0x00988110 / 0x00988190, PrepareForRender(0) inline (SetVSConstants 0x0098B930,
//   UpdatePixelShader 0x00988A20, UpdatePixelShaderConstants 0x00989BF0, UpdateAmbient 0x00989760, UpdateFogColour 0x009897C0,
//   UpdateLightGlobals 0x0098A760, ResetWorldTransform 0x00988290, UpdateCombinedProjectionTransform 0x00988A50), CBaseClass ctor/dtor
//   0x0099A2D0 / 0x0099A2E0, CRenderTarget(CSurface&,CSurface&) 0x00A10DC0, CSurface::operator= 0x009F2E70, CSurface::Uninitialise
//   0x009F2E20, CTexture::GetSurfaceLevel 0x009F9E00, CRenderTarget::AttachTarget 0x00A10E60, CDisplayManager::SetRenderTarget 0x009BF160,
//   SetRenderTargetAsBackScreen 0x009BF220, DrawAsIndexedTris 0x00A0AEA0, CEngineCamera::RefreshD3DTransforms 0x00B2FC50,
//   CRenderTarget::Uninitialise 0x00A10E30, RestoreCaptureBlock 0x00A05840.
// Shape notes: `tex` sits in its own block so it shares the loop counter's slot (esp0+0x14); the blur offsets are `static volatile float`
//   (retail copies every positive read through fld/fstp: 0x00B8A917, 0x00B8A927, 0x00B8AAF9, 0x00B8AB0E -- non-volatile reads compile to
//   integer moves); BlurOffsetU / BlurOffsetV are descriptive names (no name in the PDBs / strings).
// Retail-volatile: BlurOffsetU / BlurOffsetV (0x0139AB54 / 0x0139AB58) are copied through the x87 at every read, the six positive reads
//   included: 0x00B8A917 fld dword ptr [0139AB54h] / 0x00B8A91D fstp [esp], 0x00B8A927 fld dword ptr [0139AB58h] / 0x00B8A936 fstp [esp],
//   0x00B8AAF9 fld [0139AB58h], 0x00B8AB0E fld [0139AB54h] (10 reads, ten fld). Without `volatile` VC7.1 keeps all ten reads but copies
//   the six positive ones with integer `mov reg,[G] ; push reg` (DIFFER 6408v6431, 10 hunks from 0x00B8A906): retail's bytes depend on it.
//   No non-volatile spelling found (plain, unary +, `+ 0.0f`, (float)(double), inline float accessors: all integer moves; `1.0f *`: fmul).
//   Same tweakable-static pattern as the landed sibling 0x00B5FAB0 (BlurOffsetScale / NoBlurPasses, owner ruling 2026-11-13).
// Leaf: ?ApplyOutline@CEngineScreenEffectOutlineGlow@@AAEXXZ
// Inferred: ONE FABLE_RETAIL_ASSERT statement (dead arm, no bytes) before the pass-0 vertex constants:
//   FABLE_RETAIL_ASSERT(OutlineTexture->GetWidth(0) == 256) -- pass 0 hard-codes one-texel offsets of 1/256 for its OutlineTexture
//   reads, and the constructor creates the outline texture 0x100 x 0x100 (0x00B8926B `mov ecx,100h`; the blur textures 0x80 x 0x80,
//   0x00B89298, matching the 1/128 blur offsets). CTexture::GetWidth(unsigned long) is the PDB-backed inline (Ego_r / FableWin
//   ?GetWidth@CTexture@@QBEKK@Z, landed opt-in FABLE_RETAIL_CTEXTURE_GETWIDTH). FITTED (owner acceptance device d): the dead arm's
//   inline expansion (GetWidth -> GetDimensions result temporary) feeds c2's first-pass record ids, which pick the bookmark-test operand
//   order of the SetAlphaRef block 0x00B8AF59 (retail `mov eax,[eax+2814h]` before `mov esi,[edx+8]`). Measured: one assert with ONE
//   GetWidth call matches at any position (function top, pass 0, loop top, before SetAlphaRef); two GetWidth calls overshoot (20 hunks);
//   asserts without an inline call (`ASSERT(Blur1Texture)`, `->IsValid()`) change nothing (2 sites). No unused locals are needed.
#define FABLE_RETAIL_ASSERT_DEAD_ARM
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CSYSTEMMANAGER
#define FABLE_RETAIL_CSURFACE
#define FABLE_RETAIL_CSURFACE_COPYFROMMEMORY
#define FABLE_RETAIL_CSURFACE_ASSIGN
#define FABLE_RETAIL_CDISPLAYMANAGER
#define FABLE_RETAIL_CDISPLAYMANAGER_BACKSCREEN
#define FABLE_RETAIL_CDISPLAYMANAGER_GETVIEWPORT
#define FABLE_RETAIL_CDISPLAYMANAGER_GETRENDERTARGET
#define FABLE_RETAIL_CDISPLAYMANAGER_SETRENDERTARGET
#define FABLE_RETAIL_CDISPLAYMANAGER_GETRENDERTARGET
#define FABLE_RETAIL_CRENDERMANAGERCORE
#define FABLE_RETAIL_CRENDERMANAGERCORE_DRAWASINDEXEDTRIS
#define FABLE_RETAIL_CRENDERSTATEMANAGER_STENCIL_INLINE
#define FABLE_RETAIL_CRENDERSTATEMANAGER_OUTLINE_INLINE
#define FABLE_RETAIL_CTEXTURE
#define FABLE_RETAIL_CTEXTURE_SIZED
#define FABLE_RETAIL_CTEXTURE_GETWIDTH
#define FABLE_RETAIL_CENGINECAMERA
#define FABLE_RETAIL_CENGINECAMERA_REFRESHD3DTRANSFORMS
#define FABLE_RETAIL_CENGINESCREENEFFECTOUTLINEGLOW
#define FABLE_RETAIL_CENGINESCREENEFFECTOUTLINEGLOW_MEMBERS
#include "fable_assert.h"
#include "engine/Direct3D9Types.h"
#include "engine/CSurface.h"
#include "engine/CDisplayManager.h"
#include "engine/CRenderManager.h"
#include "engine/CShaderRenderManager.h"
#include "engine/CRenderStateCaptureBlock.h"
#include "engine/CScopedShadersEnable.h"
#include "engine/CEngineCamera.h"
#include "engine/CEngineScreenEffectOutlineGlow.h"

namespace NEngineInternal { extern CRenderManager* GPRenderManager; }              // 0x01436E18
namespace NEngineInternal { extern CShaderRenderManager* GPShaderRenderManager; }  // 0x01436E14
namespace NEngineInternal { extern CDisplayManager* GPDisplayManager; }            // 0x01436E1C
namespace NEngineInternal { extern CEngineCamera* GPCamera; }                                // 0x01436EA0


void CEngineScreenEffectOutlineGlow::ApplyOutline()
{
    static volatile float BlurOffsetU = 1.0f / 128.0f;                                      // 0x0139AB54 (descriptive name)
    static volatile float BlurOffsetV = 1.0f / 128.0f;                                      // 0x0139AB58 (descriptive name)
    static unsigned short QuadIndices[6] = { 0, 1, 3, 1, 2, 3 };                   // 0x0139AB5C
    if (!OutlineTexture)
        return;
    CRenderStateScopedCaptureBlock capture;
    CScopedShadersEnable shaders(true, true);
    OldRenderTarget = NEngineInternal::GPDisplayManager->GetRenderTarget();
    OldViewport = NEngineInternal::GPDisplayManager->GetViewport();
    NEngineInternal::GPRenderManager->SetAlphaBlendEnable(false);
    NEngineInternal::GPRenderManager->SetAlphaTestEnable(false);
    NEngineInternal::GPRenderManager->SetZBufferEnable(false);
    NEngineInternal::GPRenderManager->SetPolyCullMode(POLY_CULL_NONE);
    NEngineInternal::GPRenderManager->SetDitherEnable(false);
    NEngineInternal::GPRenderManager->SetStencilEnable(false);
    NEngineInternal::GPRenderManager->SetTextureFilterModeMag(0, TEXTURE_FILTER_POINT);
    NEngineInternal::GPRenderManager->SetTextureFilterModeMin(0, TEXTURE_FILTER_POINT);
    NEngineInternal::GPRenderManager->SetTextureFilterModeMag(1, TEXTURE_FILTER_POINT);
    NEngineInternal::GPRenderManager->SetTextureFilterModeMin(1, TEXTURE_FILTER_POINT);
    NEngineInternal::GPRenderManager->SetTextureFilterModeMag(2, TEXTURE_FILTER_POINT);
    NEngineInternal::GPRenderManager->SetTextureFilterModeMin(2, TEXTURE_FILTER_POINT);
    NEngineInternal::GPRenderManager->SetTextureFilterModeMag(3, TEXTURE_FILTER_POINT);
    NEngineInternal::GPRenderManager->SetTextureFilterModeMin(3, TEXTURE_FILTER_POINT);
    NEngineInternal::GPRenderManager->SetTextureUVModeU(0, TEXTURE_UV_CLAMP);
    NEngineInternal::GPRenderManager->SetTextureUVModeV(0, TEXTURE_UV_CLAMP);
    NEngineInternal::GPRenderManager->SetTextureUVModeU(1, TEXTURE_UV_CLAMP);
    NEngineInternal::GPRenderManager->SetTextureUVModeV(1, TEXTURE_UV_CLAMP);
    NEngineInternal::GPRenderManager->SetTextureUVModeU(2, TEXTURE_UV_CLAMP);
    NEngineInternal::GPRenderManager->SetTextureUVModeV(2, TEXTURE_UV_CLAMP);
    NEngineInternal::GPRenderManager->SetTextureUVModeU(3, TEXTURE_UV_CLAMP);
    NEngineInternal::GPRenderManager->SetTextureUVModeV(3, TEXTURE_UV_CLAMP);
    NEngineInternal::GPShaderRenderManager->ApplyVertexShader(OutlineToBlurVertexShader, false);
    NEngineInternal::GPShaderRenderManager->ApplyPixelShader(OutlineToBlurPixelShader);
    FABLE_RETAIL_ASSERT(OutlineTexture->GetWidth(0) == 256);                       // Inferred (see header)
    NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(0, 0.0f, 0.0f, 0.0f, 0.0f);
    NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(1, 1.0f / 256.0f, 0.0f, 0.0f, 0.0f);
    NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(2, 0.0f, 1.0f / 256.0f, 0.0f, 0.0f);
    NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(3, 1.0f / 256.0f, 1.0f / 256.0f, 0.0f, 0.0f);
    {
        CRenderTarget target;
        target.AttachTarget(Blur1Texture->GetSurfaceLevel(0));
        NEngineInternal::GPDisplayManager->SetRenderTarget(target);
        NEngineInternal::GPShaderRenderManager->PrepareForRender(0);
        const CTexture* textures[4] = { OutlineTexture, OutlineTexture, OutlineTexture, OutlineTexture };
        NEngineInternal::GPRenderManager->DrawAsIndexedTris(textures, 4, OutlineToBlurVerts, sizeof(CTVertexColTex1), 4, QuadIndices, 2);
    }
    NEngineInternal::GPShaderRenderManager->ApplyVertexShader(BlurToBlurVertexShader, false);
    NEngineInternal::GPShaderRenderManager->ApplyPixelShader(BlurToBlurPixelShader);
    for (long i = 0; i < BlurStepCount; ++i)
    {
        NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(0, 0.0f, 0.0f, 0.0f, 0.0f);
        NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(1, -BlurOffsetU, -BlurOffsetV, 0.0f, 0.0f);
        NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(2, BlurOffsetU, -BlurOffsetV, 0.0f, 0.0f);
        NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(3, 0.0f, BlurOffsetV, 0.0f, 0.0f);
        {
            CRenderTarget target;
            target.AttachTarget(Blur2Texture->GetSurfaceLevel(0));
            NEngineInternal::GPDisplayManager->SetRenderTarget(target);
            NEngineInternal::GPShaderRenderManager->PrepareForRender(0);
            const CTexture* textures[4] = { Blur1Texture, Blur1Texture, Blur1Texture, Blur1Texture };
            NEngineInternal::GPRenderManager->DrawAsIndexedTris(textures, 4, BlurToBlurVerts, sizeof(CTVertexColTex1), 4, QuadIndices, 2);
        }
        NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(0, 0.0f, 0.0f, 0.0f, 0.0f);
        NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(1, -BlurOffsetU, BlurOffsetV, 0.0f, 0.0f);
        NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(2, BlurOffsetU, BlurOffsetV, 0.0f, 0.0f);
        NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(3, 0.0f, -BlurOffsetV, 0.0f, 0.0f);
        {
            CRenderTarget target;
            target.AttachTarget(Blur1Texture->GetSurfaceLevel(0));
            NEngineInternal::GPDisplayManager->SetRenderTarget(target);
            NEngineInternal::GPShaderRenderManager->PrepareForRender(0);
            const CTexture* textures[4] = { Blur2Texture, Blur2Texture, Blur2Texture, Blur2Texture };
            NEngineInternal::GPRenderManager->DrawAsIndexedTris(textures, 4, BlurToBlurVerts, sizeof(CTVertexColTex1), 4, QuadIndices, 2);
        }
    }
    NEngineInternal::GPDisplayManager->SetRenderTargetAsBackScreen();
    NEngineInternal::GPCamera->RefreshD3DTransforms();
    OldRenderTarget.Uninitialise();
    NEngineInternal::GPShaderRenderManager->ApplyVertexShader(BlurToBackBufferVertexShader, false);
    NEngineInternal::GPShaderRenderManager->ApplyPixelShader(BlurToBackBufferPixelShader);
    NEngineInternal::GPRenderManager->SetTextureFilterModeMag(0, TEXTURE_FILTER_LINEAR);
    NEngineInternal::GPRenderManager->SetTextureFilterModeMin(0, TEXTURE_FILTER_LINEAR);
    NEngineInternal::GPRenderManager->SetAlphaBlendEnable(true);
    NEngineInternal::GPRenderManager->SetAlphaTestEnable(true);
    NEngineInternal::GPRenderManager->SetAlphaTestFunc(ALPHA_TEST_GREATER_EQUAL);
    NEngineInternal::GPRenderManager->SetAlphaRef(1);
    NEngineInternal::GPRenderManager->SetSourceAlphaBlendMode(ALPHA_BLEND_SRC_ALPHA);
    NEngineInternal::GPRenderManager->SetDestAlphaBlendMode(ALPHA_BLEND_INV_SRC_ALPHA);
    NEngineInternal::GPRenderManager->SetZBufferEnable(false);
    NEngineInternal::GPRenderManager->SetPolyCullMode(POLY_CULL_NONE);
    NEngineInternal::GPRenderManager->SetDitherEnable(true);
    NEngineInternal::GPRenderManager->SetStencilEnable(true);
    NEngineInternal::GPRenderManager->SetStencilFunc(STENCIL_FUNC_EQUAL);
    NEngineInternal::GPRenderManager->SetStencilOpPass(STENCIL_OP_MODE_ZERO);
    NEngineInternal::GPRenderManager->SetStencilMask(4);
    NEngineInternal::GPRenderManager->SetStencilRef(4);
    NEngineInternal::GPShaderRenderManager->PrepareForRender(0);
    {
        const CTexture* tex = Blur1Texture;
        NEngineInternal::GPRenderManager->DrawAsIndexedTris(&tex, 1, BlurToBackBufferVerts, sizeof(CTVertexColTex1), 4, QuadIndices, 2);
    }
}
