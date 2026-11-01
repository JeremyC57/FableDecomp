// CEngineScreenEffectRadialBlurRenderer::~CEngineScreenEffectRadialBlurRenderer() -- free the dynamic texture page, then destroy the four pixel shaders and two vertex shaders in reverse order and ~CEngineComponent
// Leaf: ??1CEngineScreenEffectRadialBlurRenderer@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B5C050, 205 bytes, tail jump to ~CEngineComponent 0x00B59730; sole caller the scalar
//   deleting dtor 0x00B5EB00 (slot 0 of the class vtable 0x012A2658).
// Identity (proven): rtti-vtables.tsv / the RTTI COL at 0x012A2658-4 (.?AVCEngineScreenEffectRadialBlurRenderer@@ :
//   CEngineComponent) and the body's own vtable store; Ego_r.pdb ??1CEngineScreenEffectRadialBlurRenderer@@UAE@XZ. The
//   catalog label is right.
// Layout: Ego_r.pdb CEngineScreenEffectRadialBlurRenderer (engine/CEngineScreenEffectRadialBlurRenderer.h has the same
//   offsets but flattens the base and the shader members to byte arrays and has no methods, so the class is restated with the
//   real member types; NOTES.md "Header debt"): CEngineComponent 0xc, BlurTexture256_1 +0x0c ... DynamicTexturePageHandle +0x1c,
//   ScreenCopyVertexShader +0x20, RadialBlurVertexShader +0x28, ScreenCopyPixelShader +0x30, RadialBlurPixelShader +0x38,
//   TextureOnlyPixelShader +0x40, TextureDiffusePixelShader +0x48 (the rest of the 0x22c record is trivially destructible).
// Body: `if (DynamicTexturePageHandle >= 0) FreeMemoryPage(...)` as the landed InitialiseTextures 0x00B5BF10 writes it, on the
//   engine resource manager's scratch-texture manager (+0x3190).
// Layout: engine/CEngineComponent.h and engine/CVertexShader.h are flattened pack(1) records (void* vptr slot, no virtual
//   functions), so CEngineComponent and the shader classes are restated locally with their RTTI bases (NOTES.md "Header debt").
// Callees: CGPUScratchBufferTextureManager::FreeMemoryPage(long) 0x00F37030 (Ego_r ?FreeMemoryPage@CGPUScratchBufferTextureManager
//   @@QAEXJ@Z), the shader dtors inline (virtual deleting dtor of the shader data, flag 1), ~CEngineComponent 0x00B59730.
// Shader models as the landed 0x00BABB50 / 0x00BAD040 (RTTI: CVertexShader 0x0129B3D0 / CPixelShader 0x0129A054 : CShaderBase
// 0x0129A030, 8 bytes each; Ego_r has no ??1 for any of the three, so their user dtors are inline). The shader data objects are
// intrusively counted (ReferenceCount +0x10 / +0x08) with a virtual deleting dtor.
struct CVertexShaderData {
    virtual ~CVertexShaderData();                  // slot 0
    void*         VertexDeclaration;               // +0x04
    void*         VertexShader;                    // +0x08
    const void*   FormatDecl;                      // +0x0c
    long          ReferenceCount;                  // +0x10
    long RemoveReference() { return --ReferenceCount; }
};
struct CPixelShaderData {
    virtual ~CPixelShaderData();                   // slot 0
    void*         PixelShader;                     // +0x04
    long          ReferenceCount;                  // +0x08
    long RemoveReference() { return --ReferenceCount; }
};
class CShaderBase { public:                        // vtable 0x0129A030
    virtual ~CShaderBase() { }
};
class CVertexShader : public CShaderBase { public: // vtable 0x0129B3D0
    CVertexShader();                               // 0x00A62FB0
    virtual ~CVertexShader() { if (Data) { CVertexShaderData* current = Data; if (!current->RemoveReference()) delete current; } }
    CVertexShaderData* Data;                       // +0x04
};
class CPixelShader : public CShaderBase { public:  // vtable 0x0129A054
    CPixelShader();                                // 0x00A5EC00
    virtual ~CPixelShader() { if (Data) { CPixelShaderData* current = Data; if (!current->RemoveReference()) delete current; } }
    CPixelShaderData* Data;                        // +0x04
};
class CGPUScratchBufferTextureManager { public:
    void FreeMemoryPage(long);                     // 0x00F37030
};
struct CEngineGlobal_01436e98 {
    unsigned char                   _pad0000[0x3190];
    CGPUScratchBufferTextureManager ScratchTextures;   // +0x3190
};
extern CEngineGlobal_01436e98* g_pEngine_01436e98;   // 0x01436E98
class CEngineComponent { public:                   // RTTI base, 0xc bytes (engine/CEngineComponent.h layout)
    virtual ~CEngineComponent();                   // 0x00B59730
    void**        GlobalPointerAddress;            // +0x04
    bool          RenderEnabled;                   // +0x08
    unsigned char KeepAlive;                       // +0x09
};
class CTexture;
class CEngineInit;
class CEngineScreenEffectRadialBlurRenderer : public CEngineComponent {
public:
    CEngineScreenEffectRadialBlurRenderer(const CEngineInit&);
    virtual ~CEngineScreenEffectRadialBlurRenderer();
    CTexture*     BlurTexture256_1;                // +0x0c
    CTexture*     BlurTexture256_2;                // +0x10
    long          BlurTextureHandle256_1;          // +0x14
    long          BlurTextureHandle256_2;          // +0x18
    long          DynamicTexturePageHandle;        // +0x1c
    CVertexShader ScreenCopyVertexShader;          // +0x20
    CVertexShader RadialBlurVertexShader;          // +0x28
    CPixelShader  ScreenCopyPixelShader;           // +0x30
    CPixelShader  RadialBlurPixelShader;           // +0x38
    CPixelShader  TextureOnlyPixelShader;          // +0x40
    CPixelShader  TextureDiffusePixelShader;       // +0x48
};
CEngineScreenEffectRadialBlurRenderer::~CEngineScreenEffectRadialBlurRenderer()
{
    if (DynamicTexturePageHandle >= 0)
        g_pEngine_01436e98->ScratchTextures.FreeMemoryPage(DynamicTexturePageHandle);
}
