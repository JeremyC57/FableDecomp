// CEngineScreenEffectRadialBlurRenderer::SetEffectQuality(const CEngineVideoQualitySettings&) -- enable the radial blur when the quality settings ask for it and the hardware supports it (and its textures could be allocated); otherwise free its dynamic texture page and disable it. Returns false only when the textures could not be allocated
// Leaf: SetEffectQuality
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B5BFD0, length 113 bytes (+ int3 padding).
// Callees: CEngineScreenEffectRadialBlurRenderer::InitialiseTextures @ 0x00B5BF10, CGPUScratchBufferTextureManager::FreeMemoryPage
// @ 0x00F37030 (engine global @ 0x01436E98 + 0x3190).
// Layout: engine/CEngineScreenEffectRadialBlurRenderer.h -- DynamicTexturePageHandle +0x1c, EffectSupported +0x1c8; the
// enabled flag is the retail-only byte at +0x08 (the header's `_pad_0x08`); the settings' radial-blur byte is at +0x6f.
// The method is attached through a derived shell (the header is a plain layout struct).
// Shape: the page is freed through an inline `if (handle >= 0)` helper (`test ; jl`, the handle not reset); the success
// arm returns the stored 1 (`mov al,1 ; mov [esi+8],al`), the failure arm the stored 0, the disabled arm stores an
// immediate 0 after the free and returns 1.
#include "engine/CEngineScreenEffectRadialBlurRenderer.h"

struct CEngineVideoQualitySettings {
    unsigned char _pad[0x6f];
    bool          RadialBlurEnabled;               // +0x6f
};

struct CGPUScratchBufferTextureManager {
    void FreeMemoryPage(long page);                // 0x00F37030
};
struct CEngineGlobal_01436e98 {
    unsigned char                   _pad[0x3190];
    CGPUScratchBufferTextureManager m_scratchTextures;   // +0x3190
};
extern CEngineGlobal_01436e98* g_pEngine_01436e98;   // 0x01436E98

struct CEngineScreenEffectRadialBlurRenderer_Quality : CEngineScreenEffectRadialBlurRenderer {
    bool InitialiseTextures();                     // 0x00B5BF10
    void FreeTexturePage()
    {
        if (DynamicTexturePageHandle >= 0)
            g_pEngine_01436e98->m_scratchTextures.FreeMemoryPage(DynamicTexturePageHandle);
    }
    bool SetEffectQuality(const CEngineVideoQualitySettings& quality);
};

bool CEngineScreenEffectRadialBlurRenderer_Quality::SetEffectQuality(const CEngineVideoQualitySettings& quality)
{
    if (quality.RadialBlurEnabled && EffectSupported) {
        if (InitialiseTextures()) {
            _pad_0x08[0] = true;
            return true;
        }
        FreeTexturePage();
        _pad_0x08[0] = false;
        return false;
    }
    FreeTexturePage();
    _pad_0x08[0] = false;
    return true;
}