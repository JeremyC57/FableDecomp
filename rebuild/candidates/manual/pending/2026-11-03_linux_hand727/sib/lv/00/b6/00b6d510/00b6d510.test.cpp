// CEngineWaterRenderer::InitialiseDynamicTextures -- allocate shared scratch page and four texture handles.
// Retail 00b6d510,235 bytes; PDB shared CEngineWaterRenderer has handles +0xc later and incorrect pointer labels.
// Native handles page1d8, environment1dc,sea1e4,water1ec,ice1f4; manager global1436e98+3190.
#ifdef _MSC_VER
#include "engine/CEngineWaterRenderer.h"
#include "engine/CEngineResourceManager.h"
#include "engine/CPixelFormat.h"
#endif
#ifndef _MSC_VER
class CPixelFormat { public:long FormatIndex;};
#endif
struct PixelFormatMethods:CPixelFormat {void Initialise(int);}; //009e3830 D3DFORMAT enum passed as32bit int
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
 PixelFormatMethods format;format.Initialise(21);
 CGPUScratchBufferTextureManager& manager=gResourceManager->ScratchTextures;
 DynamicTexturePageHandle=manager.AllocteMemoryPage(0,true);
 WaterBumpMapHandle=manager.AllocateTexture(DynamicTexturePageHandle,C2DExtentsI(128,128),3,format,1,false);
 IceBumpMapHandle=manager.AllocateTexture(DynamicTexturePageHandle,C2DExtentsI(128,128),3,format,1,false);
 SeaBumpMapHandle=manager.AllocateTexture(DynamicTexturePageHandle,C2DExtentsI(128,128),3,format,1,false);
 EnvironmentMapHandle=manager.AllocateTexture(DynamicTexturePageHandle,C2DExtentsI(256,256),1,format,1,false);
}

#include <cstdio>
#include <cstring>
#include <cstddef>
static int calls;static bool ok=true,mutate;static CGPUScratchBufferTextureManager* expected;static CWaterDynamicTextures* active;
CEngineResourceManagerNative resourceManager,other; CEngineResourceManagerNative* gResourceManager=&resourceManager;
void PixelFormatMethods::Initialise(int f){ok=ok&&f==21;FormatIndex=123;}
long CGPUScratchBufferTextureManager::AllocteMemoryPage(long n,bool flag){ok=ok&&this==expected&&n==0&&flag&&calls++==0;if(mutate)gResourceManager=&other;return 42;}
long CGPUScratchBufferTextureManager::AllocateTexture(long page,const C2DExtentsI& e,long type,const CPixelFormat& f,unsigned long usage,bool flag){
 ok=ok&&this==expected&&page==(mutate&&calls>1?60+calls-1:42)&&f.FormatIndex==123&&usage==1&&!flag&&calls>=1&&calls<=4;
 ok=ok&&e.X==(calls==4?256:128)&&e.Y==e.X&&type==(calls==4?1:3);
 ok=ok&&active->WaterBumpMapHandle==(calls>1?101:9)&&active->IceBumpMapHandle==(calls>2?102:9)&&active->SeaBumpMapHandle==(calls>3?103:9)&&active->EnvironmentMapHandle==9;
 if(mutate)active->DynamicTexturePageHandle=60+calls;
 return 100+calls++;
}
int main(){
#ifdef _MSC_VER
 ok=ok&&offsetof(CWaterDynamicTextures,DynamicTexturePageHandle)==0x1d8&&offsetof(CWaterDynamicTextures,EnvironmentMapHandle)==0x1dc&&offsetof(CWaterDynamicTextures,SeaBumpMapHandle)==0x1e4&&offsetof(CWaterDynamicTextures,WaterBumpMapHandle)==0x1ec&&offsetof(CWaterDynamicTextures,IceBumpMapHandle)==0x1f4;
#endif
for(int run=0;run<2;++run){CWaterDynamicTextures r;active=&r;std::memset(&r,0x5a,sizeof(r));unsigned char oldBytes[sizeof(r)];std::memcpy(oldBytes,&r,sizeof(r));
 r.WaterBumpMapHandle=r.IceBumpMapHandle=r.SeaBumpMapHandle=r.EnvironmentMapHandle=9;calls=0;mutate=run!=0;gResourceManager=&resourceManager;expected=&resourceManager.ScratchTextures;r.InitialiseDynamicTextures();
 ok=ok&&calls==5&&r.DynamicTexturePageHandle==(mutate?64:42)&&r.WaterBumpMapHandle==101&&r.IceBumpMapHandle==102&&r.SeaBumpMapHandle==103&&r.EnvironmentMapHandle==104;
 CWaterDynamicTextures before;std::memcpy(&before,oldBytes,sizeof(before));before.DynamicTexturePageHandle=r.DynamicTexturePageHandle;before.WaterBumpMapHandle=101;before.IceBumpMapHandle=102;before.SeaBumpMapHandle=103;before.EnvironmentMapHandle=104;ok=ok&&std::memcmp(&r,&before,sizeof(r))==0;}
 std::printf("00b6d510_TEST %s\n",ok?"PASS":"FAIL");return ok?0:1;}