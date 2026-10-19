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
struct CCharString {CCharString();~CCharString();void* Storage;};
#endif
enum EShaderBankNo {SHADER_BANK_DEFAULT=0};
struct CCharString;struct CVertexShaderInput;struct CShaderDataBankNative;
struct CShaderResource {void OpenShaderBank(const CShaderDataBankNative*);void SetVertexShaderInput(const CVertexShaderInput&);}; //00a5d7c0,00a5d8b0
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
struct CShaderRenderManager {CShaderResourcePointer CreateResource();}; //0098b860
extern CShaderRenderManager* g_pShaderRenderManager; //01436e14
struct CEngineResourceManagerNative {unsigned char EarlierMembers[0x960];CShaderDataBankNative VShaderBanks[27];unsigned char PixelShaderAndOtherData[0x128];CShaderResourcePointer VShaderResources[27];void SetupVertexShaderResource(EShaderBankNo bank,const CCharString&,const CVertexShaderInput&);};
void CEngineResourceManagerNative::SetupVertexShaderResource(EShaderBankNo bank,const CCharString& name,const CVertexShaderInput& input){
 CShaderDataBankNative* shaderBank=&VShaderBanks[bank];shaderBank->OpenReadOnly(name,0x48);
 VShaderResources[bank]=g_pShaderRenderManager->CreateResource();
 VShaderResources[bank].Data->OpenShaderBank(shaderBank);
 VShaderResources[bank].Data->SetVertexShaderInput(input);
}