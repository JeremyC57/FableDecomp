// CEngineWaterRenderer::Render(const CEngineComponentRenderParams& params) -- per-frame water pass: works out which render lists hold
//   patches, updates the settings constants, creates the dynamic textures, then (inside a scoped capture block and with shaders
//   enabled) sets the common water render states and runs the foreground / background / ice / sea passes, restoring all state at the end.
// Leaf: ?Render@CEngineWaterRenderer@@UAEXABVCEngineComponentRenderParams@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B783F0, length 9343 bytes, __fastcall, `ret 4`.
// Identity: Ego_r.pdb and FableWin.pdb ?Render@CEngineWaterRenderer@@UAEXABVCEngineComponentRenderParams@@@Z (virtual, public).
// Overlay: $W/hdr (export FABLE_HDR_OVERLAY=$W/hdr), opt-ins listed in hdr/README.md.
// FITTED (chosen by grading; all code-neutral, retail behaviour unchanged):
//   - `bool fg = ForegroundRenderList.begin() != ForegroundRenderList.end();` (the other nine lists use !empty()): byte-identical code;
//     the two inline begin()/end() calls in place of empty() change c2's whole-function record count by the 1..3 late records that
//     decide the bookmark test/or operand order of the SetRenderState block at 0x00B79938 (2nd SetTextureBumpEnvMatrix of the zero
//     arm in the sea block's first SetCommonRenderStates; lk comparator entry 76 was an exact tie that kept the wrong order).
//   - `bool created = CreateDynamicTextures(...); if (!created) return;` (code-identical to the direct test; restores bit A of the
//     quartet 0x00B7945D..0x00B79589).
//   - the early return tests `!(iceFg || iceBg)` in place of a named `ice` bool (the named materialised || costs 3 late records).
// Fidelity: 5 unused named locals (unused1..unused5, last-resort device c): they re-key the c2 first-pass ids that decide the
//   mask/flag load order (bit A) of the bookmark blocks 0x00B7945D/0x00B794C1/0x00B79525/0x00B79589, 0x00B7A565, 0x00B7A5D0/0x00B7A635.
// Inferred: 6 dead release asserts FABLE_RETAIL_ASSERT(this) through fable_assert.h (release dead arm, emits no bytes; last-resort
//   device d; FableWin's debug Render is 504 instructions v ours 414 at /Od, i.e. retail's source carried asserts): each dead arm takes
//   one 32-record block of c2 ids (with the 5 locals: k = 6 is the only count that closes every bit-A site, tmp/e8/o_v4.txt).
// Callees: CEngineWaterSettings::UpdateConstants 0x00BEA110, CreateDynamicTextures 0x00B72180, Enable/DisableVertexShaders
//   0x00987FE0/0x009880E0, Enable/DisablePixelShaders 0x00988110/0x00988190, ResetWorldTransform 0x00988290, RenderForeground
//   0x00B761D0, RenderBackground 0x00B76BC0, RenderIceForeground 0x00B751F0, RenderIceBackground 0x00B75CA0,
//   RenderSeaReflectiveForeground 0x00B77370, RestoreCaptureBlock 0x00A05840, CEngineSkyRenderer::GetEnvironmentMap 0x00B665E0,
//   CTexture::GenerateMipmapsWithGPU 0x009FAF70, RenderSeaNonReflectiveForeground 0x00B77E20, RenderSeaMidground 0x00B6FA40,
//   RenderSeaBackground 0x00B70230, FinishUsingDynamicTextures 0x00B6D630; SetCommonRenderStates (private, no out-of-line retail
//   copy: expanded three times) is defined above the row.
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
#define FABLE_RETAIL_ASSERT_DEAD_ARM
#include "fable_compiler.h"
#include "fable_assert.h"
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
namespace NEngineInternal { extern CEngineSkyRenderer* GPSkyRenderer; }                  // 0x01436E50 (anniversary PDB NEngineInternal::GPSkyRenderer; reloc_bindings: g_pEngineSkyRenderer)

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
    long unused1 = 0, unused2 = 0, unused3 = 0, unused4 = 0, unused5 = 0;   // Fidelity: unused (header comment)
    const CEngineComponentParams& component_params = *params.Params;
    FABLE_RETAIL_ASSERT(this);
    FABLE_RETAIL_ASSERT(this);
    FABLE_RETAIL_ASSERT(this);
    FABLE_RETAIL_ASSERT(this);
    FABLE_RETAIL_ASSERT(this);
    FABLE_RETAIL_ASSERT(this);
    float time = component_params.InterpolationInfo.BulletTimeCurrent.WFInterpolate;
    bool fg = ForegroundRenderList.begin() != ForegroundRenderList.end();  // FITTED (header comment)
    bool bg = !BackgroundRenderList.empty() || !AlphaedBackgroundRenderList.empty();
    bool iceFg = !IceForegroundRenderList.empty();
    bool iceBg = !IceBackgroundRenderList.empty() || !IceAlphaedBackgroundRenderList.empty();
    bool seaRefl = !SeaReflectiveForegroundRenderList.empty();
    bool seaNonRefl = !SeaNonReflectiveForegroundRenderList.empty();
    bool seaMid = !SeaMidgroundRenderList.empty() || !SeaAlphaedMidgroundRenderList.empty();
    bool seaBg = HaveBackgroundSea && Settings.EnableSea;
    if (!(iceFg || iceBg) && !fg && !bg && !seaRefl && !seaNonRefl && !seaMid && !seaBg)
        return;
    Settings.UpdateConstants(params);
    bool created = CreateDynamicTextures(fg || bg, seaRefl || seaNonRefl || seaMid || seaBg, iceFg || iceBg);
    if (!created)
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
            NEngineInternal::GPSkyRenderer->GetEnvironmentMap(EnvironmentMap, params);
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