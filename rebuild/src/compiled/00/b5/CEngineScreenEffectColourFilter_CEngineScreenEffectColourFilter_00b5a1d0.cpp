// CEngineScreenEffectColourFilter::CEngineScreenEffectColourFilter(const CEngineScreenEffectCaps& caps) -- construct the colour-filter screen effect: default filter settings, saved render target and shaders, supported only when the device caps flag it, Rec. 709 grey weights (0.212671, 0.715160, 0.072169), grey level 1/3, the full-screen quad positions and the VSHADER_SCREEN_FX_COPY / PSHADER_SCREEN_FX_COLOUR_FILTER shaders
// Leaf: ??0CEngineScreenEffectColourFilter
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B5A1D0, length 542 bytes (`ret 4`). Label correction: the catalog name
// "CEngineBillboardGenerator::CEngineBillboardGenerator" is propagated; the vtable 0x012A25CC stored after the base
// ctor is RTTI CEngineScreenEffectColourFilter (rtti-vtables.tsv) and the members match its Ego_r.pdb layout.
// Callees: CEngineComponent::CEngineComponent 0x00B59710 (catalog "CNavigationLayer" is propagated; its dtor is the landed
// 0x00B59730), CBaseClass::CBaseClass 0x0099A2D0 (the render target's base), CVertexShader::CVertexShader 0x00A62FB0,
// CPixelShader::CPixelShader 0x00A5EC00, CCharString 0x0099EBF0 / 0x0099EAE0, CShaderResource::GetVertexShader 0x00A5D5F0
// and GetPixelShader 0x00A5D720 (by hidden pointer) on the engine global 0x01436E98 (+0x2a28 / +0x2a98).
// Vtables: 0x0129C800 CRenderTarget, 0x0122F84C CSurface, 0x0129B3D0 CVertexShader, 0x0129A054 CPixelShader, 0x0129A030
// CShaderBase. Strings "VSHADER_SCREEN_FX_COPY" 0x012A11E4, "PSHADER_SCREEN_FX_COLOUR_FILTER" 0x012A2620.
// Layout: engine/CEngineScreenEffectColourFilter.h offsets (Enabled +0x08, ForceColourFilter +0x0c, Settings +0x10,
// GrayRGBWeights +0x30, GrayLevel +0x40, EffectSupported +0x44, TextureToBackBufferVerts +0x48 (4 x 0x18), OldRenderTarget
// +0xa8, OldViewport +0xcc, ScreenCopyVertexShader +0xdc, ColourFilterPixelShader +0xe4); the header flattens the member
// classes to byte arrays, so the class is modelled locally with the real member classes (the landed GlowRenderer /
// OutlineGlow destructors 0x00B5EE80 / 0x00B898D0 model).
// Shape: member ctors inline (settings defaults, the render target's CBaseClass call + surface zeroing) or out of line
// (the two shaders); the grey weights are an assigned CRGBFloatColour temporary (built in the frame, copied as dwords);
// the shader name strings are implicit const char* -> CCharString temporaries built in the dead `caps` slot; the shader
// resource is read through an inline const getter on the engine global so it is loaded before the string is built;
// operator= releases through a named data pointer (`dec [ecx+N] ; jne`), the by-value temporary's destructor through the
// pointee's inline Release() (`mov edi,[eax+N] ; add eax,N ; dec edi`).
#pragma pack(push, 4)
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};

struct CPixelShaderData {
    virtual ~CPixelShaderData();
    void* m_pShader;                                    // +0x04
    long  m_refCount;                                   // +0x08
    void DecRef() { --m_refCount; }
    long GetRefCount() const { return m_refCount; }
    void Release() { DecRef(); if (GetRefCount() == 0) delete this; }
};
struct CVertexShaderData {
    virtual ~CVertexShaderData();
    unsigned char _pad04[0xc];
    long  m_refCount;                                   // +0x10
    void DecRef() { --m_refCount; }
    long GetRefCount() const { return m_refCount; }
    void Release() { DecRef(); if (GetRefCount() == 0) delete this; }
};
struct CShaderBase {                                    // vtable 0x0129A030
    virtual ~CShaderBase() { }
};
struct CVertexShader : CShaderBase {                    // vtable 0x0129B3D0
    CVertexShaderData* Data;
    CVertexShader();                                    // 0x00A62FB0
    CVertexShader(const CVertexShader& other);
    CVertexShader& operator=(const CVertexShader& other)
    {
        CVertexShaderData* data = Data;
        if (data) {
            if (--data->m_refCount == 0)
                delete data;
        }
        Data = other.Data;
        ++Data->m_refCount;
        return *this;
    }
    virtual ~CVertexShader()
    {
        if (Data)
            Data->Release();
    }
};
struct CPixelShader : CShaderBase {                     // vtable 0x0129A054
    CPixelShaderData* Data;
    CPixelShader();                                     // 0x00A5EC00
    CPixelShader(const CPixelShader& other);
    CPixelShader& operator=(const CPixelShader& other)
    {
        CPixelShaderData* data = Data;
        if (data) {
            if (--data->m_refCount == 0)
                delete data;
            Data = 0;
        }
        Data = other.Data;
        ++Data->m_refCount;
        return *this;
    }
    virtual ~CPixelShader()
    {
        if (Data)
            Data->Release();
    }
};
struct CShaderResource {
    CVertexShader GetVertexShader(const CCharString& name);   // 0x00A5D5F0
    CPixelShader GetPixelShader(const CCharString& name);     // 0x00A5D720
};
struct CEngineGlobal_01436e98 {
    unsigned char    _pad[0x2a28];
    CShaderResource* VertexShaderResource;              // +0x2a28
    unsigned char    _pad2a2c[0x2a98 - 0x2a2c];
    CShaderResource* PixelShaderResource;               // +0x2a98
    CShaderResource* GetVertexShaderResource() const { return VertexShaderResource; }
    CShaderResource* GetPixelShaderResource() const { return PixelShaderResource; }
};
extern CEngineGlobal_01436e98* g_pEngine_01436e98;     // 0x01436E98

struct CBaseClass {
    virtual ~CBaseClass();
    CBaseClass();                                       // 0x0099A2D0
};
struct IDirect3DSurface9;
struct CSurface {                                       // vtable 0x0122F84C, 0x10 bytes
    virtual ~CSurface();
    IDirect3DSurface9* PSurface;
    void*              PTexture;
    long               Level;
    CSurface() : PSurface(0), PTexture(0) { }
};
struct CRenderTarget : CBaseClass {                     // vtable 0x0129C800, 0x24 bytes
    CSurface RenderToSurface;                           // +0x04
    CSurface ZBufferSurface;                            // +0x14
    virtual ~CRenderTarget();
    CRenderTarget() { }
};

struct CRGBFloatColour {
    float R, G, B, A;
    CRGBFloatColour() { }
    CRGBFloatColour(float r, float g, float b, float a) : R(r), G(g), B(b), A(a) { }
};
struct CEngineColourFilterSettings {                    // 0x20 bytes
    float           Alpha;
    float           Saturation;
    float           Contrast;
    float           Brightness;
    CRGBFloatColour Tint;
    CEngineColourFilterSettings() : Alpha(0.0f), Saturation(1.0f), Contrast(0.0f), Brightness(0.0f), Tint(1.0f, 1.0f, 1.0f, 1.0f) { }
};
struct CTVertexColTex1 {                                // 0x18 bytes
    float         X, Y, Z;
    unsigned long Colour;
    float         U, V;
};
struct C2DBoxI { long TLX, TLY, BRX, BRY; };

struct CEngineScreenEffectCaps {
    unsigned char _pad[0x60];
    unsigned char _bits : 7;
    unsigned char ColourFilterSupported : 1;            // +0x60 bit 7
};

struct CEngineComponent {
    virtual void v00();
    void*         m_pOwnerLink;                         // +0x04
    bool          Enabled;                              // +0x08
    CEngineComponent();                                 // 0x00B59710
};
struct CEngineScreenEffectColourFilter : CEngineComponent {   // vtable 0x012A25CC
    bool                        ForceColourFilter;      // +0x0c
    CEngineColourFilterSettings Settings;               // +0x10
    CRGBFloatColour             GrayRGBWeights;         // +0x30
    float                       GrayLevel;              // +0x40
    bool                        EffectSupported;        // +0x44
    CTVertexColTex1             TextureToBackBufferVerts[4];   // +0x48
    CRenderTarget               OldRenderTarget;        // +0xa8
    C2DBoxI                     OldViewport;            // +0xcc
    CVertexShader               ScreenCopyVertexShader; // +0xdc
    CPixelShader                ColourFilterPixelShader;   // +0xe4
    virtual void v00();
    CEngineScreenEffectColourFilter(const CEngineScreenEffectCaps& caps);
};
#pragma pack(pop)

CEngineScreenEffectColourFilter::CEngineScreenEffectColourFilter(const CEngineScreenEffectCaps& caps)
{
    if (caps.ColourFilterSupported) {
        EffectSupported = true;
    } else {
        EffectSupported = false;
        Enabled = false;
    }
    GrayRGBWeights = CRGBFloatColour(0.212671f, 0.715160f, 0.072169f, 1.0f);
    ForceColourFilter = false;
    GrayLevel = 0.333f;
    TextureToBackBufferVerts[0].X = -1.0f;
    TextureToBackBufferVerts[0].Y = -1.0f;
    TextureToBackBufferVerts[0].Z = 0.0f;
    TextureToBackBufferVerts[1].X = 1.0f;
    TextureToBackBufferVerts[1].Y = -1.0f;
    TextureToBackBufferVerts[1].Z = 0.0f;
    TextureToBackBufferVerts[2].X = 1.0f;
    TextureToBackBufferVerts[2].Y = 1.0f;
    TextureToBackBufferVerts[2].Z = 0.0f;
    TextureToBackBufferVerts[3].X = -1.0f;
    TextureToBackBufferVerts[3].Y = 1.0f;
    TextureToBackBufferVerts[3].Z = 0.0f;
    ScreenCopyVertexShader = g_pEngine_01436e98->GetVertexShaderResource()->GetVertexShader("VSHADER_SCREEN_FX_COPY");
    ColourFilterPixelShader = g_pEngine_01436e98->GetPixelShaderResource()->GetPixelShader("PSHADER_SCREEN_FX_COLOUR_FILTER");
}