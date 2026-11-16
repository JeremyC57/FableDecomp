// CEnginePrimitiveManagerParticleSpriteTrailGroup::CEnginePrimitiveManagerParticleSpriteTrailGroup(CEngineSceneArray* scenes, const CCountedPointer<CEnginePrimitiveManagerSpriteTrails>& trailManager) -- constructs the sprite-trail group manager: keeps a counted reference to the trail manager, sizes the point-light vertex shader table to five and looks up its vertex and pixel shaders by name
// Leaf: ??0CEnginePrimitiveManagerParticleSpriteTrailGroup
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8). Also: the local CCountedPointer members RefCount->Info are the header CCountedPointer's (same offsets).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BBEAF0, length 1299 bytes (`ret 8`, no EH frame). Vtable 0x012A5D94.
// Callees: CEnginePrimitiveManager ctor @ 0x00BACDB0, CVertexShader() @ 0x00A62FB0, CPixelShader() @ 0x00A5EC00,
//   vector<CVertexShader>::resize @ 0x00B6CBD0, CCharString @ 0x0099EBF0 / ~ @ 0x0099EAE0, GetVertexShader @ 0x00A5D5F0 /
//   GetPixelShader @ 0x00A5D720.
// Layout: engine/CEnginePrimitiveManagerParticleSpriteTrailGroup.h has the PDB 16-byte CArray at +0x0c; retail's STLport vector is
//   12 bytes, so every later member sits 4 lower (VertexShaderDisplacement +0x18 ... PTrailManager +0x38 {Data, RefCount}).
// Shape: as the landed line-group constructor 0x00BC4D90; the counted pointer is copied in the mem-initialiser list.
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
class CEnginePrimitiveManager { public:                   // 0x0c bytes
    virtual ~CEnginePrimitiveManager();
    unsigned char _body[0x8];
    CEnginePrimitiveManager(CEngineSceneArray* scenes);   // 0x00BACDB0
};
class CEnginePrimitiveManagerSpriteTrails;
#include "engine/CCountedPointer.h"

class CEnginePrimitiveManagerParticleSpriteTrailGroup : public CEnginePrimitiveManager { public:   // vtable 0x012A5D94
    CVertexShaderArray VertexShadersSpriteTrailPointLights;   // +0x0c
    CVertexShader      VertexShaderDisplacement;   // +0x18
    CPixelShader       PixelShader;                // +0x20
    CPixelShader       PixelShaderStipple;         // +0x28
    CPixelShader       PixelShaderDisplacement;    // +0x30
    CCountedPointer<CEnginePrimitiveManagerSpriteTrails> PTrailManager;   // +0x38
    CEnginePrimitiveManagerParticleSpriteTrailGroup(CEngineSceneArray* scenes, const CCountedPointer<CEnginePrimitiveManagerSpriteTrails>& trailManager);
    virtual ~CEnginePrimitiveManagerParticleSpriteTrailGroup();
};

CEnginePrimitiveManagerParticleSpriteTrailGroup::CEnginePrimitiveManagerParticleSpriteTrailGroup(CEngineSceneArray* scenes, const CCountedPointer<CEnginePrimitiveManagerSpriteTrails>& trailManager)
    : CEnginePrimitiveManager(scenes), PTrailManager(trailManager)
{
    VertexShadersSpriteTrailPointLights.resize(5);
    VertexShadersSpriteTrailPointLights[0] = NEngineInternal::GPResourceManager->GetVShaderResource((EShaderBankNo)24)->GetVertexShader("VSHADER_SPRITE_TRAIL_GROUP_DIRLIGHT_FOG");
    VertexShadersSpriteTrailPointLights[1] = NEngineInternal::GPResourceManager->GetVShaderResource((EShaderBankNo)24)->GetVertexShader("VSHADER_SPRITE_TRAIL_GROUP_DIRLIGHT_3POINTLIGHTS_FOG");
    VertexShadersSpriteTrailPointLights[2] = NEngineInternal::GPResourceManager->GetVShaderResource((EShaderBankNo)24)->GetVertexShader("VSHADER_SPRITE_TRAIL_GROUP_DIRLIGHT_3POINTLIGHTS_FOG");
    VertexShadersSpriteTrailPointLights[3] = NEngineInternal::GPResourceManager->GetVShaderResource((EShaderBankNo)24)->GetVertexShader("VSHADER_SPRITE_TRAIL_GROUP_DIRLIGHT_3POINTLIGHTS_FOG");
    VertexShadersSpriteTrailPointLights[4] = NEngineInternal::GPResourceManager->GetVShaderResource((EShaderBankNo)24)->GetVertexShader("VSHADER_SPRITE_TRAIL_GROUP_DIRLIGHT_5POINTLIGHTS_FOG");
    VertexShaderDisplacement = NEngineInternal::GPResourceManager->GetVShaderResource((EShaderBankNo)24)->GetVertexShader("VSHADER_SPRITE_TRAIL_GROUP_DISPLACEMENT");
    PixelShader = NEngineInternal::GPResourceManager->GetPShaderResource()->GetPixelShader("PSHADER_SPRITE_GROUP");
    PixelShaderStipple = NEngineInternal::GPResourceManager->GetPShaderResource()->GetPixelShader("PSHADER_SPRITE_GROUP_STIPPLE");
    PixelShaderDisplacement = NEngineInternal::GPResourceManager->GetPShaderResource()->GetPixelShader("PSHADER_SPRITE_GROUP_DISPLACEMENT");
}