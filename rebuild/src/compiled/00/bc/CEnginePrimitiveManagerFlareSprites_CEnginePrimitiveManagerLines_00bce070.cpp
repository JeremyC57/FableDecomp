// CEnginePrimitiveManagerFlareSprites::CEnginePrimitiveManagerFlareSprites -- constructs flare buffer map, loads VSHADER_STATIC_UNLIT and sets fade-in speed0.75; retail00bce070 232bytes. RTTI012a6c18 and adjacent destructor00bce160 prove propagated Lines label wrong. Shared flare header has donor16byte map and wrong shader/speed offsets1c/24; retail map12 shifts shader18/speed20. Shader method model and hidden result ABI follow proven00bc7e90. Callees base00bacf70,malloc00bfea0e,shaderctor00a62fb0,string0099ebf0/0099eae0,getter00a5d5f0; global01436e98 shader resource29c0.
// Leaf: CEnginePrimitiveManagerLines
#ifdef _MSC_VER
#include "engine/CVertexShader.h"
#include "fable_string.h"
#else
#define __cdecl
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
struct CEngineData { unsigned char Prefix[0x29c0];CShaderResource* ShaderResource; CShaderResource* GetShaderResource(){return ShaderResource;} };
extern CEngineData* g_pEngineData; //01436e98

#include <stddef.h>
extern "C" void* __cdecl malloc(size_t);
struct CEngineSceneArray;
class CEnginePrimitiveManager { public:
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
class CEnginePrimitiveManagerFlareSprites:public CEnginePrimitiveGroupManager { public:
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