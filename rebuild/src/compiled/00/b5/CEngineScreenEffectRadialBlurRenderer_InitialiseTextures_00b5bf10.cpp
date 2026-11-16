// CEngineScreenEffectRadialBlurRenderer::InitialiseTextures() -- replace scratch page and allocate two 256-square textures.
// Leaf: ?InitialiseTextures@RadialBlurMethods@@QAEJXZ
// Naming wave 25: CPixelFormat::Initialise 0x009E3830 is ?Initialise@CPixelFormat@@QAEXW4_D3DFORMAT@@@Z (Ego_r.pdb; no constructor:
//   the body returns nothing), the format a _D3DFORMAT; a CPixelFormat built from a format is the inline constructor calling it.
// Retail 00B5BF10,185 bytes. Shared header matches retail handle offsets14/18/1c.
// Calls F37030 FreeMemoryPage,F37660 AllocteMemoryPage,F37440 AllocateTexture; global1436e98 pool3190.
// NF46 (round 7): the 0x01436E98 stand-in EngineTextureGlobal / gEngine (+0x3190 Scratch) is NEngineInternal::GPResourceManager
//   (?GPResourceManager@NEngineInternal@@3PAVCEngineResourceManager@@A) on engine/CEngineResourceManager.h and its
//   GetGPUScratchBufferTextureManager() (engine/CGPUScratchBufferTextureManager.h, opt-ins _TEXTURES / _FREEMEMORYPAGE: the Ego_r.pdb
//   spellings AllocteMemoryPage (sic) / AllocateTexture / FreeMemoryPage, the idiom of landed 0x00B2CB40). The local scratch-manager
//   class, the CPixelFormat renaming macro + local CPixelFormat, the local C2DExtentsI and _D3DFORMAT moved to engine/CPixelFormat.h
//   (FABLE_RETAIL_CPIXELFORMAT), engine/C2DExtentsI.h and engine/Direct3D9Types.h. The derived RadialBlurMethods shell (self) is unchanged:
//   the PDB self is ?InitialiseTextures@CEngineScreenEffectRadialBlurRenderer@@AAE_NXZ (name debt, NOTES.md).
#define FABLE_RETAIL_CPIXELFORMAT
#define FABLE_RETAIL_CENGINERESOURCEMANAGER
#define FABLE_RETAIL_CGPUSCRATCHBUFFERTEXTUREMANAGER_TEXTURES
#define FABLE_RETAIL_CGPUSCRATCHBUFFERTEXTUREMANAGER_FREEMEMORYPAGE
#include "engine/CEngineScreenEffectRadialBlurRenderer.h"
#include "engine/CPixelFormat.h"
#include "engine/C2DExtentsI.h"
#include "engine/CEngineResourceManager.h"
#include "engine/CGPUScratchBufferTextureManager.h"
namespace NEngineInternal { extern CEngineResourceManager* GPResourceManager; }   // 0x01436E98
struct RadialBlurMethods : CEngineScreenEffectRadialBlurRenderer { long InitialiseTextures(); };
long RadialBlurMethods::InitialiseTextures() {
 if(DynamicTexturePageHandle>=0) NEngineInternal::GPResourceManager->GetGPUScratchBufferTextureManager().FreeMemoryPage(DynamicTexturePageHandle);
 CPixelFormat format; format.Initialise(D3DFMT_A8R8G8B8);
 CGPUScratchBufferTextureManager* pool=&NEngineInternal::GPResourceManager->GetGPUScratchBufferTextureManager();
 DynamicTexturePageHandle=pool->AllocteMemoryPage(0,false);
 BlurTextureHandle256_1=pool->AllocateTexture(DynamicTexturePageHandle,C2DExtentsI(256,256),1,format,1,false);
 BlurTextureHandle256_2=pool->AllocateTexture(DynamicTexturePageHandle,C2DExtentsI(256,256),1,format,1,false);
 return BlurTextureHandle256_1>=0 && BlurTextureHandle256_2>=0;
}
