// CEngineScreenEffectRadialBlurRenderer::Render(const CEngineComponentRenderParams&) -- when enabled: pick up the environment's blur parameters if asked to, and when the blur source is tracked on screen with a visible fade capture the back buffer and apply the radial blur; show the debug texture if requested; then release this frame's two blur textures
// Leaf: Render
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
#include "engine/CEngineScreenEffectRadialBlurRenderer.h"

class CEngineComponentRenderParams;

struct CTexturePool_00f36f00 {
    void Release(long texture);                    // 0x00F36F00
};
struct CEngineGlobal_01436e98 {
    unsigned char         _pad[0x3190];
    CTexturePool_00f36f00 m_texturePool;           // +0x3190
};
extern CEngineGlobal_01436e98* g_pEngine_01436e98;   // 0x01436E98

struct CEngineScreenEffectRadialBlurRenderer_Render : CEngineScreenEffectRadialBlurRenderer {
    void CaptureEnvironmentParameters();           // 0x00B5BD70
    bool UpdateWorldPositionTracking();            // 0x00B5C120
    void CaptureBackBuffer();                      // 0x00B5CA70
    void ApplyRadialBlur();                        // 0x00B5E0F0
    void ShowBlurTexture();                        // 0x00B5E700
    void Render(const CEngineComponentRenderParams& params);
};

void CEngineScreenEffectRadialBlurRenderer_Render::Render(const CEngineComponentRenderParams& params)
{
    if (_pad_0x08[0]) {
        if (UseEnvironmentParameters)
            CaptureEnvironmentParameters();
        if (UpdateWorldPositionTracking() && CurrentParameters_Fade > 1.0f / 255.0f) {
            CaptureBackBuffer();
            ApplyRadialBlur();
        }
        if (ShowDebug)
            ShowBlurTexture();
        if (BlurTexture256_1) {
            CTexturePool_00f36f00* pool = &g_pEngine_01436e98->m_texturePool;
            pool->Release(BlurTextureHandle256_1);
            pool->Release(BlurTextureHandle256_2);
            BlurTexture256_1 = 0;
            BlurTexture256_2 = 0;
        }
    }
}