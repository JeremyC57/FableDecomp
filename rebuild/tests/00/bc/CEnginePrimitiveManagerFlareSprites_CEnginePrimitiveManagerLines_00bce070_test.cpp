// CEnginePrimitiveManagerFlareSprites::CEnginePrimitiveManagerFlareSprites -- constructs flare buffer map, loads VSHADER_STATIC_UNLIT and sets fade-in speed0.75; retail00bce070 232bytes. RTTI012a6c18 and adjacent destructor00bce160 prove propagated Lines label wrong. Shared flare header has donor16byte map and wrong shader/speed offsets1c/24; retail map12 shifts shader18/speed20. Shader method model and hidden result ABI follow proven00bc7e90. Callees base00bacf70,malloc00bfea0e,shaderctor00a62fb0,string0099ebf0/0099eae0,getter00a5d5f0; global01436e98 shader resource29c0.
#ifdef _MSC_VER
#include "engine/CVertexShader.h"
#include "fable_string.h"
#else
#define __cdecl
struct CCharStringData;
class CCharString { CCharStringData* storage_; public: CCharString(const char*,long); ~CCharString(); operator const char*() const; };
#endif
struct CVertexShaderData { virtual ~CVertexShaderData(); void* VertexDeclaration;void* VertexShader;const void* FormatDecl;long ReferenceCount; long RemoveReference(){return --ReferenceCount;} void ReleaseReference(){if(--ReferenceCount==0)delete this;} };
struct CShaderBase { virtual ~CShaderBase(){} };
struct CVertexShaderNativeView : CShaderBase {
 CVertexShaderData* Data;
 CVertexShaderNativeView(); //00a62fb0
 CVertexShaderNativeView(const CVertexShaderNativeView& o):Data(o.Data){if(Data)++Data->ReferenceCount;}
 CVertexShaderNativeView& operator=(const CVertexShaderNativeView& other) { CVertexShaderData* old=Data; if(old)old->ReleaseReference(); Data=other.Data; ++Data->ReferenceCount;return *this; }
 virtual ~CVertexShaderNativeView(){if(Data){ CVertexShaderData* current=Data; if(!current->RemoveReference())delete current;}}
};
struct CShaderResource { CVertexShaderNativeView* GetVertexShader(CVertexShaderNativeView* result,const CCharString&); }; //00a5d5f0 (propagated GetPixelShader label)
struct CEngineData { unsigned char Prefix[0x29c0];CShaderResource* ShaderResource; CShaderResource* GetShaderResource(){return ShaderResource;} };
extern CEngineData* g_pEngineData; //01436e98

#include <stddef.h>
extern "C" void* __cdecl malloc(size_t);
struct CEngineSceneArray;
struct CEnginePrimitiveManager {
 virtual ~CEnginePrimitiveManager();
 CEngineSceneArray* SceneList;bool Enable;
 CEnginePrimitiveManager(CEngineSceneArray*);
};
namespace N_00bacf70 { struct CEnginePrimitiveGroupManager:CEnginePrimitiveManager { CEnginePrimitiveGroupManager(CEngineSceneArray*); }; } using N_00bacf70::CEnginePrimitiveGroupManager; //00bacf70
struct SFlareBufferTreeNode { unsigned char Colour; SFlareBufferTreeNode *Parent,*Left,*Right; unsigned long Key; void* HandleVtable;void* Buffer; };
struct CFlareBufferMap {
 SFlareBufferTreeNode* Header;unsigned long Count;unsigned char Compare;
 CFlareBufferMap() { Header=0;Header=static_cast<SFlareBufferTreeNode*>(malloc(sizeof(SFlareBufferTreeNode)));Count=0;Header->Colour=0;Header->Parent=0;Header->Left=Header;Header->Right=Header; }
 ~CFlareBufferMap();
};
struct CEnginePrimitiveManagerFlareSprites:CEnginePrimitiveGroupManager {
 CFlareBufferMap FlareVertexBuffers;
 CVertexShaderNativeView VertexShaderFlareTest;
 float FadeInSpeed;
 CEnginePrimitiveManagerFlareSprites(CEngineSceneArray*);
 virtual ~CEnginePrimitiveManagerFlareSprites();
};
CEnginePrimitiveManagerFlareSprites::CEnginePrimitiveManagerFlareSprites(CEngineSceneArray* scenes):CEnginePrimitiveGroupManager(scenes),FlareVertexBuffers(),VertexShaderFlareTest()
{
 { CCharString name("VSHADER_STATIC_UNLIT",-1); union ShaderResult { unsigned char Bytes[sizeof(CVertexShaderNativeView)]; void* Alignment; } result; CVertexShaderNativeView* temporary=reinterpret_cast<CVertexShaderNativeView*>(result.Bytes); VertexShaderFlareTest=*g_pEngineData->GetShaderResource()->GetVertexShader(temporary,name); temporary->CVertexShaderNativeView::~CVertexShaderNativeView(); }
 FadeInSpeed=0.75f;
}

#ifdef _MSC_VER
FABLE_STATIC_ASSERT(sizeof(CVertexShaderNativeView)==sizeof(CVertexShader));
#endif
#include <cstdio>
#include <cstring>
#include <new>
static int groupCalls=0,mapDestroyed=0;
static int failures=0,baseCalls=0,nullShaderCtors=0,stringsMade=0,stringsFreed=0,enableCalls=0,getterCalls=0,dataDeleted=0,baseDestroyed=0;
static CVertexShaderData* returnedData=0;
static void check(bool ok){if(!ok)++failures;}
CVertexShaderData::~CVertexShaderData(){++dataDeleted;check(ReferenceCount==0);}
CVertexShaderNativeView::CVertexShaderNativeView():Data(0){++nullShaderCtors;}
CCharString::CCharString(const char* text,long length):storage_(reinterpret_cast<CCharStringData*>(const_cast<char*>(text))){check(length==-1);++stringsMade;}
CCharString::operator const char*() const{return reinterpret_cast<const char*>(storage_);}
CCharString::~CCharString(){++stringsFreed;if(std::strcmp(reinterpret_cast<const char*>(storage_),"VSHADER_STATIC_UNLIT")==0)check(returnedData && returnedData->ReferenceCount==1);}
struct CEngineSceneArray { unsigned long Marker; };
CEnginePrimitiveManager::CEnginePrimitiveManager(CEngineSceneArray* scenes):SceneList(scenes),Enable(true){++baseCalls;}
CEnginePrimitiveGroupManager::CEnginePrimitiveGroupManager(CEngineSceneArray* scenes):CEnginePrimitiveManager(scenes){++groupCalls;}
CEnginePrimitiveManagerFlareSprites::~CEnginePrimitiveManagerFlareSprites(){}
extern "C" void free(void*);
CFlareBufferMap::~CFlareBufferMap(){check(dataDeleted==1 && Count==0);++mapDestroyed;::free(Header);}
CEnginePrimitiveManager::~CEnginePrimitiveManager(){++baseDestroyed;check(dataDeleted==1);}
CVertexShaderNativeView* CShaderResource::GetVertexShader(CVertexShaderNativeView* result,const CCharString& name)
{
 ++getterCalls;check(std::strcmp(static_cast<const char*>(name),"VSHADER_STATIC_UNLIT")==0);
 // The retail getter's hidden output parameter starts a fresh wrapper lifetime.
 new(result) CVertexShaderNativeView;
 returnedData=new CVertexShaderData;returnedData->ReferenceCount=1;result->Data=returnedData;
 return result;
}
CEngineData* g_pEngineData=0;
int main()
{
 CEngineData engine;CShaderResource resources;engine.ShaderResource=&resources;g_pEngineData=&engine;
 CEngineSceneArray scenes;scenes.Marker=17;
 {
  CEnginePrimitiveManagerFlareSprites manager(&scenes);
  check(baseCalls==1 && groupCalls==1 && manager.SceneList==&scenes && manager.Enable && manager.FadeInSpeed==0.75f);
  SFlareBufferTreeNode* header=manager.FlareVertexBuffers.Header;
  check(header && header->Colour==0 && !header->Parent && header->Left==header && header->Right==header && manager.FlareVertexBuffers.Count==0);
  check(nullShaderCtors==2 && stringsMade==1 && stringsFreed==1 && enableCalls==0 && getterCalls==1);
  check(manager.VertexShaderFlareTest.Data==returnedData && returnedData->ReferenceCount==1 && !dataDeleted && !baseDestroyed);
#ifdef _MSC_VER
  check(sizeof(CEnginePrimitiveManagerFlareSprites)==0x24 && reinterpret_cast<unsigned char*>(&manager.VertexShaderFlareTest)-reinterpret_cast<unsigned char*>(&manager)==0x18);
#endif
 }
 check(dataDeleted==1 && baseDestroyed==1 && mapDestroyed==1);
 if(failures){std::printf("00bce070_TEST FAIL %d\n",failures);return 1;}
 std::printf("00bce070_TEST PASS\n");return 0;
}