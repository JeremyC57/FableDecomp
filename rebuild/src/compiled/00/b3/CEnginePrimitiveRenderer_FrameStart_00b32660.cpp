// CEnginePrimitiveRenderer::FrameStart(const CEngineComponentParams& params) -- per-frame renderer setup: pick the colour- or depth-buffer shadow pixel shaders, publish the frame's interpolation info and bump the engine frame number, FrameStart every primitive manager and the cloth renderer, refresh the LOD tolerances from the texture pool, then derive the mipmap size multiplier and the FOV-adjusted LOD screen size
// Leaf: ?FrameStart@CEnginePrimitiveRenderer@@UAEXABVCEngineComponentParams@@@Z
// NF95 (part 2, q11381): 0x01436EB0 is NEngineInternal::GPVideoOptions (FableWin.pdb public ?GPVideoOptions@NEngineInternal@@3PBVCEngineVideoOptions@@B,
//   FableWin 0x04AC8094; Ego_r.pdb data order GPLightingManager / GPEngine / GPVideoOptions; CEngine::SetVideoOptions 0x00B24690 stores
//   &VideoOptions there), a pointer to const CEngineVideoOptions (Ego_r.pdb layout, 0x34 bytes).
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8). Also: the local CCountedPointer members PItem->Data, PRefCount->Info are the header CCountedPointer's (same offsets).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B32660, length 448 bytes (body 0x1b2 + int3 padding), `ret 4`.
// Callees: CTextureManager::GetPoolTextureSize @ 0x00B244F0 (on the global @ 0x01436EA4; its result is read as a record:
//   +0x40/+0x44 are the LOD tolerances), CDisplayManager::GetRenderTargetDimensions() @ 0x009BEDC0 (C2DExtentsI by value, on the
//   global @ 0x01436E1C), __ftol2 @ 0x00BFEA70; virtual slot 3 (+0xc) of each primitive manager, slot 1 (+4) of the cloth renderer.
// Globals: the shadow renderer @ 0x01436E60 (mode +0x334, 2 = colour-buffer shadows), the frame interpolation info @ 0x0143B8E0
//   (0x70 bytes, copied from params +4), the engine frame number @ 0x0143B8D8 (short), NEngineInternal::GPVideoOptions @ 0x01436EB0 (+0x20
//   MeshDetail), the camera @ 0x01436EA0 (zoom +0xd4).
// Layout: as the landed FrameEnd 0x00B32820: the renderer derives from the 0x268-byte CEnginePrimitiveRendererSharedData and a
//   secondary engine-component base at +0x268 (vtable 0x012A1348, FrameStart = slot 1), so `this` is the subobject and the
//   shared-data members are reached at negative offsets: the shadow shader pointers +0x21c..+0x238 (-0x4c..-0x30), the depth/colour
//   CPixelShader members +0x11c..+0x194, LODErrorTolerance +0x9c, CharacterLODErrorTolerance +0xa0, LODVirtualScreenSize +0xa4,
//   LODVirtualScreenSizeFOVAdjusted +0xa8, MipmapQuality +0xac, MipmapSizeMultiplier +0xb0; PrimitiveManagers +0x274 (12-byte
//   CArray of 8-byte counted pointers), ClothRenderer +0x2f0 (the PDB +0x2f4 less 4).
// Shape: the screen size is `min((float)width, LODVirtualScreenSize)` converted to unsigned (ftol + 2^32 fix-up); the by-value
//   C2DExtentsI result must land in a NAMED local (`fild [esp+4]`; a temporary member read gives `fild [eax]`).
class CEngineComponentParams;
class CPixelShader { public: void* _vptr; void* PShader; };

class CEngineInterpolationInfo { public: long _body[0x70 / 4]; };
extern CEngineInterpolationInfo g_EngineInterpolationInfo;   // 0x0143B8E0
extern unsigned short g_engineFrameNumber;                     // 0x0143B8D8

class CEngineComponentParams {
public:
    void*                    _vptr;                // +0x00
    CEngineInterpolationInfo InterpolationInfo;    // +0x04
};

// NF45 (round 7): 0x01436E60 is NEngineInternal::GPShadowRenderer (Ego_r.pdb / FableWin.pdb
//   ?GPShadowRenderer@NEngineInternal@@3PAVCEngineShadowRenderer@@A; engine/CEngineShadowRenderer.h, its RenderingTechnique +0x334 opaque in
//   the default layout) and 0x01436EA4 is NEngineInternal::GPEngine (?GPEngine@NEngineInternal@@3PAVCEngine@@A; engine/CEngine.h) whose
//   0x00B244F0 is ?PeekVideoOptionsDef@CEngine@@QAEABVCEngineVideoOptionsDef@@W4EEngineVideoOptionQuality@@@Z (repo opt-in
//   PEEKVIDEOOPTIONSDEF; the landed self row is misnamed CTextureManager::GetPoolTextureSize, NAME_FIXES) returning the def whose
//   LODErrorTolerance / CharacterLODErrorTolerance are +0x40 / +0x44 (engine/CEngineVideoOptionsDef.h PASS22_FIELDS). Were the
//   CEngineShadowRendererView / g_pShadowRenderer_01436e60 and CTextureManager / STexturePoolRecord / g_pTextureManager_01436ea4 stand-ins.
//   0x01436EA0 is NEngineInternal::GPCamera (?GPCamera@NEngineInternal@@3PAVCEngineCamera@@A, engine/CEngineCamera.h) and its +0xD4 is
//   HomogenousViewScaleX (was CCamera_01436ea0::Zoom / g_pCamera_01436ea0); the secondary base is the shared 0xC-byte CEngineComponent
//   (FABLE_RETAIL_CENGINECOMPONENT, FrameStart slot 1), which absorbs the old 8-byte pad before PrimitiveManagers +0x274.
#define FABLE_RETAIL_CENGINE_PEEKVIDEOOPTIONSDEF
#define FABLE_RETAIL_CENGINECOMPONENT
#include "engine/CEngineComponent.h"
#include "engine/CEngineCamera.h"
#define FABLE_RETAIL_CENGINEVIDEOOPTIONSDEF_PASS22_FIELDS
#include "engine/CEngineShadowRenderer.h"
#include "engine/CEngine.h"
#include "engine/CEngineVideoOptionsDef.h"
namespace NEngineInternal { extern CEngineShadowRenderer* GPShadowRenderer; }   // 0x01436E60
namespace NEngineInternal { extern CEngine* GPEngine; }                         // 0x01436EA4

class C2DExtentsI { public: long Width; long Height; };
#include "engine/C2DExtentsF.h"
#include "engine/EEngineVideoOptionQuality.h"
#include "engine/ESurfaceMultisampleType.h"
class CEngineVideoOptions { public:                  // Ego_r.pdb, 0x34 bytes
    C2DExtentsI               ScreenResolution;   // +0x00
    C2DExtentsF               ScreenAspectRatio;  // +0x08
    long                      BitsPerPixel;       // +0x10
    long                      RefreshRate;        // +0x14
    bool                      VerticalSync;       // +0x18
    EEngineVideoOptionQuality LandscapeDetail;    // +0x1c
    EEngineVideoOptionQuality MeshDetail;         // +0x20
    EEngineVideoOptionQuality TextureDetail;      // +0x24
    EEngineVideoOptionQuality ShadowQuality;      // +0x28
    EEngineVideoOptionQuality EffectsQuality;     // +0x2c
    ESurfaceMultisampleType   Antialiasing;       // +0x30
};
namespace NEngineInternal { extern const CEngineVideoOptions* GPVideoOptions; }   // 0x01436EB0 (NF95 q11381: FableWin.pdb ?GPVideoOptions@NEngineInternal@@3PBVCEngineVideoOptions@@B)
class CDisplayManager { public:
    const C2DExtentsI GetRenderTargetDimensions();   // 0x009BEDC0
};
namespace NEngineInternal { extern CDisplayManager* GPDisplayManager; }   // 0x01436E1C ?GPDisplayManager@NEngineInternal@@3PAVCDisplayManager@@A
namespace NEngineInternal { extern CEngineCamera* GPCamera; }   // 0x01436EA0

class CEnginePrimitiveManager { public:
    virtual void v00(); virtual void v04(); virtual void v08();
    virtual void FrameStart();                     // slot 3 (+0xc)
};
class CEngineClothRenderer { public:
    virtual void v00();
    virtual void FrameStart();                     // slot 1 (+4)
};
#include "engine/CCountedPointer.h"
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

class CEnginePrimitiveRenderer : public CEnginePrimitiveRendererSharedData, public CEngineComponent { public:
    CManagerArray         PrimitiveManagers;       // +0x274
    unsigned char         _pad280[0x2f0 - 0x280];
    CCountedPointer<CEngineClothRenderer> ClothRenderer;   // +0x2f0
    virtual void FrameStart(const CEngineComponentParams& params);
};

void CEnginePrimitiveRenderer::FrameStart(const CEngineComponentParams& params)
{
    if (*reinterpret_cast<const long*>(NEngineInternal::GPShadowRenderer->RenderingTechnique) == 2) {
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
        PrimitiveManagers.m_pFirst[i].Data->FrameStart();
    ClothRenderer.Data->FrameStart();
    if (NEngineInternal::GPVideoOptions) {
        LODErrorTolerance = NEngineInternal::GPEngine->PeekVideoOptionsDef(NEngineInternal::GPVideoOptions->MeshDetail).LODErrorTolerance;
        CharacterLODErrorTolerance = NEngineInternal::GPEngine->PeekVideoOptionsDef(NEngineInternal::GPVideoOptions->MeshDetail).CharacterLODErrorTolerance;
    }
    C2DExtentsI dims = NEngineInternal::GPDisplayManager->GetRenderTargetDimensions();
    float width = (float)dims.Width;
    float screen = width < LODVirtualScreenSize ? width : LODVirtualScreenSize;
    MipmapSizeMultiplier = NEngineInternal::GPCamera->HomogenousViewScaleX * MipmapQuality * (unsigned long)screen;
    LODVirtualScreenSizeFOVAdjusted = NEngineInternal::GPCamera->HomogenousViewScaleX * LODVirtualScreenSize;
}