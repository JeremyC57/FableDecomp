// CEngineWaterRenderer::InitialiseDynamicTextures -- allocate shared scratch page and four texture handles.
// Leaf: ?InitialiseDynamicTextures@CWaterDynamicTextures@@QAEXXZ
// Naming wave 25: CPixelFormat::Initialise 0x009E3830 is ?Initialise@CPixelFormat@@QAEXW4_D3DFORMAT@@@Z (Ego_r.pdb; no constructor:
//   the body returns nothing), the format a _D3DFORMAT; a CPixelFormat built from a format is the inline constructor calling it.
// Retail 00b6d510,235 bytes; PDB shared CEngineWaterRenderer has handles +0xc later and incorrect pointer labels.
// Native handles page1d8, environment1dc,sea1e4,water1ec,ice1f4; manager global1436e98+3190.
enum _D3DFORMAT { D3DFMT_UNKNOWN = 0, D3DFMT_A8R8G8B8 = 21, D3DFMT_D24S8 = 75, D3DFMT_DXT1 = 0x31545844, D3DFMT_DXT3 = 0x33545844, D3DFMT_DXT5 = 0x35545844, D3DFMT_FORCE_DWORD = 0x7fffffff };   // naming wave 25: the D3D format enum (Initialise takes it)
#ifdef _MSC_VER
#include "engine/CEngineWaterRenderer.h"
#include "engine/CEngineResourceManager.h"
#define CPixelFormat CPixelFormatLayout
#include "engine/CPixelFormat.h"
#undef CPixelFormat
#endif
#ifndef _MSC_VER
class CPixelFormat { public:long FormatIndex;};
#endif
#ifdef _MSC_VER
class CPixelFormat : public CPixelFormatLayout { public: void Initialise(_D3DFORMAT); }; //009e3830 (was a PixelFormatMethods stand-in)
#endif
class C2DExtentsI { public: long X,Y; C2DExtentsI(long x,long y):X(x),Y(y){} };
class CGPUScratchBufferTextureManager { public:
 long AllocteMemoryPage(long,bool); // 00f37660
 long AllocateTexture(long,const C2DExtentsI&,long,const CPixelFormat&,unsigned long,bool); // 00f37440
};
// Resource manager donor scratch offset40e4 differs from native3190.
struct CEngineResourceManagerNative { unsigned char OtherManagers[0x3190]; CGPUScratchBufferTextureManager ScratchTextures; };
extern CEngineResourceManagerNative* gResourceManager; // 01436e98
struct CWaterDynamicTextures {
 unsigned char Shaders[0x1d8];long DynamicTexturePageHandle,EnvironmentMapHandle; void* EnvironmentMap; long SeaBumpMapHandle; void* SeaBumpMap; long WaterBumpMapHandle; void* WaterBumpMap; long IceBumpMapHandle;
 void InitialiseDynamicTextures();
};
void CWaterDynamicTextures::InitialiseDynamicTextures(){
 CPixelFormat format;format.Initialise(D3DFMT_A8R8G8B8);
 CGPUScratchBufferTextureManager& manager=gResourceManager->ScratchTextures;
 DynamicTexturePageHandle=manager.AllocteMemoryPage(0,true);
 WaterBumpMapHandle=manager.AllocateTexture(DynamicTexturePageHandle,C2DExtentsI(128,128),3,format,1,false);
 IceBumpMapHandle=manager.AllocateTexture(DynamicTexturePageHandle,C2DExtentsI(128,128),3,format,1,false);
 SeaBumpMapHandle=manager.AllocateTexture(DynamicTexturePageHandle,C2DExtentsI(128,128),3,format,1,false);
 EnvironmentMapHandle=manager.AllocateTexture(DynamicTexturePageHandle,C2DExtentsI(256,256),1,format,1,false);
}