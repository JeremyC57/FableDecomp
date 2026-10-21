// CEngineScreenEffectOutlineGlow::~CEngineScreenEffectOutlineGlow() -- free the dynamic texture page, close the outline render-state capture block, release the six outline/blur shaders (last declared first), uninitialise the saved render target's two surfaces and run the CEngineComponent base destructor
// Leaf: ??1CEngineScreenEffectOutlineGlow
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B898D0, length 336 bytes (tail `jmp` to the base destructor).
// Callees: CGPUScratchBufferTextureManager::FreeMemoryPage(long) @ 0x00F37030 (engine global @ 0x01436E98 + 0x3190),
//   CRenderStateManager::RestoreCaptureBlock @ 0x00A05840 (global @ 0x013CB508), CSurface::Uninitialise @ 0x009F2E20,
//   CBaseClass::~CBaseClass @ 0x0099A2E0, CEngineComponent::~CEngineComponent @ 0x00B59730 (landed).
// Vtables: 0x012A3B6C CEngineScreenEffectOutlineGlow, 0x0129A054 CPixelShader, 0x0129B3D0 CVertexShader,
//   0x0129A030 CShaderBase, 0x0122F84C CSurface (rtti-vtables.tsv).
// Layout: engine/CEngineScreenEffectOutlineGlow.h offsets (DynamicTexturePageHandle +0x24, OldRenderTarget +0x14c,
//   OutlineToBlurVertexShader +0x180 .. BlurToBackBufferPixelShader +0x1a8, OutlineRenderingCaptureBlock +0x1e0); the
//   header flattens the shader, render-target and capture-block members to byte arrays, so the class is modelled
//   locally with their real classes (needed for the destructor chain), as the landed CEngineScreenEffectGlowRenderer
//   destructor 0x00B5EE80. PDB CRenderTarget (0x24) = CBaseClass + RenderToSurface +4 + ZBufferSurface +0x14 (CSurface 0x10).
// Shape: the body frees the page unconditionally; member destructors inline in reverse order: the capture block
//   (`if (Open) { Open = false; Restore(); }`), each shader (own vtable, null-tested `dec ; jne ; call [vt]` release,
//   CShaderBase vtable), then the render target's implicit destructor (no vtable store): ZBufferSurface then
//   RenderToSurface (own vtable + Uninitialise), then ~CBaseClass; the base destructor is tail-called.
struct CGPUScratchBufferTextureManager {
    void FreeMemoryPage(long page);                // 0x00F37030
};
struct CEngineGlobal_01436e98 {
    unsigned char                   _pad[0x3190];
    CGPUScratchBufferTextureManager m_scratchTextures;   // +0x3190
};
extern CEngineGlobal_01436e98* g_pEngine_01436e98;   // 0x01436E98

struct CRenderStateManager {
    void RestoreCaptureBlock();                    // 0x00A05840
};
extern CRenderStateManager* g_pRenderStateManager;   // 0x013CB508

struct CRenderStateCaptureBlock {
    bool Open;
    __forceinline ~CRenderStateCaptureBlock()
    {
        if (Open) {
            Open = false;
            g_pRenderStateManager->RestoreCaptureBlock();
        }
    }
};

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

struct CBaseClass {
    virtual void v00();
    ~CBaseClass();                                 // 0x0099A2E0
};
struct IDirect3DSurface9;
struct CSurface {                                  // vtable 0x0122F84C, 0x10 bytes
    IDirect3DSurface9* PD3DSurface;                // +0x04
    unsigned long      AllocationSource;           // +0x08
    void*              PAllocatedMemory;           // +0x0c
    virtual void v00();
    void Uninitialise();                           // 0x009F2E20
    __forceinline ~CSurface() { Uninitialise(); }
};
struct CRenderTarget : CBaseClass {                // 0x24 bytes, implicit destructor
    CSurface RenderToSurface;                      // +0x04
    CSurface ZBufferSurface;                       // +0x14
};

struct CEngineComponent {
    virtual void v00();
    void** m_ppOwnerLink;                          // +0x04
    ~CEngineComponent();                           // 0x00B59730
};

struct CTexture;
struct CEngineScreenEffectOutlineGlow : CEngineComponent {   // vtable 0x012A3B6C, 0x1e4 bytes
    unsigned char  _pad_0x08[0x4];                 // +0x08 active flag
    CTexture*      OutlineTexture;                 // +0x0c
    CTexture*      Blur1Texture;                   // +0x10
    CTexture*      Blur2Texture;                   // +0x14
    long           OutlineTextureHandle;           // +0x18
    long           Blur1TextureHandle;             // +0x1c
    long           Blur2TextureHandle;             // +0x20
    long           DynamicTexturePageHandle;       // +0x24
    bool           HasContent;                     // +0x28
    unsigned char  _pad_0x29[0x3];
    unsigned char  OutlineToBlurVerts[0x60];       // +0x2c CTVertexColTex1[4]
    unsigned char  BlurToBlurVerts[0x60];          // +0x8c
    unsigned char  BlurToBackBufferVerts[0x60];    // +0xec
    CRenderTarget  OldRenderTarget;                // +0x14c
    long           OldViewport_TLX;                // +0x170 C2DBoxI
    long           OldViewport_TLY;                // +0x174
    long           OldViewport_BRX;                // +0x178
    long           OldViewport_BRY;                // +0x17c
    CVertexShader  OutlineToBlurVertexShader;      // +0x180
    CPixelShader   OutlineToBlurPixelShader;       // +0x188
    CVertexShader  BlurToBlurVertexShader;         // +0x190
    CPixelShader   BlurToBlurPixelShader;          // +0x198
    CVertexShader  BlurToBackBufferVertexShader;   // +0x1a0
    CPixelShader   BlurToBackBufferPixelShader;    // +0x1a8
    float          BlurFactor;                     // +0x1b0
    float          SharpenFactor;                  // +0x1b4
    float          OuterStencilExtraNormalExtrusion;   // +0x1b8
    float          OutlineGlowScaleFactor;         // +0x1bc
    bool           ParametersOverridden;           // +0x1c0
    float          OverriddenParamsWidthWorldSpace;    // +0x1c4
    float          OverriddenParamsWidthScreenSpace;   // +0x1c8
    unsigned long  OverriddenParamsColour;         // +0x1cc CRGBColour
    long           OverriddenParamsTextureAlphaRef;    // +0x1d0
    long           OverriddenParamsTextureAlphaBias;   // +0x1d4
    long           BlurStepCount;                  // +0x1d8
    float          DepthBufferBias;                // +0x1dc
    CRenderStateCaptureBlock OutlineRenderingCaptureBlock;   // +0x1e0
    bool           EnableDrawDebugTextures;        // +0x1e1
    virtual void v00();
    ~CEngineScreenEffectOutlineGlow();
};

CEngineScreenEffectOutlineGlow::~CEngineScreenEffectOutlineGlow()
{
    g_pEngine_01436e98->m_scratchTextures.FreeMemoryPage(DynamicTexturePageHandle);
}