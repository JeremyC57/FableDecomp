// CEnginePrimitiveManagerText::CEnginePrimitiveManagerText(CEngineSceneArray* scenes) -- constructs the text primitive manager: looks up its two vertex shaders and two pixel shaders by name
// Leaf: ??0CEnginePrimitiveManagerText
// Layout: local model reused from the landed 2DSprites ctor 0x00BAD040; vertex shader resource at engine data +0x2A90 here, pixel at +0x2A98.
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
    unsigned char    _pad0000[0x2a90];
    CShaderResource* VertexShaderResource;         // +0x2A90
    unsigned char    _pad2a94[0x2a98 - 0x2a94];
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

class CEnginePrimitiveManagerText : public CEnginePrimitiveManager { public:   // vtable 0x012A68B0
    CVertexShader  VertexShaderText;               // +0x0C
    CVertexShader  VertexShaderTextShadow;         // +0x14
    CPixelShader   PixelShaderText;                // +0x1C
    CPixelShader   PixelShaderTextShadow;          // +0x24
    CEnginePrimitiveManagerText(CEngineSceneArray* scenes);
    virtual ~CEnginePrimitiveManagerText();
};

CEnginePrimitiveManagerText::CEnginePrimitiveManagerText(CEngineSceneArray* scenes)
    : CEnginePrimitiveManager(scenes)
{
    VertexShaderText = g_pEngineData->GetVertexShaderResource()->GetVertexShader("VSHADER_TEXT");
    VertexShaderTextShadow = g_pEngineData->GetVertexShaderResource()->GetVertexShader("VSHADER_TEXT_SHADOW");
    PixelShaderText = g_pEngineData->GetPixelShaderResource()->GetPixelShader("PSHADER_TEXT");
    PixelShaderTextShadow = g_pEngineData->GetPixelShaderResource()->GetPixelShader("PSHADER_TEXT_SHADOW");
}