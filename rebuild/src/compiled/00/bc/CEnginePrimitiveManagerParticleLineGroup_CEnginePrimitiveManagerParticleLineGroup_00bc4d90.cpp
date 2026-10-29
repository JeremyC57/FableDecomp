// CEnginePrimitiveManagerParticleLineGroup::CEnginePrimitiveManagerParticleLineGroup(CEngineSceneArray* scenes) -- constructs the particle line group manager: sizes the point-light vertex shader table to five, looks up the line-group vertex shaders and the sprite-group pixel shaders by name, then builds the buffers
// Leaf: ??0CEnginePrimitiveManagerParticleLineGroup
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BC4D90, length 1345 bytes (`ret 4`, no EH frame). Vtable 0x012A6590.
// Callees: CEnginePrimitiveManager ctor @ 0x00BACDB0, CVertexShader() @ 0x00A62FB0, CPixelShader() @ 0x00A5EC00, CBaseClassNonCopyable() @ 0x0099A2F0,
//   CIndexBuffer() @ 0x00AC1EC0, vector<CVertexShader>::resize @ 0x00B6CBD0 (catalogued "std::vector::erase"; the call site passes
//   only the count 5), CCharString @ 0x0099EBF0 / ~ @ 0x0099EAE0, GetVertexShader @ 0x00A5D5F0 / GetPixelShader @ 0x00A5D720,
//   BuildBuffers @ 0x00BC3E10.
// Layout: engine/CEnginePrimitiveManagerParticleLineGroup.h has the PDB 16-byte CArray at +0x0c; retail's STLport vector is 12 bytes,
//   so every later member sits 4 lower (VertexShaderDisplacement +0x18, PixelShader +0x20 ... IndexBuffer +0x6c), restated locally.
// Shape: the vector is a real _Vector_base over an allocator temporary (its zero stores interleave with the next lea); element access
//   is `*(begin() + index)` with a SIGNED long index, which loads _M_start ahead of the pushes and folds the element offset.
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
    unsigned char    _pad0000[0x2a68];
    CShaderResource* ParticleVertexShaderResource; // +0x2A68
    unsigned char    _pad2a6c[0x2a98 - 0x2a6c];
    CShaderResource* PixelShaderResource;          // +0x2A98
    CShaderResource* GetParticleVertexShaderResource() { return ParticleVertexShaderResource; }
    CShaderResource* GetPixelShaderResource() { return PixelShaderResource; }
};
extern CEngineData* g_pEngineData;                 // 0x01436E98
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
struct CEngineParticleConstantBlock { unsigned char _body[0x10]; };

class CEnginePrimitiveManagerParticleLineGroup : public CEnginePrimitiveManager { public:   // vtable 0x012A6590
    CVertexShaderArray VertexShadersLinePointLights;   // +0x0c
    CVertexShader      VertexShaderDisplacement;   // +0x18
    CPixelShader       PixelShader;                // +0x20
    CPixelShader       PixelShaderStipple;         // +0x28
    CPixelShader       PixelShaderDisplacement;    // +0x30
    CVertexBufferWin32 VertexBuffer;               // +0x38
    CIndexBuffer       IndexBuffer;                // +0x6c
    CEngineParticleConstantBlock ConstantBlock[30];    // +0x80
    CEnginePrimitiveManagerParticleLineGroup(CEngineSceneArray* scenes);
    virtual ~CEnginePrimitiveManagerParticleLineGroup();
    void BuildBuffers();                           // 0x00BC3E10
};

CEnginePrimitiveManagerParticleLineGroup::CEnginePrimitiveManagerParticleLineGroup(CEngineSceneArray* scenes)
    : CEnginePrimitiveManager(scenes)
{
    VertexShadersLinePointLights.resize(5);
    VertexShadersLinePointLights[0] = g_pEngineData->GetParticleVertexShaderResource()->GetVertexShader("VSHADER_LINE_GROUP_DIRLIGHT_FOG");
    VertexShadersLinePointLights[1] = g_pEngineData->GetParticleVertexShaderResource()->GetVertexShader("VSHADER_LINE_GROUP_DIRLIGHT_3POINTLIGHTS_FOG");
    VertexShadersLinePointLights[2] = g_pEngineData->GetParticleVertexShaderResource()->GetVertexShader("VSHADER_LINE_GROUP_DIRLIGHT_3POINTLIGHTS_FOG");
    VertexShadersLinePointLights[3] = g_pEngineData->GetParticleVertexShaderResource()->GetVertexShader("VSHADER_LINE_GROUP_DIRLIGHT_3POINTLIGHTS_FOG");
    VertexShadersLinePointLights[4] = g_pEngineData->GetParticleVertexShaderResource()->GetVertexShader("VSHADER_LINE_GROUP_DIRLIGHT_5POINTLIGHTS_FOG");
    VertexShaderDisplacement = g_pEngineData->GetParticleVertexShaderResource()->GetVertexShader("VSHADER_LINE_GROUP_DISPLACEMENT");
    PixelShader = g_pEngineData->GetPixelShaderResource()->GetPixelShader("PSHADER_SPRITE_GROUP");
    PixelShaderStipple = g_pEngineData->GetPixelShaderResource()->GetPixelShader("PSHADER_SPRITE_GROUP_STIPPLE");
    PixelShaderDisplacement = g_pEngineData->GetPixelShaderResource()->GetPixelShader("PSHADER_SPRITE_GROUP_DISPLACEMENT");
    BuildBuffers();
}