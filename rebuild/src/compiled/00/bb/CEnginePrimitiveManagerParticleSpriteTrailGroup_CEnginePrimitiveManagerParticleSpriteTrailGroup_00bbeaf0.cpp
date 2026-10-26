// CEnginePrimitiveManagerParticleSpriteTrailGroup::CEnginePrimitiveManagerParticleSpriteTrailGroup(CEngineSceneArray* scenes, const CCountedPointer<CEnginePrimitiveManagerSpriteTrails>& trailManager) -- constructs the sprite-trail group manager: keeps a counted reference to the trail manager, sizes the point-light vertex shader table to five and looks up its vertex and pixel shaders by name
// Leaf: ??0CEnginePrimitiveManagerParticleSpriteTrailGroup
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BBEAF0, length 1299 bytes (`ret 8`, no EH frame). Vtable 0x012A5D94.
// Callees: CEnginePrimitiveManager ctor @ 0x00BACDB0, CVertexShader() @ 0x00A62FB0, CPixelShader() @ 0x00A5EC00,
//   vector<CVertexShader>::resize @ 0x00B6CBD0, CCharString @ 0x0099EBF0 / ~ @ 0x0099EAE0, GetVertexShader @ 0x00A5D5F0 /
//   GetPixelShader @ 0x00A5D720.
// Layout: engine/CEnginePrimitiveManagerParticleSpriteTrailGroup.h has the PDB 16-byte CArray at +0x0c; retail's STLport vector is
//   12 bytes, so every later member sits 4 lower (VertexShaderDisplacement +0x18 ... PTrailManager +0x38 {Data, RefCount}).
// Shape: as the landed line-group constructor 0x00BC4D90; the counted pointer is copied in the mem-initialiser list.
class CCharString { public:
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                // 0x0099EAE0
    void* m_pData;
};

struct CVertexShaderData {
    virtual ~CVertexShaderData();                  // slot 0
    void*         VertexDeclaration;
    void*         VertexShader;
    const void*   FormatDecl;
    long          ReferenceCount;                  // +0x10
    long RemoveReference() { return --ReferenceCount; }
    void ReleaseReference() { if (--ReferenceCount == 0) delete this; }
};
struct CPixelShaderData {
    virtual ~CPixelShaderData();                   // slot 0
    void*         PixelShader;
    long          ReferenceCount;                  // +0x08
    long RemoveReference() { return --ReferenceCount; }
    void ReleaseReference() { if (--ReferenceCount == 0) delete this; }
};

class CShaderBase { public:                               // vtable 0x0129A030
    virtual ~CShaderBase() { }
};
class CVertexShader : public CShaderBase { public:               // vtable 0x0129B3D0, 8 bytes
    CVertexShaderData* Data;
    CVertexShader();                               // 0x00A62FB0
    CVertexShader(const CVertexShader& other) : Data(other.Data) { if (Data) ++Data->ReferenceCount; }
    CVertexShader& operator=(const CVertexShader& other)
    {
        if (Data) Data->ReleaseReference();
        Data = other.Data;
        ++Data->ReferenceCount;
        return *this;
    }
    virtual ~CVertexShader() { if (Data) { CVertexShaderData* current = Data; if (!current->RemoveReference()) delete current; } }
};
class CPixelShader : public CShaderBase { public:                // vtable 0x0129A054, 8 bytes
    CPixelShaderData* Data;
    CPixelShader();                                // 0x00A5EC00
    CPixelShader(const CPixelShader& other) : Data(other.Data) { if (Data) ++Data->ReferenceCount; }
    CPixelShader& operator=(const CPixelShader& other)
    {
        if (Data) { Data->ReleaseReference(); Data = 0; }
        Data = other.Data;
        ++Data->ReferenceCount;
        return *this;
    }
    virtual ~CPixelShader() { if (Data) { CPixelShaderData* current = Data; if (!current->RemoveReference()) delete current; } }
};

class CShaderResource { public:
    CVertexShader GetVertexShader(const CCharString& name);   // 0x00A5D5F0
    CPixelShader  GetPixelShader(const CCharString& name);    // 0x00A5D720
};
struct CEngineData {
    unsigned char    _pad0000[0x2a80];
    CShaderResource* SpriteTrailVertexShaderResource; // +0x2A80
    unsigned char    _pad2a84[0x2a98 - 0x2a84];
    CShaderResource* PixelShaderResource;          // +0x2A98
    CShaderResource* GetSpriteTrailVertexShaderResource() { return SpriteTrailVertexShaderResource; }
    CShaderResource* GetPixelShaderResource() { return PixelShaderResource; }
};
extern CEngineData* g_pEngineData;                 // 0x01436E98
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
template <class T> class CCountedPointer { public:
    T*    Data;
    long* RefCount;
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), RefCount(other.RefCount) { if (RefCount) ++*RefCount; }
};

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
    VertexShadersSpriteTrailPointLights[0] = g_pEngineData->GetSpriteTrailVertexShaderResource()->GetVertexShader("VSHADER_SPRITE_TRAIL_GROUP_DIRLIGHT_FOG");
    VertexShadersSpriteTrailPointLights[1] = g_pEngineData->GetSpriteTrailVertexShaderResource()->GetVertexShader("VSHADER_SPRITE_TRAIL_GROUP_DIRLIGHT_3POINTLIGHTS_FOG");
    VertexShadersSpriteTrailPointLights[2] = g_pEngineData->GetSpriteTrailVertexShaderResource()->GetVertexShader("VSHADER_SPRITE_TRAIL_GROUP_DIRLIGHT_3POINTLIGHTS_FOG");
    VertexShadersSpriteTrailPointLights[3] = g_pEngineData->GetSpriteTrailVertexShaderResource()->GetVertexShader("VSHADER_SPRITE_TRAIL_GROUP_DIRLIGHT_3POINTLIGHTS_FOG");
    VertexShadersSpriteTrailPointLights[4] = g_pEngineData->GetSpriteTrailVertexShaderResource()->GetVertexShader("VSHADER_SPRITE_TRAIL_GROUP_DIRLIGHT_5POINTLIGHTS_FOG");
    VertexShaderDisplacement = g_pEngineData->GetSpriteTrailVertexShaderResource()->GetVertexShader("VSHADER_SPRITE_TRAIL_GROUP_DISPLACEMENT");
    PixelShader = g_pEngineData->GetPixelShaderResource()->GetPixelShader("PSHADER_SPRITE_GROUP");
    PixelShaderStipple = g_pEngineData->GetPixelShaderResource()->GetPixelShader("PSHADER_SPRITE_GROUP_STIPPLE");
    PixelShaderDisplacement = g_pEngineData->GetPixelShaderResource()->GetPixelShader("PSHADER_SPRITE_GROUP_DISPLACEMENT");
}