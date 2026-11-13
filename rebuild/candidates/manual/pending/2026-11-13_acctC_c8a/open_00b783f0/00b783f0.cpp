// CEngineWaterRenderer::Render -- per-frame water pass: works out which render lists hold patches, updates the settings
// Grade: DIFFER(9343v9343) -- agentC8 port of the acctB best draft onto today's headers (shared engine/CEngineComponentRenderParams.h, repo opt-ins _CREATEDYNAMICTEXTURES / DISPLACEMENT_INLINE (SetTextureBumpEnvMatrix) / SKYRENDERER_ENVMAP / CTEXTURE_WATERDYNAMIC, new FABLE_RETAIL_CENGINEWATERRENDERER_RENDER decls; SetCommonRenderStates body in the row); same residue as acctB: L quartet 0x00B7945D/0x00B794C1/0x00B79525/0x00B79589 (retail CurrentBookmarkMask load first), exp3 pair 0x00B7A5D0/0x00B7A635 (retail state mask first), O pair 0x00B79938 (the Open-store scheduling at 0x00B79125 is FIXED by the new opt-in FABLE_RETAIL_CRENDERSTATECAPTUREBLOCK_RESTORE_OPEN_FIRST)
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
#define FABLE_RETAIL_CRENDERSTATEMANAGER_DISPLACEMENT_INLINE
#define FABLE_RETAIL_CRENDERSTATECAPTUREBLOCK_INLINE
#define FABLE_RETAIL_CRENDERSTATECAPTUREBLOCK_RESTORE_OPEN_FIRST
#define FABLE_RETAIL_CTEXTURE
#define FABLE_RETAIL_CTEXTURE_WATERDYNAMIC
#define FABLE_RETAIL_CENGINESKYRENDERER
#define FABLE_RETAIL_CENGINESKYRENDERER_ENVMAP
#define FABLE_RETAIL_CENGINEWATERRENDERER
#define FABLE_RETAIL_CENGINEWATERRENDERER_SEAMIDGROUND
#define FABLE_RETAIL_CENGINEWATERRENDERER_CREATEDYNAMICTEXTURES
#define FABLE_RETAIL_CENGINEWATERRENDERER_RENDER
#define FABLE_RETAIL_CENGINECOMPONENTPARAMS_INTERPOLATIONINFO
#include "fable_compiler.h"
#include "engine/CRenderManager.h"
#include "engine/CShaderRenderManager.h"
#include "engine/CRenderStateCaptureBlock.h"
#include "engine/CScopedShadersEnable.h"
#include "engine/CTexture.h"
#include "engine/CEngineComponentRenderParams.h"
#include "engine/CEngineSkyRenderer.h"
#include "engine/CEngineWaterRenderer.h"

namespace NEngineInternal { extern CRenderManager* GPRenderManager; }              // 0x01436E18
namespace NEngineInternal { extern CShaderRenderManager* GPShaderRenderManager; }  // 0x01436E14
extern CEngineSkyRenderer* g_pEngineSkyRenderer;                                   // 0x01436E50

FABLE_FORCEINLINE void CEngineWaterRenderer::SetCommonRenderStates()
{
    NEngineInternal::GPRenderManager->SetSpecularEnable(false);
    NEngineInternal::GPRenderManager->SetZBufferEnable(true);
    NEngineInternal::GPRenderManager->SetZBufferWriteEnable(true);
    if (Settings.EnableBumpMap) {
        NEngineInternal::GPRenderManager->SetTextureBumpEnvMatrix(1, Settings.BumpMatFactor, 0.0f, 0.0f, Settings.BumpMatFactor);
        NEngineInternal::GPRenderManager->SetTextureBumpEnvMatrix(2, Settings.BumpMatFactor, 0.0f, 0.0f, Settings.BumpMatFactor);
    } else {
        NEngineInternal::GPRenderManager->SetTextureBumpEnvMatrix(1, 0.0f, 0.0f, 0.0f, 0.0f);
        NEngineInternal::GPRenderManager->SetTextureBumpEnvMatrix(2, 0.0f, 0.0f, 0.0f, 0.0f);
    }
    NEngineInternal::GPRenderManager->SetAlphaTestEnable(true);
    NEngineInternal::GPRenderManager->SetAlphaTestFunc(ALPHA_TEST_GREATER);
    NEngineInternal::GPRenderManager->SetAlphaRef(1);
    NEngineInternal::GPRenderManager->SetSourceAlphaBlendMode(ALPHA_BLEND_SRC_ALPHA);
    NEngineInternal::GPRenderManager->SetDestAlphaBlendMode(ALPHA_BLEND_INV_SRC_ALPHA);
    NEngineInternal::GPRenderManager->SetAlphaBlendEnable(true);
}

void CEngineWaterRenderer::Render(const CEngineComponentRenderParams& params)
{
    const CEngineComponentParams& component_params = *params.Params;
    float time = component_params.InterpolationInfo.BulletTimeCurrent.WFInterpolate;
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
