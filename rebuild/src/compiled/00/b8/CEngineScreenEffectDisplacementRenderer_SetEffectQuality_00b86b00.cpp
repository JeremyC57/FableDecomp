// CEngineScreenEffectDisplacementRenderer::SetEffectQuality(const CEngineVideoQualitySettings&) -- sets the 2D/3D feature flags, initializes textures if either is requested and supported, otherwise frees its scratch page; retail 0x00B86B00 length 0xa3. Uses shared renderer layout and the video-quality twin's four-byte option prefix; callees 0x00B86A70 and 0x00F37030.
#include "engine/CEngineScreenEffectDisplacementRenderer.h"
struct CEngineVideoQualitySettings { unsigned char _pad[0x6c]; unsigned char Enable2D; unsigned char Enable3D; };
struct CGPUScratchBufferTextureManager { void FreeMemoryPage(long); /* 0x00F37030 */ };
struct CEngineGlobal_01436e98 { unsigned char _pad[0x3190]; CGPUScratchBufferTextureManager scratch; };
extern CEngineGlobal_01436e98 *g_pEngine_01436e98; // 0x01436E98
struct CEngineScreenEffectDisplacementRenderer_Quality : CEngineScreenEffectDisplacementRenderer {
    bool InitialiseTextures(); // 0x00B86A70
    void FreeTexturePage() { if (DynamicTexturePageHandle >= 0) g_pEngine_01436e98->scratch.FreeMemoryPage(DynamicTexturePageHandle); }
    bool SetEffectQuality(const CEngineVideoQualitySettings&);
};
bool CEngineScreenEffectDisplacementRenderer_Quality::SetEffectQuality(const CEngineVideoQualitySettings& quality)
{
    unsigned char *bytes = reinterpret_cast<unsigned char *>(this);
    unsigned char supported = bytes[0xed];
    int enabled2D;
    if (supported) {
        if (quality.Enable2D) { enabled2D = 1; goto store2D; }
    }
    enabled2D = 0;
store2D:
    bytes[0xee] = enabled2D;
    int enabled3D;
    if (supported) {
        if (quality.Enable3D) { enabled3D = 1; goto store3D; }
    }
    enabled3D = 0;
store3D:
    bytes[0xef] = enabled3D;
    if (!Enable2DDisplacement && !Enable3DDisplacement) {
        FreeTexturePage();
        _pad_0x08[0] = false;
        return true;
    }
    if (InitialiseTextures()) {
        _pad_0x08[0] = true;
        return true;
    }
    FreeTexturePage();
    _pad_0x08[0] = false;
    return false;
}