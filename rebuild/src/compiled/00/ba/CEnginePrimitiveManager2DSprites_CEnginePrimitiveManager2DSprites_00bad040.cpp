// CEnginePrimitiveManager2DSprites::CEnginePrimitiveManager2DSprites(CArray<CEngineScene*>* scenes) -- constructs the 2D sprite manager: looks up its two vertex shaders and two pixel shaders by name, fills the quad index list and registers the Enable2DSprites switch
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BAD040, length 668 bytes (`ret 4`, no EH frame).
// Callees: CEnginePrimitiveManager::CEnginePrimitiveManager @ 0x00BACDB0, CVertexShader::CVertexShader @ 0x00A62FB0,
//   CPixelShader::CPixelShader @ 0x00A5EC00, CCharString::CCharString(const char*, long) @ 0x0099EBF0 / ~CCharString @ 0x0099EAE0,
//   CShaderResource::GetVertexShader @ 0x00A5D5F0 and GetPixelShader @ 0x00A5D720 (both labelled GetPixelShader in the
//   manifest; the first returns a CVertexShader -- its temporary is reset to vtable 0x0129B3D0 and counts at +0x10),
//   CEnginePrimitiveManager::AddEnableConsoleVariable @ 0x00BACD80 (a retail no-op, `ret 4`).
// Globals: the engine data @ 0x01436E98; its shader resource at +0x2A40 (vertex) and +0x2A98 (pixel).
// Vtables: 0x012A5664 CEnginePrimitiveManager2DSprites, 0x0129B3D0 CVertexShader, 0x0129A054 CPixelShader, 0x0129A030 CShaderBase.
// Layout: engine/CEnginePrimitiveManager2DSprites.h offsets (VertexShader2DSprite +0x0C, VertexShader2DClockSprite +0x14,
//   PixelShader2DClockSprite +0x1C, PixelShader2DClockSpriteAdditive +0x24, Indices +0x2C), restated locally as in the landed
//   destructor 0x00BB1900 because the header's members are opaque and declare no constructors.
// Leaf: ??0CEnginePrimitiveManager2DSprites
// Shape: each lookup passes the literal to a `const CCharString&` parameter (implicit conversion: the name temporary
//   lands in the dead argument slot; an explicit CCharString(...) swaps it with the result spill) through an INLINE
//   resource accessor (the global and the +0x2A40 load are read before the pushes), calls the getter into a by-value
//   temporary, assigns (the vertex shader
//   operator= releases and re-points; the pixel one also nulls the old pointer inside the release) and destroys the
//   temporary through its inline destructor (`RemoveReference` copy, as the landed Lines constructor 0x00BC7E90).
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
    void AddEnableConsoleVariable(const CCharString& name);  // 0x00BACD80 ?AddEnableConsoleVariable@CEnginePrimitiveManager@@QAEXABVCCharString@@@Z (Ego_r / FableWin; the donor 2DSprites ctor calls it; self Leaf)
};

class CEnginePrimitiveManager2DSprites : public CEnginePrimitiveManager { public:   // vtable 0x012A5664
    CVertexShader  VertexShader2DSprite;           // +0x0C
    CVertexShader  VertexShader2DClockSprite;      // +0x14
    CPixelShader   PixelShader2DClockSprite;       // +0x1C
    CPixelShader   PixelShader2DClockSpriteAdditive;   // +0x24
    unsigned short Indices[6];                     // +0x2C
    CEnginePrimitiveManager2DSprites(CEngineSceneArray* scenes);
    virtual ~CEnginePrimitiveManager2DSprites();
};

CEnginePrimitiveManager2DSprites::CEnginePrimitiveManager2DSprites(CEngineSceneArray* scenes)
    : CEnginePrimitiveManager(scenes)
{
    VertexShader2DSprite = NEngineInternal::GPResourceManager->GetVShaderResource((EShaderBankNo)16)->GetVertexShader("VSHADER_2D_SPRITE");
    VertexShader2DClockSprite = NEngineInternal::GPResourceManager->GetVShaderResource((EShaderBankNo)16)->GetVertexShader("VSHADER_2D_CLOCK_SPRITE");
    PixelShader2DClockSprite = NEngineInternal::GPResourceManager->GetPShaderResource()->GetPixelShader("PSHADER_2D_CLOCK_SPRITE");
    PixelShader2DClockSpriteAdditive = NEngineInternal::GPResourceManager->GetPShaderResource()->GetPixelShader("PSHADER_2D_CLOCK_SPRITE_ADDITIVE");
    Indices[0] = 0;
    Indices[1] = 1;
    Indices[2] = 2;
    Indices[3] = 1;
    Indices[4] = 3;
    Indices[5] = 2;
    AddEnableConsoleVariable("Enable2DSprites");
}