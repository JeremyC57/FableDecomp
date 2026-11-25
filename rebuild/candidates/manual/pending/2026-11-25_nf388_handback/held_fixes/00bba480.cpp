// CEnginePrimitiveManagerDecal::CEnginePrimitiveManagerDecal(CEngineSceneArray* scenes) -- constructs the decal manager: mesh / landscape decal buffers and shader members, registers EnableDecals, fills the point-light shader index table and looks up the decal vertex and pixel shaders by name
// Leaf: ??0CEnginePrimitiveManagerDecal
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BBA480, length 1952 bytes (`ret 4`, no EH frame). Vtable 0x012A5B6C.
// Callees: as the particle decal group constructor 0x00BB6E00 (same shader names except PSHADER_TEXTURE_DIFFUSE for the landscape and
//   mesh decal pixel shaders and the EnableDecals switch).
// Layout: no engine/ header exists; members and offsets from Ego_r.pdb (struct_layouts_egor.tsv, 0x124 bytes): MeshDecal vertex /
//   index buffers lead at +0x0c / +0x40, so the shader block sits 0x48 above the particle decal group's.
// Shape: VC7.1 keeps the CVertexShader[3] constructor-loop counter in a frame slot here (ebx already holds zero).
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
class CBaseClassNonCopyable {
protected:
    virtual ~CBaseClassNonCopyable();   // 0x0099A300
    CBaseClassNonCopyable();                                       // 0x0099A2F0
};
struct IDirect3DVertexBuffer9;
class CVertexCompressionParams { public:
    float PosScaleX, PosScaleY, PosScaleZ, PosScaleW;
    float PosBiasX, PosBiasY, PosBiasZ, PosBiasW;
    CVertexCompressionParams()
        : PosScaleX(1.0f), PosScaleY(1.0f), PosScaleZ(1.0f), PosScaleW(1.0f),
          PosBiasX(0.0f), PosBiasY(0.0f), PosBiasZ(0.0f), PosBiasW(0.0f) { }
};
struct CVertexBufferBase : CBaseClassNonCopyable {                 // 0x34 bytes
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
class CEnginePrimitiveManager { public:                   // 0x0c bytes
    virtual ~CEnginePrimitiveManager();
    unsigned char _body[0x8];
    CEnginePrimitiveManager(CEngineSceneArray* scenes);   // 0x00BACDB0
    void AddEnableConsoleVariable(const CCharString& name);  // 0x00BACD80
};

class CEnginePrimitiveManagerDecal : public CEnginePrimitiveManager { public:   // vtable 0x012A5B6C, 0x124 bytes
    CVertexBufferWin32 MeshDecalVertexBuffer;                   // +0x0c
    CIndexBuffer       MeshDecalIndexBuffer;                    // +0x40
    unsigned long      PointLightShaderIndex[6];                // +0x54
    CVertexShader      LandscapeDecalVertexShader[3];           // +0x6c
    CVertexShader      MeshDecalVertexShader[3];                // +0x84
    CVertexShader      MeshDecalTwoPassStencilVertexShader;     // +0x9c
    CVertexShader      MeshDecalOnePassStencilVertexShader;     // +0xa4
    CVertexShader      MeshDecalStencilClearVertexShader;       // +0xac
    CPixelShader       LandscapeDecalPixelShader;               // +0xb4
    CPixelShader       MeshDecalPixelShader;                    // +0xbc
    CPixelShader       MeshDecalTwoPassStencilPixelShader;      // +0xc4
    CPixelShader       MeshDecalOnePassStencilPixelShader;      // +0xcc
    CPixelShader       MeshDecalStencilClearPixelShader;        // +0xd4
    CVertexBufferWin32 LandscapeDecalVertexBuffer;              // +0xdc
    CIndexBuffer       LandscapeDecalIndexBuffer;               // +0x110
    CEnginePrimitiveManagerDecal(CEngineSceneArray* scenes);
    virtual ~CEnginePrimitiveManagerDecal();
};

CEnginePrimitiveManagerDecal::CEnginePrimitiveManagerDecal(CEngineSceneArray* scenes)
    : CEnginePrimitiveManager(scenes)
{
    AddEnableConsoleVariable("EnableDecals");
    PointLightShaderIndex[0] = 0;
    PointLightShaderIndex[1] = 1;
    PointLightShaderIndex[2] = 1;
    PointLightShaderIndex[3] = 1;
    PointLightShaderIndex[4] = 2;
    PointLightShaderIndex[5] = 2;
    const CShaderResource* vertexResource = NEngineInternal::GPResourceManager->GetVShaderResource((EShaderBankNo)0);
    LandscapeDecalVertexShader[0] = vertexResource->GetVertexShader("VSHADER_LANDSCAPE_DECAL_DIRLIGHT");
    LandscapeDecalVertexShader[1] = vertexResource->GetVertexShader("VSHADER_LANDSCAPE_DECAL_DIRLIGHT_3POINTLIGHTS");
    LandscapeDecalVertexShader[2] = vertexResource->GetVertexShader("VSHADER_LANDSCAPE_DECAL_DIRLIGHT_5POINTLIGHTS");
    MeshDecalVertexShader[0] = vertexResource->GetVertexShader("VSHADER_MESH_DECAL_DIRLIGHT");
    MeshDecalVertexShader[1] = vertexResource->GetVertexShader("VSHADER_MESH_DECAL_DIRLIGHT_3POINTLIGHTS");
    MeshDecalVertexShader[2] = vertexResource->GetVertexShader("VSHADER_MESH_DECAL_DIRLIGHT_5POINTLIGHTS");
    MeshDecalTwoPassStencilVertexShader = vertexResource->GetVertexShader("VSHADER_MESH_DECAL_STENCIL_TWO_PASS");
    MeshDecalStencilClearVertexShader = NEngineInternal::GPResourceManager->GetVShaderResource((EShaderBankNo)20)->GetVertexShader("VSHADER_OCCLUSION_BOX");
    LandscapeDecalPixelShader = NEngineInternal::GPResourceManager->GetPShaderResource()->GetPixelShader("PSHADER_TEXTURE_DIFFUSE");
    MeshDecalPixelShader = NEngineInternal::GPResourceManager->GetPShaderResource()->GetPixelShader("PSHADER_TEXTURE_DIFFUSE");
    MeshDecalTwoPassStencilPixelShader = NEngineInternal::GPResourceManager->GetPShaderResource()->GetPixelShader("PSHADER_DECAL_STENCIL_TWO_PASS");
    MeshDecalStencilClearPixelShader = NEngineInternal::GPResourceManager->GetPShaderResource()->GetPixelShader("PSHADER_DECAL_STENCIL_TWO_PASS");
}