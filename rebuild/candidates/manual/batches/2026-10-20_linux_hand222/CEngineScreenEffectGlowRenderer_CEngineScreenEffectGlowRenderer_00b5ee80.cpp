// CEngineScreenEffectGlowRenderer::~CEngineScreenEffectGlowRenderer() -- free the glow renderer's dynamic texture page (when it has one), release its nine vertex/pixel shaders (last declared first) and run the CEngineComponent base destructor
// Leaf: ??1CEngineScreenEffectGlowRenderer
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B5EE80, length 279 bytes (tail `jmp` to the base destructor).
// Callees: CGPUScratchBufferTextureManager::FreeMemoryPage(long) @ 0x00F37030 (engine global @ 0x01436E98 + 0x3190, as the landed
// CEngineScreenEffectRadialBlurRenderer::SetEffectQuality 0x00B5BFD0), CEngineComponent::~CEngineComponent @ 0x00B59730 (landed);
// each shader's data object is destroyed through its virtual slot 0 with 1.
// Vtables: 0x012A27CC CEngineScreenEffectGlowRenderer, 0x0129A054 CPixelShader, 0x0129B3D0 CVertexShader, 0x0129A030 CShaderBase.
// Layout: engine/CEngineScreenEffectGlowRenderer.h (DynamicTexturePageHandle +0x30; VertexShaderCopy/Blur/ApplyGlow +0x34/+0x3c/+0x44,
// PixelShaderGlow/GlowNeg/Blur/ApplyGlow +0x4c..+0x64, VertexShaderShowDebug +0x6c, PixelShaderShowDebug +0x74) -- modelled locally
// because the members need their real shader classes for the destructor chain. The shader classes follow the landed
// CPixelShader `??_G` 0x0098C3D0 (data counts at +8 for pixel shaders, +0x10 for vertex shaders).
// Shape: own vtable store, the inline page free (`test ; jl`), then each member destructor inlined in reverse order: its own
// vtable store, the null-tested `dec [data+N] ; jne ; call [vt]` release, the CShaderBase vtable store; the base destructor
// is tail-called. The destructor is non-virtual in the model so the TU emits no `??_G` (the vtable comes from v00).
struct CGPUScratchBufferTextureManager {
    void FreeMemoryPage(long page);                // 0x00F37030
};
struct CEngineGlobal_01436e98 {
    unsigned char                   _pad[0x3190];
    CGPUScratchBufferTextureManager m_scratchTextures;   // +0x3190
};
extern CEngineGlobal_01436e98* g_pEngine_01436e98;   // 0x01436E98

struct CPixelShaderData {
    virtual ~CPixelShaderData();                   // slot 0: the deleting destructor is called with 1
    void* m_pShader;                               // +0x04
    long  m_refCount;                              // +0x08
};
struct CVertexShaderData {
    virtual ~CVertexShaderData();                  // slot 0
    unsigned char _pad04[0xc];
    long  m_refCount;                              // +0x10
};

struct CShaderBase {                               // vtable 0x0129A030
    virtual ~CShaderBase() { }
};
struct CPixelShader : CShaderBase {                // vtable 0x0129A054
    CPixelShaderData* Data;                        // +0x04
    __forceinline virtual ~CPixelShader()
    {
        CPixelShaderData* data = Data;
        if (data) {
            if (--data->m_refCount == 0)
                delete data;
        }
    }
};
struct CVertexShader : CShaderBase {               // vtable 0x0129B3D0
    CVertexShaderData* Data;                       // +0x04
    __forceinline virtual ~CVertexShader()
    {
        CVertexShaderData* data = Data;
        if (data) {
            if (--data->m_refCount == 0)
                delete data;
        }
    }
};

struct CEngineComponent {
    virtual void v00();
    void** m_ppOwnerLink;                          // +0x04
    ~CEngineComponent();                           // 0x00B59730
};

struct CTexture;
struct CEngineScreenEffectGlowRenderer : CEngineComponent {   // vtable 0x012A27CC
    unsigned char _pad_0x08[0x4];
    bool          UseEnvironmentTheme;             // +0x0c
    CTexture*     GlowTexture256;                  // +0x10
    CTexture*     GlowTexture64;                   // +0x14
    CTexture*     GlowTexture64Alt;                // +0x18
    CTexture*     GlowTexture16;                   // +0x1c
    long          GlowTexture256Handle;            // +0x20
    long          GlowTexture64Handle;             // +0x24
    long          GlowTexture64AltHandle;          // +0x28
    long          GlowTexture16Handle;             // +0x2c
    long          DynamicTexturePageHandle;        // +0x30
    CVertexShader VertexShaderCopy;                // +0x34
    CVertexShader VertexShaderBlur;                // +0x3c
    CVertexShader VertexShaderApplyGlow;           // +0x44
    CPixelShader  PixelShaderGlow;                 // +0x4c
    CPixelShader  PixelShaderGlowNeg;              // +0x54
    CPixelShader  PixelShaderBlur;                 // +0x5c
    CPixelShader  PixelShaderApplyGlow;            // +0x64
    CVertexShader VertexShaderShowDebug;           // +0x6c
    CPixelShader  PixelShaderShowDebug;            // +0x74
    virtual void v00();
    void FreeTexturePage()
    {
        if (DynamicTexturePageHandle >= 0)
            g_pEngine_01436e98->m_scratchTextures.FreeMemoryPage(DynamicTexturePageHandle);
    }
    ~CEngineScreenEffectGlowRenderer();
};

CEngineScreenEffectGlowRenderer::~CEngineScreenEffectGlowRenderer()
{
    FreeTexturePage();
}
