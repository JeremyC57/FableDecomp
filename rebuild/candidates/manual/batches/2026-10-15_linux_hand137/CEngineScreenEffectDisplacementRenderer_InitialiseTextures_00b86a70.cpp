// CEngineScreenEffectDisplacementRenderer::InitialiseTextures() -- (re)allocate the effect's dynamic texture page and its 256x256 displacement texture (pixel format 0x15) from the engine's GPU scratch texture manager; true when the texture was allocated
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B86A70, length 141 bytes (+ int3 padding).
// Callees: CGPUScratchBufferTextureManager::FreeMemoryPage @ 0x00F37030, AllocateMemoryPage(0, 0) @ 0x00F37660 and
// AllocateTexture(page, &size, 1, &format, 1, 0) @ 0x00F37440 (engine global @ 0x01436E98 + 0x3190), the CPixelFormat
// constructor from a D3D format @ 0x009E3830.
// Layout: engine/CEngineScreenEffectDisplacementRenderer.h -- BackBufferCopyTexture +0x0c, DisplacementTexture +0x10,
// DisplacementTextureHandle +0x14, DynamicTexturePageHandle +0x18. The method is attached through a derived shell.
// Shape: the old page is freed through the inline `if (handle >= 0)` helper; the manager is then re-read into a local
// (edi); the pixel format and the C2DCoordI size are frame locals (`sub esp,0Ch`); the copy-texture clear is an immediate
// scheduled right before the call; `return handle >= 0;` from a bool goes through ecx (`xor ecx,ecx ; setge cl ; mov al,cl`).
#include "engine/CEngineScreenEffectDisplacementRenderer.h"

struct CPixelFormat {
    long FormatIndex;
    CPixelFormat(unsigned long d3dFormat);         // 0x009E3830
};

struct C2DCoordI {
    long X, Y;
    C2DCoordI(long x, long y) : X(x), Y(y) { }
};

struct CGPUScratchBufferTextureManager {
    void FreeMemoryPage(long page);                // 0x00F37030
    long AllocateMemoryPage(long a, long b);       // 0x00F37660
    long AllocateTexture(long page, const C2DCoordI* size, long mipLevels, const CPixelFormat* format, long count, long flags);   // 0x00F37440
};
struct CEngineGlobal_01436e98 {
    unsigned char                   _pad[0x3190];
    CGPUScratchBufferTextureManager m_scratchTextures;   // +0x3190
};
extern CEngineGlobal_01436e98* g_pEngine_01436e98;   // 0x01436E98

struct CEngineScreenEffectDisplacementRenderer_Init : CEngineScreenEffectDisplacementRenderer {
    void FreeTexturePage()
    {
        if (DynamicTexturePageHandle >= 0)
            g_pEngine_01436e98->m_scratchTextures.FreeMemoryPage(DynamicTexturePageHandle);
    }
    bool InitialiseTextures();
};

bool CEngineScreenEffectDisplacementRenderer_Init::InitialiseTextures()
{
    FreeTexturePage();
    CGPUScratchBufferTextureManager* textures = &g_pEngine_01436e98->m_scratchTextures;
    DynamicTexturePageHandle = textures->AllocateMemoryPage(0, 0);
    CPixelFormat format(0x15);
    C2DCoordI size(256, 256);
    BackBufferCopyTexture = 0;
    DisplacementTextureHandle = textures->AllocateTexture(DynamicTexturePageHandle, &size, 1, &format, 1, 0);
    DisplacementTexture = 0;
    return DisplacementTextureHandle >= 0;
}
