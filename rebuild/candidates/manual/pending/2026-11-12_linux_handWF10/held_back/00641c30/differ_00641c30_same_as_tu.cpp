// CTCLight::OnAppearanceDraw(CTCGraphicAppearance&, const CGraphicAppearanceDrawInfo&) -- retail 0x00641C30, 490 bytes, thiscall on the
// Grade: RELOCATION_MATCH (with CTCLight::DrawLight 0x006414A0's reconstructed body earlier in the TU, as in retail CTCLight.cpp).
// CAGraphicAppearanceComponent subobject (CTCLight+0x0c), ret 8. An active light draws a light at every MESH_LIGHT_POINT_NAME helper and a flare
// at every MESH_FLARE_POINT_NAME helper of the loaded appearance; when no light helper was drawn it draws one light at the thing (for a class-2
// thing: interpolated physics position + facing*0.35, lifted 1.0 in Z; otherwise CThing::GetPos()).
// Callees: IsResourcesLoaded 0x004BE3E0, GetNoHelperPoints 0x004BDE90, GetHelperPoint 0x004C0560, DrawLight 0x006414A0, DrawFlare 0x00641AE0,
//   CThing::GetPos 0x004C73D0, CTCPhysicsBase::GetFacingVector(float) vtbl+0x114 (slot 69); CTCPhysicsBase::GetPosition(float) inline.
// DrawLight 0x006414A0 (reconstructed here, out of line under #pragma auto_inline(off)) precedes it as in retail CTCLight.cpp: VC7.1 sees
//   DrawLight only copies its C3DVector& argument, so the fallback pos stays in registers through the if/else (stored once at the merge).
//   DrawLight's callees: GetFadeDistance 0x004BC180, Lights.resize 0x006426A0, CPrimitive 0x0099A2D0 / ~0x004C4160, AddPersistentPrimitive
//   0x004C0050, PeekWorld 0x00686A80, GetFacingVector() vtbl+0x118, PeekSunVector vtbl+0x10, __ftol2 0x00BFEA70, SetPrimitiveGraphics 0x004BD620.
//   Globals MESH_LIGHT_POINT_NAME 0x013BACC8, MESH_FLARE_POINT_NAME 0x013BACCC; floats 0.35f 0x0125A264, 1.0f 0x0122DED8.
// Flags: none
// Leaf: ?OnAppearanceDraw@CTCLight@@UAE_NAAVCTCGraphicAppearance@NGraphicAppearance@@ABVCGraphicAppearanceDrawInfo@3@@Z
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CLIGHTDEF
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_MOVEMENT
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_FLARE
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTCPHYSICSBASE_INTERPOLATED_POSITION
#define FABLE_RETAIL_CTCPHYSICSBASE_INTERPOLATED_POSITION_BODY
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_HELPERNAMES
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_FOCAL
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_PEEKLIGHTINGCHANNEL
#define FABLE_RETAIL_CENGINEPRIMITIVE_ID_CTOR
#define FABLE_RETAIL_CENGINEPRIMITIVELIGHT_INLINE_CTOR
#define FABLE_RETAIL_CRGBCOLOUR
#define FABLE_RETAIL_CRGBFLOATCOLOUR
#define FABLE_RETAIL_CRGBFLOATCOLOUR_TO_CRGBCOLOUR
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLD_ENVIRONMENT
#define FABLE_RETAIL_CWORLD_PEEKENVIRONMENT
#define FABLE_RETAIL_CENVIRONMENT_VIRTUALS
#define FABLE_RETAIL_CENVIRONMENT_LIGHT_ATTENUATION
#define FABLE_RETAIL_CGAMETIMEMANAGER
#define FABLE_RETAIL_CGAMETIMEMANAGER_GETTIME
#include "engine/CThing.h"
#include "engine/CTCLight.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/NGraphicAppearance_CAGraphicAppearanceComponent.h"
// No shared header exists for CGraphicAppearanceDrawInfo: model from struct_layouts_egor (size 36).
class C3DPositionInterpolator; class C3DOrientationInterpolator; class C3DAnimationInterpolator;
namespace NGraphicAppearance {
class CGraphicAppearanceDrawInfo {
public:
 float Interpolate;                            // +0x00
 C3DPositionInterpolator* Pos;                 // +0x04
 C3DOrientationInterpolator* Orientation;      // +0x08
 C3DAnimationInterpolator* Anim;               // +0x0c
 void* PTextureMorphs;                         // +0x10 vector<CEngineTextureMorph>*
 void* PTextureDecals;                         // +0x14 vector<CEngineTextureDecal>*
 void* HiddenMaterials;                        // +0x18 set<CCharString>*
 unsigned long ReasonsForDraw;                 // +0x1c
 bool HiddenMaterialsUsed;                     // +0x20
 bool TextureMorphsUsed;                       // +0x21
};
}
#include "engine/NGraphicAppearance_CTCGraphicAppearance.h"
#include "engine/CWorld.h"
#include "engine/CEnvironment.h"
#include "engine/CGameTimeManager.h"
#include "engine/CRGBFloatColour.h"
#include "engine/CEnginePrimitiveEnums.h"
#include "engine/CEnginePrimitiveLight.h"
extern const CCharString MESH_LIGHT_POINT_NAME;   // 0x013BACC8
extern const CCharString MESH_FLARE_POINT_NAME;   // 0x013BACCC
#pragma auto_inline(off)
// CTCLight::CLightInfo (Ego_r.pdb, 24 bytes): the override values SetColour / SetInnerRadius / ... store when the light is overridden.
class CTCLight::CLightInfo {
public:
 CRGBColour Colour;                       // +0x00
 float InnerRadius;                       // +0x04
 float OuterRadius;                       // +0x08
 float Flicker;                           // +0x0c
 unsigned long LightingChannelsEffected;  // +0x10
 bool Inverted;                           // +0x14
};
// Inline accessors (FableWin.pdb emits them in CTCLight.obj after DrawFlare; retail expands them in DrawLight).
inline const CRGBColour& CTCLight::GetColour() const {if(Overridden)return PLightInfo->Colour;return PLightDef->Colour;}
inline float CTCLight::GetInnerRadius() const {if(Overridden)return PLightInfo->InnerRadius;return PLightDef->InnerRadius;}
inline float CTCLight::GetOuterRadius() const {if(Overridden)return PLightInfo->OuterRadius;return PLightDef->OuterRadius;}
inline bool CTCLight::IsInverted() const {if(Overridden)return PLightInfo->Inverted;return PLightDef->Inverted;}
extern long GNoLightsDrawn;   // 0x013BA85C (descriptive name): zeroed by CDisplayEngine::DoRender 0x004355BB, counted up per drawn light
// CTCLight::DrawLight -- retail 0x006414A0, 1596 bytes (ret 0x10).
void CTCLight::DrawLight(long light_no,const C3DVector& pos,NGraphicAppearance::CTCGraphicAppearance& tc_appearance,
 const NGraphicAppearance::CGraphicAppearanceDrawInfo& draw_info)
{
 float fade_dist=tc_appearance.GetFadeDistance();
 if((unsigned int)light_no>=Lights.size()) {
  Lights.resize(light_no+1);
  Lights[light_no]=tc_appearance.AddPersistentPrimitive(NGraphicAppearance::CPrimitive((NGraphicAppearance::EPrimitiveType)3));
 }
 CRGBFloatColour colour(GetColour());
 float flicker=(1.0f-draw_info.Interpolate)*OldAccumilativeFlickerFactor+AccumilativeFlickerFactor*draw_info.Interpolate;
 float factor=1.0f;
 if(PLightDef->DaylightAttenuator) {
  float time=PeekWorld().PeekGameTimeManager().GetTime();
  float fade_times[4];
  fade_times[0]=PLightDef->DaylightAttenuatorFadeInStart*(1.0f/24.0f);
  fade_times[1]=PLightDef->DaylightAttenuatorFadeInEnd*(1.0f/24.0f);
  fade_times[2]=PLightDef->DaylightAttenuatorFadeOutStart*(1.0f/24.0f);
  fade_times[3]=PLightDef->DaylightAttenuatorFadeOutEnd*(1.0f/24.0f);
  float fraction=0.0f;
  long stage;
  for(stage=0;stage<3;stage++) {
   float start=fade_times[stage];
   float end=fade_times[stage+1];
   if(start>end) {
    if(time>=start) {fraction=(time-start)/((1.0f-start)+end);break;}
    if(time<=end) {fraction=(time+(1.0f-start))/((1.0f-start)+end);break;}
   }
   else if(time>=start&&time<=end) {fraction=(time-start)/(end-start);break;}
  }
  switch(stage) {
   case 0: factor=fraction; break;
   case 1: factor=1.0f; break;
   case 2: factor=1.0f-fraction; break;
   default: factor=0.0f; break;
  }
 }
 else if(PLightDef->SunlightAttenuator) {
  C3DVector facing=GetParentThing().GetPhysicsTC()->GetFacingVector();
  const C3DVector& sun=PeekWorld().PeekEnvironment().PeekSunVector();
  C3DVector to_sun(-sun.X,-sun.Y,0.0f);
  to_sun.Normalise();
  facing.Z=0.0f;
  facing.Normalise();
  float dot=facing.Dot(to_sun);
  float start=PeekWorld().PeekEnvironment().PeekSunlightAttenuatorAngleFadeStart();
  factor=(dot-start)/(PeekWorld().PeekEnvironment().PeekSunlightAttenuatorAngleFadeEnd()-start);
  if(factor<0.0f) factor=0.0f;
  else if(factor>1.0f) factor=1.0f;
  colour=PeekWorld().PeekEnvironment().PeekDiffuseColour();
 }
 if(IsInverted()) factor=1.0f-factor;
 factor*=flicker;
 colour=colour*factor;
 float inner_radius=GetInnerRadius()*factor;
 float outer_radius=GetOuterRadius()*factor;
 unsigned long channel_mask=0;
 switch(PLightDef->EffectChannel) {
  case 0: channel_mask=1<<tc_appearance.PeekLightingChannel().ChannelIndex0; break;
  case 1: channel_mask=0xfffffffe; break;
  case 2: channel_mask=1; break;
  case 3: channel_mask=0xffffffff; break;
 }
 tc_appearance.SetPrimitiveGraphics(Lights[light_no],CEnginePrimitiveLight(0,pos,colour,outer_radius,inner_radius,
  ENGINE_INTERPOLATION_SET_0,channel_mask),0);
 GNoLightsDrawn++;
}
#pragma auto_inline(on)
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
