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
// NF46 (round 7): the 0x01436E98 stand-in CEngineGlobal_01436e98 / g_pEngine_01436e98 (+0x3190 m_scratchTextures) is
//   NEngineInternal::GPResourceManager (?GPResourceManager@NEngineInternal@@3PAVCEngineResourceManager@@A) on engine/CEngineResourceManager.h
//   (retail branch) and its GetGPUScratchBufferTextureManager() (engine/CGPUScratchBufferTextureManager.h, opt-ins _TEXTURES / _FREEMEMORYPAGE,
//   the idiom of landed 0x00B2CB40). The local scratch-manager class (0x00F37660 / 0x00F37440 callees now the Ego_r.pdb spellings
//   AllocteMemoryPage(long, bool) (sic) / AllocateTexture(long, const C2DExtentsI&, long, const CPixelFormat&, unsigned long, bool)), the local
//   CPixelFormat, C2DCoordI (the PDB parameter is C2DExtentsI) and _D3DFORMAT moved to their headers (FABLE_RETAIL_CPIXELFORMAT). The derived
//   _Init shell (self) is unchanged: the PDB self is ?InitialiseTextures@CEngineScreenEffectDisplacementRenderer@@AAE_NXZ (name debt, NOTES.md).
#define FABLE_RETAIL_CPIXELFORMAT
#define FABLE_RETAIL_CENGINERESOURCEMANAGER
#define FABLE_RETAIL_CGPUSCRATCHBUFFERTEXTUREMANAGER_TEXTURES
#define FABLE_RETAIL_CGPUSCRATCHBUFFERTEXTUREMANAGER_FREEMEMORYPAGE
#include "engine/CEngineScreenEffectDisplacementRenderer.h"
#include "engine/CPixelFormat.h"
#include "engine/C2DExtentsI.h"
#include "engine/CEngineResourceManager.h"
#include "engine/CGPUScratchBufferTextureManager.h"
namespace NEngineInternal { extern CEngineResourceManager* GPResourceManager; }   // 0x01436E98
struct CEngineScreenEffectDisplacementRenderer_Init : CEngineScreenEffectDisplacementRenderer {
    void FreeTexturePage()
    {
        if (DynamicTexturePageHandle >= 0)
            NEngineInternal::GPResourceManager->GetGPUScratchBufferTextureManager().FreeMemoryPage(DynamicTexturePageHandle);
    }
    bool InitialiseTextures();
};
bool CEngineScreenEffectDisplacementRenderer_Init::InitialiseTextures()
{
    FreeTexturePage();
    CGPUScratchBufferTextureManager* textures = &NEngineInternal::GPResourceManager->GetGPUScratchBufferTextureManager();
    DynamicTexturePageHandle = textures->AllocteMemoryPage(0, false);
    CPixelFormat format(D3DFMT_A8R8G8B8);
    C2DExtentsI size(256, 256);
    BackBufferCopyTexture = 0;
    DisplacementTextureHandle = textures->AllocateTexture(DynamicTexturePageHandle, size, 1, format, 1, false);
    DisplacementTexture = 0;
    return DisplacementTextureHandle >= 0;
}
