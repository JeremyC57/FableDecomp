// CEnginePrimitiveManagerRepeatedStaticMeshes::CEnginePrimitiveManagerRepeatedStaticMeshes(CEngineSceneArray* scenes) -- constructs the repeated static mesh manager: registers EnableRepeatedMeshes, looks up the plain / stipple / depth-shadowed shader pairs (and the colour-buffer-shadowed pairs when the texture quality allows), points the active shadowed shaders at the depth-buffer set and allocates the 16-aligned SIMD variable block
// Leaf: ??0CEnginePrimitiveManagerRepeatedStaticMeshes
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B99D90, length 1808 bytes (`ret 4`, no EH frame). Vtable 0x012A3E9C.
// Callees: CEnginePrimitiveGroupManager ctor @ 0x00BEBC30, CPixelShader() @ 0x00A5EC00, CVertexShader() @ 0x00A62FB0,
//   AddEnableConsoleVariable @ 0x00BACD80, CCharString @ 0x0099EBF0 / ~ @ 0x0099EAE0, GetVertexShader @ 0x00A5D5F0 / GetPixelShader @ 0x00A5D720,
//   CalculateTextureMemoryFromQuality @ 0x009BEE00 (thiscall on the global at 0x01436E1C; `cmp eax,0x578 / jb`),
//   AllocateAligned @ 0x009F2BE0 (`__fastcall`, size 0xC00, alignment 0x10).
// Layout: engine/CEnginePrimitiveManagerRepeatedStaticMeshes.h offsets hold (0x94 bytes); its shader members are opaque byte arrays,
//   so the class is restated locally with the real shader types (models as the landed 0x00BAD040).
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
struct CBase {
    virtual void VSlot0();
    CBase();                                       // 0x0099A2F0
};
struct IDirect3DVertexBuffer9;
class CVertexCompressionParams { public:
    float PosScaleX, PosScaleY, PosScaleZ, PosScaleW;
    float PosBiasX, PosBiasY, PosBiasZ, PosBiasW;
    CVertexCompressionParams()
        : PosScaleX(1.0f), PosScaleY(1.0f), PosScaleZ(1.0f), PosScaleW(1.0f),
          PosBiasX(0.0f), PosBiasY(0.0f), PosBiasZ(0.0f), PosBiasW(0.0f) { }
};
struct CVertexBufferBase : CBase {                 // 0x34 bytes
    IDirect3DVertexBuffer9*  PVertexBuffer;        // +0x04
    unsigned long            Length;               // +0x08
    long                     Type;                 // +0x0c
    unsigned char            Stride;               // +0x10
    CVertexCompressionParams CompressionParams;    // +0x14
    CVertexBufferBase() : Length(0), Type(0), CompressionParams() { }
};
class CVertexBufferWin32 : public CVertexBufferBase { public:    // vtable 0x0129BA20
    virtual void VSlot1();
    CVertexBufferWin32()
    {
        PVertexBuffer = 0;
        Stride = 0;
    }
};
class CIndexBuffer { public:                              // 0x14 bytes
    void* _vtable;
    unsigned char _body[0x10];
    CIndexBuffer();                                // 0x00AC1EC0
};
struct CEngineDisplay {
};
extern CEngineDisplay* g_pEngineDisplay;           // 0x01436E1C
struct CSIMDVariableBlock;

class CEnginePrimitiveManager { public:                   // 0x0c bytes
    virtual ~CEnginePrimitiveManager();
    unsigned char _body[0x8];
    void AddEnableConsoleVariable(const CCharString& name);  // 0x00BACD80 (FableWin.pdb + Ego_r.pdb name sets)
};
class CEnginePrimitiveGroupManager : public CEnginePrimitiveManager { public:   // 0x20 bytes
    unsigned char _group[0x14];
    CEnginePrimitiveGroupManager(CEngineSceneArray* scenes);   // 0x00BEBC30
};

class CEnginePrimitiveManagerRepeatedStaticMeshes : public CEnginePrimitiveGroupManager { public:   // vtable 0x012A3E9C, 0x94 bytes
    CPixelShader   PixelShader;                                    // +0x20
    CVertexShader  VertexShader;                                   // +0x28
    CPixelShader   PixelShaderStippleAlpha;                        // +0x30
    CVertexShader  VertexShaderStippleAlpha;                       // +0x38
    CPixelShader   PixelShaderDepthBufferShadowed;                 // +0x40
    CVertexShader  VertexShaderDepthBufferShadowed;                // +0x48
    CPixelShader   PixelShaderDepthBufferShadowedStippleAlpha;     // +0x50
    CVertexShader  VertexShaderDepthBufferShadowedStippleAlpha;    // +0x58
    CPixelShader   PixelShaderColourBufferShadowed;                // +0x60
    CVertexShader  VertexShaderColourBufferShadowed;               // +0x68
    CPixelShader   PixelShaderColourBufferShadowedStippleAlpha;    // +0x70
    CVertexShader  VertexShaderColourBufferShadowedStippleAlpha;   // +0x78
    CPixelShader*  PixelShaderShadowed;                            // +0x80
    CVertexShader* VertexShaderShadowed;                           // +0x84
    CPixelShader*  PixelShaderShadowedStippleAlpha;                // +0x88
    CVertexShader* VertexShaderShadowedStippleAlpha;               // +0x8c
    CSIMDVariableBlock* SIMDVariableBlock;                         // +0x90
    CEnginePrimitiveManagerRepeatedStaticMeshes(CEngineSceneArray* scenes);
    virtual ~CEnginePrimitiveManagerRepeatedStaticMeshes();
};

// Naming (NF438 gen, 2026-11-25): callee-only rebind of this row's calls to their PDB spellings (T1 tracker; the target
//   self rows' Leaf / PDB name sets), through a bodiless declaration (no layout; rulings r2/r3):
//   0x009BEE00 ?GetPixelShaderVersion@CDisplayManager@@QBEKXZ (was ?CalculateTextureMemoryFromQuality@CEngineDisplay@@QAEKXZ)
class CDisplayManager { public: unsigned long GetPixelShaderVersion() const; };
// Naming (NF443 gen, 2026-11-25): callee-only rebind of this row's calls to their PDB spellings (T1 tracker; the target
//   self rows' Leaf / PDB name sets), through a bodiless declaration or the row's own model (no layout; rulings r2/r3, NF438 q1 friend):
//   0x009F2BE0 ?AllocateAligned@CMemoryAllocator@@SIPAXKJ@Z (was ?AllocateAligned@@YIPAXKK@Z)
class CMemoryAllocator { public: static void* __fastcall AllocateAligned(unsigned long, long); };
CEnginePrimitiveManagerRepeatedStaticMeshes::CEnginePrimitiveManagerRepeatedStaticMeshes(CEngineSceneArray* scenes)
    : CEnginePrimitiveGroupManager(scenes)
{
    AddEnableConsoleVariable("EnableRepeatedMeshes");
    PixelShader = NEngineInternal::GPResourceManager->GetPShaderResource()->GetPixelShader("PSHADER_REPEATED_MESH");
    VertexShader = NEngineInternal::GPResourceManager->GetVShaderResource((EShaderBankNo)14)->GetVertexShader("VSHADER_REPEATED_MESH");
    PixelShaderStippleAlpha = NEngineInternal::GPResourceManager->GetPShaderResource()->GetPixelShader("PSHADER_REPEATED_MESH_STIPPLE_ALPHA");
    VertexShaderStippleAlpha = NEngineInternal::GPResourceManager->GetVShaderResource((EShaderBankNo)14)->GetVertexShader("VSHADER_REPEATED_MESH_STIPPLE_ALPHA");
    PixelShaderDepthBufferShadowed = NEngineInternal::GPResourceManager->GetPShaderResource()->GetPixelShader("PSHADER_REPEATED_MESH_SHADOWED");
    VertexShaderDepthBufferShadowed = NEngineInternal::GPResourceManager->GetVShaderResource((EShaderBankNo)14)->GetVertexShader("VSHADER_REPEATED_MESH_SHADOWED");
    PixelShaderDepthBufferShadowedStippleAlpha = NEngineInternal::GPResourceManager->GetPShaderResource()->GetPixelShader("PSHADER_REPEATED_MESH_SHADOWED_STIPPLE_ALPHA");
    VertexShaderDepthBufferShadowedStippleAlpha = NEngineInternal::GPResourceManager->GetVShaderResource((EShaderBankNo)14)->GetVertexShader("VSHADER_REPEATED_MESH_SHADOWED_STIPPLE_ALPHA");
    if (reinterpret_cast<const CDisplayManager*>(g_pEngineDisplay)->GetPixelShaderVersion() >= 1400)
    {
        PixelShaderColourBufferShadowed = NEngineInternal::GPResourceManager->GetPShaderResource()->GetPixelShader("PSHADER_REPEATED_MESH_COLBUFF_SHADOWED");
        VertexShaderColourBufferShadowed = NEngineInternal::GPResourceManager->GetVShaderResource((EShaderBankNo)14)->GetVertexShader("VSHADER_REPEATED_MESH_COLBUFF_SHADOWED");
        PixelShaderColourBufferShadowedStippleAlpha = NEngineInternal::GPResourceManager->GetPShaderResource()->GetPixelShader("PSHADER_REPEATED_MESH_COLBUFF_SHADOWED_STIPPLE_ALPHA");
        VertexShaderColourBufferShadowedStippleAlpha = NEngineInternal::GPResourceManager->GetVShaderResource((EShaderBankNo)14)->GetVertexShader("VSHADER_REPEATED_MESH_COLBUFF_SHADOWED_STIPPLE_ALPHA");
    }
    PixelShaderShadowed = &PixelShaderDepthBufferShadowed;
    VertexShaderShadowed = &VertexShaderDepthBufferShadowed;
    PixelShaderShadowedStippleAlpha = &PixelShaderDepthBufferShadowedStippleAlpha;
    VertexShaderShadowedStippleAlpha = &VertexShaderDepthBufferShadowedStippleAlpha;
    SIMDVariableBlock = (CSIMDVariableBlock*)CMemoryAllocator::AllocateAligned(0xc00, 0x10);
}