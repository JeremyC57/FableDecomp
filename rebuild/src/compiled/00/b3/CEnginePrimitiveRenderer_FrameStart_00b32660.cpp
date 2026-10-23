// CEnginePrimitiveRenderer::FrameStart(const CEngineComponentParams& params) -- per-frame renderer setup: pick the colour- or depth-buffer shadow pixel shaders, publish the frame's interpolation info and bump the engine frame number, FrameStart every primitive manager and the cloth renderer, refresh the LOD tolerances from the texture pool, then derive the mipmap size multiplier and the FOV-adjusted LOD screen size
// Leaf: ?FrameStart@CEnginePrimitiveRenderer@@UAEXABVCEngineComponentParams@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B32660, length 448 bytes (body 0x1b2 + int3 padding), `ret 4`.
// Callees: CTextureManager::GetPoolTextureSize @ 0x00B244F0 (on the global @ 0x01436EA4; its result is read as a record:
//   +0x40/+0x44 are the LOD tolerances), CDisplayManager::GetRenderTargetDimensions() @ 0x009BEDC0 (C2DExtentsI by value, on the
//   global @ 0x01436E1C), __ftol2 @ 0x00BFEA70; virtual slot 3 (+0xc) of each primitive manager, slot 1 (+4) of the cloth renderer.
// Globals: the shadow renderer @ 0x01436E60 (mode +0x334, 2 = colour-buffer shadows), the frame interpolation info @ 0x0143B8E0
//   (0x70 bytes, copied from params +4), the engine frame number @ 0x0143B8D8 (short), the texture-pool owner @ 0x01436EB0 (+0x20
//   pool id), the camera @ 0x01436EA0 (zoom +0xd4).
// Layout: as the landed FrameEnd 0x00B32820: the renderer derives from the 0x268-byte CEnginePrimitiveRendererSharedData and a
//   secondary engine-component base at +0x268 (vtable 0x012A1348, FrameStart = slot 1), so `this` is the subobject and the
//   shared-data members are reached at negative offsets: the shadow shader pointers +0x21c..+0x238 (-0x4c..-0x30), the depth/colour
//   CPixelShader members +0x11c..+0x194, LODErrorTolerance +0x9c, CharacterLODErrorTolerance +0xa0, LODVirtualScreenSize +0xa4,
//   LODVirtualScreenSizeFOVAdjusted +0xa8, MipmapQuality +0xac, MipmapSizeMultiplier +0xb0; PrimitiveManagers +0x274 (12-byte
//   CArray of 8-byte counted pointers), ClothRenderer +0x2f0 (the PDB +0x2f4 less 4).
// Shape: the screen size is `min((float)width, LODVirtualScreenSize)` converted to unsigned (ftol + 2^32 fix-up); the by-value
//   C2DExtentsI result must land in a NAMED local (`fild [esp+4]`; a temporary member read gives `fild [eax]`).
class CEngineComponentParams;
struct CPixelShader { void* _vptr; void* PShader; };

struct CEngineInterpolationInfo { long _body[0x70 / 4]; };
extern CEngineInterpolationInfo g_EngineInterpolationInfo;   // 0x0143B8E0
extern unsigned short g_engineFrameNumber;                     // 0x0143B8D8

class CEngineComponentParams {
public:
    void*                    _vptr;                // +0x00
    CEngineInterpolationInfo InterpolationInfo;    // +0x04
};

struct CEngineShadowRendererView { unsigned char _pad[0x334]; long ShadowMode; };
extern CEngineShadowRendererView* g_pShadowRenderer_01436e60;  // 0x01436E60

struct STexturePoolRecord { unsigned char _pad[0x40]; float LODErrorTolerance; float CharacterLODErrorTolerance; };
struct CTextureManager {
    const STexturePoolRecord* GetPoolTextureSize(unsigned long pool);   // 0x00B244F0
};
extern CTextureManager* g_pTextureManager_01436ea4;            // 0x01436EA4
struct CTexturePoolOwner { unsigned char _pad[0x20]; unsigned long PoolID; };
extern CTexturePoolOwner* g_pTexturePoolOwner_01436eb0;        // 0x01436EB0

struct C2DExtentsI { long Width; long Height; };
struct CDisplayManager {
    C2DExtentsI GetRenderTargetDimensions() const;   // 0x009BEDC0
};
extern CDisplayManager* g_pDisplayManager_01436e1c;         // 0x01436E1C
struct CCamera_01436ea0 { unsigned char _pad[0xd4]; float Zoom; };
extern CCamera_01436ea0* g_pCamera_01436ea0;       // 0x01436EA0

struct CEnginePrimitiveManager {
    virtual void v00(); virtual void v04(); virtual void v08();
    virtual void FrameStart();                     // slot 3 (+0xc)
};
struct CEngineClothRenderer {
    virtual void v00();
    virtual void FrameStart();                     // slot 1 (+4)
};
template <class T> struct CCountedPointer { T* PItem; long* PRefCount; };
struct CManagerArray {                             // retail CArray<CCountedPointer<CEnginePrimitiveManager> >
    CCountedPointer<CEnginePrimitiveManager>* m_pFirst;
    CCountedPointer<CEnginePrimitiveManager>* m_pLast;
    CCountedPointer<CEnginePrimitiveManager>* m_pEnd;
    unsigned long GetCount() const { return m_pLast - m_pFirst; }
};

struct CEnginePrimitiveRendererSharedData {        // primary base, 0x268 bytes
    virtual void v00();
    unsigned char _pad004[0x9c - 0x4];
    float         LODErrorTolerance;               // +0x9c
    float         CharacterLODErrorTolerance;      // +0xa0
    float         LODVirtualScreenSize;            // +0xa4
    float         LODVirtualScreenSizeFOVAdjusted; // +0xa8
    float         MipmapQuality;                   // +0xac
    float         MipmapSizeMultiplier;            // +0xb0
    unsigned char _pad0b4[0x11c - 0xb4];
    CPixelShader  PixelShaderDiffuseFogDepthBufferShadow;                    // +0x11c
    CPixelShader  PixelShaderDiffuseFogDepthBufferShadowOutputFactor;        // +0x124
    CPixelShader  PixelShaderDiffuseEnvFogDepthBufferShadow;                 // +0x12c
    CPixelShader  PixelShaderDiffuseEnvBumpFogDepthBufferShadow;             // +0x134
    CPixelShader  PixelShaderDiffuseDP3BumpFogDepthBufferShadow;             // +0x13c
    CPixelShader  PixelShaderDiffuseDP3BumpFogDepthBufferShadowOutputFactor; // +0x144
    CPixelShader  PixelShaderDepthShadowBufferRender;                        // +0x14c
    CPixelShader  PixelShaderDepthShadowBufferRenderAlpha;                   // +0x154
    CPixelShader  PixelShaderColourShadowBufferRender;                       // +0x15c
    CPixelShader  PixelShaderColourShadowBufferRenderAlpha;                  // +0x164
    CPixelShader  PixelShaderDiffuseFogColourBufferShadow;                   // +0x16c
    CPixelShader  PixelShaderDiffuseFogColourBufferShadowOutputFactor;       // +0x174
    CPixelShader  PixelShaderDiffuseEnvFogColourBufferShadow;                // +0x17c
    CPixelShader  PixelShaderDiffuseEnvBumpFogColourBufferShadow;            // +0x184
    CPixelShader  PixelShaderDiffuseDP3BumpFogColourBufferShadow;            // +0x18c
    CPixelShader  PixelShaderDiffuseDP3BumpFogColourBufferShadowOutputFactor; // +0x194
    unsigned char _pad19c[0x21c - 0x19c];
    CPixelShader* PixelShaderShadowBufferRender;                    // +0x21c
    CPixelShader* PixelShaderShadowBufferRenderAlpha;               // +0x220
    CPixelShader* PixelShaderDiffuseFogShadow;                      // +0x224
    CPixelShader* PixelShaderDiffuseFogShadowOutputFactor;          // +0x228
    CPixelShader* PixelShaderDiffuseEnvFogShadow;                   // +0x22c
    CPixelShader* PixelShaderDiffuseEnvBumpFogShadow;               // +0x230
    CPixelShader* PixelShaderDiffuseDP3BumpFogShadow;               // +0x234
    CPixelShader* PixelShaderDiffuseDP3BumpFogShadowOutputFactor;   // +0x238
    unsigned char _pad23c[0x268 - 0x23c];
};
struct CEngineComponent {                          // secondary base at +0x268, vtable 0x012A1348
    virtual void v00();
    virtual void FrameStart(const CEngineComponentParams& params);   // slot 1
};

struct CEnginePrimitiveRenderer : CEnginePrimitiveRendererSharedData, CEngineComponent {
    unsigned char         _pad26c[0x8];
    CManagerArray         PrimitiveManagers;       // +0x274
    unsigned char         _pad280[0x2f0 - 0x280];
    CCountedPointer<CEngineClothRenderer> ClothRenderer;   // +0x2f0
    virtual void FrameStart(const CEngineComponentParams& params);
};

void CEnginePrimitiveRenderer::FrameStart(const CEngineComponentParams& params)
{
    if (g_pShadowRenderer_01436e60->ShadowMode == 2) {
        PixelShaderShadowBufferRender = &PixelShaderColourShadowBufferRender;
        PixelShaderShadowBufferRenderAlpha = &PixelShaderColourShadowBufferRenderAlpha;
        PixelShaderDiffuseFogShadow = &PixelShaderDiffuseFogColourBufferShadow;
        PixelShaderDiffuseFogShadowOutputFactor = &PixelShaderDiffuseFogColourBufferShadowOutputFactor;
        PixelShaderDiffuseEnvFogShadow = &PixelShaderDiffuseEnvFogColourBufferShadow;
        PixelShaderDiffuseEnvBumpFogShadow = &PixelShaderDiffuseEnvBumpFogColourBufferShadow;
        PixelShaderDiffuseDP3BumpFogShadow = &PixelShaderDiffuseDP3BumpFogColourBufferShadow;
        PixelShaderDiffuseDP3BumpFogShadowOutputFactor = &PixelShaderDiffuseDP3BumpFogColourBufferShadowOutputFactor;
    } else {
        PixelShaderShadowBufferRender = &PixelShaderDepthShadowBufferRender;
        PixelShaderShadowBufferRenderAlpha = &PixelShaderDepthShadowBufferRenderAlpha;
        PixelShaderDiffuseFogShadow = &PixelShaderDiffuseFogDepthBufferShadow;
        PixelShaderDiffuseFogShadowOutputFactor = &PixelShaderDiffuseFogDepthBufferShadowOutputFactor;
        PixelShaderDiffuseEnvFogShadow = &PixelShaderDiffuseEnvFogDepthBufferShadow;
        PixelShaderDiffuseEnvBumpFogShadow = &PixelShaderDiffuseEnvBumpFogDepthBufferShadow;
        PixelShaderDiffuseDP3BumpFogShadow = &PixelShaderDiffuseDP3BumpFogDepthBufferShadow;
        PixelShaderDiffuseDP3BumpFogShadowOutputFactor = &PixelShaderDiffuseDP3BumpFogDepthBufferShadowOutputFactor;
    }
    g_EngineInterpolationInfo = params.InterpolationInfo;
    ++g_engineFrameNumber;
    for (unsigned long i = 0; i < PrimitiveManagers.GetCount(); ++i)
        PrimitiveManagers.m_pFirst[i].PItem->FrameStart();
    ClothRenderer.PItem->FrameStart();
    if (g_pTexturePoolOwner_01436eb0) {
        LODErrorTolerance = g_pTextureManager_01436ea4->GetPoolTextureSize(g_pTexturePoolOwner_01436eb0->PoolID)->LODErrorTolerance;
        CharacterLODErrorTolerance = g_pTextureManager_01436ea4->GetPoolTextureSize(g_pTexturePoolOwner_01436eb0->PoolID)->CharacterLODErrorTolerance;
    }
    C2DExtentsI dims = g_pDisplayManager_01436e1c->GetRenderTargetDimensions();
    float width = (float)dims.Width;
    float screen = width < LODVirtualScreenSize ? width : LODVirtualScreenSize;
    MipmapSizeMultiplier = g_pCamera_01436ea0->Zoom * MipmapQuality * (unsigned long)screen;
    LODVirtualScreenSizeFOVAdjusted = g_pCamera_01436ea0->Zoom * LODVirtualScreenSize;
}