// CEngineScreenEffectDisplacementRenderer::CEngineScreenEffectDisplacementRenderer(const CEngineInit& init) -- construct the screen displacement effect: the component base, the saved render target, the four shader wrappers and the capture block; the effect (and its 2D/3D switches) is supported only when the init flags carry both 0x80 and 8 (otherwise the component is disabled); when supported, create the textures, fill the three screen quads (back buffer -> texture, texture -> back buffer with a half-texel offset, displacement -> back buffer on the lower-left quarter) and look up the copy and displacement shaders; then the clear size 128 x 128 and displace amount 0.25
// Leaf: ??0CEngineScreenEffectDisplacementRenderer
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B86C00, length 1184 bytes, `ret 4`, no EH frame.
// Label correction: catalogued "CEnginePrimitiveManager2DSprites::CEnginePrimitiveManager2DSprites" (propagated); the vtable
//   stored is 0x012A3A94 = CEngineScreenEffectDisplacementRenderer (rtti-vtables.tsv), InitialiseTextures 0x00B86A70 is that
//   class's, and the landed destructor 0x00B870A0 releases exactly these members.
// Callees: CEngineComponent::CEngineComponent @ 0x00B59710 (manifest label "CNavigationLayer" propagated), CBaseClass::CBaseClass
//   @ 0x0099A2D0, CVertexShader::CVertexShader @ 0x00A62FB0, CPixelShader::CPixelShader @ 0x00A5EC00, InitialiseTextures
//   @ 0x00B86A70, CCharString(const char*, long) @ 0x0099EBF0 / ~CCharString @ 0x0099EAE0, CShaderResource::GetVertexShader
//   @ 0x00A5D5F0 / GetPixelShader @ 0x00A5D720 (engine data 0x01436E98: vertex resource +0x2A28, pixel resource +0x2A98).
// Layout: the landed destructor 0x00B870A0's retail view of engine/CEngineScreenEffectDisplacementRenderer.h (0x1ac bytes);
//   the shader wrappers and the lookup/assign/temporary-release shape are the landed CEnginePrimitiveManager2DSprites ctor
//   0x00BAD040 (the literal converts implicitly to the `const CCharString&` parameter; the length -1 is a default argument).
// Shape: the twelve vertex colours are ONE `for (i < 4)` loop over the three quads -- VC7.1 unrolls it fully, which is what
//   interleaves the colour stores (TextureToBackBuffer, BackBufferToTexture, Displacement per vertex) at the end and lets the
//   scheduler group the other constant stores by register; per-vertex Set()/member stores give another store order.
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
    unsigned char    _pad0000[0x2a28];
    CShaderResource* VertexShaderResource;         // +0x2A28
    unsigned char    _pad2a2c[0x2a98 - 0x2a2c];
    CShaderResource* PixelShaderResource;          // +0x2A98
    CShaderResource* GetVertexShaderResource() { return VertexShaderResource; }
    CShaderResource* GetPixelShaderResource() { return PixelShaderResource; }
};
extern CEngineData* g_pEngineData;                 // 0x01436E98

#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
struct IDirect3DSurface9;
class CSurface { public:                                  // vtable 0x0122F84C, 0x10 bytes
    IDirect3DSurface9* PD3DSurface;                // +0x04
    unsigned long      AllocationSource;           // +0x08
    void*              PAllocatedMemory;           // +0x0c
    CSurface() : PD3DSurface(0), AllocationSource(0) { }
    virtual ~CSurface();
};
class CRenderTarget : public CBaseClass { public:                     // vtable 0x0129C800, 0x24 bytes
    CSurface RenderToSurface;                      // +0x04
    CSurface ZBufferSurface;                       // +0x14
    CRenderTarget() { }
};

class CEngineInit { public:
    unsigned char _pad00[0x60];
    unsigned long Flags;                           // +0x60
};

class CEngineComponent { public:
    CEngineComponent();                            // 0x00B59710
    virtual ~CEngineComponent();
    void**        GlobalPointerAddress;            // +0x04
    bool          RenderEnabled;                   // +0x08
    unsigned char KeepAlive;                       // +0x09
    unsigned char Padding[2];
};

class CTVertexColTex1 { public:                           // 0x18 bytes
    float         X, Y, Z;
    unsigned long DiffuseColour;
    float         U1, V1;
    void Set(float x, float y, float z, unsigned long colour, float u, float v)
    {
        X = x; Y = y; Z = z; DiffuseColour = colour; U1 = u; V1 = v;
    }
};

struct CRenderStateCaptureBlock {
    bool          Open;
    unsigned char Padding;
    CRenderStateCaptureBlock() : Open(false) { }
};

class CEngineScreenEffectDisplacementRenderer : public CEngineComponent { public:   // vtable 0x012A3A94
    void*           BackBufferCopyTexture;         // +0x0c
    void*           DisplacementTexture;           // +0x10
    long            DisplacementTextureHandle;     // +0x14
    long            DynamicTexturePageHandle;      // +0x18
    bool            HasContent;                    // +0x1c
    unsigned char   _pad_0x1d[3];
    CTVertexColTex1 BackBufferToTextureVerts[4];   // +0x20
    CTVertexColTex1 TextureToBackBufferVerts[4];   // +0x80
    long            ClearX;                        // +0xe0
    long            ClearY;                        // +0xe4
    float           DisplaceAmount;                // +0xe8
    bool            DisplacementUsesShadowZBuffer; // +0xec
    bool            EffectSupported;               // +0xed
    bool            Enable2DDisplacement;          // +0xee
    bool            Enable3DDisplacement;          // +0xef
    CRenderTarget   OldRenderTarget;               // +0xf0
    long            OldViewport_TLX;               // +0x114
    long            OldViewport_TLY;               // +0x118
    long            OldViewport_BRX;               // +0x11c
    long            OldViewport_BRY;               // +0x120
    CVertexShader   ScreenCopyVertexShader;        // +0x124
    CPixelShader    ScreenCopyPixelShader;         // +0x12c
    CVertexShader   DisplacementVertexShader;      // +0x134
    CPixelShader    DisplacementPixelShader;       // +0x13c
    CRenderStateCaptureBlock StencilRenderingCaptureBlock;   // +0x144
    unsigned char   _pad_0x146[2];
    CTVertexColTex1 DisplacementToBackBufferVerts[4];   // +0x148
    bool            EnableDisplacementTextureView; // +0x1a8

    CEngineScreenEffectDisplacementRenderer(const CEngineInit& init);
    virtual ~CEngineScreenEffectDisplacementRenderer();
    bool InitialiseTextures();                     // 0x00B86A70
};

CEngineScreenEffectDisplacementRenderer::CEngineScreenEffectDisplacementRenderer(const CEngineInit& init)
{
    if ((init.Flags & 0x80) && (init.Flags & 8)) {
        EffectSupported = true;
        Enable2DDisplacement = true;
        Enable3DDisplacement = true;
    } else {
        EffectSupported = false;
        Enable2DDisplacement = false;
        Enable3DDisplacement = false;
        RenderEnabled = false;
    }
    DynamicTexturePageHandle = -1;
    EnableDisplacementTextureView = false;
    DisplacementUsesShadowZBuffer = true;
    if (EffectSupported) {
        InitialiseTextures();
        BackBufferToTextureVerts[0].X = -1.0f;
        BackBufferToTextureVerts[0].Y = -1.0f;
        BackBufferToTextureVerts[0].Z = 0.0f;
        BackBufferToTextureVerts[1].X = 1.0f;
        BackBufferToTextureVerts[1].Y = -1.0f;
        BackBufferToTextureVerts[1].Z = 0.0f;
        BackBufferToTextureVerts[2].X = 1.0f;
        BackBufferToTextureVerts[2].Y = 1.0f;
        BackBufferToTextureVerts[2].Z = 0.0f;
        BackBufferToTextureVerts[3].X = -1.0f;
        BackBufferToTextureVerts[3].Y = 1.0f;
        BackBufferToTextureVerts[3].Z = 0.0f;
        BackBufferToTextureVerts[0].U1 = 0.0f;
        BackBufferToTextureVerts[0].V1 = 1.0f;
        BackBufferToTextureVerts[1].U1 = 1.0f;
        BackBufferToTextureVerts[1].V1 = 1.0f;
        BackBufferToTextureVerts[2].U1 = 1.0f;
        BackBufferToTextureVerts[2].V1 = 0.0f;
        BackBufferToTextureVerts[3].U1 = 0.0f;
        BackBufferToTextureVerts[3].V1 = 0.0f;
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
        TextureToBackBufferVerts[0].U1 = 0.001953125f;
        TextureToBackBufferVerts[0].V1 = 1.001953125f;
        TextureToBackBufferVerts[1].U1 = 1.001953125f;
        TextureToBackBufferVerts[1].V1 = 1.001953125f;
        TextureToBackBufferVerts[2].U1 = 1.001953125f;
        TextureToBackBufferVerts[2].V1 = 0.001953125f;
        TextureToBackBufferVerts[3].U1 = 0.001953125f;
        TextureToBackBufferVerts[3].V1 = 0.001953125f;
        DisplacementToBackBufferVerts[0].X = -1.0f;
        DisplacementToBackBufferVerts[0].Y = -1.0f;
        DisplacementToBackBufferVerts[0].Z = 0.0f;
        DisplacementToBackBufferVerts[1].X = 0.0f;
        DisplacementToBackBufferVerts[1].Y = -1.0f;
        DisplacementToBackBufferVerts[1].Z = 0.0f;
        DisplacementToBackBufferVerts[2].X = 0.0f;
        DisplacementToBackBufferVerts[2].Y = 0.0f;
        DisplacementToBackBufferVerts[2].Z = 0.0f;
        DisplacementToBackBufferVerts[3].X = -1.0f;
        DisplacementToBackBufferVerts[3].Y = 0.0f;
        DisplacementToBackBufferVerts[3].Z = 0.0f;
        DisplacementToBackBufferVerts[0].U1 = 0.0f;
        DisplacementToBackBufferVerts[0].V1 = 1.0f;
        DisplacementToBackBufferVerts[1].U1 = 1.0f;
        DisplacementToBackBufferVerts[1].V1 = 1.0f;
        DisplacementToBackBufferVerts[2].U1 = 1.0f;
        DisplacementToBackBufferVerts[2].V1 = 0.0f;
        DisplacementToBackBufferVerts[3].U1 = 0.0f;
        DisplacementToBackBufferVerts[3].V1 = 0.0f;
        for (long i = 0; i < 4; ++i) {
            TextureToBackBufferVerts[i].DiffuseColour = 0xffffffff;
            BackBufferToTextureVerts[i].DiffuseColour = 0xffffffff;
            DisplacementToBackBufferVerts[i].DiffuseColour = 0xffffffff;
        }
        ScreenCopyVertexShader = g_pEngineData->GetVertexShaderResource()->GetVertexShader("VSHADER_SCREEN_FX_COPY");
        ScreenCopyPixelShader = g_pEngineData->GetPixelShaderResource()->GetPixelShader("PSHADER_TEXTURE_ONLY");
        DisplacementVertexShader = g_pEngineData->GetVertexShaderResource()->GetVertexShader("VSHADER_SCREEN_FX_DISPLACEMENT");
        DisplacementPixelShader = g_pEngineData->GetPixelShaderResource()->GetPixelShader("PSHADER_SCREEN_FX_DISPLACEMENT");
    }
    ClearX = 0x80;
    ClearY = 0x80;
    HasContent = false;
    DisplaceAmount = 0.25f;
}