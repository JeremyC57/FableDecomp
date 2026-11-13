// CEngineWaterSettings::UpdateConstants(const CEngineComponentRenderParams& params) -- per-render refresh of the water shader constants: the environment theme's water colour / blend / swell scalers, the interpolated bump-map shifts, PC bump frame indices and swell trig, and the four shader-variable matrices
// Leaf: ?UpdateConstants@CEngineWaterSettings@@QAEXABVCEngineComponentRenderParams@@@Z
// Grade: RELOCATION_MATCH (q.py, cl 13.10.3077 RTM, no flags, FABLE_HDR_OVERLAY=hdr/) -- 1607 bytes, 374 instructions.
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BEA110, 1607 bytes, `ret 4` (thiscall). Sole caller CEngineWaterRenderer::Render 0x00B783F0
//   (0x00B78524 `lea ecx,[edi+280h] ; call` on the embedded Settings).
// Identity: Ego_r.pdb / FableWin.pdb ?UpdateConstants@CEngineWaterSettings@@QAEXABVCEngineComponentRenderParams@@@Z (the donor order
//   WorldFrameUpdate, UpdateConstants, AddToConsole; retail TU order WorldFrameUpdate 0x00BE95F0, the constructor 0x00BE9920, the
//   console commands WaterReflectBlendFactors 0x00BEA080 / WaterBumpmaps 0x00BEA0B0, UpdateConstants, ??_G 0x00BEA760).
// Callees (all virtual or inline): CIDrawWorld slot 1 DrawGetEnvironment (CWorld 0x0049E350), CIDrawEnvironment slot 1
//   PeekInterpolatedParams (CEnvironment 0x006B7350); inline FableWin.pdb accessors CEngineWorldMap::PeekGameWorld,
//   CEngineResourceManager::PeekGraphicBank, CGraphicDataBank::PeekGraphic, CGraphicInfo::GetFrameCount,
//   CDisplayManager::PeekBackBufferResolution, CEngineLandscapeRenderer::GetSettings, CMatrix3x4::Set(12 floats), GFSin / GFCos
//   (x87 fsin / fcos on the angle times the float two-pi widened to double, one shared product), and this TU's own out-of-line
//   ?CalcReflectToRefractBlendFactors@CEngineWaterSettings@@QAEXMM@Z 0x00BE94B0 (defined here: VC7.1 expands it at 0x00BEA16A, and the
//   out-of-line copy this TU emits is byte-identical to retail 0x00BE94B0).
// Globals (the landed CEngine::Initialise 0x00B26360 spellings): g_pEngineWorldMap 0x01436E8C, g_pResourceManager 0x01436E98,
//   g_pEngineEnvironmentDef 0x01436E24, g_pDisplayManager_01436e1c 0x01436E1C, g_LandscapeRenderer_01436EA8 0x01436EA8.
// Layout: engine/CEngineWaterSettings.h with the TYPED opt-in (C2DVector[3][2] shifts +0xE8, histories [3][2][2] +0x118, CMatrix3x4
//   members); the interpolation fraction is Params->InterpolationInfo.BulletTimeCurrent.WFInterpolate (Params +0x3C); the theme
//   channel 0 colour 14 (+0xE0) and scalers 23..29 (+0x18C..+0x1A4) are the water values (no enumerator names recovered: the PDB
//   name sets carry no theme colour / scaler enum).
// Shape: the bump shifts are C2DVector arithmetic ((new - old) * t + old, the temporaries in the frame and the copy by dwords); the
//   other lerps are plain float expressions on the members (a GFGetInterpolatedValue call keeps the old value on the x87 stack and
//   misses); the swell angle alone is written `((new - old) * t) + old` -- the extra grouping leaves one no-code PAREN tuple that
//   moves c2's 81-tuple scheduling cut so the OtherSeaShaderVars tail interleaves as retail (0x00BEA661..0x00BEA68B), while the
//   same grouping on the other lerps moves the earlier cut and misplaces `mov edi,[ebx+0C4h]` (measured, pass 1 grade 6). The
//   matrices are CMatrix3x4::Set (a temporary CMatrix3x4 assignment copies with rep movsd); the background fade X / W are float
//   locals (a C4DVector reference reloads them for the second matrix).
// Retail-asm: 0x00BEA432 `fld dword ptr [esp+18h]` + 0x00BEA436 `fistp dword ptr [esp+14h]` (and 0x00BEA47C / 0x00BEA480,
//   0x00BEA4C2 / 0x00BEA4C6): the engine's inline-assembly float-to-long helper engine/GFFloatToLongNear.h (the lerped frame offset
//   stored to the helper's by-value parameter and reloaded for a bare dword fistp). C++ attempts at this site (fragment probes, cl
//   13.10.3077, /O2 and /O2 /QIfist): `(long)x`, `(int)x`, `(unsigned long)x`, `(short)x`, `(long)(double)x`, a named `long v = (long)x`,
//   `(long)(x + 0.5f)`, `(long)floor(x + 0.5f)` all call __ftol2 (or `fistp qword` under /QIfist); `_mm_cvtss_si32` / `_mm_cvttss_si32`
//   give cvtss2si / cvttss2si. No C++ or intrinsic spelling gives the bare dword fistp; this row carries no asm of its own.
#define FABLE_RETAIL_CENGINEWATERSETTINGS_TYPED
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_CMATRIX3X4
#define FABLE_RETAIL_CENGINECOMPONENTPARAMS_INTERPOLATIONINFO
#define FABLE_RETAIL_CENGINEWORLDMAP
#define FABLE_RETAIL_CENGINEWORLDMAP_GAMEWORLD
#define FABLE_RETAIL_CENGINERESOURCEMANAGER
#define FABLE_RETAIL_CGRAPHICDATABANK_SURFACECOMPOSITION
#define FABLE_RETAIL_CGRAPHICINFO_GETFRAMECOUNT
#define FABLE_RETAIL_CDISPLAYMANAGER
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CDISPLAYMANAGER_PEEKBACKBUFFERRESOLUTION
#define FABLE_RETAIL_CENGINELANDSCAPERENDERER
#define FABLE_RETAIL_CENGINELANDSCAPERENDERER_GETSETTINGS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CENVIRONMENTDEF_PARENT
#include "fable_string.h"
#include "engine/CEngineWaterSettings.h"
#include "engine/CEngineComponentRenderParams.h"
#include "engine/CEngineWorldMap.h"
#include "engine/CIDrawWorld.h"
#include "engine/CIDrawEnvironment.h"
#include "engine/CEngineResourceManager.h"
#include "engine/CGraphicDataBank.h"
#include "engine/CDisplayManager.h"
#include "engine/CEngineLandscapeRenderer.h"
#include "engine/CEnvironmentDef.h"
#include "engine/GFSin.h"
#include "engine/GFCos.h"
#include "engine/GFFloatToLongNear.h"

extern CEngineWorldMap*          g_pEngineWorldMap;              // 0x01436E8C
extern CEngineResourceManager*   g_pResourceManager;             // 0x01436E98
extern const CEnvironmentDef*    g_pEngineEnvironmentDef;        // 0x01436E24
extern CDisplayManager*          g_pDisplayManager_01436e1c;     // 0x01436E1C
extern CEngineLandscapeRenderer* g_LandscapeRenderer_01436EA8;   // 0x01436EA8

void CEngineWaterSettings::CalcReflectToRefractBlendFactors(float start, float end)
{
    float scale = 1.0f / (end - start);         // 0x00BE94B0: fld end ; fsub start ; fdivr 1.0
    ReflectToRefractBlendMult = -1.0f * scale;
    ReflectToRefractBlendAdd = 1.0f - start * scale;
}

void CEngineWaterSettings::UpdateConstants(const CEngineComponentRenderParams& params)
{
    // Retail keeps the fraction in the dead parameter slot ([esp+34h]).
    float interp = params.Params->InterpolationInfo.BulletTimeCurrent.WFInterpolate;
    if (!ForceWaterSettings) {
        const CIDrawEnvironmentParams& env = g_pEngineWorldMap->PeekGameWorld()->DrawGetEnvironment()->PeekInterpolatedParams();
        Colour = env.ThemeChannels[0].Colours[14];                      // +0xE0: dword copy through edi
        CalcReflectToRefractBlendFactors(env.ThemeChannels[0].Scalers[23], env.ThemeChannels[0].Scalers[24]);
        FlowSpeedFactor = env.ThemeChannels[0].Scalers[25];
        SwellOscilationSpeed = env.ThemeChannels[0].Scalers[26];
        SwellFactor = env.ThemeChannels[0].Scalers[27] * 0.0001f;
        ShoreSwellFactor = env.ThemeChannels[0].Scalers[28];
        RefOffset = env.ThemeChannels[0].Scalers[29];
    }
    BumpmapGenerationShift[0][0] = (BumpmapGenerationShiftHistory[0][0][0] - BumpmapGenerationShiftHistory[0][0][1]) * interp + BumpmapGenerationShiftHistory[0][0][1];
    BumpmapGenerationShift[0][1] = (BumpmapGenerationShiftHistory[0][1][0] - BumpmapGenerationShiftHistory[0][1][1]) * interp + BumpmapGenerationShiftHistory[0][1][1];
    BumpmapGenerationShift[1][0] = (BumpmapGenerationShiftHistory[1][0][0] - BumpmapGenerationShiftHistory[1][0][1]) * interp + BumpmapGenerationShiftHistory[1][0][1];
    BumpmapGenerationShift[1][1] = (BumpmapGenerationShiftHistory[1][1][0] - BumpmapGenerationShiftHistory[1][1][1]) * interp + BumpmapGenerationShiftHistory[1][1][1];
    EdgeWakeOffset = (EdgeWakeOffsetHistory[0] - EdgeWakeOffsetHistory[1]) * interp + EdgeWakeOffsetHistory[1];

    const CGraphicDataBank* bank = g_pResourceManager->PeekGraphicBank((EGraphicBankNo)0);
    const CGraphicInfo* ice = bank->PeekGraphic(g_pEngineEnvironmentDef->IceBumpMapPC);
    const CGraphicInfo* water = bank->PeekGraphic(g_pEngineEnvironmentDef->WaterBumpMapPC);
    const CGraphicInfo* sea = bank->PeekGraphic(g_pEngineEnvironmentDef->SeaBumpMapPC);
    if (ice && ice->GetFrameCount() > 0)
        IceBumpMapPCFrameIndex = GFFloatToLongNear((IceBumpMapPCFrameOffset[0] - IceBumpMapPCFrameOffset[1]) * interp + IceBumpMapPCFrameOffset[1]) % ice->GetFrameCount();
    else
        IceBumpMapPCFrameIndex = 0;
    if (water && water->GetFrameCount() > 0)
        WaterBumpMapPCFrameIndex = GFFloatToLongNear((WaterBumpMapPCFrameOffset[0] - WaterBumpMapPCFrameOffset[1]) * interp + WaterBumpMapPCFrameOffset[1]) % water->GetFrameCount();
    else
        WaterBumpMapPCFrameIndex = 0;
    if (sea && sea->GetFrameCount() > 0)
        SeaBumpMapPCFrameIndex = GFFloatToLongNear((SeaBumpMapPCFrameOffset[0] - SeaBumpMapPCFrameOffset[1]) * interp + SeaBumpMapPCFrameOffset[1]) % sea->GetFrameCount();
    else
        SeaBumpMapPCFrameIndex = 0;

    float swell = (((SwellTrigIncrimentHistory[0] - SwellTrigIncrimentHistory[1]) * interp) + SwellTrigIncrimentHistory[1]) * 0.15915494f;
    SinShift = (float)GFSin(swell);
    CosShift = (float)GFCos(swell);
    ShoreSinShift = SinShift - 1.0f;

    C2DExtentsI resolution = g_pDisplayManager_01436e1c->PeekBackBufferResolution();
    float inv_width = 1.0f / resolution.X;
    float inv_height = 1.0f / resolution.Y;
    float fade = g_LandscapeRenderer_01436EA8->GetSettings().ForegroundFadeStart - 22.627417f;
    float inv_fade = 1.0f / fade;
    OtherShaderVars.Set(BumpmapSpan, inv_width, ReflectToRefractBlendMult,
                                 ShoreSinShift, inv_height, ReflectToRefractBlendAdd,
                                 SinShift, fade, SoftEdgeMultFactor,
                                 CosShift, inv_fade, SoftEdgeAddFactor);
    OtherSeaShaderVars.Set(SeaBumpmapSpan, inv_width, ReflectToRefractBlendMult,
                                    ShoreSinShift, inv_height, ReflectToRefractBlendAdd,
                                    SinShift, fade, SoftEdgeMultFactor,
                                    CosShift, inv_fade, SoftEdgeAddFactor);
    float background_x = g_LandscapeRenderer_01436EA8->GetSettings().BackgroundFadeTransform.X;
    float background_w = g_LandscapeRenderer_01436EA8->GetSettings().BackgroundFadeTransform.W;
    OtherBackgroundShaderVars.Set(BumpmapSpan, inv_width, 0.0f,
                                           background_x, inv_height, 0.0f,
                                           background_w, 0.0f, 0.0f,
                                           1.0f, 0.0f, 0.0f);
    OtherSeaBackgroundShaderVars.Set(SeaBumpmapSpan, inv_width, 0.0f,
                                              background_x, inv_height, 0.0f,
                                              background_w, 0.0f, 0.0f,
                                              1.0f, 0.0f, 0.0f);
}