// CEngineWaterRenderer::Render -- per-frame water pass: works out which render lists hold patches, updates the settings
// Grade: DIFFER(9343v9343) -- size/layout/frame/calls equal; 13 real hunks: bookmark-test load-order ties in exp2 SetTextureBumpEnvMatrix(1,f,0,0,f) enabled arm x4 blocks 0x00B7945D/0x00B794C1/0x00B79525/0x00B79589 (retail CurrentBookmarkMask load first), exp3 SetDestAlphaBlendMode 0x00B7A5D0 / SetAlphaBlendEnable 0x00B7A635 (retail state.BookmarkMask first), and the esi/edx swap of the bookmark pair at 0x00B79938 (exp2 disabled arm, stage 2)
// Overlay: $W/hdr (export FABLE_HDR_OVERLAY=$W/hdr), opt-ins listed in hdr/README.md.
// constants, creates the dynamic textures, then (inside a scoped capture block and with shaders enabled) sets the common water
// render states and runs the foreground / background / ice / sea passes, restoring all state at the end.
// Retail 0x00B783F0, 9343 bytes. Callees: CEngineWaterSettings::UpdateConstants 0x00BEA110, CreateDynamicTextures 0x00B72180,
// Enable/DisableVertexShaders 0x00987FE0/0x009880E0, Enable/DisablePixelShaders 0x00988110/0x00988190, ResetWorldTransform
// 0x00988290, RenderForeground 0x00B761D0, RenderBackground 0x00B76BC0, RenderIceForeground 0x00B751F0, RenderIceBackground
// 0x00B75CA0, RenderSeaReflectiveForeground 0x00B77370, RestoreCaptureBlock 0x00A05840, CEngineSkyRenderer::GetEnvironmentMap
// 0x00B665E0, CTexture::GenerateMipmapsWithGPU 0x009FAF70, RenderSeaNonReflectiveForeground 0x00B77E20, RenderSeaMidground
// 0x00B6FA40, RenderSeaBackground 0x00B70230, FinishUsingDynamicTextures 0x00B6D630.
// Leaf: ?Render@CEngineWaterRenderer@@UAEXABVCEngineComponentRenderParams@@@Z
#define FABLE_RETAIL_CRENDERMANAGERCORE
#define FABLE_RETAIL_CSCOPEDSHADERSENABLE_FORCEINLINE
#define FABLE_RETAIL_CRENDERMANAGERCORE_FORCEINLINE_HELPERS
#define FABLE_RETAIL_CRENDERSTATEMANAGER_BUMPENVMATRIX_INLINE
#define FABLE_RETAIL_CRENDERSTATECAPTUREBLOCK_INLINE
#define FABLE_RETAIL_CTEXTURE
#define FABLE_RETAIL_CTEXTURE_GENERATEMIPMAPSWITHGPU
#define FABLE_RETAIL_CENGINESKYRENDERER
#define FABLE_RETAIL_CENGINESKYRENDERER_GETENVIRONMENTMAP
#define FABLE_RETAIL_CENGINEWATERRENDERER
#define FABLE_RETAIL_CENGINEWATERRENDERER_SEAMIDGROUND
#define FABLE_RETAIL_CENGINEWATERRENDERER_RENDER
#include "fable_compiler.h"
#include "engine/CRenderManager.h"
#include "engine/CShaderRenderManager.h"
#include "engine/CRenderStateCaptureBlock.h"
#include "engine/CScopedShadersEnable.h"
#include "engine/CTexture.h"
#include "engine/CEngineSkyRenderer.h"
#include "engine/CEngineWaterRenderer.h"

struct CEngineInterpolationInfoSet { float GTPredictedRenderAt, GTPredictedTimeSinceLastRenderFrame, WFInterpolate; };
class CEngineComponentParams { public:
    float                       FrameInterpolation;      // +0x00
    CEngineInterpolationInfoSet Current, Last, PausedCurrent, PausedLast, BulletTimeCurrent;   // +0x04..
};
class CEngineComponentRenderParams { public:
    CEngineComponentParams* Params;                      // +0x00
    long                    RenderLayer;                 // +0x04
    unsigned long           RenderLayerFlags;            // +0x08
};

namespace NEngineInternal { extern CRenderManager* GPRenderManager; }              // 0x01436E18
namespace NEngineInternal { extern CShaderRenderManager* GPShaderRenderManager; }  // 0x01436E14
extern CEngineSkyRenderer* g_pEngineSkyRenderer;                                   // 0x01436E50

void CEngineWaterRenderer::Render(const CEngineComponentRenderParams& params)
{
    const CEngineComponentParams& component_params = *params.Params;
    float time = component_params.BulletTimeCurrent.WFInterpolate;
    bool fg = !ForegroundRenderList.empty();
    bool bg = !BackgroundRenderList.empty() || !AlphaedBackgroundRenderList.empty();
    bool iceFg = !IceForegroundRenderList.empty();
    bool iceBg = !IceBackgroundRenderList.empty() || !IceAlphaedBackgroundRenderList.empty();
    bool seaRefl = !SeaReflectiveForegroundRenderList.empty();
    bool seaNonRefl = !SeaNonReflectiveForegroundRenderList.empty();
    bool seaMid = !SeaMidgroundRenderList.empty() || !SeaAlphaedMidgroundRenderList.empty();
    bool seaBg = HaveBackgroundSea && Settings.EnableSea;
    bool ice = iceFg || iceBg;
    if (!ice && !fg && !bg && !seaRefl && !seaNonRefl && !seaMid && !seaBg)
        return;
    Settings.UpdateConstants(params);
    if (!CreateDynamicTextures(fg || bg, seaRefl || seaNonRefl || seaMid || seaBg, iceFg || iceBg))
        return;

    CRenderStateScopedCaptureBlock capture;
    CScopedShadersEnable shaders(true, true);
    CRenderStateCaptureBlock block;
    block.Capture();

    NEngineInternal::GPRenderManager->SetFogEnable(false);
    SetCommonRenderStates();
    NEngineInternal::GPRenderManager->SetFogEnable(true);
    NEngineInternal::GPShaderRenderManager->ResetWorldTransform();

    if (Settings.EnableForeground && fg && HighendWaterEnabled)
        RenderForeground(time);
    if (Settings.EnableBackground && bg)
        RenderBackground(time);
    if (Settings.EnableIceForeground && iceFg && HighendWaterEnabled)
        RenderIceForeground(time);
    if (Settings.EnableIceBackground && iceBg && HighendWaterEnabled)
        RenderIceBackground(time);
    if (Settings.EnableSea) {
        if (Settings.EnableSeaForeground && seaRefl && HighendWaterEnabled)
            RenderSeaReflectiveForeground(time);
    }
    if (Settings.EnableSea && (seaNonRefl || seaMid || seaBg)) {
        block.Restore();
        NEngineInternal::GPRenderManager->AttachTextureToStage(0, 0);
        NEngineInternal::GPRenderManager->AttachTextureToStage(1, 0);
        NEngineInternal::GPRenderManager->AttachTextureToStage(2, 0);
        NEngineInternal::GPRenderManager->AttachTextureToStage(3, 0);
        if (HighendWaterEnabled) {
            g_pEngineSkyRenderer->GetEnvironmentMap(EnvironmentMap, params);
            EnvironmentMap->GenerateMipmapsWithGPU();
        }
        NEngineInternal::GPRenderManager->SetFogEnable(false);
        SetCommonRenderStates();
        NEngineInternal::GPRenderManager->SetFogEnable(true);
        SetCommonRenderStates();
        if (Settings.EnableSeaForeground && seaNonRefl && HighendWaterEnabled)
            RenderSeaNonReflectiveForeground(time);
        if (Settings.EnableSeaMidground && seaMid)
            RenderSeaMidground(time);
        if (Settings.EnableSeaBackground && seaBg) {
            NEngineInternal::GPRenderManager->SetAlphaTestEnable(false);
            NEngineInternal::GPRenderManager->SetAlphaBlendEnable(false);
            RenderSeaBackground(time);
        }
    }
    NEngineInternal::GPRenderManager->SelectVertexBuffer(0, 0);
    NEngineInternal::GPRenderManager->SelectIndexBuffer(0, 0);
    FinishUsingDynamicTextures();
}
