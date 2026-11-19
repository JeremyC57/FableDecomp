// CEnginePrimitiveRenderer::LoadNewScene(const CEngineComponentLoadNewSceneParams& params) -- per-step scene preload: the preload radius interpolated from the maximum to the minimum by the load progress around the camera, the preload mipmap tolerance floored from the progress, the mipmap size multiplier and FOV-adjusted LOD screen size refreshed, the LOD error tolerances re-read from the mesh-detail video options def, then every scene preloads by camera and by sphere, the lighting manager updates the shadow scene, and every scene preloads by the shadow caster clip volume; true while anything is still loading
// Leaf: ?LoadNewScene@CEnginePrimitiveRenderer@@UAE_NABVCEngineComponentLoadNewSceneParams@@@Z
// Flags: none
// Retail-asm: 0x00B32927 `fsub [ebp-10h]` + 0x00B3292A `fistp dword ptr [ebp+8]` -- the engine's inline-assembly floor helper
//   GFFloatToLongFloor (engine/GFFloatToLongFloor.h; FableWin calls ?GFFloatToLongFloor@@YIJM@Z at 0x02DA9018). This row carries no asm
//   of its own.
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B328C0, 517 bytes, `ret 4`, ebp frame; `this` is the CEngineComponent subobject (+0x268).
// Shape: FableWin.pdb ?LoadNewScene@CEnginePrimitiveRenderer@@UAE_NABVCEngineComponentLoadNewSceneParams@@@Z (FableWin 0x02DA8F90),
//   statement for statement, with the Ego_d.pdb locals (radius, sphere, flags, loading, c_scene per loop; c_scene is `long`, flags
//   `unsigned long`). FableWin's `xor reg,reg ; je` before each loop is the debug build's for-scope guard and emits nothing here.
// Callees: CEngine::PeekVideoOptionsDef 0x00B244F0 (x2, through NEngineInternal::GPEngine 0x01436EA4 with
//   NEngineInternal::GPVideoOptions 0x01436EB0 ->MeshDetail), CEngineScene::PreloadPrimitives 0x00B92F80 (camera) / 0x00B92E50
//   (sphere) / 0x00B930A0 (clip volume), CEngineLightingManager::UpdateShadowScene 0x00B444F0 (NEngineInternal::GPLightingManager
//   0x01436E9C), CShadowRendererSceneData copy 0x00B38600 (CEngineShadowRenderer::PeekSceneData inline, NEngineInternal::GPShadowRenderer
//   0x01436E60). Inline: GFGetInterpolatedValue, C3DBoundingSphere(centre, radius), CEngineCamera::PeekCameraPos, CArray size / [].
// TU opt-in: FABLE_RETAIL_CARRAY_INDEX_ASSERT -- FableWin's ??A?$CArray@PAVCEngineScene@@@@QAEAAPAVCEngineScene@@I@Z (0x02CF4750, called
//   at 0x02DA912E) opens with the `index < size()` ASSERT; the dead release arm puts the element load after the pushed arguments
//   (0x00B329EF / 0x00B32A0D / 0x00B32A93), where the plain `_M_start[i]` loads it first.
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CENGINECOMPONENT
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERERSHAREDDATA_RENDERSETTINGS
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERER
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERER_MEMBERS
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERER_LOADNEWSCENE
#define FABLE_RETAIL_CENGINESCENE_PRELOADPRIMITIVES
#define FABLE_RETAIL_CENGINE_PEEKVIDEOOPTIONSDEF
#define FABLE_RETAIL_CENGINEVIDEOOPTIONSDEF_PASS22_FIELDS
#define FABLE_RETAIL_CENGINESHADOWRENDERER
#define FABLE_RETAIL_CENGINESHADOWRENDERER_PEEKSCENEDATA
#define FABLE_RETAIL_CSHADOWRENDERERSCENEDATA_COPY
#define FABLE_RETAIL_C3DBOUNDINGSPHERE
#define FABLE_RETAIL_CARRAY_INDEX_ASSERT
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C4DVECTOR
#define FABLE_RETAIL_CMATRIX3X4
#define FABLE_RETAIL_CENGINECAMERA_POSITION
#define FABLE_RETAIL_CENGINECAMERADESC_STRUCT
#define FABLE_RETAIL_CENGINESHADOWRENDERER_MEMBERS
#define FABLE_RETAIL_CENGINELIGHTINGMANAGER
#define FABLE_RETAIL_CENGINELIGHTINGMANAGER_FRAMESTART
#include "fable_compiler.h"
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/C3DVector.h"
#include "engine/C2DExtentsI.h"
#include "engine/C2DExtentsF.h"
#include "engine/EEngineVideoOptionQuality.h"
#include "engine/ESurfaceMultisampleType.h"
#include "engine/CEngineInterpolationInfo.h"
#include "engine/C3DBoundingSphere.h"
#include "engine/CShadowClipVolume.h"
#include "engine/CShadowRendererSceneData.h"
#include "engine/CEngineComponent.h"
#include "engine/CEngineCamera.h"
#include "engine/CEngineScene.h"
#include "engine/CEngineLightingManager.h"
#include "engine/CEngineShadowRenderer.h"
#include "engine/CEngineVideoOptionsDef.h"
#include "engine/CEngine.h"
#include "engine/GFGetInterpolatedValue.h"
#include "engine/GFFloatToLongFloor.h"
#include "engine/CEnginePrimitiveRenderer.h"

// Stand-ins (no shared header yet): the real classes with their Ego_r.pdb member names / types / offsets (struct_layouts_egor.tsv), up to
// the members the row reads.
class CEngineComponentParams {
public:
    float                    FrameInterpolation;                          // +0x00
    CEngineInterpolationInfo InterpolationInfo;                           // +0x04
    unsigned long            FrameID;                                     // +0x74
    unsigned long            SceneFilterFlags;                            // +0x78
    bool                     OffWorldTickProcessFlag;                     // +0x7c
};
class CEngineComponentLoadNewSceneParams {
public:
    CEngineComponentParams Params;                                        // +0x00
    CEngineCamera*         Camera;                                        // +0x80
    float                  Progress;                                      // +0x84
    unsigned long          Flags;                                         // +0x88
};
class CEngineVideoOptions {
public:
    C2DExtentsI               ScreenResolution;                           // +0x00
    C2DExtentsF               ScreenAspectRatio;                          // +0x08
    long                      BitsPerPixel;                               // +0x10
    long                      RefreshRate;                                // +0x14
    bool                      VerticalSync;                               // +0x18
    EEngineVideoOptionQuality LandscapeDetail;                            // +0x1c
    EEngineVideoOptionQuality MeshDetail;                                 // +0x20
    EEngineVideoOptionQuality TextureDetail;                              // +0x24
    EEngineVideoOptionQuality ShadowQuality;                              // +0x28
    EEngineVideoOptionQuality EffectsQuality;                             // +0x2c
    ESurfaceMultisampleType   Antialiasing;                               // +0x30
};
FABLE_STATIC_ASSERT(sizeof(CEngineComponentLoadNewSceneParams) == 0x8c);
FABLE_STATIC_ASSERT(sizeof(CEngineVideoOptions) == 0x34);
namespace NEngineInternal {
    extern CEngine* GPEngine;                                             // 0x01436EA4 ?GPEngine@NEngineInternal@@3PAVCEngine@@A
    extern const CEngineVideoOptions* const GPVideoOptions;               // 0x01436EB0 ?GPVideoOptions@NEngineInternal@@3PBVCEngineVideoOptions@@B
    extern CEngineLightingManager* GPLightingManager;                     // 0x01436E9C
    extern CEngineShadowRenderer* GPShadowRenderer;                       // 0x01436E60
}

bool CEnginePrimitiveRenderer::LoadNewScene(const CEngineComponentLoadNewSceneParams& params)
{
    float radius = GFGetInterpolatedValue(MaxPreloadRadius, MinPreloadRadius, params.Progress);
    C3DBoundingSphere sphere(params.Camera->PeekCameraPos(), radius);
    PreloadMipmapTolerance = GFFloatToLongFloor(params.Progress * MaxPreloadMipmapTolerance);
    MipmapSizeMultiplier = MipmapQuality * LODVirtualScreenSize;
    LODVirtualScreenSizeFOVAdjusted = LODVirtualScreenSize;
    if (NEngineInternal::GPVideoOptions) {
        LODErrorTolerance = NEngineInternal::GPEngine->PeekVideoOptionsDef(NEngineInternal::GPVideoOptions->MeshDetail).LODErrorTolerance;
        CharacterLODErrorTolerance = NEngineInternal::GPEngine->PeekVideoOptionsDef(NEngineInternal::GPVideoOptions->MeshDetail).CharacterLODErrorTolerance;
    }
    unsigned long flags = 0;
    if (!(params.Flags & 2))
        flags |= 1;
    bool loading = false;
    for (long c_scene = 0; c_scene < SceneList.size(); ++c_scene) {
        loading |= SceneList[c_scene]->PreloadPrimitives(*params.Camera, flags | 2);
        loading |= SceneList[c_scene]->PreloadPrimitives(sphere, flags);
    }
    flags |= 4;
    NEngineInternal::GPLightingManager->UpdateShadowScene(*params.Camera);
    for (long c_scene = 0; c_scene < SceneList.size(); ++c_scene)
        loading |= SceneList[c_scene]->PreloadPrimitives(NEngineInternal::GPShadowRenderer->PeekSceneData().CasterClipVolume, params.Camera->PeekCameraPos(), flags);
    return loading;
}