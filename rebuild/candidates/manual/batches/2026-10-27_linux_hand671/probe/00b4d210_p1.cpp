// CEngineShadowRenderer::~CEngineShadowRenderer() -- shadow renderer destructor: release the default-pool resources, then the members in reverse order (lookup / colour textures, the two shadow-buffer surfaces, the shadow-buffer and render-surface textures, the render target, the shadow-buffer capture block) and ~CEngineComponent
// Leaf: ??1CEngineShadowRenderer@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B4D210, 171 bytes, thiscall, tail jump to CEngineComponent::~CEngineComponent 0x00B59730.
// Identity (proven): the first store is vtable 0x012A2374 (rtti-vtables.tsv: CEngineShadowRenderer, RTTI
//   .?AVCEngineShadowRenderer@@ : CEngineComponent); the sole caller 0x00B50B83 is the scalar deleting destructor 0x00B50B80,
//   slot 0 of that vtable. Ego_r.pdb / FableWin.pdb ??1CEngineShadowRenderer@@UAE@XZ; the catalog label is right. The members
//   walked are Ego_r's (retail offsets equal the PDB here).
// Callees: ?OnReleaseDefaultPoolResources@CEngineShadowRenderer@@UAEXXZ 0x00B4B830 (virtual, bound directly in the dtor),
//   CTexture::Uninitialise 0x009F9F70 (x4, inline ~CTexture), CSurface::Uninitialise 0x009F2E20 (x4, inline user ~CSurface),
//   CBaseClass::~CBaseClass 0x0099A2E0, CRenderStateManager::RestoreCaptureBlock() 0x00A05840 (global 0x013CB508,
//   ?g_pRenderStateManager@@3PAVCRenderStateManager@@A as landed), ??1CEngineComponent@@UAE@XZ 0x00B59730 (tail).
// Vtables: CSurface 0x0122F84C (rtti-vtables.tsv).
// Layout: Ego_r.pdb CEngineShadowRenderer (0x340): SceneData +0x0C, ShadowBufferRenderStateCaptureBlock +0x24C, ...,
//   RenderTarget +0x2C4 (CRenderTarget: CBaseClass, RenderToSurface +0x04, ZBufferSurface +0x14), ShadowBufferTexture +0x2E8,
//   RenderSurface +0x2F0, ShadowBufferDepthSurface +0x2F8, ShadowBufferColourSurface +0x308, ShadowBufferColourTexture +0x318,
//   ShadowBufferTextureUVScaler +0x320, DepthPackingLookupTexture +0x32C, RenderingTechnique +0x334,
//   UserVideoOptionShadowBufferSize +0x338, UserVideoOptionShadowDistanceScale +0x33C. engine/CEngineShadowRenderer.h is a
//   flattened record (members as byte arrays), so the class is a local model with the PDB member classes.
// Shape: user body `OnReleaseDefaultPoolResources();`; member dtors in reverse order; ~CRenderStateCaptureBlock (PDB
//   ??1CRenderStateCaptureBlock@@QAE@XZ) inline: `if (Open) { Open = false; g_pRenderStateManager->RestoreCaptureBlock(); }`.
#include "engine/CBaseClass.h"                              // ~CBaseClass 0x0099A2E0

struct IDirect3DSurface9;

class CSurface { public:                                    // vtable 0x0122F84C, 0x10 bytes
    virtual ~CSurface() { Uninitialise(); }
    void Uninitialise();                                    // 0x009F2E20
    IDirect3DSurface9* PD3DSurface;                         // +0x04
    unsigned long      AllocationSource;                    // +0x08
    void*              PAllocatedMemory;                    // +0x0C
};
class CRenderTarget : public CBaseClass { public:           // 0x24 bytes, implicit destructor
    CSurface RenderToSurface;                               // +0x04
    CSurface ZBufferSurface;                                // +0x14
};


class CRenderStateManager { public:
    void RestoreCaptureBlock();                             // 0x00A05840
};
extern CRenderStateManager* g_pRenderStateManager;          // 0x013CB508
class CRenderStateCaptureBlock { public:
    bool Open;                                              // +0x00
    ~CRenderStateCaptureBlock()
    {
        if (Open) {
            Open = false;
            g_pRenderStateManager->RestoreCaptureBlock();
        }
    }
};
struct IDirect3DTexture9;
class CTexture { public:
    ~CTexture() { Uninitialise(); }
    void Uninitialise();                                    // 0x009F9F70
    IDirect3DTexture9* PD3DTexture;                         // +0x00
    unsigned long      ByteLength : 28;                     // +0x04
    unsigned long      AllocationSource : 4;
};

class CEngineComponent {                                    // engine/CEngineComponent.h offsets, 0x0C bytes
public:
    virtual ~CEngineComponent();                            // 0x00B59730
    void**        GlobalPointerAddress;                     // +0x04
    bool          RenderEnabled;                            // +0x08
    unsigned char KeepAlive;                                // +0x09
};

class C3DVector { public: float X, Y, Z; };
class CMatrix3x4 { public: float M[12]; };
class CShadowRendererSceneData { public: unsigned char _body[0x240]; };
enum EShadowRenderingTechnique { };
class CEngineShadowRenderer : public CEngineComponent {     // vtable 0x012A2374
public:
    virtual ~CEngineShadowRenderer();
    virtual void OnReleaseDefaultPoolResources();           // 0x00B4B830
    CShadowRendererSceneData  SceneData;                    // +0x00C
    CRenderStateCaptureBlock  ShadowBufferRenderStateCaptureBlock;   // +0x24C
    float                     ShadowProjectionZOffset;      // +0x250
    float                     ShadowProjectionZSlopeOffset; // +0x254
    float                     ColourBufferShadowTextureZBias;   // +0x258
    float                     ColourBufferShadowMinWClamp;  // +0x25C
    bool                      EnableShadowBufferDraw;       // +0x260
    unsigned char             StaticDataOrigin[12];         // +0x261 C3DVector (packed in Ego_r)
    unsigned char             CameraLookVector[12];         // +0x26D C3DVector (packed in Ego_r)
    CMatrix3x4                CameraViewMatrix;             // +0x27C
    bool                      PerspectiveShadowsEnabled;    // +0x2AC
    bool                      SceneOriginLocked;            // +0x2AD
    bool                      EnableFrustrumDebugDisplay;   // +0x2AE
    bool                      TexturesAllocated;            // +0x2AF
    float                     PerspectiveInterpStart;       // +0x2B0
    float                     PerspectiveInterpEnd;         // +0x2B4
    float                     ForegroundBufferSpan;         // +0x2B8
    float                     MaxShadowDistance;            // +0x2BC
    float                     ShadowFadeRatio;              // +0x2C0
    CRenderTarget             RenderTarget;                 // +0x2C4
    CTexture                  ShadowBufferTexture;          // +0x2E8
    CTexture                  RenderSurface;                // +0x2F0
    CSurface                  ShadowBufferDepthSurface;     // +0x2F8
    CSurface                  ShadowBufferColourSurface;    // +0x308
    CTexture                  ShadowBufferColourTexture;    // +0x318
    C3DVector                 ShadowBufferTextureUVScaler;  // +0x320
    CTexture                  DepthPackingLookupTexture;    // +0x32C
    EShadowRenderingTechnique RenderingTechnique;           // +0x334
    long                      UserVideoOptionShadowBufferSize;      // +0x338
    float                     UserVideoOptionShadowDistanceScale;   // +0x33C
};

CEngineShadowRenderer::~CEngineShadowRenderer()
{
    OnReleaseDefaultPoolResources();
}
