// CEnginePrimitiveManagerText::CEnginePrimitiveManagerText(CEngineSceneArray* scenes) -- constructs the text primitive manager: looks up its two vertex shaders and two pixel shaders by name
// Leaf: ??0CEnginePrimitiveManagerText
// Layout: local model reused from the landed 2DSprites ctor 0x00BAD040; vertex shader resource at engine data +0x2A90 here, pixel at +0x2A98.
// NF46 (round 7): the 0x01436E98 global is NEngineInternal::GPResourceManager (Ego_r.pdb / FableWin.pdb
// ?GPResourceManager@NEngineInternal@@3PAVCEngineResourceManager@@A); the vertex shader resources are
// CEngineResourceManager::VShaderResources[bank] (retail +0x29C0 + 8*bank, FableWin.pdb inline GetVShaderResource) and the pixel one
// ShaderResourcePixel (+0x2A98, GetPShaderResource). Shader classes from the shared headers (CShaderResource::GetVertexShader /
// GetPixelShader const, Ego_r.pdb / FableWin.pdb); were a local CEngineData / g_pEngineData stand-in and local shader models.
#define FABLE_RETAIL_CENGINERESOURCEMANAGER
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CVERTEXSHADER
#include "fable_string.h"
#include "engine/CVertexShader.h"
#include "engine/CPixelShader.h"
#include "engine/CEngineResourceManager.h"
#include "engine/CShaderResource.h"
namespace NEngineInternal { extern CEngineResourceManager* GPResourceManager; }   // 0x01436E98

struct CEngineSceneArray;
class CEnginePrimitiveManager { public:
    virtual ~CEnginePrimitiveManager();
    CEngineSceneArray* SceneList;                  // +0x04
    bool               Enable;                     // +0x08
    CEnginePrimitiveManager(CEngineSceneArray* scenes);   // 0x00BACDB0
    void RegisterEnable(const CCharString& name);  // 0x00BACD80
};

class CEnginePrimitiveManagerText : public CEnginePrimitiveManager { public:   // vtable 0x012A68B0
    CVertexShader  VertexShaderText;               // +0x0C
    CVertexShader  VertexShaderTextShadow;         // +0x14
    CPixelShader   PixelShaderText;                // +0x1C
    CPixelShader   PixelShaderTextShadow;          // +0x24
    CEnginePrimitiveManagerText(CEngineSceneArray* scenes);
    virtual ~CEnginePrimitiveManagerText();
};

CEnginePrimitiveManagerText::CEnginePrimitiveManagerText(CEngineSceneArray* scenes)
    : CEnginePrimitiveManager(scenes)
{
    VertexShaderText = NEngineInternal::GPResourceManager->GetVShaderResource((EShaderBankNo)26)->GetVertexShader("VSHADER_TEXT");
    VertexShaderTextShadow = NEngineInternal::GPResourceManager->GetVShaderResource((EShaderBankNo)26)->GetVertexShader("VSHADER_TEXT_ALPHA_MASK");
    PixelShaderText = NEngineInternal::GPResourceManager->GetPShaderResource()->GetPixelShader("PSHADER_TEXT");
    PixelShaderTextShadow = NEngineInternal::GPResourceManager->GetPShaderResource()->GetPixelShader("PSHADER_TEXT_ALPHA_MASK");
}