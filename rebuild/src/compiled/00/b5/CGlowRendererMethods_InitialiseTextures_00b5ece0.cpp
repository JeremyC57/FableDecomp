// CGlowRendererMethods::InitialiseTextures() -- allocates four glow render targets on a new scratch page.
// Leaf: ?InitialiseTextures@CGlowRendererMethods@@QAEHXZ
// Naming wave 25: CPixelFormat::Initialise 0x009E3830 is ?Initialise@CPixelFormat@@QAEXW4_D3DFORMAT@@@Z (Ego_r.pdb; no constructor:
//   the body returns nothing), the format a _D3DFORMAT; a CPixelFormat built from a format is the inline constructor calling it.
// Retail00b5ece0 length286. Actual glow header offsets agree. Resource-manager donor
// GPU manager+40e4 differs from retail+3190, represented as a documented native view.
// NF46 (round 7): the 0x01436E98 stand-in CGlowResourceManager / gGlowResources (+0x3190 Scratch) and the CEngineResourceManager renaming
//   macro are NEngineInternal::GPResourceManager (?GPResourceManager@NEngineInternal@@3PAVCEngineResourceManager@@A) on
//   engine/CEngineResourceManager.h (retail branch: +0x3190) and its GetGPUScratchBufferTextureManager() (engine/CGPUScratchBufferTextureManager.h,
//   opt-ins _TEXTURES / _FREEMEMORYPAGE, the idiom of landed 0x00B2CB40). The local CGPUScratchBufferTextureManager / CGlowScratchManager
//   (their 0x00F37660 / 0x00F37440 callees now the Ego_r.pdb AllocteMemoryPage (sic) / AllocateTexture), CGlowExtents (C2DExtentsI), the
//   CPixelFormat renaming macro + local CPixelFormat and _D3DFORMAT moved to their headers (FABLE_RETAIL_CPIXELFORMAT). The derived
//   CGlowRendererMethods shell (self) is unchanged: the PDB self is CEngineScreenEffectGlowRenderer::InitialiseTextures (name debt, NOTES.md).
#define FABLE_RETAIL_CPIXELFORMAT
#define FABLE_RETAIL_CENGINERESOURCEMANAGER
#define FABLE_RETAIL_CGPUSCRATCHBUFFERTEXTUREMANAGER_TEXTURES
#define FABLE_RETAIL_CGPUSCRATCHBUFFERTEXTUREMANAGER_FREEMEMORYPAGE
#include "engine/CEngineScreenEffectGlowRenderer.h"
#include "engine/CPixelFormat.h"
#include "engine/C2DExtentsI.h"
#include "engine/CEngineResourceManager.h"
#include "engine/CGPUScratchBufferTextureManager.h"
namespace NEngineInternal { extern CEngineResourceManager* GPResourceManager; }   // 0x01436E98
struct CGlowRendererMethods:CEngineScreenEffectGlowRenderer {int InitialiseTextures();};
int CGlowRendererMethods::InitialiseTextures(){
 if(DynamicTexturePageHandle>=0)NEngineInternal::GPResourceManager->GetGPUScratchBufferTextureManager().FreeMemoryPage(DynamicTexturePageHandle);
 CPixelFormat format;format.Initialise(D3DFMT_A8R8G8B8);
 CGPUScratchBufferTextureManager* manager=&NEngineInternal::GPResourceManager->GetGPUScratchBufferTextureManager();
 DynamicTexturePageHandle=manager->AllocteMemoryPage(0,false);
 C2DExtentsI extents(256,256);
 GlowTexture256Handle=manager->AllocateTexture(DynamicTexturePageHandle,extents,1,format,1,false);
 extents.X=64;extents.Y=64;
 GlowTexture64Handle=manager->AllocateTexture(DynamicTexturePageHandle,extents,1,format,1,false);
 extents.X=64;extents.Y=64;
 GlowTexture64AltHandle=manager->AllocateTexture(DynamicTexturePageHandle,extents,1,format,1,false);
 extents.X=16;extents.Y=16;
 GlowTexture16Handle=manager->AllocateTexture(DynamicTexturePageHandle,extents,1,format,1,false);
 return GlowTexture256Handle>=0&&GlowTexture64Handle>=0&&GlowTexture64AltHandle>=0&&GlowTexture16Handle>=0?1:0;
}
