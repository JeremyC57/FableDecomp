// CEngineResourceManagerNative::SetupVertexShaderResource -- open vertex bank with flags0x48, replace counted shader resource, bind bank/input. Retail00b3b6d0 complete225bytes. Donor shared resource-manager/bank headers differ: native banks+960 size128, resources+29c0 size8. Nativeview same reviewed00b3b0c0. Callees98b860CreateResource,A5D7C0OpenShaderBank,A5D8B0SetVertexShaderInput,BFE9BCoperator delete; global1436e14. Shared fable_string.h models CCharString; no shaderresource/shader-render-manager headers. Bank virtualslot1 OpenReadOnly009d06f0; callback98ba10 accepts resource in ecx.
#ifndef _MSC_VER
#define __forceinline inline
#define __cdecl
#endif
#ifdef _MSC_VER
#include "engine/CEngineResourceManager.h"
#include "engine/CShaderDataBank.h"
#include "fable_string.h"
#endif
void __cdecl operator delete(void*); //00bfe9bc
#ifndef _MSC_VER
class CCharString { public:CCharString();~CCharString();void* Storage;};
#endif
enum EShaderBankNo {SHADER_BANK_DEFAULT=0};
class CCharString;class CVertexShaderInput;struct CShaderDataBankNative;
class CShaderResource { public:void OpenShaderBank(const CShaderDataBankNative*);void SetVertexShaderInput(const CVertexShaderInput&);}; //00a5d7c0,00a5d8b0
struct CCountedObject {void Destroy();};
struct CCPPointerInfoNative {unsigned long RefCount;void (CCountedObject::*DeleteFunc)();CCountedObject* Data;};
struct CShaderResourcePointer {
 CShaderResource* Data;CCPPointerInfoNative* Info;
 CShaderResourcePointer():Data(0),Info(0){}
 CShaderResourcePointer(const CShaderResourcePointer& p):Data(p.Data),Info(p.Info){if(Info)++Info->RefCount;}
 __forceinline ~CShaderResourcePointer(){Release();}
 __forceinline void Release(){if(Info){--Info->RefCount;if(Info->RefCount==0){(Info->Data->*Info->DeleteFunc)();operator delete(Info);}}}
 __forceinline CShaderResourcePointer& operator=(const CShaderResourcePointer& p){CCPPointerInfoNative* info=p.Info;CShaderResource* data=p.Data;if(Info!=info){Release();Data=data;Info=info;if(Info)++Info->RefCount;}return *this;}
};
struct CShaderDataBankNative {virtual ~CShaderDataBankNative();virtual bool OpenReadOnly(const CCharString&,unsigned long);unsigned char NativeMembers[0x124];};
class CShaderRenderManager { public:CShaderResourcePointer CreateResource();}; //0098b860
extern CShaderRenderManager* g_pShaderRenderManager; //01436e14
struct CEngineResourceManagerNative {unsigned char EarlierMembers[0x960];CShaderDataBankNative VShaderBanks[27];unsigned char PixelShaderAndOtherData[0x128];CShaderResourcePointer VShaderResources[27];void SetupVertexShaderResource(EShaderBankNo bank,const CCharString&,const CVertexShaderInput&);};
void CEngineResourceManagerNative::SetupVertexShaderResource(EShaderBankNo bank,const CCharString& name,const CVertexShaderInput& input){
 CShaderDataBankNative* shaderBank=&VShaderBanks[bank];shaderBank->OpenReadOnly(name,0x48);
 VShaderResources[bank]=g_pShaderRenderManager->CreateResource();
 VShaderResources[bank].Data->OpenShaderBank(shaderBank);
 VShaderResources[bank].Data->SetVertexShaderInput(input);
}

#include <cstdio>
#include <cstddef>
CCharString::CCharString(){}
CCharString::~CCharString(){}class CVertexShaderInput { public:int ID;};
static int fail=0,step=0,destroys=0,deallocs=0;static void* lastDestroyed=0;static void* lastDeleted=0;static CEngineResourceManagerNative* active=0;static long selected=0;static const CCharString* expectedName=0;static CVertexShaderInput* expectedInput=0;static CShaderResource* created=0;static CCPPointerInfoNative* fresh=0;static CCPPointerInfoNative* swapDeleteInfo=0;static CShaderResource* swapResource=0;static CShaderResource* inputReceiver=0;
static void check(bool b){if(!b)++fail;}
void __cdecl operator delete(void* p){++deallocs;lastDeleted=p;}
void CCountedObject::Destroy(){++destroys;lastDestroyed=this;if(swapDeleteInfo)active->VShaderResources[selected].Info=swapDeleteInfo;}
CShaderDataBankNative::~CShaderDataBankNative(){}
bool CShaderDataBankNative::OpenReadOnly(const CCharString& name,unsigned long flags){check(step==0&&this==&active->VShaderBanks[selected]&&&name==expectedName&&flags==0x48);step=1;return false;}
CShaderResourcePointer CShaderRenderManager::CreateResource(){check(step==1&&this==g_pShaderRenderManager);step=2;CShaderResourcePointer result;result.Data=created;result.Info=fresh;return result;}
void CShaderResource::OpenShaderBank(const CShaderDataBankNative* bank){check(step==2&&this==created&&bank==&active->VShaderBanks[selected]);step=3;if(swapResource)active->VShaderResources[selected].Data=swapResource;}
void CShaderResource::SetVertexShaderInput(const CVertexShaderInput& input){check(step==3&&this==inputReceiver&&&input==expectedInput);step=4;}
CShaderRenderManager* g_pShaderRenderManager=0;
static void run(long index,int mode){CEngineResourceManagerNative manager;active=&manager;selected=index;CShaderRenderManager shader;g_pShaderRenderManager=&shader;CShaderResource oldResource,newResource,otherResource;CCharString name;CVertexShaderInput input;expectedName=&name;expectedInput=&input;created=&newResource;inputReceiver=&newResource;swapResource=0;swapDeleteInfo=0;CCPPointerInfoNative oldInfo,newInfo,otherInfo;oldInfo.RefCount=mode==1?2:1;newInfo.RefCount=1;otherInfo.RefCount=4;oldInfo.DeleteFunc=newInfo.DeleteFunc=otherInfo.DeleteFunc=&CCountedObject::Destroy;oldInfo.Data=reinterpret_cast<CCountedObject*>(&oldResource);newInfo.Data=reinterpret_cast<CCountedObject*>(&newResource);otherInfo.Data=reinterpret_cast<CCountedObject*>(&otherResource);fresh=&newInfo;
 manager.VShaderResources[index].Data=&oldResource;manager.VShaderResources[index].Info=mode==2?0:&oldInfo;
 if(mode==3){manager.VShaderResources[index].Data=&newResource;manager.VShaderResources[index].Info=&newInfo;newInfo.RefCount=2;}
 if(mode==4){swapDeleteInfo=&otherInfo;swapResource=&otherResource;inputReceiver=&otherResource;}
 step=destroys=deallocs=0;lastDeleted=lastDestroyed=0;manager.SetupVertexShaderResource(static_cast<EShaderBankNo>(index),name,input);check(step==4&&newInfo.RefCount==1&&manager.VShaderResources[index].Info==&newInfo&&manager.VShaderResources[index].Data==inputReceiver);
 check(destroys==(mode==0||mode==4?1:0)&&deallocs==destroys);if(destroys)check(lastDestroyed==reinterpret_cast<CCountedObject*>(&oldResource)&&lastDeleted==(mode==4?&otherInfo:&oldInfo));if(mode==1)check(oldInfo.RefCount==1);if(mode==4)check(otherInfo.RefCount==4);
 for(int i=0;i<27;++i)manager.VShaderResources[i].Info=0;swapResource=0;swapDeleteInfo=0;}
int main(){run(0,0);run(1,1);run(13,2);run(26,3);run(26,4);
#ifdef _MSC_VER
 check(sizeof(CShaderDataBankNative)==0x128&&sizeof(CCPPointerInfoNative)==12&&sizeof(CShaderResourcePointer)==8&&offsetof(CEngineResourceManagerNative,VShaderResources)==0x29c0&&offsetof(CEngineResourceManagerNative,VShaderBanks)==0x960);
#endif
 if(fail){std::printf("00b3b6d0_TEST FAIL %d\n",fail);return 1;}std::printf("00b3b6d0_TEST PASS\n");return 0;}