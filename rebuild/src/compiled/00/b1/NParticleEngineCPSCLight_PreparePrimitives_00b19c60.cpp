// 00b19c60 NParticleEngine::CPSCLight::PreparePrimitives
// Leaf: ?PreparePrimitives@CPSCLight@NParticleEngine@@UAEXMABVCEnginePrimitivePrepareInfo@@AAVCParticleSystem@2@ABV?$CParticleUpdateParameters@KK@2@AAVCEnginePrimitiveHandle@@@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B19C60, 2516 bytes, thiscall, `ret 14h`.
// Identity: FableWin.pdb ?PreparePrimitives@CPSCLight@NParticleEngine@@...; slot 13 of the CPSCLight vtable 0x012A0624 (the landed
//   CPSCUpdateNormal::PreparePrimitives 0x00B0C910 is slot 13 of its own, same signature). Catalog label "vfunc_13".
// Callees: GetTickRate 0x00AEA9F0, CPSCLight::SmoothFadeValue 0x00B1AD90, CParticleSystem::ScaleParticleDimensions(float&) 0x00AF8F80,
//   CPSCBase::GetCurrentPosition 0x00AF2640, CParticleSystem::GetSystemTransform 0x00AF2250, CEnginePrimitiveLight ctor 0x00642000,
//   CIEngine slot 23 SetPersistentPrimitive, CEnginePrimitiveHandle::Clear inline (slot 3 NotifyReleased + counted Reset).
// Retail-asm-helper: GFFloatToLongNear
// Retail-asm: 0x00B1A4DA `fistp dword ptr [esp+20h]` (GetRGBColourClamped).
// Retail-asm: 0x00B1A4F4 `fistp dword ptr [esp+20h]` (GetRGBColourClamped).
// Retail-asm: 0x00B1A512 `fistp dword ptr [esp+20h]` (GetRGBColourClamped).
// Retail-asm: 0x00B1A52E `fistp dword ptr [esp+20h]` (GetRGBColourClamped).
#define FABLE_RETAIL_CRGBFLOATCOLOUR_BYTE_ASSIGN_PASS28
#define FABLE_RETAIL_CRGBFLOATCOLOUR_TO_CRGBCOLOUR
#define FABLE_RETAIL_CRGBFLOATCOLOUR_ORIGINAL_FLOAT_CTORS
#define FABLE_RETAIL_CPARTICLESYSTEM_SCALE
#define FABLE_RETAIL_CPARTICLESYSTEM_SCALEPARTICLEDIMENSIONS_FLOAT_INLINE
#define FABLE_RETAIL_CENGINEPRIMITIVE_ID_CTOR
#define FABLE_RETAIL_CENGINEPRIMITIVELIGHT_INLINE
// Inline-budget: native expands each CRGBFloatColour byte assignment and the shared colour interpolation. Plain assignment, global thresholds and scoped depth grades are recorded in PROBES.md.
#define FABLE_RETAIL_CPSCLIGHT_PREPARE_PRIMITIVES_PASS28
#define FABLE_RETAIL_PASS22_CPSCLIGHT_DATABASE_SERIALISE
#define FABLE_RETAIL_CRGBCOLOUR
#define FABLE_RETAIL_CRGBFLOATCOLOUR
#define FABLE_RETAIL_CPARTICLESYSTEM
#define FABLE_RETAIL_CMATRIX3X4
#define FABLE_RETAIL_CPARTICLEEMITTER
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CIENGINE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CENGINEPRIMITIVEHANDLE
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#include "engine/GFGetInterpolatedValue.h"
#include "engine/CRGBColour.h"
#include "engine/CRGBFloatColour.h"
#include "engine/C3DVector.h"
#include "engine/CMatrix3x4.h"
#include "engine/CArray.h"
#include "engine/GFFloatToLongNear.h"
#include "engine/CBaseClassNonCopyable.h"
#include "engine/CEnginePrimitiveHandle.h"
#include "engine/CEnginePrimitivePrepareInfo.h"
#include "engine/CEnginePrimitiveLight.h"
#include "engine/CIEngine.h"
#include "engine/CParticleSystem.h"
#include "engine/CPSCLight.h"
extern const float DELTA_NOTIONAL_ZERO;                 // 0x0129BA3C
namespace NParticleEngine {
long FABLE_FASTCALL GetTickRate();                          // 0x00AEA9F0
void CPSCLight::PreparePrimitives(float interpolate, const CEnginePrimitivePrepareInfo& info, CParticleSystem& system,
                                  const CParticleUpdateParameters<unsigned long, unsigned long>& params, CEnginePrimitiveHandle& system_group)
{
    if (!LightEnabled || !LightIsAlive) {
        PrimHandle.Clear();
        return;
    }
    {
        float tick_rate = (float)GetTickRate();
        float fade_in_end = LightFadeInEndInteger * 0.001f;
        float fade_out_begin = LightFadeOutBeginInteger * 0.001f;
        CRGBFloatColour start_colour;
        start_colour = LightUseStartColour ? LightStartColour : LightUseMidColour ? LightMidColour
                     : LightUseEndColour ? LightEndColour : CRGBColour(255, 255, 255);
        CRGBFloatColour end_colour;
        end_colour = LightUseEndColour ? LightEndColour : LightUseMidColour ? LightMidColour
                   : LightUseStartColour ? LightStartColour : CRGBColour(255, 255, 255);
        CRGBFloatColour mid_colour;
        mid_colour = LightUseMidColour ? LightMidColour : start_colour.Interpolate(end_colour, 0.5f).operator CRGBColour();
        float life_timer = LightLifeTimer + interpolate;
        float max_life = tick_rate * LightLifeSecs;
        float normalised_time = 0.0f;
        float anim_percent = 0.0f;
        if (LightUseLifeSecs) {
            normalised_time = life_timer / max_life;
            if (normalised_time < 0.0f) normalised_time = 0.0f;
            else if (normalised_time > 1.0f) normalised_time = 1.0f;
            anim_percent = normalised_time;
            if (anim_percent < 0.0f) anim_percent = 0.0f;
            else if (anim_percent > 1.0f - DELTA_NOTIONAL_ZERO) anim_percent = 1.0f - DELTA_NOTIONAL_ZERO;
        }
        CRGBFloatColour colour_float;
        if (normalised_time < 0.5f)
            colour_float = start_colour.Interpolate(mid_colour, normalised_time * 2.0f);
        else
            colour_float = mid_colour.Interpolate(end_colour, (normalised_time - 0.5f) * 2.0f);
        float fade_value = system.GetGlobalAlpha();
        if (normalised_time < fade_in_end) {
            float fade_in = SmoothFadeValue(normalised_time / fade_in_end);
            fade_value *= fade_in;
        }
        if (normalised_time > fade_out_begin) {
            float fade_out = SmoothFadeValue((1.0f - normalised_time) / (1.0f - fade_out_begin));
            fade_value *= fade_out;
        }
        float rad_scale = 1.0f;
        if (fade_value < 1.0f) {
            if (LightColourFadeEnable) {
                colour_float *= CRGBFloatColour(fade_value, fade_value, fade_value) * (CRGBFloatColour(1.0f, 1.0f, 1.0f) - CRGBFloatColour(LightColourFadeMinimum)) + CRGBFloatColour(LightColourFadeMinimum);
                CRGBFloatColour light_colour_fade_minimum_float(LightColourFadeMinimum);
                colour_float.R = colour_float.R > light_colour_fade_minimum_float.R ? colour_float.R : light_colour_fade_minimum_float.R;
                colour_float.G = colour_float.G > light_colour_fade_minimum_float.G ? colour_float.G : light_colour_fade_minimum_float.G;
                colour_float.B = colour_float.B > light_colour_fade_minimum_float.B ? colour_float.B : light_colour_fade_minimum_float.B;
            }
            if (LightWorldRadiusFadeEnable)
                rad_scale = (1.0f - LightWorldRadiusFadeMinimum) * fade_value + LightWorldRadiusFadeMinimum;
        }
        system.ScaleParticleDimensions(rad_scale);
        // Fidelity: the grouped attenuation expression preserves the native scheduling before GetCurrentPosition.
        float attenuation_factor = ((LightAttenuationFactor + 1) / 1000.0f);
        float start_render_spot_radius = attenuation_factor * LightStartRenderWorldRadius;
        float end_render_spot_radius = attenuation_factor * LightEndRenderWorldRadius;
        float world_rad = GFGetInterpolatedValue(LightStartRenderWorldRadius, LightEndRenderWorldRadius, anim_percent);
        world_rad *= rad_scale;
        float spot_rad = GFGetInterpolatedValue(start_render_spot_radius, end_render_spot_radius, anim_percent);
        spot_rad *= rad_scale;
        C3DVector pos = GetCurrentPosition(params, LightPositionParam, interpolate, false, 0);
        system.TransformPosIfWorld(pos);
        info.Engine->SetPersistentPrimitive(PrimHandle, CEnginePrimitiveLight(0, pos, colour_float.GetRGBColourClamped(), world_rad, spot_rad,
                                            (EEngineInterpolationSet)0, 0xffffffff), 0);
    }
}
}