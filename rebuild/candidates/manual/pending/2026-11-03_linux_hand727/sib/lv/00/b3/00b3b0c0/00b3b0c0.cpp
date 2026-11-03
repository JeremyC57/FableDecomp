// CEngineResourceManagerNative::UninitialiseShaders -- unbind stream/index/textures, close the populated vertex shader resources and their banks. Retail00b3b0c0 complete189bytes+3int3. Shared CEngineResourceManager/CShaderDataBank differ materially: donor bank1a8/resourcesc versus nativebank128/resources8, nativeVShaderBanks960/resources29c0 versusdonora94/38f4. Localnativeview required. Shared CRenderManagerCore direct exact fields. COM slots65 SetTexture,100 SetStreamSource,104 SetIndices; no virtual COM destructor. ResourceCloseShaderBank00a5d750; bankClose virtualslot5=00a62110 genuine inheriteddtor slot0.
#ifndef _MSC_VER
#define __forceinline inline
#define __stdcall
struct IDirect3DDevice9;struct IDirect3DBaseTexture9;
struct CRenderManagerCore {long TextureStagesInUse;IDirect3DDevice9* PD3DDevice;IDirect3DBaseTexture9* CurrentlySelectedTextures[8];};
#else
#include "engine/CEngineResourceManager.h"
#include "engine/CShaderDataBank.h"
#include "engine/CRenderManagerCore.h"
#endif
struct IDirect3DDevice9 {
 virtual void __stdcall ReservedSlot0()=0;
 virtual void __stdcall ReservedSlot1()=0;
 virtual void __stdcall ReservedSlot2()=0;
 virtual void __stdcall ReservedSlot3()=0;
 virtual void __stdcall ReservedSlot4()=0;
 virtual void __stdcall ReservedSlot5()=0;
 virtual void __stdcall ReservedSlot6()=0;
 virtual void __stdcall ReservedSlot7()=0;
 virtual void __stdcall ReservedSlot8()=0;
 virtual void __stdcall ReservedSlot9()=0;
 virtual void __stdcall ReservedSlot10()=0;
 virtual void __stdcall ReservedSlot11()=0;
 virtual void __stdcall ReservedSlot12()=0;
 virtual void __stdcall ReservedSlot13()=0;
 virtual void __stdcall ReservedSlot14()=0;
 virtual void __stdcall ReservedSlot15()=0;
 virtual void __stdcall ReservedSlot16()=0;
 virtual void __stdcall ReservedSlot17()=0;
 virtual void __stdcall ReservedSlot18()=0;
 virtual void __stdcall ReservedSlot19()=0;
 virtual void __stdcall ReservedSlot20()=0;
 virtual void __stdcall ReservedSlot21()=0;
 virtual void __stdcall ReservedSlot22()=0;
 virtual void __stdcall ReservedSlot23()=0;
 virtual void __stdcall ReservedSlot24()=0;
 virtual void __stdcall ReservedSlot25()=0;
 virtual void __stdcall ReservedSlot26()=0;
 virtual void __stdcall ReservedSlot27()=0;
 virtual void __stdcall ReservedSlot28()=0;
 virtual void __stdcall ReservedSlot29()=0;
 virtual void __stdcall ReservedSlot30()=0;
 virtual void __stdcall ReservedSlot31()=0;
 virtual void __stdcall ReservedSlot32()=0;
 virtual void __stdcall ReservedSlot33()=0;
 virtual void __stdcall ReservedSlot34()=0;
 virtual void __stdcall ReservedSlot35()=0;
 virtual void __stdcall ReservedSlot36()=0;
 virtual void __stdcall ReservedSlot37()=0;
 virtual void __stdcall ReservedSlot38()=0;
 virtual void __stdcall ReservedSlot39()=0;
 virtual void __stdcall ReservedSlot40()=0;
 virtual void __stdcall ReservedSlot41()=0;
 virtual void __stdcall ReservedSlot42()=0;
 virtual void __stdcall ReservedSlot43()=0;
 virtual void __stdcall ReservedSlot44()=0;
 virtual void __stdcall ReservedSlot45()=0;
 virtual void __stdcall ReservedSlot46()=0;
 virtual void __stdcall ReservedSlot47()=0;
 virtual void __stdcall ReservedSlot48()=0;
 virtual void __stdcall ReservedSlot49()=0;
 virtual void __stdcall ReservedSlot50()=0;
 virtual void __stdcall ReservedSlot51()=0;
 virtual void __stdcall ReservedSlot52()=0;
 virtual void __stdcall ReservedSlot53()=0;
 virtual void __stdcall ReservedSlot54()=0;
 virtual void __stdcall ReservedSlot55()=0;
 virtual void __stdcall ReservedSlot56()=0;
 virtual void __stdcall ReservedSlot57()=0;
 virtual void __stdcall ReservedSlot58()=0;
 virtual void __stdcall ReservedSlot59()=0;
 virtual void __stdcall ReservedSlot60()=0;
 virtual void __stdcall ReservedSlot61()=0;
 virtual void __stdcall ReservedSlot62()=0;
 virtual void __stdcall ReservedSlot63()=0;
 virtual void __stdcall ReservedSlot64()=0;
 virtual long __stdcall SetTexture(unsigned long,IDirect3DBaseTexture9*)=0;
 virtual void __stdcall ReservedSlot66()=0;
 virtual void __stdcall ReservedSlot67()=0;
 virtual void __stdcall ReservedSlot68()=0;
 virtual void __stdcall ReservedSlot69()=0;
 virtual void __stdcall ReservedSlot70()=0;
 virtual void __stdcall ReservedSlot71()=0;
 virtual void __stdcall ReservedSlot72()=0;
 virtual void __stdcall ReservedSlot73()=0;
 virtual void __stdcall ReservedSlot74()=0;
 virtual void __stdcall ReservedSlot75()=0;
 virtual void __stdcall ReservedSlot76()=0;
 virtual void __stdcall ReservedSlot77()=0;
 virtual void __stdcall ReservedSlot78()=0;
 virtual void __stdcall ReservedSlot79()=0;
 virtual void __stdcall ReservedSlot80()=0;
 virtual void __stdcall ReservedSlot81()=0;
 virtual void __stdcall ReservedSlot82()=0;
 virtual void __stdcall ReservedSlot83()=0;
 virtual void __stdcall ReservedSlot84()=0;
 virtual void __stdcall ReservedSlot85()=0;
 virtual void __stdcall ReservedSlot86()=0;
 virtual void __stdcall ReservedSlot87()=0;
 virtual void __stdcall ReservedSlot88()=0;
 virtual void __stdcall ReservedSlot89()=0;
 virtual void __stdcall ReservedSlot90()=0;
 virtual void __stdcall ReservedSlot91()=0;
 virtual void __stdcall ReservedSlot92()=0;
 virtual void __stdcall ReservedSlot93()=0;
 virtual void __stdcall ReservedSlot94()=0;
 virtual void __stdcall ReservedSlot95()=0;
 virtual void __stdcall ReservedSlot96()=0;
 virtual void __stdcall ReservedSlot97()=0;
 virtual void __stdcall ReservedSlot98()=0;
 virtual void __stdcall ReservedSlot99()=0;
 virtual long __stdcall SetStreamSource(unsigned long,void*,unsigned long,unsigned long)=0;
 virtual void __stdcall ReservedSlot101()=0;
 virtual void __stdcall ReservedSlot102()=0;
 virtual void __stdcall ReservedSlot103()=0;
 virtual long __stdcall SetIndices(void*)=0;
};
class CShaderResource { public:void CloseShaderBank();}; //00a5d750
struct CShaderDataBankNative {
 virtual ~CShaderDataBankNative();
 virtual void ReservedSlot1();virtual void ReservedSlot2();virtual void ReservedSlot3();virtual void ReservedSlot4();virtual void Close(); //00a62110
 unsigned char NativeMembers[0x124];
};
struct CShaderResourcePointer {CShaderResource* Data;void* Info;};
struct CEngineResourceManagerNative {
 unsigned char EarlierMembers[0x960];CShaderDataBankNative VShaderBanks[27];unsigned char PixelShaderAndOtherData[0x128];CShaderResourcePointer VShaderResources[27];
 void UninitialiseShaders();
};
extern CRenderManagerCore* g_pRenderManagerCore; //01436e18
static __forceinline void UnbindBuffersAndTextures(CRenderManagerCore* core)
{
 IDirect3DDevice9*& device=core->PD3DDevice;
 device->SetStreamSource(0,0,0,0);
 device->SetIndices(0);
 for(long stage=0;stage<core->TextureStagesInUse;++stage){if(core->CurrentlySelectedTextures[stage]){core->CurrentlySelectedTextures[stage]=0;device->SetTexture(stage,0);}}
 core->TextureStagesInUse=0;
}
void CEngineResourceManagerNative::UninitialiseShaders()
{
 UnbindBuffersAndTextures(g_pRenderManagerCore);
 for(long i=0;i<27;++i){if(VShaderResources[i].Data){VShaderResources[i].Data->CloseShaderBank();VShaderBanks[i].Close();}}
}