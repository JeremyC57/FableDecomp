// CEnginePrimitiveManagerLines::CEnginePrimitiveManagerLines -- constructs line manager, registers EnableLines, and acquires VSHADER_TRANSFORM_COL_TEX0; retail00bc7e90 210bytes. Shader header is a flat record with no constructors/destructor, so this polymorphic method model follows its same8byte layout and existing landed destructor00bc9a00. Calls base00bacdb0, shader null ctor00a62fb0, string ctor/dtor0099ebf0/0099eae0, enable hook00bacd80, shader resource getter00a5d5f0. Global01436e98 resource+2a28. Getter uses explicit hidden-result ABI: it placement-constructs the shader in caller storage and returns its address; caller destroys that result after assignment.
// Leaf: CEnginePrimitiveManagerLines
#ifdef _MSC_VER
#include "engine/CVertexShader.h"
#include "fable_string.h"
#else
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
struct CEngineData { unsigned char Prefix[0x2a28];CShaderResource* ShaderResource; CShaderResource* GetShaderResource(){return ShaderResource;} };
extern CEngineData* g_pEngineData; //01436e98
struct CEngineSceneArray;
struct CEnginePrimitiveManager {
 virtual ~CEnginePrimitiveManager();
 CEngineSceneArray* SceneList;bool Enable;
 CEnginePrimitiveManager(CEngineSceneArray*); //00bacdb0
 void RegisterEnable(const CCharString&); //00bacd80, retail no-op
};
struct CEnginePrimitiveManagerLines : CEnginePrimitiveManager {
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