// test for 0x00B8B630 CEngineBillboardGenerator::CEngineBillboardGenerator -- bases built, both shaders fetched by name and add-ref'd, temporaries released
#include <stdio.h>
#include <string.h>
static int g_vsDeleted = 0, g_psDeleted = 0, g_baseCtors = 0, g_componentCtors = 0;
static char g_lastVName[64], g_lastPName[64];
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
class CCPPointerInfo;
template <class T> class CCountedPointer { public:
    T*              Data;
    CCPPointerInfo* Info;
    T* operator->() const { return Data; }
    T* Get() const { return Data; }
};
class CVertexShader;
class CPixelShader;
class CVertexShaderData {
    friend class CVertexShader;
    friend int main();
    friend class CShaderResource;
    virtual ~CVertexShaderData() { ++g_vsDeleted; }
    void*         VertexDeclaration;                    // +0x04
    void*         VertexShader;                         // +0x08
    const void*   FormatDecl;                           // +0x0c
    unsigned long ReferenceCount;                       // +0x10
    void AddReference() { ++ReferenceCount; }
    unsigned long CountReferences() const { return ReferenceCount; }
    void Release() { --ReferenceCount; if (CountReferences() == 0) delete this; }
};
class CPixelShaderData {
    friend class CPixelShader;
    friend int main();
    friend class CShaderResource;
    virtual ~CPixelShaderData() { ++g_psDeleted; }
    void*         PixelShader;                          // +0x04
    unsigned long ReferenceCount;                       // +0x08
    unsigned long ConstCount;                           // +0x0c
    long*         ConstIndices;                         // +0x10
    void AddReference() { ++ReferenceCount; }
    unsigned long CountReferences() const { return ReferenceCount; }
    void Release() { --ReferenceCount; if (CountReferences() == 0) delete this; }
};
class CShaderBase { public:                             // vtable 0x0129A030
protected:
    virtual ~CShaderBase() { }
    virtual CCharString GetDebugName() const;
};
class CVertexShader : public CShaderBase { public:      // vtable 0x0129B3D0
    CVertexShaderData* Data;                            // +0x04
    CVertexShader();                                    // 0x00A62FB0
    CVertexShader(const CVertexShader& other) : Data(other.Data) { if (Data) Data->AddReference(); }   // PDB ??0CVertexShader@@QAE@ABV0@@Z (test only)
    const CVertexShader& operator=(const CVertexShader& other)
    {
        CVertexShaderData* data = Data;
        if (data) {
            if (--data->ReferenceCount == 0)
                delete data;
        }
        Data = other.Data;
        Data->AddReference();
        return *this;
    }
    virtual ~CVertexShader()
    {
        if (Data)
            Data->Release();
    }
};
class CPixelShader : public CShaderBase { public:       // vtable 0x0129A054
    CPixelShaderData* Data;                             // +0x04
    CPixelShader();                                     // 0x00A5EC00
    CPixelShader(const CPixelShader& other) : Data(other.Data) { if (Data) Data->AddReference(); }   // PDB ??0CPixelShader@@QAE@ABV0@@Z (test only)
    void operator=(const CPixelShader& other)
    {
        CPixelShaderData* data = Data;
        if (data) {
            if (--data->ReferenceCount == 0)
                delete data;
            Data = 0;
        }
        Data = other.Data;
        Data->AddReference();
    }
    virtual ~CPixelShader()
    {
        if (Data)
            Data->Release();
    }
};
class CShaderResource { public:
    CVertexShader GetVertexShader(const CCharString& name) const;   // 0x00A5D5F0
    CPixelShader GetPixelShader(const CCharString& name) const;     // 0x00A5D720
};
enum EShaderBankNo { };
class CEngineResourceManager { public:
    unsigned char                    _pad0000[0x29c0];
    CCountedPointer<CShaderResource> VShaderResources[27];   // +0x29c0 (retail)
    CCountedPointer<CShaderResource> ShaderResourcePixel;    // +0x2a98 (retail)
    const CShaderResource* GetVShaderResource(EShaderBankNo bank) const { return VShaderResources[bank].Get(); }
    const CShaderResource* GetPShaderResource() const { return ShaderResourcePixel.Get(); }
};
CEngineResourceManager* g_pEngineResourceManager_01436e98;   // 0x01436E98
class CBaseClass { public:
    CBaseClass();                                       // 0x0099A2D0
    virtual ~CBaseClass();
};
class CBaseClassNonCopyable : public CBaseClass {
protected:
    CBaseClassNonCopyable();                            // 0x0099A2F0
    virtual ~CBaseClassNonCopyable();
};
class CEnginePrimitiveRenderUtil : public CBaseClassNonCopyable { public:   // vtable 0x012A3BD0
    CEnginePrimitiveRenderUtil() { }
    virtual ~CEnginePrimitiveRenderUtil();
};
class CEngineComponent { public:
    void**        GlobalPointerAddress;                 // +0x04
    bool          RenderEnabled;                        // +0x08
    unsigned char KeepAlive;                            // +0x09
    CEngineComponent();                                 // 0x00B59710
    virtual ~CEngineComponent();
};
class CEngineBillboardGenerator : public CEnginePrimitiveRenderUtil, public CEngineComponent { public:   // vtables 0x012A3BF4 / 0x012A3BFC
    CVertexShader VertexShader;                         // +0x10
    CPixelShader  PixelShader;                          // +0x18
    CEngineBillboardGenerator();
    virtual ~CEngineBillboardGenerator();
    virtual unsigned long PeekSceneFilterFlags() const;
};
CEngineBillboardGenerator::CEngineBillboardGenerator()
{
    VertexShader = g_pEngineResourceManager_01436e98->GetVShaderResource((EShaderBankNo)1)->GetVertexShader("VSHADER_STATIC_DIRLIGHT_FOG");
    const CShaderResource* pixelResource = g_pEngineResourceManager_01436e98->GetPShaderResource();
    PixelShader = pixelResource->GetPixelShader("PSHADER_TEXTURE_DIFFUSE");
}

CCharString::CCharString(const char* text, long) { PStringData = (void*)text; }
CCharString::~CCharString() { }
CCharString CShaderBase::GetDebugName() const { return CCharString(""); }
static CVertexShaderData* g_vdata;
static CPixelShaderData* g_pdata;
CVertexShader::CVertexShader() : Data(0) { }
CPixelShader::CPixelShader() : Data(0) { }
CVertexShader CShaderResource::GetVertexShader(const CCharString& name) const
{
    strcpy(g_lastVName, (const char*)name.PStringData);
    CVertexShader s; s.Data = g_vdata; s.Data->AddReference(); return s;
}
CPixelShader CShaderResource::GetPixelShader(const CCharString& name) const
{
    strcpy(g_lastPName, (const char*)name.PStringData);
    CPixelShader s; s.Data = g_pdata; s.Data->AddReference(); return s;
}
CBaseClass::CBaseClass() { }
CBaseClass::~CBaseClass() { }
CBaseClassNonCopyable::CBaseClassNonCopyable() { ++g_baseCtors; }
CBaseClassNonCopyable::~CBaseClassNonCopyable() { }
CEnginePrimitiveRenderUtil::~CEnginePrimitiveRenderUtil() { }
CEngineComponent::CEngineComponent() : GlobalPointerAddress(0), RenderEnabled(true), KeepAlive(0) { ++g_componentCtors; }
CEngineComponent::~CEngineComponent() { }
CEngineBillboardGenerator::~CEngineBillboardGenerator() { }
unsigned long CEngineBillboardGenerator::PeekSceneFilterFlags() const { return 0; }
int main()
{
    int ok = 1;
    static CEngineResourceManager manager;
    static CShaderResource vres, pres;
    manager.VShaderResources[1].Data = &vres;
    manager.ShaderResourcePixel.Data = &pres;
    g_pEngineResourceManager_01436e98 = &manager;
    g_vdata = new CVertexShaderData(); g_vdata->ReferenceCount = 1;
    g_pdata = new CPixelShaderData(); g_pdata->ReferenceCount = 1;
    {
        CEngineBillboardGenerator gen;
        if (gen.VertexShader.Data != g_vdata || gen.PixelShader.Data != g_pdata) ok = 0;
        if (g_vdata->ReferenceCount != 2 || g_pdata->ReferenceCount != 2) ok = 0;   // the holder's and the generator's
        if (strcmp(g_lastVName, "VSHADER_STATIC_DIRLIGHT_FOG") || strcmp(g_lastPName, "PSHADER_TEXTURE_DIFFUSE")) ok = 0;
        if (g_baseCtors != 1 || g_componentCtors != 1 || !gen.RenderEnabled) ok = 0;
        if (g_vsDeleted || g_psDeleted) ok = 0;
        // reassigning the last reference deletes the old data
        g_vdata->ReferenceCount = 1;
        CVertexShaderData* other = new CVertexShaderData(); other->ReferenceCount = 1;
        CVertexShader s; s.Data = other;
        gen.VertexShader = s;
        if (g_vsDeleted != 1 || gen.VertexShader.Data != other || other->ReferenceCount != 2) ok = 0;
        g_pdata->ReferenceCount = 1;
        CPixelShaderData* otherp = new CPixelShaderData(); otherp->ReferenceCount = 1;
        CPixelShader p; p.Data = otherp;
        gen.PixelShader = p;
        if (g_psDeleted != 1 || gen.PixelShader.Data != otherp || otherp->ReferenceCount != 2) ok = 0;
    }
    if (!ok) { printf("00b8b630_TEST FAIL\n"); return 1; }
    printf("00b8b630_TEST PASS\n");
    return 0;
}