// CEngineBillboardGenerator::CEngineBillboardGenerator() -- construct the billboard generator engine component: the render-util and engine-component bases, then the VSHADER_STATIC_DIRLIGHT_FOG vertex shader (vertex shader bank 1) and the PSHADER_TEXTURE_DIFFUSE pixel shader from the engine resource manager
// Leaf: ??0CEngineBillboardGenerator@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B8B630, length 333 bytes, `ret`, returns `this`.
// Name: FableWin.pdb `??0CEngineBillboardGenerator@@QAE@XZ`; the final vtables 0x012A3BF4 (+0) / 0x012A3BFC (+4, the CEngineComponent
//   subobject) are its two. Label correction: the catalog spelling is right here, but the same label was propagated to the
//   colour-filter ctor 0x00B5A1D0 (landed as CEngineScreenEffectColourFilter).
// Callees: CBaseClassNonCopyable::CBaseClassNonCopyable 0x0099A2F0 (FableWin `??0CBaseClassNonCopyable@@IAE@XZ`, protected; catalog
//   "CBase::CBase"), CEngineComponent::CEngineComponent 0x00B59710 (catalog "CNavigationLayer::CNavigationLayer" is propagated),
//   CVertexShader::CVertexShader 0x00A62FB0 and CPixelShader::CPixelShader 0x00A5EC00 (catalog "CBaseIntelligentPointer"),
//   CCharString::CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0 (catalog "_Cons_val<...>" for the dtor),
//   CShaderResource::GetVertexShader(const CCharString&) const 0x00A5D5F0 / GetPixelShader(const CCharString&) const 0x00A5D720
//   (FableWin names; catalog "GetPixelShader" on both). Vtables: 0x012A3BD0 CEnginePrimitiveRenderUtil (its ctor inlined; the
//   out-of-line copy is 0x00B8B5E0), 0x0129B3D0 CVertexShader, 0x0129A054 CPixelShader, 0x0129A030 CShaderBase.
// Inlines (FableWin.pdb names): CEngineResourceManager::GetVShaderResource(EShaderBankNo) const / GetPShaderResource() const,
//   CVertexShader::operator= (returns const&) / CPixelShader::operator= (returns void, nulls Data after the release),
//   ~CVertexShader / ~CPixelShader / ~CShaderBase for the by-value temporaries, C*ShaderData::Release / AddReference /
//   CountReferences. Shape: the pixel shader resource is fetched into a local BEFORE the name string is built (retail holds it
//   in edi across the CCharString ctor); the vertex resource is read after its string. operator= releases through a local copy
//   of Data (`mov ecx,[edi+4] ; test ; dec [ecx+N] ; jne`), the temporaries' dtors through Release() (`mov edi,[eax+N] ; dec edi`).
// Globals: 0x01436E98 the engine resource manager (spelling g_pEngineResourceManager_01436e98 is descriptive, see NOTES); retail
//   VShaderResources +0x29C0 (CCountedPointer<CShaderResource>[27]; bank 1 = +0x29C8), ShaderResourcePixel +0x2A98 -- the Ego_r
//   offsets less 0xF34 (engine/CEngineResourceManager.h is 0x230 off for these members: header debt, so the class is modelled
//   locally). The EShaderBankNo enumerator for bank 1 has no recovered name (cast literal).
// Layout: Ego_r.pdb CEngineBillboardGenerator VertexShader +0x10, PixelShader +0x18; CEngineComponent GlobalPointerAddress +0x04,
//   RenderEnabled +0x08, KeepAlive +0x09; CVertexShaderData ReferenceCount +0x10, CPixelShaderData ReferenceCount +0x08. The
//   flattened engine headers carry the vptr as a data member, so the polymorphic classes are modelled locally.
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
    virtual ~CVertexShaderData();
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
    virtual ~CPixelShaderData();
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
extern CEngineResourceManager* g_pEngineResourceManager_01436e98;   // 0x01436E98
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