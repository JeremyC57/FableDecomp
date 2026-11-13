// CTCLight::DrawLight(long, const C3DVector&, NGraphicAppearance::CTCGraphicAppearance&, const NGraphicAppearance::CGraphicAppearanceDrawInfo&) -- draw one point light: grow the light-primitive list on demand, take the light's colour (override info or def), blend the old and new flicker factors by the draw interpolation, attenuate by the def's daylight curve (four fade times, midnight wrap) or by the sunlight attenuator (the thing's facing against the flattened sun direction, clamped, with the environment's diffuse colour), invert if asked, then send the scaled CEnginePrimitiveLight with the def's lighting-channel mask and count the light
// Leaf: ?DrawLight@CTCLight@@IAEXJABVC3DVector@@AAVCTCGraphicAppearance@NGraphicAppearance@@ABVCGraphicAppearanceDrawInfo@4@@Z
// Retail: Fable.exe (TLC, VC7.1 RTM) @ 0x006414A0, 1596 bytes incl. the 4-entry EffectChannel jump table 0x00641ACC..0x00641ADB (code ends
//   `ret 10h` at 0x00641AC8 + one nop; DrawFlare 0x00641AE0 follows after int3 padding). Ego_r.pdb / FableWin.pdb
//   ?DrawLight@CTCLight@@IAEXJABVC3DVector@@AAVCTCGraphicAppearance@NGraphicAppearance@@ABVCGraphicAppearanceDrawInfo@4@@Z (protected);
//   called from OnAppearanceDraw (0x00641CA6 / 0x00641E09). Jump table checked case by case: 0 -> 1 << LightingChannel.ChannelIndex0
//   (0x006419CA), 1 -> ~1 (0x006419DA), 2 -> 1 (0x006419E1), 3 -> all channels (0x006419E8); our labels sit at the same offsets.
// Callees (each checked against retail with relva.py): CTCGraphicAppearance::GetFadeDistance 0x004BC180 (result dropped) /
//   AddPersistentPrimitive 0x004C0050 / SetPrimitiveGraphics 0x004BD620, vector<CPrimitive*>::resize 0x006426A0, CBaseClass ctor 0x0099A2D0
//   (the CPrimitive temporary's handle, vtable 0x0122F598) / ~CPrimitive 0x004C4160, CTCBase::PeekWorld 0x00686A80 (x4), the facing vector
//   (CTCPhysicsBase slot 70 GetFacingVector, +0x118) and CEnvironment slot 4 PeekSunVector (+0x10), __ftol2 0x00BFEA70 (x4).
//   Data: GFastOneOverSqrtTable 0x013CE558, the light counter 0x013BA85C (reset by the display engine each frame, 0x004355BB; no
//   data-symbol name in either PDB name set: descriptive g_LightsDrawnThisFrame), float pool 1/255, 1/24, 0, 1, 3, 0.5, 255.
// Shape: GetColour / GetInnerRadius / GetOuterRadius / IsInverted / GetTime / PeekSunlightAttenuatorAngleFade* / PeekDiffuseColour /
//   the CRGBFloatColour conversions and operator*=(float) / the CEnginePrimitiveLight constructor are all inline and expanded (overlay
//   opt-ins, hdr/README.md). The segment result is an if / else-if chain (`test ecx,ecx ; je`, `cmp ecx,1 ; jne`, 0x006416C9; a switch
//   gives `sub ecx,0 ; dec ecx`). The sun direction is built from a reference with a literal zero Z (`C3DVector sun(-v.X, -v.Y, 0.0f)`)
//   and dotted as sun.Dot(facing): the zero Z then stays an expression temporary that the Z product consumes (`fxch ; fmul [Z]`,
//   0x0064186E); `-v` then `sun.Z = 0` keeps it as a variable (`fld [Z] ; fmul st(2)` and an extra pop). GFFastOneOverSqrt is the
//   NEWTON_PAREN body (C3DVector.h opt-in): its extra no-code PAREN tuple moves the scheduling-region cut so that `mov ecx,esi` for the
//   next PeekWorld issues between `fld [esp+1Ch]` and `fmul st(1)` (0x00641842), as retail.
// Layout: shared headers plus the chunk 2020 overlay: CTCLight DRAWLIGHT (nested CLightInfo + the inline getters), the new
//   NGraphicAppearance_CGraphicAppearanceDrawInfo.h, CEnginePrimitiveLight INLINE, CGameTimeManager GETTIME, CEnvironment
//   SUNLIGHTATTENUATOR, CRGBFloatColour SCALE, and the CTCGraphicAppearance LIGHTINGCHANNEL fix for MOVEMENT includers.
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CLIGHTDEF
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_GFFASTONEOVERSQRT_NEWTON_PAREN
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_MOVEMENT
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_FLARE
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_LIGHTINGCHANNEL
#define FABLE_RETAIL_CENGINEPRIMITIVE_ID_CTOR
#define FABLE_RETAIL_CENGINEPRIMITIVELIGHT_INLINE
#define FABLE_RETAIL_CTCLIGHT_DRAWLIGHT
#define FABLE_RETAIL_CRGBFLOATCOLOUR_SCALE
#define FABLE_RETAIL_CRGBFLOATCOLOUR_TO_CRGBCOLOUR
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLD_ENVIRONMENT
#define FABLE_RETAIL_CWORLD_PEEKENVIRONMENT
#define FABLE_RETAIL_CENGINEPRIMITIVE2DSPRITE_ENUM_CTORS
#define FABLE_RETAIL_CGAMETIMEMANAGER
#define FABLE_RETAIL_CGAMETIMEMANAGER_GETTIME
#define FABLE_RETAIL_CENVIRONMENT
#define FABLE_RETAIL_CENVIRONMENT_VIRTUALS
#define FABLE_RETAIL_CENVIRONMENT_SUNLIGHTATTENUATOR
#define FABLE_RETAIL_CTCPHYSICSBASE
#include "engine/CRGBFloatColour.h"
#include "engine/CThing.h"
#include "engine/CTCLight.h"
#include "engine/NGraphicAppearance_CTCGraphicAppearance.h"
#include "engine/NGraphicAppearance_CGraphicAppearanceDrawInfo.h"
#include "engine/NGraphicAppearance_CPrimitive.h"
#include "engine/CEnginePrimitive2DSprite.h"
#include "engine/CEnginePrimitiveLight.h"
#include "engine/CEnvironment.h"
#include "engine/CGameTimeManager.h"
#include "engine/CWorld.h"
#include "engine/CTCPhysicsBase.h"

extern long g_LightsDrawnThisFrame;                    // 0x013BA85C (descriptive)

void CTCLight::DrawLight(long light_no, const C3DVector& pos, NGraphicAppearance::CTCGraphicAppearance& tc_appearance,
                         const NGraphicAppearance::CGraphicAppearanceDrawInfo& draw_info)
{
    float fade_dist = tc_appearance.GetFadeDistance();
    if ((unsigned int)light_no >= Lights.size()) {
        Lights.resize(light_no + 1);
        Lights[light_no] = tc_appearance.AddPersistentPrimitive(NGraphicAppearance::CPrimitive((NGraphicAppearance::EPrimitiveType)3));
    }
    CRGBFloatColour colour(GetColour());
    float flicker = OldAccumilativeFlickerFactor * (1.0f - draw_info.Interpolate) + AccumilativeFlickerFactor * draw_info.Interpolate;
    float factor = 1.0f;
    if (PLightDef->DaylightAttenuator) {
        float time = PeekWorld().PeekGameTimeManager().GetTime();
        float times[4];
        times[0] = PLightDef->DaylightAttenuatorFadeInStart * (1.0f / 24.0f);
        times[1] = PLightDef->DaylightAttenuatorFadeInEnd * (1.0f / 24.0f);
        times[2] = PLightDef->DaylightAttenuatorFadeOutStart * (1.0f / 24.0f);
        times[3] = PLightDef->DaylightAttenuatorFadeOutEnd * (1.0f / 24.0f);
        float lerp = 0.0f;
        long segment;
        for (segment = 0; segment < 3; ++segment) {
            float start = times[segment];
            float end = times[segment + 1];
            if (start > end) {
                if (time >= start) {
                    lerp = (time - start) / ((1.0f - start) + end);
                    break;
                }
                if (time <= end) {
                    lerp = (time + (1.0f - start)) / ((1.0f - start) + end);
                    break;
                }
            } else if (time >= start && time <= end) {
                lerp = (time - start) / (end - start);
                break;
            }
        }
        if (segment == 0)
            factor = lerp;
        else if (segment == 1)
            factor = 1.0f;
        else if (segment == 2)
            factor = 1.0f - lerp;
        else
            factor = 0.0f;
    } else if (PLightDef->SunlightAttenuator) {
        C3DVector facing = GetParentThing().GetPhysicsTC()->GetFacingVector();
        const C3DVector& sun_vector = PeekWorld().PeekEnvironment().PeekSunVector();
        C3DVector sun(-sun_vector.X, -sun_vector.Y, 0.0f);
        sun.Normalise();
        facing.Z = 0.0f;
        facing.Normalise();
        float dot = sun.Dot(facing);
        float fade_start = PeekWorld().PeekEnvironment().PeekSunlightAttenuatorAngleFadeStart();
        factor = (dot - fade_start) / (PeekWorld().PeekEnvironment().PeekSunlightAttenuatorAngleFadeEnd() - fade_start);
        if (factor < 0.0f)
            factor = 0.0f;
        else if (factor > 1.0f)
            factor = 1.0f;
        colour = PeekWorld().PeekEnvironment().PeekDiffuseColour();
    }
    if (IsInverted())
        factor = 1.0f - factor;
    float scale = factor * flicker;
    colour *= scale;
    float inner_radius = GetInnerRadius() * scale;
    float outer_radius = GetOuterRadius() * scale;
    unsigned long channel_mask = 0;
    switch (PLightDef->EffectChannel) {
    case 0: channel_mask = 1 << tc_appearance.PeekLightingChannel().ChannelIndex0; break;
    case 1: channel_mask = 0xfffffffe; break;
    case 2: channel_mask = 1; break;
    case 3: channel_mask = 0xffffffff; break;
    }
    tc_appearance.SetPrimitiveGraphics(Lights[light_no], CEnginePrimitiveLight(GetParentThing().GetUniqueID(), pos, colour, outer_radius,
        inner_radius, ENGINE_INTERPOLATION_SET_0, channel_mask), 0);
    ++g_LightsDrawnThisFrame;
}