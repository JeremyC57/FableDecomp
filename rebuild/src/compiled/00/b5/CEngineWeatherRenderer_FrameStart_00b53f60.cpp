// CEngineWeatherRenderer::FrameStart(const CEngineComponentParams& cparams) -- move the particle boxes with the camera while it rains or snows, interpolate the rain / snow / mist time passed and the wind vector between their history samples, and fetch the mist volume texture (an immediate load: no size-reduce delay, busy textures reusable) when mist is on
// Leaf: ?FrameStart@CEngineWeatherRenderer@@UAEXABVCEngineComponentParams@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B53F60, 411 bytes, thiscall `ret 4`.
// Flags: /d2inlT94
// Inline-budget: /d2inlT94 (owner rule 2026-11-08, preference 2; source untouched) -- retail expands CGraphicDataBank::GetVolumeTexture
//   (PeekGraphic's index test 0x00B540A3..0x00B540D5, then CGraphicInfo::GetVolumeTexture 0x009FFB20) and calls UpdateBoxPositions
//   0x00B52990. c2 trace (hand883 inl.py at 0x1078D1AA): GetVolumeTexture c1 = 82, site score 93 (f 0.5) -> called at the default
//   threshold 74 (358v411); UpdateBoxPositions score 422. Measured: /d2inlT93 DIFFER(358v411), /d2inlT94 .. /d2inlT422 RELOCATION_MATCH,
//   /d2inlT423 DIFFER(798v411, UpdateBoxPositions expanded); smallest N = 94.
// Identity: Ego_r.pdb / FableWin.pdb ?FrameStart@CEngineWeatherRenderer@@UAEXABVCEngineComponentParams@@@Z (the CEngineComponent slot
//   override; Ego_r 0x009F7360). Locals: Ego_d.pdb (cparams; interpolate (const float); block {old_mipmap_delay (unsigned long),
//   old_reuse_busy (bool), graphic_bank (CGraphicDataBank&), index (unsigned long)}: not const, Ego_d types 0x22 / 0x30). The debug Engine_Weather_Renderer.obj carries the COMDATs this body uses:
//   CTextureManager::GetReuseBusyTextures / SetReuseBusyTextures, CGraphicDataBank::GetReduceSizeFrameDelay / GetVolumeTexture.
// Inline bodies (the T94 score depends on them): /Od sizes equal the Debug Engine_Weather_Renderer.obj COMDATs -- GetVolumeTexture 40,
//   GetReduceSizeFrameDelay 20, Get/SetReuseBusyTextures 10 / 19; PeekGraphic's release text is the shared engine/CGraphicInfo.h form
//   (landed 0x00BB0970, 0x00B1DC70), retail 0x00B540A3..0x00B540D5.
// Same TU (the lever): retail keeps `this` in ecx across the UpdateBoxPositions call (0x00B53F77 .. 0x00B53F83) because VC7.1 has
//   already compiled the two same-TU callees and knows neither writes ecx; Ego_r compiles BuildBoxPositionMatrix (order 14) and
//   UpdateBoxPositions (34) before FrameStart (38). Both are defined here ahead of the row (the Ego_d / debug source order, FrameStart first, matches too: audit 2026-11-19):
//   BuildBoxPositionMatrix = batch b30's text
//   (RELOCATION_MATCH at 0x00B51AD0 in this TU too), UpdateBoxPositions = the landed 0x00B52990 body on the shared Settings members with
//   Ego_d's names and its block scope (`if (!locked) { ... }`; RELOCATION_MATCH at 0x00B52990, 406-byte body, in this TU). Without
//   either body `this` moves to esi (416v411 at /d2inlT94, audit 2026-11-19; 418v411 was the parked retry1).
// Layout / data: Settings.Raining / Snowing / Mist, the time-passed histories and WindVector(History) (CEngineWeatherSettings TAIL);
//   MistVolumeTexture +0x110; CEngineComponentParams.InterpolationInfo.BulletTimeCurrent.WFInterpolate (+0x3C, as 0x00BEA110 reads it);
//   graphic bank 0 of NEngineInternal::GPResourceManager 0x01436E98 (+0x954), ReduceSizeFrameDelay +0x26C, SetReduceSizeFrameDelay
//   0x009FD100; g_pEngineEnvironmentDef 0x01436E24 MistAlphaGraphicPC +0xF0; CTextureManager::ReuseBusyTextures 0x013D28E8.
// Order (FITTED among equivalents): the mist block reads index before taking the bank (retail loads the environment def into ecx
//   before the resource manager, 0x00B5406B / 0x00B54071); every order with the bank first swaps those two registers (411v411).
// Wind: C3DVector::Interpolate (FableWin donor 0x01D09580, shared C3DVector.h FABLE_RETAIL_C3DVECTOR_INTERPOLATE).
// Retail-asm: 0x00B529C7, 0x00B52A01, 0x00B52A3B, 0x00B52A7C, 0x00B52AB5, 0x00B52AEE `fistp dword ptr [ebp-8]` .. `[ebp-0x14]` --
//   the shared engine/GFFloatToLongFloor.h inline-assembly floor in the same-TU UpdateBoxPositions body (and BuildBoxPositionMatrix
//   0x00B51AFC .. 0x00B51C83 `fistp dword ptr [ebp-4]`, batch b30). FrameStart's own 411 bytes carry no asm.
#define FABLE_RETAIL_CENGINECOMPONENT
#define FABLE_RETAIL_CENGINEWEATHERRENDERER
#define FABLE_RETAIL_CENGINEWEATHERRENDERER_BUFFERS
#define FABLE_RETAIL_CENGINEWEATHERRENDERER_BOXMATRIX
#define FABLE_RETAIL_CENGINEWEATHERRENDERER_FRAMESTART
#define FABLE_RETAIL_CENGINEWEATHERSETTINGS_TAIL
#define FABLE_RETAIL_CENGINEWEATHERSETTINGS_BOXCENTER
#define FABLE_RETAIL_CENGINECAMERA_POSITION
#define FABLE_RETAIL_CENGINECOMPONENTPARAMS_INTERPOLATIONINFO
#define FABLE_RETAIL_C3DVECTOR_INTERPOLATE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS
#define FABLE_RETAIL_CENGINERESOURCEMANAGER
#define FABLE_RETAIL_CENGINERESOURCEMANAGER_GETGRAPHICBANK
#define FABLE_RETAIL_CGRAPHICDATABANK_ORIGINAL_METHODS_PASS22
#define FABLE_RETAIL_CGRAPHICDATABANK_DELAYS_PASS23
#define FABLE_RETAIL_CTEXTUREMANAGER_REUSEBUSY
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CGRAPHICINFO_GETVOLUMETEXTURE
#define FABLE_RETAIL_CENVIRONMENTDEF_PARENT
#include "fable_compiler.h"
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CEngineWeatherRenderer.h"
#include "engine/CEngineComponentRenderParams.h"
#include "engine/CEngineCamera.h"
#include "engine/CEngineResourceManager.h"
#include "engine/CGraphicDataBank.h"
#include "engine/CGraphicInfo.h"
#include "engine/CTextureManager.h"
#include "engine/CEnvironmentDef.h"
#include "engine/GFFloatToLongFloor.h"
extern "C" double __cdecl fabs(double);
#pragma intrinsic(fabs)
namespace NEngineInternal { extern CEngineCamera* GPCamera; }                          // 0x01436EA0
namespace NEngineInternal { extern CEngineResourceManager* GPResourceManager; }        // 0x01436E98
extern const CEnvironmentDef* g_pEngineEnvironmentDef;                                // 0x01436E24

inline void CEngineWeatherRenderer::GetBoxCoord(long& coord, long box_span)
{
    const long distance_to_nearest_box_centre = coord % box_span;
    const long half_box_span = box_span / 2;
    coord = coord / box_span * box_span;
    if (distance_to_nearest_box_centre >= half_box_span)
        coord += box_span;
}

// Fidelity: same-TU strong definition of 0x00B51AD0 (retail TU-mate)
void CEngineWeatherRenderer::BuildBoxPositionMatrix()
{
    const C3DVector& cam_pos = NEngineInternal::GPCamera->PeekCameraPos();
    const long half_side_length = 4;
    Settings.ParticleBoxCenter.X = GFFloatToLongFloor(cam_pos.X);
    GetBoxCoord(Settings.ParticleBoxCenter.X, GFFloatToLongFloor(Settings.ParticleBoxSpan.X));
    Settings.ParticleBoxCenter.Y = GFFloatToLongFloor(cam_pos.Y);
    GetBoxCoord(Settings.ParticleBoxCenter.Y, GFFloatToLongFloor(Settings.ParticleBoxSpan.Y));
    if (!Settings.ParticleBoxZLocked) {
        Settings.ParticleBoxCenter.Z = GFFloatToLongFloor(cam_pos.Z);
        GetBoxCoord(Settings.ParticleBoxCenter.Z, GFFloatToLongFloor(Settings.ParticleBoxSpan.Z));
    }
    for (long c_x = 0; c_x < half_side_length * 2 + 1; c_x++) {
        for (long c_y = 0; c_y < half_side_length * 2 + 1; c_y++) {
            const float this_mul_x = (float)(c_x - half_side_length);
            const float this_mul_y = (float)(c_y - half_side_length);
            const long index = c_x * (half_side_length * 2 + 1) + c_y;
            Settings.ParticleBoxPos[index].X = this_mul_x * Settings.ParticleBoxSpan.X + Settings.ParticleBoxCenter.X;
            Settings.ParticleBoxPos[index].Y = this_mul_y * Settings.ParticleBoxSpan.Y + Settings.ParticleBoxCenter.Y;
            Settings.ParticleBoxPos[index].Z = (float)Settings.ParticleBoxCenter.Z;
        }
    }
}

// Copy of the re-ported owner text (owner_00b52990/src/00b52990.cpp, audit 2026-11-19): the landed 2026-11-04 text cannot be used verbatim
//   here (its local class model gives C2011 / C2065 against the shared header).
// Fidelity: same-TU strong definition of 0x00B52990 (retail TU-mate)
void CEngineWeatherRenderer::UpdateBoxPositions()
{
    if (!Settings.ParticleBoxLocked) {
        const long half_box_span_X = GFFloatToLongFloor(Settings.ParticleBoxSpan.X * 0.5f);
        const long half_box_span_Y = GFFloatToLongFloor(Settings.ParticleBoxSpan.Y * 0.5f);
        const long half_box_span_Z = GFFloatToLongFloor(Settings.ParticleBoxSpan.Z * 0.5f);
        const C3DVector& cam_pos = NEngineInternal::GPCamera->PeekCameraPos();
        const long diff_X = GFFloatToLongFloor((float)fabs((float)Settings.ParticleBoxCenter.X - cam_pos.X));
        const long diff_Y = GFFloatToLongFloor((float)fabs((float)Settings.ParticleBoxCenter.Y - cam_pos.Y));
        const long diff_Z = GFFloatToLongFloor((float)fabs((float)Settings.ParticleBoxCenter.Z - cam_pos.Z));
        if (diff_X >= half_box_span_X || diff_Y >= half_box_span_Y || diff_Z >= half_box_span_Z)
            BuildBoxPositionMatrix();
    }
}

inline const CGraphicInfo* CGraphicDataBank::PeekGraphic(unsigned long index) const
{
    return index < GraphicList.size() ? &GraphicList[index] : 0;
}

inline bool CGraphicDataBank::GetVolumeTexture(CVolumeTexture** texture, unsigned long index, unsigned long level) const
{
    return PeekGraphic(index)->GetVolumeTexture(texture, level);
}

void CEngineWeatherRenderer::FrameStart(const CEngineComponentParams& cparams)
{
    if (Settings.Raining || Settings.Snowing)
        UpdateBoxPositions();
    const float interpolate = cparams.InterpolationInfo.BulletTimeCurrent.WFInterpolate;
    Settings.RainTimePassed = (Settings.RainTimePassedHistory[1] - Settings.RainTimePassedHistory[0]) * interpolate + Settings.RainTimePassedHistory[0];
    Settings.SnowTimePassed = (Settings.SnowTimePassedHistory[1] - Settings.SnowTimePassedHistory[0]) * interpolate + Settings.SnowTimePassedHistory[0];
    Settings.MistTimePassed = (Settings.MistTimePassedHistory[1] - Settings.MistTimePassedHistory[0]) * interpolate + Settings.MistTimePassedHistory[0];
    Settings.WindVector = Settings.WindVectorHistory[0].Interpolate(Settings.WindVectorHistory[1], interpolate);
    MistVolumeTexture = 0;
    if (Settings.Mist) {
        unsigned long index = g_pEngineEnvironmentDef->MistAlphaGraphicPC;
        CGraphicDataBank& graphic_bank = *NEngineInternal::GPResourceManager->GetGraphicBank((EGraphicBankNo)0);
        unsigned long old_mipmap_delay = graphic_bank.GetReduceSizeFrameDelay();
        bool old_reuse_busy = CTextureManager::GetReuseBusyTextures();
        graphic_bank.SetReduceSizeFrameDelay(0);
        CTextureManager::SetReuseBusyTextures(true);
        graphic_bank.GetVolumeTexture(&MistVolumeTexture, index, 0);
        graphic_bank.SetReduceSizeFrameDelay(old_mipmap_delay);
        CTextureManager::SetReuseBusyTextures(old_reuse_busy);
    }
}