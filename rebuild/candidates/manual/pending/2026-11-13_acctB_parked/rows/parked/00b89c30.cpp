// CEngineScreenEffectOutlineGlow::ApplyOutline()
// Grade: DIFFER(6431v6431) -- 1 scheduling hunk: in the SetAlphaRef(1) block (0x00B8AF59) ours loads state.BookmarkMask ([edx+8]) before CurrentBookmarkMask ([eax+0x2814]); retail loads the mask first. Everything else byte-identical (the 2 other hunks fg prints are relocation-mask artefacts of identical bytes).
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
// Retail-volatile: BlurOffsetU / BlurOffsetV (0x0139AB54 / 0x0139AB58) are read through the x87 at every use (0x00B8A913..0x00B8A91D
//   `fstp [esp+4] ; fld dword ptr [0139AB54h] ; fstp [esp]`, 0x00B8A927..0x00B8A936 `fld dword ptr [0139AB58h] ... fstp [esp]`); without
//   `volatile` VC7.1 copies the positive offsets with integer moves (14 hunks). Tweakable function statics.
// Leaf: ?ApplyOutline@CEngineScreenEffectOutlineGlow@@AAEXXZ
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CSYSTEMMANAGER
#define FABLE_RETAIL_CSURFACE
#define FABLE_RETAIL_CSURFACE_COPYFROMMEMORY
#define FABLE_RETAIL_CSURFACE_ASSIGN
#define FABLE_RETAIL_CDISPLAYMANAGER
#define FABLE_RETAIL_CDISPLAYMANAGER_BACKSCREEN
#define FABLE_RETAIL_CDISPLAYMANAGER_GETVIEWPORT
#define FABLE_RETAIL_CDISPLAYMANAGER_SETRENDERTARGET
#define FABLE_RETAIL_CDISPLAYMANAGER_GETRENDERTARGET
#define FABLE_RETAIL_CRENDERMANAGERCORE
#define FABLE_RETAIL_CRENDERMANAGERCORE_DRAWASINDEXEDTRIS
#define FABLE_RETAIL_CRENDERSTATEMANAGER_STENCIL_INLINE
#define FABLE_RETAIL_CRENDERSTATEMANAGER_OUTLINE_INLINE
#define FABLE_RETAIL_CTEXTURE
#define FABLE_RETAIL_CTEXTURE_SIZED
#define FABLE_RETAIL_CENGINECAMERA
#define FABLE_RETAIL_CENGINECAMERA_REFRESHD3DTRANSFORMS
#define FABLE_RETAIL_CENGINESCREENEFFECTOUTLINEGLOW
#define FABLE_RETAIL_CENGINESCREENEFFECTOUTLINEGLOW_MEMBERS
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
extern CEngineCamera* g_pEngineCamera;                                             // 0x01436EA0

void CEngineScreenEffectOutlineGlow::ApplyOutline()
{
    static volatile float BlurOffsetU = 1.0f / 128.0f;                                      // 0x0139AB54 (descriptive name)
    static volatile float BlurOffsetV = 1.0f / 128.0f;                                      // 0x0139AB58 (descriptive name)
    static unsigned short QuadIndices[6] = { 0, 1, 3, 1, 2, 3 };                   // 0x0139AB5C
    if (!OutlineTexture)
        return;
    CRenderStateScopedCaptureBlock capture;
    CScopedShadersEnable shaders(true, true);
    {
        CRenderTarget current = NEngineInternal::GPDisplayManager->GetRenderTarget();
        OldRenderTarget = current;
    }
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
    g_pEngineCamera->RefreshD3DTransforms();
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
