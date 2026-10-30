// CEngineScreenEffectDisplacementRenderer::~CEngineScreenEffectDisplacementRenderer() -- free its scratch page, restore an open capture block, release shader references and destroy the old render target; retail 0x00b870a0 is 266 bytes.
// Leaf: ??1CEngineScreenEffectDisplacementRenderer
// Owner evidence: vtable 0x012A3A94 slot0 is the deleting destructor 0x00B89050; slots 11 and 13 name this class's resource callbacks.
// Actual donor headers are included under *_PDB aliases. The retail view below uses the header's real member names and the PDB's 0x1ac extent; it expands the opaque 0x24 CRenderTarget as CBase plus two standalone CSurface members. CSurface is its own polymorphic COM-surface wrapper (vtable 0x0122F84C), not a CBase subclass: its genuine destructor calls Uninitialise, which releases PD3DSurface and clears the surface state. Genuine virtual destructors on the owner, component, CRenderTarget base, surfaces, and shader hierarchy let VC7.1 emit native destructor-phase vptr stores; shader-data deletion uses their RTTI-verified slot-0 destructors.
#include <stddef.h>
#include "rebuild_abi.h"

#define CEngineScreenEffectDisplacementRenderer CEngineScreenEffectDisplacementRenderer_PDB
#include "engine/CEngineScreenEffectDisplacementRenderer.h"
#undef CEngineScreenEffectDisplacementRenderer

#define CEngineComponent CEngineComponent_PDB
#include "engine/CEngineComponent.h"
#undef CEngineComponent

#define CSurface CSurface_PDB
#include "engine/CSurface.h"
#undef CSurface

#define CVertexShader CVertexShader_PDB
#include "engine/CVertexShader.h"
#undef CVertexShader

#include "engine/CRenderStateManager.h"
#include "fable_render_capture.h"

#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0

class CShaderBase { public:
    virtual ~CShaderBase(); // native shared shader-base vtable is 0x0129A030
};

class CEngineComponent { public:
    virtual ~CEngineComponent();
    void** GlobalPointerAddress;
    bool RenderEnabled;
    unsigned char KeepAlive;
    unsigned char Padding[2];
};

struct IDirect3DSurface9;
class CSurface { public:
    IDirect3DSurface9* PD3DSurface;
    unsigned int AllocationSource;
    void* PAllocatedMemory;
    void Uninitialise(); // 0x009F2E20; release COM surface, clear PD3DSurface and AllocationSource
    virtual ~CSurface(); // vtable 0x0122F84C; slot 0 destructor
};

class CRenderTarget : public CBaseClass { public:
    CSurface RenderToSurface;
    CSurface ZBufferSurface;
};

struct CVertexShaderData {
    virtual ~CVertexShaderData(); // RTTI vtable 0x0129DA50 slot0 -> deleting dtor 0x00A630D0
    void* VertexDeclaration;
    void* VertexShader;
    void* FormatDecl;
    unsigned int ReferenceCount;
};

struct CPixelShaderData {
    virtual ~CPixelShaderData(); // RTTI vtable 0x0129D58C slot0 -> deleting dtor 0x00A5ED00
    void* PixelShader;
    unsigned int ReferenceCount;
    unsigned int ConstCount;
    int* ConstIndices;
};

class CVertexShader : public CShaderBase { public:
    CVertexShaderData* Data;
    virtual ~CVertexShader();
};

class CPixelShader : public CShaderBase { public:
    CPixelShaderData* Data;
    virtual ~CPixelShader();
};

struct CRenderStateCaptureBlockRetail {
    bool Open;
    unsigned char Padding;
};

class CEngineInit; // No quarantined header exists; the exact PDB constructor signature uses this reference.

class CGPUScratchBufferTextureManager { public:
    void FreeMemoryPage(long page); // 0x00F37030
};

struct CEngineGlobal_01436e98 {
    unsigned char Prefix[0x3190];
    CGPUScratchBufferTextureManager ScratchBufferTextureManager;
};

extern CEngineGlobal_01436e98* g_pEngine_01436e98; // 0x01436E98
extern CRenderStateManagerCaptureView* g_pRenderStateManager_013cb508; // 0x013CB508
extern void FABLE_FASTCALL RestoreCaptureBlockAtGlobal(CRenderStateManagerCaptureView* manager); // 0x00A05840

class CEngineScreenEffectDisplacementRenderer : public CEngineComponent { public:
    void* BackBufferCopyTexture;                         // native +0x0c
    void* DisplacementTexture;                           // native +0x10
    fable_i32 DisplacementTextureHandle;                 // native +0x14
    fable_i32 DynamicTexturePageHandle;                  // native +0x18
    bool HasContent;                                     // native +0x1c
    unsigned char _pad_0x1d[3];                          // native +0x1d
    unsigned char BackBufferToTextureVerts[0x60];        // native +0x20, CTVertexColTex1[4]
    unsigned char TextureToBackBufferVerts[0x60];        // native +0x80, CTVertexColTex1[4]
    fable_i32 ClearX;                                    // native +0xe0
    fable_i32 ClearY;                                    // native +0xe4
    float DisplaceAmount;                                // native +0xe8
    bool DisplacementUsesShadowZBuffer;                  // native +0xec
    bool EffectSupported;                                // native +0xed
    bool Enable2DDisplacement;                            // native +0xee
    bool Enable3DDisplacement;                            // native +0xef
    CRenderTarget OldRenderTarget;                       // native +0xf0
    fable_i32 OldViewport_TLX;                            // native +0x114
    fable_i32 OldViewport_TLY;                            // native +0x118
    fable_i32 OldViewport_BRX;                            // native +0x11c
    fable_i32 OldViewport_BRY;                            // native +0x120
    CVertexShader ScreenCopyVertexShader;                 // native +0x124
    CPixelShader ScreenCopyPixelShader;                   // native +0x12c
    CVertexShader DisplacementVertexShader;               // native +0x134
    CPixelShader DisplacementPixelShader;                 // native +0x13c
    CRenderStateCaptureBlockRetail StencilRenderingCaptureBlock; // native +0x144
    unsigned char _pad_0x146[2];                         // native +0x146
    unsigned char DisplacementToBackBufferVerts[0x60];    // native +0x148, CTVertexColTex1[4]
    bool EnableDisplacementTextureView;                  // native +0x1a8
    unsigned char _pad_0x1a9[3];                         // native +0x1a9

    CEngineScreenEffectDisplacementRenderer(const CEngineInit& init); // PDB 0x02DB90D0
    virtual ~CEngineScreenEffectDisplacementRenderer();
};

#ifdef _MSC_VER
FABLE_STATIC_ASSERT(sizeof(CBaseClass) == 4);
FABLE_STATIC_ASSERT(sizeof(CShaderBase) == 4);
FABLE_STATIC_ASSERT(sizeof(CEngineComponent) == 0x0c);
FABLE_STATIC_ASSERT(sizeof(CSurface) == 0x10);
FABLE_STATIC_ASSERT(sizeof(CRenderTarget) == 0x24);
FABLE_STATIC_ASSERT(sizeof(CVertexShader) == 8);
FABLE_STATIC_ASSERT(sizeof(CPixelShader) == 8);
FABLE_STATIC_ASSERT(sizeof(CVertexShaderData) == 0x14);
FABLE_STATIC_ASSERT(sizeof(CPixelShaderData) == 0x14);
FABLE_STATIC_ASSERT(offsetof(CEngineScreenEffectDisplacementRenderer, DynamicTexturePageHandle) == 0x18);
FABLE_STATIC_ASSERT(offsetof(CEngineScreenEffectDisplacementRenderer, OldRenderTarget) == 0xf0);
FABLE_STATIC_ASSERT(offsetof(CEngineScreenEffectDisplacementRenderer, ScreenCopyVertexShader) == 0x124);
FABLE_STATIC_ASSERT(offsetof(CEngineScreenEffectDisplacementRenderer, DisplacementPixelShader) == 0x13c);
FABLE_STATIC_ASSERT(offsetof(CEngineScreenEffectDisplacementRenderer, StencilRenderingCaptureBlock) == 0x144);
FABLE_STATIC_ASSERT(sizeof(CEngineScreenEffectDisplacementRenderer) == 0x1ac);
#endif

CSurface::~CSurface()
{
    Uninitialise();
}

CShaderBase::~CShaderBase()
{
}

CVertexShader::~CVertexShader()
{
    CVertexShaderData* data = Data;
    if (data == 0)
        return;
    if (--data->ReferenceCount == 0)
        delete data;
}

CPixelShader::~CPixelShader()
{
    CPixelShaderData* data = Data;
    if (data == 0)
        return;
    if (--data->ReferenceCount == 0)
        delete data;
}

CEngineScreenEffectDisplacementRenderer::~CEngineScreenEffectDisplacementRenderer()
{
    const fable_i32 page = DynamicTexturePageHandle;
    if (page >= 0)
        g_pEngine_01436e98->ScratchBufferTextureManager.FreeMemoryPage(page);
    if (StencilRenderingCaptureBlock.Open) {
        StencilRenderingCaptureBlock.Open = false;
        RestoreCaptureBlockAtGlobal(g_pRenderStateManager_013cb508);
    }
}