// CTCLight::OnAppearanceDraw(CTCGraphicAppearance&, const CGraphicAppearanceDrawInfo&) -- an active light draws a light at every light helper point and a flare at every flare helper point of the loaded appearance; when no light helper was drawn it draws one light at the thing
// Leaf: ?OnAppearanceDraw@CTCLight@@UAE_NAAVCTCGraphicAppearance@NGraphicAppearance@@ABVCGraphicAppearanceDrawInfo@3@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00641C30, 490 bytes, thiscall on the CAGraphicAppearanceComponent subobject (CTCLight+0x0C), `ret 8`.
//   Ego_r.pdb / FableWin.pdb ?OnAppearanceDraw@CTCLight@@UAE_NAAVCTCGraphicAppearance@NGraphicAppearance@@ABVCGraphicAppearanceDrawInfo@3@@Z.
// Callees: IsResourcesLoaded 0x004BE3E0, GetNoHelperPoints 0x004BDE90, GetHelperPoint 0x004C0560, DrawLight 0x006414A0, DrawFlare 0x00641AE0,
//   CThing::GetPos 0x004C73D0, CTCPhysicsBase::GetFacingVector(float) (slot 69, +0x114); CTCPhysicsBase::GetPosition(float) inline.
//   Globals MESH_LIGHT_POINT_NAME 0x013BACC8, MESH_FLARE_POINT_NAME 0x013BACCC; floats 0.35f 0x0125A264, 1.0f 0x0122DED8.
// Shape: for a class-2 thing the fallback position is the interpolated physics position + facing * 0.35, lifted 1.0 in Z; otherwise
//   CThing::GetPos().
// Same-TU callee: CTCLight::DrawLight 0x006414A0 is defined first in retail's CTCLight.cpp. Its body below is the landed row's text,
//   unchanged (rebuild/src/compiled/00/64/CTCLight_DrawLight_006414a0.cpp, byte-proven; this file also grades RELOCATION_MATCH against
//   0x006414A0). With the body visible VC7.1 knows the `const C3DVector&` argument does not escape and keeps the fallback position in
//   registers through the if / else (stored once at the merge); with a declaration only the row is DIFFER(502v490).
#define FABLE_RETAIL_CTCPHYSICSBASE_INTERPOLATED_POSITION
#define FABLE_RETAIL_CTCPHYSICSBASE_INTERPOLATED_POSITION_BODY
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_HELPERNAMES
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_FOCAL
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

extern const CCharString MESH_LIGHT_POINT_NAME;   // 0x013BACC8
extern const CCharString MESH_FLARE_POINT_NAME;   // 0x013BACCC

bool CTCLight::OnAppearanceDraw(NGraphicAppearance::CTCGraphicAppearance& tc_appearance,const NGraphicAppearance::CGraphicAppearanceDrawInfo& draw_info)
{
 if(Active) {
  if(!tc_appearance.IsResourcesLoaded(true)) return false;
  long lights_drawn=0;
  long no_lights=tc_appearance.GetNoHelperPoints(MESH_LIGHT_POINT_NAME,1.0f);
  long i;
  C3DVector helper_pos;
  for(i=0;i<no_lights;i++) {
   if(tc_appearance.GetHelperPoint(MESH_LIGHT_POINT_NAME,i,0,helper_pos,draw_info.Interpolate)) {
    DrawLight(i,helper_pos,tc_appearance,draw_info);
    lights_drawn++;
   }
  }
  long no_flares=tc_appearance.GetNoHelperPoints(MESH_FLARE_POINT_NAME,1.0f);
  for(i=0;i<no_flares;i++) {
   if(tc_appearance.GetHelperPoint(MESH_FLARE_POINT_NAME,i,0,helper_pos,draw_info.Interpolate))
    DrawFlare(i,helper_pos,tc_appearance,draw_info);
  }
  if(lights_drawn==0) {
   C3DVector pos;
   if(GetParentThing().GetType()==2) {
    C3DVector p=GetParentThing().GetPhysicsTC()->GetPosition(draw_info.Interpolate)+GetParentThing().GetPhysicsTC()->GetFacingVector(draw_info.Interpolate)*0.35f;
    pos=C3DVector(p.X,p.Y,p.Z+1.0f);
   }
   else
    pos=GetParentThing().GetPos();
   DrawLight(0,pos,tc_appearance,draw_info);
  }
 }
 return true;
}