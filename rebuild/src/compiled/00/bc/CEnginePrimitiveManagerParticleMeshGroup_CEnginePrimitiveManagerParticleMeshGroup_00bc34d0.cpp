// CEnginePrimitiveManagerParticleMeshGroup::CEnginePrimitiveManagerParticleMeshGroup(CEngineSceneArray* scenes) -- constructs the particle mesh group manager: sizes the point-light vertex shader table to five and looks up the mesh-group vertex and pixel shaders by name
// Leaf: ??0CEnginePrimitiveManagerParticleMeshGroup
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BC34D0, length 1520 bytes (`ret 4`, no EH frame). Vtable 0x012A63B8.
// Callees: CEnginePrimitiveGroupManager ctor @ 0x00BEBC30, CVertexShader() @ 0x00A62FB0, CPixelShader() @ 0x00A5EC00,
//   vector<CVertexShader>::resize @ 0x00B6CBD0, CCharString @ 0x0099EBF0 / ~ @ 0x0099EAE0, GetVertexShader @ 0x00A5D5F0 /
//   GetPixelShader @ 0x00A5D720.
// Layout: engine/CEnginePrimitiveManagerParticleMeshGroup.h has the PDB 16-byte CArray at +0x20; retail's STLport vector is 12 bytes,
//   so every later member sits 4 lower (VertexShaderDisplacement +0x2c ... PixelShaderDisplacement +0x54), restated locally.
// Shape: as the landed line-group constructor 0x00BC4D90 (signed-index `*(begin() + index)` element access, real _Vector_base).
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
template <class T> struct allocator { allocator() { } };
template <class V, class A> struct _STLP_alloc_proxy : A {
    V _M_data;
    _STLP_alloc_proxy(const A& a, V v) : A(a), _M_data(v) { }
};
template <class T> struct _Vector_base {
    T* _M_start;
    T* _M_finish;
    _STLP_alloc_proxy<T*, allocator<T> > _M_end_of_storage;
    _Vector_base(const allocator<T>& a) : _M_start(0), _M_finish(0), _M_end_of_storage(a, (T*)0) { }
};
struct CVertexShaderArray : _Vector_base<CVertexShader> {   // retail 12-byte STLport vector<CVertexShader>
    CVertexShaderArray(const allocator<CVertexShader>& a = allocator<CVertexShader>()) : _Vector_base<CVertexShader>(a) { }
    void resize(unsigned int count);               // 0x00B6CBD0
    CVertexShader* begin() { return _M_start; }
    CVertexShader& operator[](long index) { return *(begin() + index); }
};
class CEnginePrimitiveGroupManager { public:              // 0x20 bytes
    virtual ~CEnginePrimitiveGroupManager();
    unsigned char _body[0x1c];
    CEnginePrimitiveGroupManager(CEngineSceneArray* scenes);   // 0x00BEBC30
};
struct CEngineParticleConstantBlock { unsigned char _body[0x10]; };

class CEnginePrimitiveManagerParticleMeshGroup : public CEnginePrimitiveGroupManager { public:   // vtable 0x012A63B8
    CVertexShaderArray VertexShadersMeshPointLights;   // +0x20
    CVertexShader      VertexShaderDisplacement;   // +0x2c
    CPixelShader       PixelShader;                // +0x34
    CPixelShader       PixelShaderStipple;         // +0x3c
    CPixelShader       PixelShaderAdditiveAlpha;   // +0x44
    CPixelShader       PixelShaderAdditiveAlphaStipple;   // +0x4c
    CPixelShader       PixelShaderDisplacement;    // +0x54
    CEngineParticleConstantBlock ConstantBlock[40];    // +0x5c
    CEnginePrimitiveManagerParticleMeshGroup(CEngineSceneArray* scenes);
    virtual ~CEnginePrimitiveManagerParticleMeshGroup();
};

CEnginePrimitiveManagerParticleMeshGroup::CEnginePrimitiveManagerParticleMeshGroup(CEngineSceneArray* scenes)
    : CEnginePrimitiveGroupManager(scenes)
{
    VertexShadersMeshPointLights.resize(5);
    VertexShadersMeshPointLights[0] = NEngineInternal::GPResourceManager->GetVShaderResource((EShaderBankNo)23)->GetVertexShader("VSHADER_MESH_GROUP_DIRLIGHT_FOG");
    VertexShadersMeshPointLights[1] = NEngineInternal::GPResourceManager->GetVShaderResource((EShaderBankNo)23)->GetVertexShader("VSHADER_MESH_GROUP_DIRLIGHT_3POINTLIGHTS_FOG");
    VertexShadersMeshPointLights[2] = NEngineInternal::GPResourceManager->GetVShaderResource((EShaderBankNo)23)->GetVertexShader("VSHADER_MESH_GROUP_DIRLIGHT_3POINTLIGHTS_FOG");
    VertexShadersMeshPointLights[3] = NEngineInternal::GPResourceManager->GetVShaderResource((EShaderBankNo)23)->GetVertexShader("VSHADER_MESH_GROUP_DIRLIGHT_3POINTLIGHTS_FOG");
    VertexShadersMeshPointLights[4] = NEngineInternal::GPResourceManager->GetVShaderResource((EShaderBankNo)23)->GetVertexShader("VSHADER_MESH_GROUP_DIRLIGHT_5POINTLIGHTS_FOG");
    VertexShaderDisplacement = NEngineInternal::GPResourceManager->GetVShaderResource((EShaderBankNo)23)->GetVertexShader("VSHADER_MESH_GROUP_DISPLACEMENT");
    PixelShader = NEngineInternal::GPResourceManager->GetPShaderResource()->GetPixelShader("PSHADER_MESH_GROUP");
    PixelShaderStipple = NEngineInternal::GPResourceManager->GetPShaderResource()->GetPixelShader("PSHADER_MESH_GROUP_STIPPLE");
    PixelShaderAdditiveAlpha = NEngineInternal::GPResourceManager->GetPShaderResource()->GetPixelShader("PSHADER_MESH_GROUP_ADDITIVE_ALPHA");
    PixelShaderAdditiveAlphaStipple = NEngineInternal::GPResourceManager->GetPShaderResource()->GetPixelShader("PSHADER_MESH_GROUP_ADDITIVE_ALPHA_STIPPLE");
    PixelShaderDisplacement = NEngineInternal::GPResourceManager->GetPShaderResource()->GetPixelShader("PSHADER_MESH_GROUP_DISPLACEMENT");
}