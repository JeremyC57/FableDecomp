// CEnginePrimitiveManagerLines::CEnginePrimitiveManagerLines -- constructs line manager, registers EnableLines, and acquires VSHADER_TRANSFORM_COL_TEX0; retail00bc7e90 210bytes. Shader header is a flat record with no constructors/destructor, so this polymorphic method model follows its same8byte layout and existing landed destructor00bc9a00. Calls base00bacdb0, shader null ctor00a62fb0, string ctor/dtor0099ebf0/0099eae0, enable hook00bacd80, shader resource getter00a5d5f0. Global01436e98 resource+2a28. Getter uses explicit hidden-result ABI: it placement-constructs the shader in caller storage and returns its address; caller destroys that result after assignment.
// Leaf: CEnginePrimitiveManagerLines
// NF46 (round 7): the 0x01436E98 global is NEngineInternal::GPResourceManager (Ego_r.pdb / FableWin.pdb
// ?GPResourceManager@NEngineInternal@@3PAVCEngineResourceManager@@A); vertex shader resource +0x2A28 = CEngineResourceManager::VShaderResources[13]
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
struct CEngineSceneArray;
class CEnginePrimitiveManager { public:
 virtual ~CEnginePrimitiveManager();
 CEngineSceneArray* SceneList;bool Enable;
 CEnginePrimitiveManager(CEngineSceneArray*); //00bacdb0
 void AddEnableConsoleVariable(const CCharString&); //00bacd80, retail no-op
};
class CEnginePrimitiveManagerLines : public CEnginePrimitiveManager { public:
 CVertexShader VertexShader;
 CEnginePrimitiveManagerLines(CEngineSceneArray*);
};
CEnginePrimitiveManagerLines::CEnginePrimitiveManagerLines(CEngineSceneArray* scenes):CEnginePrimitiveManager(scenes),VertexShader()
{
 { CCharString name("EnableLines",-1); AddEnableConsoleVariable(name); }
 VertexShader = NEngineInternal::GPResourceManager->GetVShaderResource((EShaderBankNo)13)->GetVertexShader("VSHADER_TRANSFORM_COL_TEX0");
}
