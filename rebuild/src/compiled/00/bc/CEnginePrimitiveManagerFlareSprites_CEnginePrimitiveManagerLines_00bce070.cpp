// CEnginePrimitiveManagerFlareSprites::CEnginePrimitiveManagerFlareSprites -- constructs flare buffer map, loads VSHADER_STATIC_UNLIT and sets fade-in speed0.75; retail00bce070 232bytes. RTTI012a6c18 and adjacent destructor00bce160 prove propagated Lines label wrong. Shared flare header has donor16byte map and wrong shader/speed offsets1c/24; retail map12 shifts shader18/speed20. Shader method model and hidden result ABI follow proven00bc7e90. Callees base00bacf70,malloc00bfea0e,shaderctor00a62fb0,string0099ebf0/0099eae0,getter00a5d5f0; global01436e98 shader resource29c0.
// Leaf: ??0CEnginePrimitiveManagerFlareSprites@@QAE@PAUCEngineSceneArray@@@Z
// NF46 (round 7): the 0x01436E98 global is NEngineInternal::GPResourceManager (Ego_r.pdb / FableWin.pdb
// ?GPResourceManager@NEngineInternal@@3PAVCEngineResourceManager@@A); vertex shader resource +0x29C0 = CEngineResourceManager::VShaderResources[0]
// (FableWin.pdb inline GetVShaderResource). Shader classes from the shared headers (CShaderResource::GetVertexShader const by value,
// Ego_r.pdb / FableWin.pdb); were a local CEngineData / g_pEngineData stand-in, a CVertexShader model and an explicit
// hidden-result call.
#define FABLE_RETAIL_CENGINERESOURCEMANAGER
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CVERTEXSHADER
#include "fable_string.h"
#include "engine/CVertexShader.h"
#include "engine/CEngineResourceManager.h"
#include "engine/CShaderResource.h"
namespace NEngineInternal { extern CEngineResourceManager* GPResourceManager; }   // 0x01436E98

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
 CVertexShader VertexShaderFlareTest;
 float FadeInSpeed;
 CEnginePrimitiveManagerFlareSprites(CEngineSceneArray*);
 virtual ~CEnginePrimitiveManagerFlareSprites();
};
CEnginePrimitiveManagerFlareSprites::CEnginePrimitiveManagerFlareSprites(CEngineSceneArray* scenes):CEnginePrimitiveGroupManager(scenes),FlareVertexBuffers(),VertexShaderFlareTest()
{
 VertexShaderFlareTest = NEngineInternal::GPResourceManager->GetVShaderResource((EShaderBankNo)0)->GetVertexShader("VSHADER_STATIC_UNLIT");
 FadeInSpeed=0.75f;
}
