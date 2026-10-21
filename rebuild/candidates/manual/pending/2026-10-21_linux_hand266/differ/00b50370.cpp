// Grade: DIFFER(ours v retail) -- 406v406, scheduling only: retail hoists the 8.0/0.1/1.0 constants into ecx/edx/eax and keeps the stores in source order, our build pairs each constant's two stores right after its load (and moves `mov ecx,esi` up)
// CEngineShadowRenderer::CEngineShadowRenderer() -- CEngineComponent base, clear the two shadow clip volumes, the capture block, the render target, textures and surfaces, set the default shadow distances / biases / perspective interpolation / video options, then InitialiseTextures()
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B50370, length 416 bytes (406 + int3 padding), plain `ret`, no EH frame.
// Label correction: the catalog's "CPSCOrbit::CPSCOrbit" is propagated; the body stores vtable 0x012A2374, which
//   rtti-vtables.tsv names CEngineShadowRenderer, and the member layout is Ego_r.pdb CEngineShadowRenderer exactly.
// Callees: CEngineComponent::CEngineComponent 0x00B59710, CBase::CBase 0x0099A2D0 (the CRenderTarget base),
//   CEngineShadowRenderer::InitialiseTextures 0x00B4F8E0. Vtables: CRenderTarget 0x0129C800, CSurface 0x0122F84C.
// Layout: PDB CEngineShadowRenderer (retail offsets equal the PDB): SceneData +0x0c (ShadowFadeStart/End +0xfc/+0x100,
//   CasterClipVolume +0x11c, RecieverClipVolume +0x1b4), capture block +0x24c, biases +0x250..+0x25c, flags +0x2ac..+0x2af,
//   interp/distances +0x2b0..+0x2c0, RenderTarget +0x2c4, textures/surfaces +0x2e8..+0x32c, technique +0x334, video options
//   +0x338/+0x33c. engine/CEngineShadowRenderer.h is a flat packed byte view (no member types), so the members are modelled
//   with their PDB types here; the CRenderTarget/CSurface/capture-block inline ctors are the landed CSurfaceCompositor
//   0x00F38990 models.
struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
};
struct CPlane { float A, B, C, D; };
struct CMatrix4x4 { float M[4][4]; };
struct CMatrix3x4 { float M[3][4]; };
struct C3DBoundingSphere {
    C3DVector Centre;
    float Radius;
    C3DBoundingSphere() : Centre(0.0f, 0.0f, 0.0f), Radius(0.0f) {}
};
struct CShadowClipVolume {
    CPlane            ClipPlanes[8];               // +0x00
    long              ClipPlaneCount;              // +0x80
    C3DBoundingSphere BoundingSphere;              // +0x84
    bool              ClipToSphere;                // +0x94
    CShadowClipVolume() : ClipPlaneCount(0), ClipToSphere(false) {}
};
struct CShadowRendererSceneData {
    CMatrix4x4        WorldToShadowBufferMatrix;                        // +0x000
    CMatrix4x4        WorldToShadowBufferMatrixHomogenousScreenSpace;   // +0x040
    CMatrix4x4        WorldToShadowBufferMatrixTextureSpace;            // +0x080
    CMatrix3x4        SpotLightFalloffMatrix;                           // +0x0c0
    float             ShadowFadeStart;                                  // +0x0f0
    float             ShadowFadeEnd;                                    // +0x0f4
    C3DVector         ShadowLightVector;                                // +0x0f8
    C3DVector         CameraPos;                                        // +0x104
    CShadowClipVolume CasterClipVolume;                                 // +0x110
    CShadowClipVolume RecieverClipVolume;                               // +0x1a8
};
struct CRenderStateCaptureBlock {
    bool Open;
    CRenderStateCaptureBlock() : Open(false) {}
};
struct CBase {
    CBase();                                    // 0x0099A2D0
    virtual ~CBase();
};
struct IDirect3DSurface9;
struct IDirect3DTexture9;
struct CSurface {                               // vtable 0x0122F84C, 0x10 bytes
    virtual ~CSurface();
    IDirect3DSurface9* PD3DSurface;             // +0x04
    unsigned long AllocationSource;             // +0x08
    void* PAllocatedMemory;                     // +0x0c
    CSurface() : PD3DSurface(0), AllocationSource(0) {}
};
struct CRenderTarget : CBase {                  // vtable 0x0129C800, 0x24 bytes
    CSurface RenderToSurface;                   // +0x04
    CSurface ZBufferSurface;                    // +0x14
    CRenderTarget() {}
    virtual ~CRenderTarget();
};
struct CTexture {
    IDirect3DTexture9* PD3DTexture;             // +0x00
    unsigned long      Bits;                    // +0x04 ByteLength:28, AllocationSource:4
    CTexture() : PD3DTexture(0), Bits(0) {}
};
struct CEngineComponent {                       // vtable 0x012A2544, 0x0c bytes
    CEngineComponent();                         // 0x00B59710
    virtual ~CEngineComponent();
    long m_value;                               // +0x04
    bool m_flag8;                               // +0x08
    bool m_flag9;                               // +0x09
};
enum EShadowRenderingTechnique { SHADOW_RENDERING_TECHNIQUE_0 = 0 };

struct CEngineShadowRenderer : CEngineComponent {   // vtable 0x012A2374
    CShadowRendererSceneData  SceneData;                           // +0x00c
    CRenderStateCaptureBlock  ShadowBufferRenderStateCaptureBlock; // +0x24c
    float                     ShadowProjectionZOffset;             // +0x250
    float                     ShadowProjectionZSlopeOffset;        // +0x254
    float                     ColourBufferShadowTextureZBias;      // +0x258
    float                     ColourBufferShadowMinWClamp;         // +0x25c
    bool                      EnableShadowBufferDraw;              // +0x260
    C3DVector                 StaticDataOrigin;                    // +0x264
    C3DVector                 CameraLookVector;                    // +0x270
    CMatrix3x4                CameraViewMatrix;                    // +0x27c
    bool                      PerspectiveShadowsEnabled;           // +0x2ac
    bool                      SceneOriginLocked;                   // +0x2ad
    bool                      EnableFrustrumDebugDisplay;          // +0x2ae
    bool                      TexturesAllocated;                   // +0x2af
    float                     PerspectiveInterpStart;              // +0x2b0
    float                     PerspectiveInterpEnd;                // +0x2b4
    float                     ForegroundBufferSpan;                // +0x2b8
    float                     MaxShadowDistance;                   // +0x2bc
    float                     ShadowFadeRatio;                     // +0x2c0
    CRenderTarget             RenderTarget;                        // +0x2c4
    CTexture                  ShadowBufferTexture;                 // +0x2e8
    CTexture                  RenderSurface;                       // +0x2f0
    CSurface                  ShadowBufferDepthSurface;            // +0x2f8
    CSurface                  ShadowBufferColourSurface;           // +0x308
    CTexture                  ShadowBufferColourTexture;           // +0x318
    C3DVector                 ShadowBufferTextureUVScaler;         // +0x320
    CTexture                  DepthPackingLookupTexture;           // +0x32c
    EShadowRenderingTechnique RenderingTechnique;                  // +0x334
    long                      UserVideoOptionShadowBufferSize;     // +0x338
    float                     UserVideoOptionShadowDistanceScale;  // +0x33c
    CEngineShadowRenderer();
    virtual ~CEngineShadowRenderer();
    void InitialiseTextures();                                     // 0x00B4F8E0
};

CEngineShadowRenderer::CEngineShadowRenderer()
{
    SceneData.ShadowFadeEnd = 32.0f;
    MaxShadowDistance = 32.0f;
    SceneData.ShadowFadeStart = 8.0f;
    ForegroundBufferSpan = 8.0f;
    ShadowProjectionZOffset = 0.0001f;
    ShadowProjectionZSlopeOffset = 2.0f;
    ColourBufferShadowTextureZBias = -0.001f;
    ColourBufferShadowMinWClamp = 0.1f;
    TexturesAllocated = false;
    SceneOriginLocked = false;
    ShadowFadeRatio = 0.5f;
    PerspectiveInterpStart = 0.1f;
    PerspectiveInterpEnd = 0.2f;
    UserVideoOptionShadowBufferSize = 0x400;
    UserVideoOptionShadowDistanceScale = 1.0f;
    RenderingTechnique = SHADOW_RENDERING_TECHNIQUE_0;
    PerspectiveShadowsEnabled = true;
    EnableFrustrumDebugDisplay = false;
    ShadowBufferTextureUVScaler.X = 1.0f;
    ShadowBufferTextureUVScaler.Y = 1.0f;
    ShadowBufferTextureUVScaler.Z = 1.0f;
    InitialiseTextures();
}
