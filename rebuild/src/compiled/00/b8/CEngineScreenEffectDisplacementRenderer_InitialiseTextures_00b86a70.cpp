// CEngineScreenEffectDisplacementRenderer::InitialiseTextures() -- (re)allocate the effect's dynamic texture page and its 256x256 displacement texture (pixel format 0x15) from the engine's GPU scratch texture manager; true when the texture was allocated
// Leaf: ?InitialiseTextures@CEngineScreenEffectDisplacementRenderer_Init@@QAE_NXZ
// Naming wave 25: CPixelFormat::Initialise 0x009E3830 is ?Initialise@CPixelFormat@@QAEXW4_D3DFORMAT@@@Z (Ego_r.pdb; no constructor:
//   the body returns nothing), the format a _D3DFORMAT; a CPixelFormat built from a format is the inline constructor calling it.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B86A70, length 141 bytes (+ int3 padding).
// Callees: CGPUScratchBufferTextureManager::FreeMemoryPage @ 0x00F37030, AllocateMemoryPage(0, 0) @ 0x00F37660 and
// AllocateTexture(page, &size, 1, &format, 1, 0) @ 0x00F37440 (engine global @ 0x01436E98 + 0x3190), the CPixelFormat
// constructor from a D3D format @ 0x009E3830.
// Layout: engine/CEngineScreenEffectDisplacementRenderer.h -- BackBufferCopyTexture +0x0c, DisplacementTexture +0x10,
// DisplacementTextureHandle +0x14, DynamicTexturePageHandle +0x18. The method is attached through a derived shell.
// Shape: the old page is freed through the inline `if (handle >= 0)` helper; the manager is then re-read into a local
// (edi); the pixel format and the C2DCoordI size are frame locals (`sub esp,0Ch`); the copy-texture clear is an immediate
// scheduled right before the call; `return handle >= 0;` from a bool goes through ecx (`xor ecx,ecx ; setge cl ; mov al,cl`).
enum _D3DFORMAT { D3DFMT_UNKNOWN = 0, D3DFMT_A8R8G8B8 = 21, D3DFMT_D24S8 = 75, D3DFMT_DXT1 = 0x31545844, D3DFMT_DXT3 = 0x33545844, D3DFMT_DXT5 = 0x35545844, D3DFMT_FORCE_DWORD = 0x7fffffff };   // naming wave 25: the D3D format enum (Initialise takes it)
#include "engine/CEngineScreenEffectDisplacementRenderer.h"

class CPixelFormat { public:
    long FormatIndex;
    CPixelFormat(_D3DFORMAT d3dFormat) { Initialise(d3dFormat); }
    void Initialise(_D3DFORMAT d3dFormat);         // 0x009E3830
};

class C2DCoordI { public:
    long X, Y;
    C2DCoordI(long x, long y) : X(x), Y(y) { }
};

class CGPUScratchBufferTextureManager { public:
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
    CPixelFormat format(D3DFMT_A8R8G8B8);
    C2DCoordI size(256, 256);
    BackBufferCopyTexture = 0;
    DisplacementTextureHandle = textures->AllocateTexture(DynamicTexturePageHandle, &size, 1, &format, 1, 0);
    DisplacementTexture = 0;
    return DisplacementTextureHandle >= 0;
}