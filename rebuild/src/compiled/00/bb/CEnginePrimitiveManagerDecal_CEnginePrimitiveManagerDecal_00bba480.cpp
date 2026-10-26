// CEnginePrimitiveManagerDecal::CEnginePrimitiveManagerDecal(CEngineSceneArray* scenes) -- constructs the decal manager: mesh / landscape decal buffers and shader members, registers EnableDecals, fills the point-light shader index table and looks up the decal vertex and pixel shaders by name
// Leaf: ??0CEnginePrimitiveManagerDecal
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BBA480, length 1952 bytes (`ret 4`, no EH frame). Vtable 0x012A5B6C.
// Callees: as the particle decal group constructor 0x00BB6E00 (same shader names except PSHADER_TEXTURE_DIFFUSE for the landscape and
//   mesh decal pixel shaders and the EnableDecals switch).
// Layout: no engine/ header exists; members and offsets from Ego_r.pdb (struct_layouts_egor.tsv, 0x124 bytes): MeshDecal vertex /
//   index buffers lead at +0x0c / +0x40, so the shader block sits 0x48 above the particle decal group's.
// Shape: VC7.1 keeps the CVertexShader[3] constructor-loop counter in a frame slot here (ebx already holds zero).
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
class CEnginePrimitiveManager { public:                   // 0x0c bytes
    virtual ~CEnginePrimitiveManager();
    unsigned char _body[0x8];
    CEnginePrimitiveManager(CEngineSceneArray* scenes);   // 0x00BACDB0
    void RegisterEnable(const CCharString& name);  // 0x00BACD80
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
    RegisterEnable("EnableDecals");
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
    LandscapeDecalPixelShader = g_pEngineData->GetPixelShaderResource()->GetPixelShader("PSHADER_TEXTURE_DIFFUSE");
    MeshDecalPixelShader = g_pEngineData->GetPixelShaderResource()->GetPixelShader("PSHADER_TEXTURE_DIFFUSE");
    MeshDecalTwoPassStencilPixelShader = g_pEngineData->GetPixelShaderResource()->GetPixelShader("PSHADER_DECAL_STENCIL_TWO_PASS");
    MeshDecalStencilClearPixelShader = g_pEngineData->GetPixelShaderResource()->GetPixelShader("PSHADER_DECAL_STENCIL_TWO_PASS");
}