// CEnginePrimitiveManagerParticleDecalGroup::CEnginePrimitiveManagerParticleDecalGroup(CEngineSceneArray* scenes) -- constructs the particle decal group manager: shader / vertex buffer / index buffer members, registers EnableGroupDecals, fills the point-light shader index table and looks up its landscape / mesh decal vertex and pixel shaders by name
// Leaf: ??0CEnginePrimitiveManagerParticleDecalGroup
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BB6E00, length 1841 bytes (`ret 4`, no EH frame).
// Callees: CEnginePrimitiveManager ctor @ 0x00BACDB0, CVertexShader() @ 0x00A62FB0 (eh-free array loops), CPixelShader() @ 0x00A5EC00,
//   CBase() @ 0x0099A2F0, CIndexBuffer() @ 0x00AC1EC0, RegisterEnable @ 0x00BACD80, CCharString @ 0x0099EBF0 / ~ @ 0x0099EAE0,
//   CShaderResource::GetVertexShader @ 0x00A5D5F0 / GetPixelShader @ 0x00A5D720. Vtable 0x012A5998.
// Layout: engine/CEnginePrimitiveManagerParticleDecalGroup.h offsets hold (0x124 bytes) but its members are opaque byte arrays;
//   the class is restated locally with the real shader / buffer types (models as the landed 0x00BABB50 / 0x00BAD040).
// Shape: the decal vertex resource (+0x29C0) is read once into a named local ahead of the first name temporary (retail edi);
//   the occlusion vertex (+0x2A60) and pixel (+0x2A98) resources go through inline getters so each is loaded right before the pushes.
struct CCharString {
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

struct CShaderBase {                               // vtable 0x0129A030
    virtual ~CShaderBase() { }
};
struct CVertexShader : CShaderBase {               // vtable 0x0129B3D0, 8 bytes
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
struct CPixelShader : CShaderBase {                // vtable 0x0129A054, 8 bytes
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

struct CShaderResource {
    CVertexShader GetVertexShader(const CCharString& name);   // 0x00A5D5F0
    CPixelShader  GetPixelShader(const CCharString& name);    // 0x00A5D720
};
struct CEngineData {
    unsigned char    _pad0000[0x29c0];
    CShaderResource* DecalVertexShaderResource;    // +0x29C0
    unsigned char    _pad29c4[0x2a60 - 0x29c4];
    CShaderResource* OcclusionVertexShaderResource;   // +0x2A60
    unsigned char    _pad2a64[0x2a98 - 0x2a64];
    CShaderResource* PixelShaderResource;          // +0x2A98
    CShaderResource* GetOcclusionVertexShaderResource() { return OcclusionVertexShaderResource; }
    CShaderResource* GetPixelShaderResource() { return PixelShaderResource; }
};
extern CEngineData* g_pEngineData;                 // 0x01436E98
struct CEngineSceneArray;
struct CBase {
    virtual void VSlot0();
    CBase();                                       // 0x0099A2F0
};
struct IDirect3DVertexBuffer9;
struct CVertexCompressionParams {
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
struct CVertexBufferWin32 : CVertexBufferBase {    // vtable 0x0129BA20
    virtual void VSlot1();
    CVertexBufferWin32()
    {
        PVertexBuffer = 0;
        Stride = 0;
    }
};
struct CIndexBuffer {                              // 0x14 bytes
    void* _vtable;
    unsigned char _body[0x10];
    CIndexBuffer();                                // 0x00AC1EC0
};
struct CEnginePrimitiveManager {                   // 0x0c bytes
    virtual ~CEnginePrimitiveManager();
    unsigned char _body[0x8];
    CEnginePrimitiveManager(CEngineSceneArray* scenes);   // 0x00BACDB0
    void RegisterEnable(const CCharString& name);  // 0x00BACD80
};

struct CEnginePrimitiveManagerParticleDecalGroup : CEnginePrimitiveManager {   // vtable 0x012A5998, 0x124 bytes
    unsigned long      PointLightShaderIndex[6];                // +0x0c
    CVertexShader      LandscapeDecalVertexShader[3];           // +0x24
    CVertexShader      MeshDecalVertexShader[3];                // +0x3c
    CVertexShader      MeshDecalTwoPassStencilVertexShader;     // +0x54
    CVertexShader      MeshDecalOnePassStencilVertexShader;     // +0x5c
    CVertexShader      MeshDecalStencilClearVertexShader;       // +0x64
    CPixelShader       LandscapeDecalPixelShader;               // +0x6c
    CPixelShader       MeshDecalPixelShader;                    // +0x74
    CPixelShader       MeshDecalTwoPassStencilPixelShader;      // +0x7c
    CPixelShader       MeshDecalOnePassStencilPixelShader;      // +0x84
    CPixelShader       MeshDecalStencilClearPixelShader;        // +0x8c
    CVertexBufferWin32 LandscapeDecalVertexBuffer;              // +0x94
    CIndexBuffer       LandscapeDecalIndexBuffer;               // +0xc8
    CVertexBufferWin32 MeshDecalVertexBuffer;                   // +0xdc
    CIndexBuffer       MeshDecalIndexBuffer;                    // +0x110
    CEnginePrimitiveManagerParticleDecalGroup(CEngineSceneArray* scenes);
    virtual ~CEnginePrimitiveManagerParticleDecalGroup();
};

CEnginePrimitiveManagerParticleDecalGroup::CEnginePrimitiveManagerParticleDecalGroup(CEngineSceneArray* scenes)
    : CEnginePrimitiveManager(scenes)
{
    RegisterEnable("EnableGroupDecals");
    PointLightShaderIndex[0] = 0;
    PointLightShaderIndex[1] = 1;
    PointLightShaderIndex[2] = 1;
    PointLightShaderIndex[3] = 1;
    PointLightShaderIndex[4] = 2;
    PointLightShaderIndex[5] = 2;
    CShaderResource* vertexResource = g_pEngineData->DecalVertexShaderResource;
    LandscapeDecalVertexShader[0] = vertexResource->GetVertexShader("VSHADER_LANDSCAPE_DECAL_DIRLIGHT");
    LandscapeDecalVertexShader[1] = vertexResource->GetVertexShader("VSHADER_LANDSCAPE_DECAL_3POINTLIGHTS");
    LandscapeDecalVertexShader[2] = vertexResource->GetVertexShader("VSHADER_LANDSCAPE_DECAL_5POINTLIGHTS");
    MeshDecalVertexShader[0] = vertexResource->GetVertexShader("VSHADER_MESH_DECAL_DIRLIGHT");
    MeshDecalVertexShader[1] = vertexResource->GetVertexShader("VSHADER_MESH_DECAL_3POINTLIGHTS");
    MeshDecalVertexShader[2] = vertexResource->GetVertexShader("VSHADER_MESH_DECAL_5POINTLIGHTS");
    MeshDecalTwoPassStencilVertexShader = vertexResource->GetVertexShader("VSHADER_MESH_DECAL_STENCIL_TWO_PASS");
    MeshDecalStencilClearVertexShader = g_pEngineData->GetOcclusionVertexShaderResource()->GetVertexShader("VSHADER_OCCLUSION_BOX");
    LandscapeDecalPixelShader = g_pEngineData->GetPixelShaderResource()->GetPixelShader("PSHADER_DECAL");
    MeshDecalPixelShader = g_pEngineData->GetPixelShaderResource()->GetPixelShader("PSHADER_DECAL");
    MeshDecalTwoPassStencilPixelShader = g_pEngineData->GetPixelShaderResource()->GetPixelShader("PSHADER_DECAL_STENCIL_TWO_PASS");
    MeshDecalStencilClearPixelShader = g_pEngineData->GetPixelShaderResource()->GetPixelShader("PSHADER_DECAL_STENCIL_TWO_PASS");
}
