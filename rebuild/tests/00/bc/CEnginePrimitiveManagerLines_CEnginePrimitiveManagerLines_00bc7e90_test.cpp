// CEnginePrimitiveManagerLines::CEnginePrimitiveManagerLines -- constructs line manager, registers EnableLines, and acquires VSHADER_TRANSFORM_COL_TEX0; retail00bc7e90 210bytes. Shader header is a flat record with no constructors/destructor, so this polymorphic method model follows its same8byte layout and existing landed destructor00bc9a00. Calls base00bacdb0, shader null ctor00a62fb0, string ctor/dtor0099ebf0/0099eae0, enable hook00bacd80, shader resource getter00a5d5f0. Global01436e98 resource+2a28. Getter uses explicit hidden-result ABI: it placement-constructs the shader in caller storage and returns its address; caller destroys that result after assignment.
#ifdef _MSC_VER
#include "engine/CVertexShader.h"
#include "fable_string.h"
#else
class CCharStringData;
class CCharString { CCharStringData* storage_; public: CCharString(const char*,long); ~CCharString(); operator const char*() const; };
#endif
struct CVertexShaderData { virtual ~CVertexShaderData(); void* VertexDeclaration;void* VertexShader;const void* FormatDecl;long ReferenceCount; long RemoveReference(){return --ReferenceCount;} void ReleaseReference(){if(--ReferenceCount==0)delete this;} };
class CShaderBase { public: virtual ~CShaderBase(){} };
struct CVertexShaderNativeView : CShaderBase {
 CVertexShaderData* Data;
 CVertexShaderNativeView(); //00a62fb0
 CVertexShaderNativeView(const CVertexShaderNativeView& o):Data(o.Data){if(Data)++Data->ReferenceCount;}
 CVertexShaderNativeView& operator=(const CVertexShaderNativeView& other) { CVertexShaderData* old=Data; if(old)old->ReleaseReference(); Data=other.Data; ++Data->ReferenceCount;return *this; }
 virtual ~CVertexShaderNativeView(){if(Data){ CVertexShaderData* current=Data; if(!current->RemoveReference())delete current;}}
};
class CShaderResource { public: CVertexShaderNativeView* GetVertexShader(CVertexShaderNativeView* result,const CCharString&); }; //00a5d5f0 (propagated GetPixelShader label)
struct CEngineData { unsigned char Prefix[0x2a28];CShaderResource* ShaderResource; CShaderResource* GetShaderResource(){return ShaderResource;} };
extern CEngineData* g_pEngineData; //01436e98
struct CEngineSceneArray;
class CEnginePrimitiveManager { public:
 virtual ~CEnginePrimitiveManager();
 CEngineSceneArray* SceneList;bool Enable;
 CEnginePrimitiveManager(CEngineSceneArray*); //00bacdb0
 void RegisterEnable(const CCharString&); //00bacd80, retail no-op
};
class CEnginePrimitiveManagerLines : public CEnginePrimitiveManager { public:
 CVertexShaderNativeView VertexShader;
 CEnginePrimitiveManagerLines(CEngineSceneArray*);
};
CEnginePrimitiveManagerLines::CEnginePrimitiveManagerLines(CEngineSceneArray* scenes):CEnginePrimitiveManager(scenes),VertexShader()
{
 { CCharString name("EnableLines",-1); RegisterEnable(name); }
 { CCharString name("VSHADER_TRANSFORM_COL_TEX0",-1); union ShaderResult { unsigned char Bytes[sizeof(CVertexShaderNativeView)]; void* Alignment; } result; CVertexShaderNativeView* temporary=reinterpret_cast<CVertexShaderNativeView*>(result.Bytes); VertexShader=*g_pEngineData->GetShaderResource()->GetVertexShader(temporary,name); temporary->CVertexShaderNativeView::~CVertexShaderNativeView(); }
}

#ifdef _MSC_VER
FABLE_STATIC_ASSERT(sizeof(CVertexShaderNativeView)==sizeof(CVertexShader));
#endif
#include <cstdio>
#include <cstring>
#include <new>
static int failures=0,baseCalls=0,nullShaderCtors=0,stringsMade=0,stringsFreed=0,enableCalls=0,getterCalls=0,dataDeleted=0,baseDestroyed=0;
static CVertexShaderData* returnedData=0;
static void check(bool ok){if(!ok)++failures;}
CVertexShaderData::~CVertexShaderData(){++dataDeleted;check(ReferenceCount==0);}
CVertexShaderNativeView::CVertexShaderNativeView():Data(0){++nullShaderCtors;}
CCharString::CCharString(const char* text,long length):storage_(reinterpret_cast<CCharStringData*>(const_cast<char*>(text))){check(length==-1);++stringsMade;}
CCharString::operator const char*() const{return reinterpret_cast<const char*>(storage_);}
CCharString::~CCharString(){++stringsFreed;if(std::strcmp(reinterpret_cast<const char*>(storage_),"VSHADER_TRANSFORM_COL_TEX0")==0)check(returnedData && returnedData->ReferenceCount==1);}
struct CEngineSceneArray { unsigned long Marker; };
CEnginePrimitiveManager::CEnginePrimitiveManager(CEngineSceneArray* scenes):SceneList(scenes),Enable(true){++baseCalls;}
CEnginePrimitiveManager::~CEnginePrimitiveManager(){++baseDestroyed;check(dataDeleted==1);}
void CEnginePrimitiveManager::RegisterEnable(const CCharString& name){++enableCalls;check(std::strcmp(static_cast<const char*>(name),"EnableLines")==0);}
CVertexShaderNativeView* CShaderResource::GetVertexShader(CVertexShaderNativeView* result,const CCharString& name)
{
 ++getterCalls;check(std::strcmp(static_cast<const char*>(name),"VSHADER_TRANSFORM_COL_TEX0")==0);
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
  CEnginePrimitiveManagerLines manager(&scenes);
  check(baseCalls==1 && manager.SceneList==&scenes && manager.Enable);
  check(nullShaderCtors==2 && stringsMade==2 && stringsFreed==2 && enableCalls==1 && getterCalls==1);
  check(manager.VertexShader.Data==returnedData && returnedData->ReferenceCount==1 && !dataDeleted && !baseDestroyed);
#ifdef _MSC_VER
  check(sizeof(CEnginePrimitiveManagerLines)==0x14 && reinterpret_cast<unsigned char*>(&manager.VertexShader)-reinterpret_cast<unsigned char*>(&manager)==0xc);
#endif
 }
 check(dataDeleted==1 && baseDestroyed==1);
 if(failures){std::printf("00bc7e90_TEST FAIL %d\n",failures);return 1;}
 std::printf("00bc7e90_TEST PASS\n");return 0;
}