// CEnginePrimitiveManagerRepeatedStaticMeshes::CEnginePrimitiveManagerRepeatedStaticMeshes(CEngineSceneArray* scenes) -- constructs the repeated static mesh manager: registers EnableRepeatedMeshes, looks up the plain / stipple / depth-shadowed shader pairs (and the colour-buffer-shadowed pairs when the texture quality allows), points the active shadowed shaders at the depth-buffer set and allocates the 16-aligned SIMD variable block
// Leaf: ??0CEnginePrimitiveManagerRepeatedStaticMeshes
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B99D90, length 1808 bytes (`ret 4`, no EH frame). Vtable 0x012A3E9C.
// Callees: CEnginePrimitiveGroupManager ctor @ 0x00BEBC30, CPixelShader() @ 0x00A5EC00, CVertexShader() @ 0x00A62FB0,
//   RegisterEnable @ 0x00BACD80, CCharString @ 0x0099EBF0 / ~ @ 0x0099EAE0, GetVertexShader @ 0x00A5D5F0 / GetPixelShader @ 0x00A5D720,
//   CalculateTextureMemoryFromQuality @ 0x009BEE00 (thiscall on the global at 0x01436E1C; `cmp eax,0x578 / jb`),
//   AllocateAligned @ 0x009F2BE0 (`__fastcall`, size 0xC00, alignment 0x10).
// Layout: engine/CEnginePrimitiveManagerRepeatedStaticMeshes.h offsets hold (0x94 bytes); its shader members are opaque byte arrays,
//   so the class is restated locally with the real shader types (models as the landed 0x00BAD040).
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
    unsigned char    _pad0000[0x2a30];
    CShaderResource* RepeatedMeshVertexShaderResource; // +0x2A30
    unsigned char    _pad2a34[0x2a98 - 0x2a34];
    CShaderResource* PixelShaderResource;          // +0x2A98
    CShaderResource* GetRepeatedMeshVertexShaderResource() { return RepeatedMeshVertexShaderResource; }
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
struct CEngineDisplay {
    unsigned long CalculateTextureMemoryFromQuality();   // 0x009BEE00
};
extern CEngineDisplay* g_pEngineDisplay;           // 0x01436E1C
void* __fastcall AllocateAligned(unsigned long size, unsigned long alignment);   // 0x009F2BE0
struct CSIMDVariableBlock;

struct CEnginePrimitiveManager {                   // 0x0c bytes
    virtual ~CEnginePrimitiveManager();
    unsigned char _body[0x8];
    void RegisterEnable(const CCharString& name);  // 0x00BACD80
};
struct CEnginePrimitiveGroupManager : CEnginePrimitiveManager {   // 0x20 bytes
    unsigned char _group[0x14];
    CEnginePrimitiveGroupManager(CEngineSceneArray* scenes);   // 0x00BEBC30
};

struct CEnginePrimitiveManagerRepeatedStaticMeshes : CEnginePrimitiveGroupManager {   // vtable 0x012A3E9C, 0x94 bytes
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

CEnginePrimitiveManagerRepeatedStaticMeshes::CEnginePrimitiveManagerRepeatedStaticMeshes(CEngineSceneArray* scenes)
    : CEnginePrimitiveGroupManager(scenes)
{
    RegisterEnable("EnableRepeatedMeshes");
    PixelShader = g_pEngineData->GetPixelShaderResource()->GetPixelShader("PSHADER_REPEATED_MESH");
    VertexShader = g_pEngineData->GetRepeatedMeshVertexShaderResource()->GetVertexShader("VSHADER_REPEATED_MESH");
    PixelShaderStippleAlpha = g_pEngineData->GetPixelShaderResource()->GetPixelShader("PSHADER_REPEATED_MESH_STIPPLE_ALPHA");
    VertexShaderStippleAlpha = g_pEngineData->GetRepeatedMeshVertexShaderResource()->GetVertexShader("VSHADER_REPEATED_MESH_STIPPLE_ALPHA");
    PixelShaderDepthBufferShadowed = g_pEngineData->GetPixelShaderResource()->GetPixelShader("PSHADER_REPEATED_MESH_SHADOWED");
    VertexShaderDepthBufferShadowed = g_pEngineData->GetRepeatedMeshVertexShaderResource()->GetVertexShader("VSHADER_REPEATED_MESH_SHADOWED");
    PixelShaderDepthBufferShadowedStippleAlpha = g_pEngineData->GetPixelShaderResource()->GetPixelShader("PSHADER_REPEATED_MESH_SHADOWED_STIPPLE_ALPHA");
    VertexShaderDepthBufferShadowedStippleAlpha = g_pEngineData->GetRepeatedMeshVertexShaderResource()->GetVertexShader("VSHADER_REPEATED_MESH_SHADOWED_STIPPLE_ALPHA");
    if (g_pEngineDisplay->CalculateTextureMemoryFromQuality() >= 1400)
    {
        PixelShaderColourBufferShadowed = g_pEngineData->GetPixelShaderResource()->GetPixelShader("PSHADER_REPEATED_MESH_COLBUFF_SHADOWED");
        VertexShaderColourBufferShadowed = g_pEngineData->GetRepeatedMeshVertexShaderResource()->GetVertexShader("VSHADER_REPEATED_MESH_COLBUFF_SHADOWED");
        PixelShaderColourBufferShadowedStippleAlpha = g_pEngineData->GetPixelShaderResource()->GetPixelShader("PSHADER_REPEATED_MESH_COLBUFF_SHADOWED_STIPPLE_ALPHA");
        VertexShaderColourBufferShadowedStippleAlpha = g_pEngineData->GetRepeatedMeshVertexShaderResource()->GetVertexShader("VSHADER_REPEATED_MESH_COLBUFF_SHADOWED_STIPPLE_ALPHA");
    }
    PixelShaderShadowed = &PixelShaderDepthBufferShadowed;
    VertexShaderShadowed = &VertexShaderDepthBufferShadowed;
    PixelShaderShadowedStippleAlpha = &PixelShaderDepthBufferShadowedStippleAlpha;
    VertexShaderShadowedStippleAlpha = &VertexShaderDepthBufferShadowedStippleAlpha;
    SIMDVariableBlock = (CSIMDVariableBlock*)AllocateAligned(0xc00, 0x10);
}
