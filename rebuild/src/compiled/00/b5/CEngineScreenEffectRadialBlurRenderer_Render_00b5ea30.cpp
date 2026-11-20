// CEngineScreenEffectRadialBlurRenderer::Render(const CEngineComponentRenderParams&) -- when enabled: pick up the environment's blur parameters if asked to, and when the blur source is tracked on screen with a visible fade capture the back buffer and apply the radial blur; show the debug texture if requested; then release this frame's two blur textures
// Leaf: ?Render@CEngineScreenEffectRadialBlurRenderer@@UAEXABVCEngineComponentRenderParams@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B5EA30, length 151 bytes (+ int3 padding); virtual
// `?Render@CEngineScreenEffectRadialBlurRenderer@@UAEXABVCEngineComponentRenderParams@@@Z` (`ret 4`, the params unused).
// Callees: CaptureEnvironmentParameters @ 0x00B5BD70, UpdateWorldPositionTracking @ 0x00B5C120 (bool), CaptureBackBuffer
// @ 0x00B5CA70, ApplyRadialBlur @ 0x00B5E0F0, ShowBlurTexture @ 0x00B5E700, the texture pool release @ 0x00F36F00 twice
// (engine global @ 0x01436E98 + 0x3190, kept in edi). Float pool 1/255 @ 0x01231724.
// Layout: engine/CEngineScreenEffectRadialBlurRenderer.h -- BlurTexture256_1/_2 +0x0c/+0x10, their handles +0x14/+0x18,
// CurrentParameters_Fade +0x1b4, ShowDebug +0x1c4, UseEnvironmentParameters +0x1c9; the enabled flag is the retail-only
// byte at +0x08 (the header's `_pad_0x08`). The method is attached through a derived shell.
// Shape: `this` is still in ecx for the first call (no reload); the fade test is `Fade > 1/255` (`fcomp ; test ah,41h ;
// jne`); the release tail tests only the first texture, frees both handles through one pool local, then clears both.
// Naming (NF172, 2026-11-20, q11739): the derived _Render shell dropped; the row is the CEngineComponent::Render override itself on
//   engine/CEngineScreenEffectRadialBlurRenderer.h MEMBERS mode (+ NF172 opt-in FABLE_RETAIL_CENGINESCREENEFFECTRADIALBLURRENDERER_RENDER,
//   + CAPTUREBACKBUFFER): own leaf Ego_r.pdb / FableWin.pdb ?Render@CEngineScreenEffectRadialBlurRenderer@@UAEXABVCEngineComponentRenderParams@@@Z
//   (was ?Render@CEngineScreenEffectRadialBlurRenderer_Render@@QAEXABVCEngineComponentRenderParams@@@Z), and the five private callees bind
//   their PDB spellings (...@CEngineScreenEffectRadialBlurRenderer@@AAE...: CaptureEnvironmentParameters 0x00B5BD70, UpdateWorldPositionTracking
//   0x00B5C120, CaptureBackBuffer 0x00B5CA70, ApplyRadialBlur 0x00B5E0F0, ShowBlurTexture 0x00B5E700; were the _Render shell's QAE stand-ins).
//   The enabled flag at +0x08 is CEngineComponent::RenderEnabled through the inline FableWin.pdb PeekRenderEnabled.
#define FABLE_RETAIL_CTEXTURE
#define FABLE_RETAIL_CENGINESCREENEFFECTRADIALBLURRENDERER
#define FABLE_RETAIL_CENGINESCREENEFFECTRADIALBLURRENDERER_MEMBERS
#define FABLE_RETAIL_CENGINESCREENEFFECTRADIALBLURRENDERER_RENDER
#define FABLE_RETAIL_CENGINESCREENEFFECTRADIALBLURRENDERER_CAPTUREBACKBUFFER
#include "engine/CEngineScreenEffectRadialBlurRenderer.h"

// NF44 (round 6): the 0x01436E98 global is NEngineInternal::GPResourceManager (Ego_r.pdb / FableWin.pdb
// ?GPResourceManager@NEngineInternal@@3PAVCEngineResourceManager@@A) and its +0x3190 member the CGPUScratchBufferTextureManager
// (engine/CEngineResourceManager.h GetGPUScratchBufferTextureManager(), engine/CGPUScratchBufferTextureManager.h, Ego_r.pdb member
// spellings); were the address-named CEngineGlobal_01436e98 / g_pEngine_01436e98 and local scratch-manager stand-ins.
#define FABLE_RETAIL_CENGINERESOURCEMANAGER
#define FABLE_RETAIL_CGPUSCRATCHBUFFERTEXTUREMANAGER_FREEMEMORYPAGE
#define FABLE_RETAIL_CGPUSCRATCHBUFFERTEXTUREMANAGER_ENDTEXTUREUSE
#define FABLE_RETAIL_CGPUSCRATCHBUFFERTEXTUREMANAGER_STARTTEXTUREUSE
#include "engine/CEngineResourceManager.h"
#include "engine/CGPUScratchBufferTextureManager.h"
namespace NEngineInternal { extern CEngineResourceManager* GPResourceManager; }   // 0x01436E98

void CEngineScreenEffectRadialBlurRenderer::Render(const CEngineComponentRenderParams& params)
{
    if (PeekRenderEnabled()) {
        if (UseEnvironmentParameters)
            CaptureEnvironmentParameters();
        if (UpdateWorldPositionTracking() && CurrentParameters.Fade > 1.0f / 255.0f) {
            CaptureBackBuffer();
            ApplyRadialBlur();
        }
        if (ShowDebug)
            ShowBlurTexture();
        if (BlurTexture256_1) {
            CGPUScratchBufferTextureManager* pool = &NEngineInternal::GPResourceManager->GetGPUScratchBufferTextureManager();
            pool->EndTextureUse(BlurTextureHandle256_1);
            pool->EndTextureUse(BlurTextureHandle256_2);
            BlurTexture256_1 = 0;
            BlurTexture256_2 = 0;
        }
    }
}