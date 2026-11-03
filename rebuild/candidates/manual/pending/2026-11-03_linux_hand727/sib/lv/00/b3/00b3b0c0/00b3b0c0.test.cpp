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

#include <cstdio>
#include <stddef.h>
static int failures=0,events[100],eventCount=0,textureCalls=0,resourceCalls=0,bankCalls=0;static bool swapDevice=false,growCount=false,injectResource=false;
static CEngineResourceManagerNative* manager=0;static CShaderResource* resourceArray=0;
static void check(bool b){if(!b)++failures;}static void event(int n){events[eventCount++]=n;}
CRenderManagerCore* g_pRenderManagerCore=0;
struct TestDevice:IDirect3DDevice9 {
 int ID;
 void __stdcall ReservedSlot0(){check(false);}
 void __stdcall ReservedSlot1(){check(false);}
 void __stdcall ReservedSlot2(){check(false);}
 void __stdcall ReservedSlot3(){check(false);}
 void __stdcall ReservedSlot4(){check(false);}
 void __stdcall ReservedSlot5(){check(false);}
 void __stdcall ReservedSlot6(){check(false);}
 void __stdcall ReservedSlot7(){check(false);}
 void __stdcall ReservedSlot8(){check(false);}
 void __stdcall ReservedSlot9(){check(false);}
 void __stdcall ReservedSlot10(){check(false);}
 void __stdcall ReservedSlot11(){check(false);}
 void __stdcall ReservedSlot12(){check(false);}
 void __stdcall ReservedSlot13(){check(false);}
 void __stdcall ReservedSlot14(){check(false);}
 void __stdcall ReservedSlot15(){check(false);}
 void __stdcall ReservedSlot16(){check(false);}
 void __stdcall ReservedSlot17(){check(false);}
 void __stdcall ReservedSlot18(){check(false);}
 void __stdcall ReservedSlot19(){check(false);}
 void __stdcall ReservedSlot20(){check(false);}
 void __stdcall ReservedSlot21(){check(false);}
 void __stdcall ReservedSlot22(){check(false);}
 void __stdcall ReservedSlot23(){check(false);}
 void __stdcall ReservedSlot24(){check(false);}
 void __stdcall ReservedSlot25(){check(false);}
 void __stdcall ReservedSlot26(){check(false);}
 void __stdcall ReservedSlot27(){check(false);}
 void __stdcall ReservedSlot28(){check(false);}
 void __stdcall ReservedSlot29(){check(false);}
 void __stdcall ReservedSlot30(){check(false);}
 void __stdcall ReservedSlot31(){check(false);}
 void __stdcall ReservedSlot32(){check(false);}
 void __stdcall ReservedSlot33(){check(false);}
 void __stdcall ReservedSlot34(){check(false);}
 void __stdcall ReservedSlot35(){check(false);}
 void __stdcall ReservedSlot36(){check(false);}
 void __stdcall ReservedSlot37(){check(false);}
 void __stdcall ReservedSlot38(){check(false);}
 void __stdcall ReservedSlot39(){check(false);}
 void __stdcall ReservedSlot40(){check(false);}
 void __stdcall ReservedSlot41(){check(false);}
 void __stdcall ReservedSlot42(){check(false);}
 void __stdcall ReservedSlot43(){check(false);}
 void __stdcall ReservedSlot44(){check(false);}
 void __stdcall ReservedSlot45(){check(false);}
 void __stdcall ReservedSlot46(){check(false);}
 void __stdcall ReservedSlot47(){check(false);}
 void __stdcall ReservedSlot48(){check(false);}
 void __stdcall ReservedSlot49(){check(false);}
 void __stdcall ReservedSlot50(){check(false);}
 void __stdcall ReservedSlot51(){check(false);}
 void __stdcall ReservedSlot52(){check(false);}
 void __stdcall ReservedSlot53(){check(false);}
 void __stdcall ReservedSlot54(){check(false);}
 void __stdcall ReservedSlot55(){check(false);}
 void __stdcall ReservedSlot56(){check(false);}
 void __stdcall ReservedSlot57(){check(false);}
 void __stdcall ReservedSlot58(){check(false);}
 void __stdcall ReservedSlot59(){check(false);}
 void __stdcall ReservedSlot60(){check(false);}
 void __stdcall ReservedSlot61(){check(false);}
 void __stdcall ReservedSlot62(){check(false);}
 void __stdcall ReservedSlot63(){check(false);}
 void __stdcall ReservedSlot64(){check(false);}
 long __stdcall SetTexture(unsigned long stage,IDirect3DBaseTexture9* texture);
 void __stdcall ReservedSlot66(){check(false);}
 void __stdcall ReservedSlot67(){check(false);}
 void __stdcall ReservedSlot68(){check(false);}
 void __stdcall ReservedSlot69(){check(false);}
 void __stdcall ReservedSlot70(){check(false);}
 void __stdcall ReservedSlot71(){check(false);}
 void __stdcall ReservedSlot72(){check(false);}
 void __stdcall ReservedSlot73(){check(false);}
 void __stdcall ReservedSlot74(){check(false);}
 void __stdcall ReservedSlot75(){check(false);}
 void __stdcall ReservedSlot76(){check(false);}
 void __stdcall ReservedSlot77(){check(false);}
 void __stdcall ReservedSlot78(){check(false);}
 void __stdcall ReservedSlot79(){check(false);}
 void __stdcall ReservedSlot80(){check(false);}
 void __stdcall ReservedSlot81(){check(false);}
 void __stdcall ReservedSlot82(){check(false);}
 void __stdcall ReservedSlot83(){check(false);}
 void __stdcall ReservedSlot84(){check(false);}
 void __stdcall ReservedSlot85(){check(false);}
 void __stdcall ReservedSlot86(){check(false);}
 void __stdcall ReservedSlot87(){check(false);}
 void __stdcall ReservedSlot88(){check(false);}
 void __stdcall ReservedSlot89(){check(false);}
 void __stdcall ReservedSlot90(){check(false);}
 void __stdcall ReservedSlot91(){check(false);}
 void __stdcall ReservedSlot92(){check(false);}
 void __stdcall ReservedSlot93(){check(false);}
 void __stdcall ReservedSlot94(){check(false);}
 void __stdcall ReservedSlot95(){check(false);}
 void __stdcall ReservedSlot96(){check(false);}
 void __stdcall ReservedSlot97(){check(false);}
 void __stdcall ReservedSlot98(){check(false);}
 void __stdcall ReservedSlot99(){check(false);}
 long __stdcall SetStreamSource(unsigned long stream,void* buffer,unsigned long offset,unsigned long stride);
 void __stdcall ReservedSlot101(){check(false);}
 void __stdcall ReservedSlot102(){check(false);}
 void __stdcall ReservedSlot103(){check(false);}
 long __stdcall SetIndices(void* indices);
};
static TestDevice* secondDevice=0;
long __stdcall TestDevice::SetStreamSource(unsigned long stream,void* buffer,unsigned long offset,unsigned long stride){check(eventCount==0 && ID==1 && stream==0 && !buffer && offset==0 && stride==0);event(1);if(swapDevice)g_pRenderManagerCore->PD3DDevice=secondDevice;return -17;}
long __stdcall TestDevice::SetIndices(void* indices){check(eventCount==1 && !indices && ID==(swapDevice?2:1));event(2);return -19;}
long __stdcall TestDevice::SetTexture(unsigned long stage,IDirect3DBaseTexture9* texture){check(eventCount>=2 && !texture && stage<8 && !g_pRenderManagerCore->CurrentlySelectedTextures[stage] && ID==(swapDevice?2:1));++textureCalls;event(10+stage);if(growCount && stage==0)g_pRenderManagerCore->TextureStagesInUse=4;return -23;}
CShaderDataBankNative::~CShaderDataBankNative(){}
void CShaderDataBankNative::ReservedSlot1(){check(false);}void CShaderDataBankNative::ReservedSlot2(){check(false);}void CShaderDataBankNative::ReservedSlot3(){check(false);}void CShaderDataBankNative::ReservedSlot4(){check(false);}
void CShaderDataBankNative::Close(){long index=this-manager->VShaderBanks;check(index>=0 && index<27 && eventCount>2 && events[eventCount-1]==100+index && g_pRenderManagerCore->TextureStagesInUse==0);++bankCalls;event(200+index);}
void CShaderResource::CloseShaderBank(){long index=this-resourceArray;check(index>=0 && index<27 && g_pRenderManagerCore->TextureStagesInUse==0);++resourceCalls;event(100+index);if(injectResource && index==0)manager->VShaderResources[8].Data=resourceArray+8;}
static void run(long stages,bool swap,bool grow,bool inject,bool empty){
 CEngineResourceManagerNative m;CRenderManagerCore core;TestDevice first,second;CShaderResource resources[27];first.ID=1;second.ID=2;secondDevice=&second;
 manager=&m;resourceArray=resources;g_pRenderManagerCore=&core;core.PD3DDevice=&first;core.TextureStagesInUse=stages;
 for(int i=0;i<8;++i)core.CurrentlySelectedTextures[i]=(i==0||i==2||i==3)?reinterpret_cast<IDirect3DBaseTexture9*>(&m):0;
 for(int j=0;j<27;++j){m.VShaderResources[j].Data=(!empty&&(j==0||j==4||j==26))?resources+j:0;m.VShaderResources[j].Info=&core;}
 eventCount=textureCalls=resourceCalls=bankCalls=0;swapDevice=swap;growCount=grow;injectResource=inject;
 m.UninitialiseShaders();check(events[0]==1 && events[1]==2 && core.TextureStagesInUse==0);
 int expectedTexture=stages<=0?0:(grow?3:2);check(textureCalls==expectedTexture && resourceCalls==(empty?0:(inject?4:3)) && bankCalls==resourceCalls);
 if(stages>0)check(!core.CurrentlySelectedTextures[0] && !core.CurrentlySelectedTextures[2]);else check(core.CurrentlySelectedTextures[0]!=0 && core.CurrentlySelectedTextures[2]!=0);
 check((core.CurrentlySelectedTextures[3]==0)==(stages>0 && grow));
 for(int k=0;k<27;++k){check(m.VShaderResources[k].Info==&core);bool populated=!empty&&(k==0||k==4||k==26||(inject&&k==8));check(m.VShaderResources[k].Data==(populated?resources+k:0));}
 int offset=2+expectedTexture;for(int k=0;k<27;++k)if(m.VShaderResources[k].Data){check(events[offset++]==100+k);check(events[offset++]==200+k);}check(offset==eventCount);
}
int main(){
#ifdef _MSC_VER
 check(sizeof(CShaderDataBankNative)==0x128 && offsetof(CEngineResourceManagerNative,VShaderBanks)==0x960 && offsetof(CEngineResourceManagerNative,VShaderResources)==0x29c0 && sizeof(CShaderResourcePointer)==8);
#endif
 run(3,false,false,false,false);run(3,true,true,true,false);run(0,false,false,false,true);run(-2,true,false,false,false);
 if(failures){std::printf("00b3b0c0_TEST FAIL %d\n",failures);return 1;}std::printf("00b3b0c0_TEST PASS\n");return 0;}