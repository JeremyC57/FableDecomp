// CEngineWaterRenderer::InitialiseDynamicTextures -- allocate shared scratch page and four texture handles.
// Leaf: ?InitialiseDynamicTextures@CEngineWaterRenderer@@AAEXXZ
// Name-fix NF38: Ego_r.pdb / FableWin.pdb ?InitialiseDynamicTextures@CEngineWaterRenderer@@AAEXXZ (private), the header's
// FABLE_RETAIL_CENGINEWATERRENDERER(_CTOR) opt-in names it for 0x00B6D510 (handles +0x1d8..+0x1f4 as native); was the local stand-in CWaterDynamicTextures.
// Naming wave 25: CPixelFormat::Initialise 0x009E3830 is ?Initialise@CPixelFormat@@QAEXW4_D3DFORMAT@@@Z (Ego_r.pdb; no constructor:
//   the body returns nothing), the format a _D3DFORMAT; a CPixelFormat built from a format is the inline constructor calling it.
// Retail 00b6d510,235 bytes; PDB shared CEngineWaterRenderer has handles +0xc later and incorrect pointer labels.
// Native handles page1d8, environment1dc,sea1e4,water1ec,ice1f4; manager global1436e98+3190.
// NF46 (round 7): the 0x01436E98 stand-in CEngineResourceManagerNative / gResourceManager (+0x3190 ScratchTextures) is
//   NEngineInternal::GPResourceManager (?GPResourceManager@NEngineInternal@@3PAVCEngineResourceManager@@A) on engine/CEngineResourceManager.h
//   (retail branch) and its GetGPUScratchBufferTextureManager() (engine/CGPUScratchBufferTextureManager.h, opt-in _TEXTURES, the idiom of landed
//   0x00B2CB40). The local CGPUScratchBufferTextureManager, the CPixelFormat renaming macro + local CPixelFormat, the local C2DExtentsI and
//   _D3DFORMAT moved to their headers (FABLE_RETAIL_CPIXELFORMAT, engine/C2DExtentsI.h, engine/Direct3D9Types.h); the _MSC_VER-less twin dropped.
//   Retail builds the format with no -1 store (`push 15h ; lea ecx ; call 0x009E3830`): the header CPixelFormat(_D3DFORMAT) constructor.
#define FABLE_RETAIL_CENGINEWATERRENDERER
#define FABLE_RETAIL_CENGINEWATERRENDERER_CTOR
#define FABLE_RETAIL_CPIXELFORMAT
#define FABLE_RETAIL_CENGINERESOURCEMANAGER
#define FABLE_RETAIL_CGPUSCRATCHBUFFERTEXTUREMANAGER_TEXTURES
#include "engine/CEngineWaterRenderer.h"
#include "engine/CPixelFormat.h"
#include "engine/C2DExtentsI.h"
#include "engine/CEngineResourceManager.h"
#include "engine/CGPUScratchBufferTextureManager.h"
namespace NEngineInternal { extern CEngineResourceManager* GPResourceManager; }   // 0x01436E98
void CEngineWaterRenderer::InitialiseDynamicTextures(){
 CPixelFormat format(D3DFMT_A8R8G8B8);
 CGPUScratchBufferTextureManager& manager=NEngineInternal::GPResourceManager->GetGPUScratchBufferTextureManager();
 DynamicTexturePageHandle=manager.AllocteMemoryPage(0,true);
 WaterBumpMapHandle=manager.AllocateTexture(DynamicTexturePageHandle,C2DExtentsI(128,128),3,format,1,false);
 IceBumpMapHandle=manager.AllocateTexture(DynamicTexturePageHandle,C2DExtentsI(128,128),3,format,1,false);
 SeaBumpMapHandle=manager.AllocateTexture(DynamicTexturePageHandle,C2DExtentsI(128,128),3,format,1,false);
 EnvironmentMapHandle=manager.AllocateTexture(DynamicTexturePageHandle,C2DExtentsI(256,256),1,format,1,false);
}
