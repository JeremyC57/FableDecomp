// CEnginePrimitiveManager2DSprites::CEnginePrimitiveManager2DSprites(CArray<CEngineScene*>* scenes) -- constructs the 2D sprite manager: looks up its two vertex shaders and two pixel shaders by name, fills the quad index list and registers the Enable2DSprites switch
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BAD040, length 668 bytes (`ret 4`, no EH frame).
// Callees: CEnginePrimitiveManager::CEnginePrimitiveManager @ 0x00BACDB0, CVertexShader::CVertexShader @ 0x00A62FB0,
//   CPixelShader::CPixelShader @ 0x00A5EC00, CCharString::CCharString(const char*, long) @ 0x0099EBF0 / ~CCharString @ 0x0099EAE0,
//   CShaderResource::GetVertexShader @ 0x00A5D5F0 and GetPixelShader @ 0x00A5D720 (both labelled GetPixelShader in the
//   manifest; the first returns a CVertexShader -- its temporary is reset to vtable 0x0129B3D0 and counts at +0x10),
//   CEnginePrimitiveManager::RegisterEnable @ 0x00BACD80 (a retail no-op, `ret 4`).
// Globals: the engine data @ 0x01436E98; its shader resource at +0x2A40 (vertex) and +0x2A98 (pixel).
// Vtables: 0x012A5664 CEnginePrimitiveManager2DSprites, 0x0129B3D0 CVertexShader, 0x0129A054 CPixelShader, 0x0129A030 CShaderBase.
// Layout: engine/CEnginePrimitiveManager2DSprites.h offsets (VertexShader2DSprite +0x0C, VertexShader2DClockSprite +0x14,
//   PixelShader2DClockSprite +0x1C, PixelShader2DClockSpriteAdditive +0x24, Indices +0x2C), restated locally as in the landed
//   destructor 0x00BB1900 because the header's members are opaque and declare no constructors.
// Leaf: ??0CEnginePrimitiveManager2DSprites
// Shape: each lookup passes the literal to a `const CCharString&` parameter (implicit conversion: the name temporary
//   lands in the dead argument slot; an explicit CCharString(...) swaps it with the result spill) through an INLINE
//   resource accessor (the global and the +0x2A40 load are read before the pushes), calls the getter into a by-value
//   temporary, assigns (the vertex shader
//   operator= releases and re-points; the pixel one also nulls the old pointer inside the release) and destroys the
//   temporary through its inline destructor (`RemoveReference` copy, as the landed Lines constructor 0x00BC7E90).
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
    unsigned char    _pad0000[0x2a40];
    CShaderResource* VertexShaderResource;         // +0x2A40
    unsigned char    _pad2a44[0x2a98 - 0x2a44];
    CShaderResource* PixelShaderResource;          // +0x2A98
    CShaderResource* GetVertexShaderResource() { return VertexShaderResource; }
    CShaderResource* GetPixelShaderResource() { return PixelShaderResource; }
};
extern CEngineData* g_pEngineData;                 // 0x01436E98

struct CEngineSceneArray;
class CEnginePrimitiveManager { public:
    virtual ~CEnginePrimitiveManager();
    CEngineSceneArray* SceneList;                  // +0x04
    bool               Enable;                     // +0x08
    CEnginePrimitiveManager(CEngineSceneArray* scenes);   // 0x00BACDB0
    void RegisterEnable(const CCharString& name);  // 0x00BACD80
};

class CEnginePrimitiveManager2DSprites : public CEnginePrimitiveManager { public:   // vtable 0x012A5664
    CVertexShader  VertexShader2DSprite;           // +0x0C
    CVertexShader  VertexShader2DClockSprite;      // +0x14
    CPixelShader   PixelShader2DClockSprite;       // +0x1C
    CPixelShader   PixelShader2DClockSpriteAdditive;   // +0x24
    unsigned short Indices[6];                     // +0x2C
    CEnginePrimitiveManager2DSprites(CEngineSceneArray* scenes);
    virtual ~CEnginePrimitiveManager2DSprites();
};

CEnginePrimitiveManager2DSprites::CEnginePrimitiveManager2DSprites(CEngineSceneArray* scenes)
    : CEnginePrimitiveManager(scenes)
{
    VertexShader2DSprite = g_pEngineData->GetVertexShaderResource()->GetVertexShader("VSHADER_2D_SPRITE");
    VertexShader2DClockSprite = g_pEngineData->GetVertexShaderResource()->GetVertexShader("VSHADER_2D_CLOCK_SPRITE");
    PixelShader2DClockSprite = g_pEngineData->GetPixelShaderResource()->GetPixelShader("PSHADER_2D_CLOCK_SPRITE");
    PixelShader2DClockSpriteAdditive = g_pEngineData->GetPixelShaderResource()->GetPixelShader("PSHADER_2D_CLOCK_SPRITE_ADDITIVE");
    Indices[0] = 0;
    Indices[1] = 1;
    Indices[2] = 2;
    Indices[3] = 1;
    Indices[4] = 3;
    Indices[5] = 2;
    RegisterEnable("Enable2DSprites");
}