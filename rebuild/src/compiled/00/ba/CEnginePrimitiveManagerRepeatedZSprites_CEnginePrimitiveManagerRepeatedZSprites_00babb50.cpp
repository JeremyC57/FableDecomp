// CEnginePrimitiveManagerRepeatedZSprites::CEnginePrimitiveManagerRepeatedZSprites(CArray<CEngineScene*>* scenes) -- constructs the repeated z-sprite manager: looks up its vertex shader and its two pixel shaders by name, registers the EnableZSprites switch, builds the quad buffers, then allocates the dynamic texture page, the 256x256 billboard texture and z-buffer surface and sets the stipple / control-point defaults
// Leaf: ??0CEnginePrimitiveManagerRepeatedZSprites
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BABB50, length 770 bytes (`ret 4`, no EH frame).
// Label correction: catalogued "CEnginePrimitiveManager2DSprites" (propagated); the vtable store 0x012A5314 is
//   CEnginePrimitiveManagerRepeatedZSprites (rtti-vtables.tsv: slot 1 CreateInternalPrimitive 0x00BAA820) and it calls BuildBuffers 0x00BAA6A0.
// Callees: CEnginePrimitiveManagerOcclusionBase::CEnginePrimitiveManagerOcclusionBase @ 0x00B90270, CVertexShader::CVertexShader
//   @ 0x00A62FB0, CPixelShader::CPixelShader @ 0x00A5EC00, CBase::CBase @ 0x0099A2F0 (vertex buffer root), CIndexBuffer::CIndexBuffer
//   @ 0x00AC1EC0, CCharString(const char*, long) @ 0x0099EBF0 / ~CCharString @ 0x0099EAE0, CShaderResource::GetVertexShader @ 0x00A5D5F0 /
//   GetPixelShader @ 0x00A5D720, CEnginePrimitiveManager::RegisterEnable @ 0x00BACD80, BuildBuffers @ 0x00BAA6A0,
//   CGPUScratchBufferTextureManager AllocateMemoryPage @ 0x00F37660 / AllocateTexture @ 0x00F37440 / AllocateDepthBufferSurface @ 0x00F37390,
//   CPixelFormat::Initialise @ 0x009E3790 and CPixelFormat(D3DFORMAT) @ 0x009E3830.
// Layout: engine/CEnginePrimitiveManagerRepeatedZSprites.h names the members, but retail's PrimitiveSortList is a 12-byte
//   vector (PDB CArray 16), so every member after +0x80 sits 4 lower (BillboardTexture +0x8C, handles +0xA4/+0xA8/+0xAC,
//   NonStippleAlphaFactor +0xB0 ... CurrentRenderedMesh +0xC0): the class is restated locally (shader models as the landed 0x00BAD040).
// Shape: the shader lookups as the 2D sprite constructor (implicit CCharString temporaries); the pixel resource is read once into a
//   named local ahead of both pixel lookups; the vertex buffer's constructor is inline (CBase root, compression params in the
//   base, then the vtable store before the PVertexBuffer / Stride body stores of the derived constructor); the sort list is a real
//   STLport vector (_Vector_base + _STLP_alloc_proxy over an allocator temporary) so its three zero stores interleave with the first
//   CCharString pushes; the two 256x256 sizes are C2DCoordI TEMPORARIES passed by const reference (they share the shader-temporary
//   frame slot: sub esp,10h; named locals grow the frame to 0x18/0x20).
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
struct CPixelFormat {
    long FormatIndex;
    CPixelFormat(unsigned long d3dFormat);         // 0x009E3830
    void Initialise(long r, long g, long b, long a, bool alpha);   // 0x009E3790
    CPixelFormat() { }
};
struct C2DCoordI {
    long X, Y;
    C2DCoordI(long x, long y) : X(x), Y(y) { }
};
struct CGPUScratchBufferTextureManager {
    long AllocateMemoryPage(long a, long b);       // 0x00F37660
    long AllocateTexture(long page, const C2DCoordI& size, long mipLevels, const CPixelFormat* format, long count, long flags);   // 0x00F37440
    long AllocateDepthBufferSurface(long page, const C2DCoordI& size, const CPixelFormat* format, long flags);   // 0x00F37390
};
struct CEngineData {
    unsigned char    _pad0000[0x2a48];
    CShaderResource* VertexShaderResource;         // +0x2A48
    unsigned char    _pad2a4c[0x2a98 - 0x2a4c];
    CShaderResource* PixelShaderResource;          // +0x2A98
    unsigned char    _pad2a9c[0x3190 - 0x2a9c];
    CGPUScratchBufferTextureManager ScratchTextures;   // +0x3190
    CShaderResource* GetVertexShaderResource() { return VertexShaderResource; }
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
template <class T> struct CPODArray : _Vector_base<T> {   // retail 12-byte STLport vector
    CPODArray(const allocator<T>& a = allocator<T>()) : _Vector_base<T>(a) { }
};
struct CEngineBillboard {                          // 0x18 bytes
    void* Texture;
    void* Surface;
    unsigned char _rest[0x10];
};
struct CEngineInternalPrimitiveRepeatedZSprite;
struct C3DMeshInfo;

struct CEnginePrimitiveManagerOcclusionBase {      // 0x20 bytes
    virtual ~CEnginePrimitiveManagerOcclusionBase();
    unsigned char _body[0x1c];
    CEnginePrimitiveManagerOcclusionBase(CEngineSceneArray* scenes);   // 0x00B90270
    void RegisterEnable(const CCharString& name);  // 0x00BACD80
};

struct CEnginePrimitiveManagerRepeatedZSprites : CEnginePrimitiveManagerOcclusionBase {   // vtable 0x012A5314
    CVertexShader      VertexShader;               // +0x20
    CPixelShader       PixelShader;                // +0x28
    CPixelShader       PixelShaderNoStipple;       // +0x30
    CVertexBufferWin32 VertexBuffer;               // +0x38
    CIndexBuffer       IndexBuffer;                // +0x6c
    CPODArray<CEngineInternalPrimitiveRepeatedZSprite*> PrimitiveSortList;   // +0x80
    CEngineBillboard   BillboardTexture;           // +0x8c
    long               BillboardTextureHandle;     // +0xa4
    long               ZBufferTextureHandle;       // +0xa8
    long               DynamicTexturePageHandle;   // +0xac
    float              NonStippleAlphaFactor;      // +0xb0
    bool               EnableZSpriteAlpha;         // +0xb4
    float              VerticalControlPointWeightingFactor;   // +0xb8
    float              ControlPointWeightingPower; // +0xbc
    C3DMeshInfo*       CurrentRenderedMesh;        // +0xc0
    CEnginePrimitiveManagerRepeatedZSprites(CEngineSceneArray* scenes);
    virtual ~CEnginePrimitiveManagerRepeatedZSprites();
    void BuildBuffers();                           // 0x00BAA6A0
};

CEnginePrimitiveManagerRepeatedZSprites::CEnginePrimitiveManagerRepeatedZSprites(CEngineSceneArray* scenes)
    : CEnginePrimitiveManagerOcclusionBase(scenes)
{
    VertexShader = g_pEngineData->GetVertexShaderResource()->GetVertexShader("VSHADER_ZSPRITE");
    CShaderResource* pixelResource = g_pEngineData->PixelShaderResource;
    PixelShader = pixelResource->GetPixelShader("PSHADER_ZSPRITE");
    PixelShaderNoStipple = pixelResource->GetPixelShader("PSHADER_ZSPRITE_NO_STIPPLE");
    RegisterEnable("EnableZSprites");
    BuildBuffers();
    CGPUScratchBufferTextureManager* textures = &g_pEngineData->ScratchTextures;
    DynamicTexturePageHandle = textures->AllocateMemoryPage(0, 1);
    CPixelFormat format;
    format.Initialise(8, 8, 8, 8, false);
    BillboardTextureHandle = textures->AllocateTexture(DynamicTexturePageHandle, C2DCoordI(256, 256), 1, &format, 1, 0);
    BillboardTexture.Texture = 0;
    BillboardTexture.Surface = 0;
    CPixelFormat depthFormat(0x4b);
    ZBufferTextureHandle = textures->AllocateDepthBufferSurface(DynamicTexturePageHandle, C2DCoordI(256, 256), &depthFormat, 0);
    CurrentRenderedMesh = 0;
    NonStippleAlphaFactor = 1.2f;
    EnableZSpriteAlpha = true;
    VerticalControlPointWeightingFactor = 5.0f;
    ControlPointWeightingPower = 1.0f;
}