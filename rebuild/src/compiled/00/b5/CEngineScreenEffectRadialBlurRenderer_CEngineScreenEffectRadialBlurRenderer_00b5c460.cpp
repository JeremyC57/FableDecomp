// CEngineScreenEffectRadialBlurRenderer::CEngineScreenEffectRadialBlurRenderer(const CEngineInit& init) -- constructs the radial blur screen effect: supported only when the init flags carry 0x80 (otherwise the component is disabled); when supported, looks up the copy / blur / texture shaders, creates the textures and fills the four screen quads; then enables the environment parameters and resets them
// Leaf: ??0CEngineScreenEffectRadialBlurRenderer
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B5C460, length 1552 bytes (`ret 4`, no EH frame).
// Label correction: catalogued "CEnginePrimitiveManagerText::CEnginePrimitiveManagerText" (propagated); the body calls the
//   CEngineComponent ctor @ 0x00B59710 and stores vtable 0x012A2658 = CEngineScreenEffectRadialBlurRenderer (rtti-vtables.tsv).
// Callees: CEngineComponent() @ 0x00B59710, CVertexShader() @ 0x00A62FB0, CPixelShader() @ 0x00A5EC00, CCharString @ 0x0099EBF0 /
//   ~ @ 0x0099EAE0, GetVertexShader @ 0x00A5D5F0 / GetPixelShader @ 0x00A5D720, InitialiseTextures @ 0x00B5BF10,
//   ResetParameters @ 0x00B5BBC0.
// Layout: engine/CEngineScreenEffectRadialBlurRenderer.h offsets hold (0x22c bytes); restated locally with the real shader / vertex
//   types (model as the landed sibling 0x00B86C00).
// Shape: the per-quad X/Y/Z then U/V stores and a 4-iteration colour loop over the four quads; VC7.1 fully unrolls the loop and
//   schedules the stores exactly as retail.
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
    unsigned char    _pad0000[0x2a80];
    CShaderResource* VertexShaderResource;         // +0x2A80
    unsigned char    _pad2a84[0x2a98 - 0x2a84];
    CShaderResource* PixelShaderResource;          // +0x2A98
    CShaderResource* GetVertexShaderResource() { return VertexShaderResource; }
    CShaderResource* GetPixelShaderResource() { return PixelShaderResource; }
};
extern CEngineData* g_pEngineData;                 // 0x01436E98

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

class CTexture;
class C2DVector { public: float X, Y; };
class C3DVector { public: float X, Y, Z; };
struct CRadialBlurParameters {                     // CEngineScreenEffectRadialBlurRenderer::CParameters, 0x28 bytes
    float ThresholdR, ThresholdG, ThresholdB;
    float IntensityR, IntensityG, IntensityB;
    float Length, Fade, Range, Twist;
};

class CEngineScreenEffectRadialBlurRenderer : public CEngineComponent { public:   // vtable 0x012A2658, 0x22c bytes
    CTexture*       BlurTexture256_1;              // +0x0c
    CTexture*       BlurTexture256_2;              // +0x10
    long            BlurTextureHandle256_1;        // +0x14
    long            BlurTextureHandle256_2;        // +0x18
    long            DynamicTexturePageHandle;      // +0x1c
    CVertexShader   ScreenCopyVertexShader;        // +0x20
    CVertexShader   RadialBlurVertexShader;        // +0x28
    CPixelShader    ScreenCopyPixelShader;         // +0x30
    CPixelShader    RadialBlurPixelShader;         // +0x38
    CPixelShader    TextureOnlyPixelShader;        // +0x40
    CPixelShader    TextureDiffusePixelShader;     // +0x48
    CTVertexColTex1 BackBufferToTextureVerts[4];   // +0x50
    CTVertexColTex1 Blur256ToBlur256Verts[4];      // +0xb0
    CTVertexColTex1 TextureToBackBufferVerts[4];   // +0x110
    C2DVector       BlurCenter;                    // +0x170
    C2DVector       AreaCenter;                    // +0x178
    C3DVector       BlurCenterWorldTrackPosition;  // +0x180
    C3DVector       AreaCenterWorldTrackPosition;  // +0x18c
    CRadialBlurParameters CurrentParameters;       // +0x198
    bool            AreaTrack3DPos;                // +0x1c0
    bool            BlurTrack3DPos;                // +0x1c1
    long            ShowDebug;                     // +0x1c4
    bool            EffectSupported;               // +0x1c8
    bool            UseEnvironmentParameters;      // +0x1c9
    CTVertexColTex1 BlurToBackBufferVerts[4];      // +0x1cc

    CEngineScreenEffectRadialBlurRenderer(const CEngineInit& init);
    virtual ~CEngineScreenEffectRadialBlurRenderer();
    bool InitialiseTextures();                     // 0x00B5BF10
    void ResetParameters();                        // 0x00B5BBC0
};

CEngineScreenEffectRadialBlurRenderer::CEngineScreenEffectRadialBlurRenderer(const CEngineInit& init)
{
    if (init.Flags & 0x80) {
        EffectSupported = true;
    } else {
        EffectSupported = false;
        RenderEnabled = false;
    }
    if (EffectSupported) {
        ScreenCopyVertexShader = g_pEngineData->GetVertexShaderResource()->GetVertexShader("VSHADER_SCREEN_FX_RADIAL_BLUR_BBCOPY");
        ScreenCopyPixelShader = g_pEngineData->GetPixelShaderResource()->GetPixelShader("PSHADER_SCREEN_FX_RADIAL_BLUR_BBCOPY");
        RadialBlurVertexShader = g_pEngineData->GetVertexShaderResource()->GetVertexShader("VSHADER_SCREEN_FX_RADIAL_BLUR");
        RadialBlurPixelShader = g_pEngineData->GetPixelShaderResource()->GetPixelShader("PSHADER_SCREEN_FX_RADIAL_BLUR");
        TextureOnlyPixelShader = g_pEngineData->GetPixelShaderResource()->GetPixelShader("PSHADER_TEXTURE_ONLY");
        TextureDiffusePixelShader = g_pEngineData->GetPixelShaderResource()->GetPixelShader("PSHADER_TEXTURE_DIFFUSE");
    }
    ShowDebug = 0;
    BlurTexture256_1 = 0;
    BlurTexture256_2 = 0;
    DynamicTexturePageHandle = -1;
    if (EffectSupported) {
        InitialiseTextures();
        if (EffectSupported) {
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
            Blur256ToBlur256Verts[0].X = -1.0f;
            Blur256ToBlur256Verts[0].Y = -1.0f;
            Blur256ToBlur256Verts[0].Z = 0.0f;
            Blur256ToBlur256Verts[1].X = 1.0f;
            Blur256ToBlur256Verts[1].Y = -1.0f;
            Blur256ToBlur256Verts[1].Z = 0.0f;
            Blur256ToBlur256Verts[2].X = 1.0f;
            Blur256ToBlur256Verts[2].Y = 1.0f;
            Blur256ToBlur256Verts[2].Z = 0.0f;
            Blur256ToBlur256Verts[3].X = -1.0f;
            Blur256ToBlur256Verts[3].Y = 1.0f;
            Blur256ToBlur256Verts[3].Z = 0.0f;
            Blur256ToBlur256Verts[0].U1 = -0.5f;
            Blur256ToBlur256Verts[0].V1 = 0.5f;
            Blur256ToBlur256Verts[1].U1 = 0.5f;
            Blur256ToBlur256Verts[1].V1 = 0.5f;
            Blur256ToBlur256Verts[2].U1 = 0.5f;
            Blur256ToBlur256Verts[2].V1 = -0.5f;
            Blur256ToBlur256Verts[3].U1 = -0.5f;
            Blur256ToBlur256Verts[3].V1 = -0.5f;
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
            BlurToBackBufferVerts[0].X = -1.0f;
            BlurToBackBufferVerts[0].Y = -1.0f;
            BlurToBackBufferVerts[0].Z = 0.0f;
            BlurToBackBufferVerts[1].X = 0.0f;
            BlurToBackBufferVerts[1].Y = -1.0f;
            BlurToBackBufferVerts[1].Z = 0.0f;
            BlurToBackBufferVerts[2].X = 0.0f;
            BlurToBackBufferVerts[2].Y = 0.0f;
            BlurToBackBufferVerts[2].Z = 0.0f;
            BlurToBackBufferVerts[3].X = -1.0f;
            BlurToBackBufferVerts[3].Y = 0.0f;
            BlurToBackBufferVerts[3].Z = 0.0f;
            BlurToBackBufferVerts[0].U1 = 0.0f;
            BlurToBackBufferVerts[0].V1 = 1.0f;
            BlurToBackBufferVerts[1].U1 = 1.0f;
            BlurToBackBufferVerts[1].V1 = 1.0f;
            BlurToBackBufferVerts[2].U1 = 1.0f;
            BlurToBackBufferVerts[2].V1 = 0.0f;
            BlurToBackBufferVerts[3].U1 = 0.0f;
            BlurToBackBufferVerts[3].V1 = 0.0f;
            for (long i = 0; i < 4; ++i) {
                BackBufferToTextureVerts[i].DiffuseColour = 0xffffffff;
                Blur256ToBlur256Verts[i].DiffuseColour = 0xffffffff;
                TextureToBackBufferVerts[i].DiffuseColour = 0xffffffff;
                BlurToBackBufferVerts[i].DiffuseColour = 0xffffffff;
            }
        }
    }
    UseEnvironmentParameters = true;
    ResetParameters();
}