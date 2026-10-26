// CEngineScreenEffectGlowRenderer::SetEffectQuality(const CEngineVideoQualitySettings&) -- enable the glow when the quality settings ask for it and the hardware supports it (and its textures could be allocated); otherwise free its dynamic texture page and disable it. Returns false only when the textures could not be allocated
// Leaf: SetEffectQuality
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B5EE00, length 114 bytes (+ int3 padding).
// Callees: CEngineScreenEffectGlowRenderer::InitialiseTextures @ 0x00B5ECE0, CGPUScratchBufferTextureManager::FreeMemoryPage
// @ 0x00F37030 (engine global @ 0x01436E98 + 0x3190).
// Layout: engine/CEngineScreenEffectGlowRenderer.h -- DynamicTexturePageHandle +0x30, EffectSupported +0x21c; the
// enabled flag is the retail-only byte at +0x08 (the header's `_pad_0x08`); the settings' glow byte is at +0x6e.
// The method is attached through a derived shell (the header is a plain layout struct).
// Shape: the page is freed through an inline `if (handle >= 0)` helper (`test ; jl`, the handle not reset); the success
// arm returns the stored 1 (`mov al,1 ; mov [esi+8],al`); unlike the radial-blur twin @ 0x00B5BFD0 the disabled arm
// clears the flag BEFORE freeing the page (the failure arm's immediate store + `xor al,al` comes from the same source
// as the twin's `xor al,al ; mov [esi+8],al`).
#include "engine/CEngineScreenEffectGlowRenderer.h"

struct CEngineVideoQualitySettings {
    unsigned char _pad[0x6e];
    bool          GlowEnabled;                     // +0x6e
};

class CGPUScratchBufferTextureManager { public:
    void FreeMemoryPage(long page);                // 0x00F37030
};
struct CEngineGlobal_01436e98 {
    unsigned char                   _pad[0x3190];
    CGPUScratchBufferTextureManager m_scratchTextures;   // +0x3190
};
extern CEngineGlobal_01436e98* g_pEngine_01436e98;   // 0x01436E98

struct CEngineScreenEffectGlowRenderer_Quality : CEngineScreenEffectGlowRenderer {
    bool InitialiseTextures();                     // 0x00B5ECE0
    void FreeTexturePage()
    {
        if (DynamicTexturePageHandle >= 0)
            g_pEngine_01436e98->m_scratchTextures.FreeMemoryPage(DynamicTexturePageHandle);
    }
    bool SetEffectQuality(const CEngineVideoQualitySettings& quality);
};

bool CEngineScreenEffectGlowRenderer_Quality::SetEffectQuality(const CEngineVideoQualitySettings& quality)
{
    if (quality.GlowEnabled && EffectSupported) {
        if (InitialiseTextures()) {
            _pad_0x08[0] = true;
            return true;
        }
        FreeTexturePage();
        _pad_0x08[0] = false;
        return false;
    }
    _pad_0x08[0] = false;
    FreeTexturePage();
    return true;
}