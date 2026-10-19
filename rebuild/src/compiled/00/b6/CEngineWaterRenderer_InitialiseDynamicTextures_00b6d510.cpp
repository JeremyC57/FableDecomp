// CEngineWaterRenderer::InitialiseDynamicTextures -- allocate shared scratch page and four texture handles.
// Retail 00b6d510,235 bytes; PDB shared CEngineWaterRenderer has handles +0xc later and incorrect pointer labels.
// Native handles page1d8, environment1dc,sea1e4,water1ec,ice1f4; manager global1436e98+3190.
#ifdef _MSC_VER
#include "engine/CEngineWaterRenderer.h"
#include "engine/CEngineResourceManager.h"
#include "engine/CPixelFormat.h"
#endif
#ifndef _MSC_VER
struct CPixelFormat {long FormatIndex;};
#endif
struct PixelFormatMethods:CPixelFormat {void Initialise(int);}; //009e3830 D3DFORMAT enum passed as32bit int
struct C2DExtentsI { long X,Y; C2DExtentsI(long x,long y):X(x),Y(y){} };
struct CGPUScratchBufferTextureManager {
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
 PixelFormatMethods format;format.Initialise(21);
 CGPUScratchBufferTextureManager& manager=gResourceManager->ScratchTextures;
 DynamicTexturePageHandle=manager.AllocteMemoryPage(0,true);
 WaterBumpMapHandle=manager.AllocateTexture(DynamicTexturePageHandle,C2DExtentsI(128,128),3,format,1,false);
 IceBumpMapHandle=manager.AllocateTexture(DynamicTexturePageHandle,C2DExtentsI(128,128),3,format,1,false);
 SeaBumpMapHandle=manager.AllocateTexture(DynamicTexturePageHandle,C2DExtentsI(128,128),3,format,1,false);
 EnvironmentMapHandle=manager.AllocateTexture(DynamicTexturePageHandle,C2DExtentsI(256,256),1,format,1,false);
}