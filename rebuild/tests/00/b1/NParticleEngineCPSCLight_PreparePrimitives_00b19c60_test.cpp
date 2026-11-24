// Render the submitted light update against a recording engine. Colour conversion and interpolation use the project headers.
#include <cassert>
#include <cmath>
#include <cstdio>
#define FABLE_PORTABLE
#define FABLE_RETAIL_CRGBCOLOUR
#define FABLE_RETAIL_CRGBFLOATCOLOUR
#define FABLE_RETAIL_CRGBFLOATCOLOUR_BYTE_ASSIGN_PASS28
#define FABLE_RETAIL_CRGBFLOATCOLOUR_ORIGINAL_FLOAT_CTORS
#define FABLE_RETAIL_CRGBFLOATCOLOUR_TO_CRGBCOLOUR
#include "engine/CRGBFloatColour.h"
#include "engine/GFGetInterpolatedValue.h"
const float DELTA_NOTIONAL_ZERO=0.0001f;
enum EEngineInterpolationSet { DefaultSet=0 };
struct CEnginePrimitiveHandle {int clears;void Clear(){++clears;}};
struct CEnginePrimitiveLight {
 C3DVector pos;CRGBColour colour;float world,spot;
 CEnginePrimitiveLight(int id,const C3DVector& p,const CRGBColour& c,float w,float s,EEngineInterpolationSet i,unsigned long flags):
 pos(p),colour(c),world(w),spot(s){assert(!id && i==DefaultSet && flags==0xffffffffUL);}
};
struct LightEngine {int sets;CRGBColour colour;C3DVector pos;float world,spot;CEnginePrimitiveHandle* handle;
 void SetPersistentPrimitive(CEnginePrimitiveHandle& h,const CEnginePrimitiveLight& v,int flags){assert(!flags);++sets;handle=&h;pos=v.pos;colour=v.colour;world=v.world;spot=v.spot;}
};
struct CEnginePrimitivePrepareInfo{LightEngine* Engine;};
namespace NParticleEngine {
static long GetTickRate(){return 4;}
template<class A,class B> struct CParticleUpdateParameters{C3DVector position;};
struct CParticleSystem {float alpha,scale;bool transform;int scaled,transformed;
 float GetGlobalAlpha()const{return alpha;}
 void ScaleParticleDimensions(float& f){++scaled;f*=scale;}
 void TransformPosIfWorld(C3DVector& p){++transformed;if(transform)p.X+=8;}
};
struct CPSCLight {
 bool LightEnabled,LightIsAlive,LightUseStartColour,LightUseMidColour,LightUseEndColour,LightUseLifeSecs,LightColourFadeEnable,LightWorldRadiusFadeEnable;
 long LightLifeTimer,LightAttenuationFactor,LightFadeInEndInteger,LightFadeOutBeginInteger;float LightLifeSecs,LightWorldRadiusFadeMinimum,LightStartRenderWorldRadius,LightEndRenderWorldRadius;
 unsigned long LightPositionParam;CEnginePrimitiveHandle PrimHandle;CRGBColour LightStartColour,LightMidColour,LightEndColour,LightColourFadeMinimum;
 float SmoothFadeValue(float t)const{return static_cast<float>(0.5-0.5*std::cos(3.141592653589793*t));}
 C3DVector GetCurrentPosition(const CParticleUpdateParameters<unsigned long,unsigned long>& params,const unsigned long& index,float interpolation,bool end,unsigned long flags)const{
  assert(index==7 && !end && !flags);C3DVector p=params.position;p.Z+=interpolation;return p;
 }
 void PreparePrimitives(float,const CEnginePrimitivePrepareInfo&,CParticleSystem&,const CParticleUpdateParameters<unsigned long,unsigned long>&,CEnginePrimitiveHandle&);
};
}

namespace NParticleEngine {
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

static float channelFraction(unsigned char value){return value/255.0f;}
static float clamp01(float v){return v<0?0:v>1?1:v;}
static unsigned char channel(const CRGBColour& c,int i){return i==0?c.R:i==1?c.G:i==2?c.B:c.A;}
int main(){
 using namespace NParticleEngine;
 const long timers[]={-2,0,1,2,4,6,7,8,10};
 const CRGBColour white(255,255,255,255);
 for(unsigned int mask=0;mask<512;++mask)for(int j=0;j<9;++j){
  CPSCLight light;light.PrimHandle.clears=0;light.LightEnabled=(mask&3)!=0;light.LightIsAlive=(mask&3)!=1;
  light.LightUseStartColour=(mask&4)!=0;light.LightUseMidColour=(mask&8)!=0;light.LightUseEndColour=(mask&16)!=0;light.LightUseLifeSecs=(mask&32)!=0;
  light.LightColourFadeEnable=(mask&64)!=0;light.LightWorldRadiusFadeEnable=(mask&128)!=0;
  light.LightLifeTimer=timers[j];light.LightLifeSecs=2;light.LightAttenuationFactor=499;light.LightFadeInEndInteger=250;light.LightFadeOutBeginInteger=750;
  light.LightWorldRadiusFadeMinimum=0.25f;light.LightStartRenderWorldRadius=8;light.LightEndRenderWorldRadius=16;light.LightPositionParam=7;
  light.LightStartColour=CRGBColour(255,0,64,255);light.LightMidColour=CRGBColour(16,192,32,255);light.LightEndColour=CRGBColour(0,32,255,255);
  light.LightColourFadeMinimum=CRGBColour(0,51,102,255);
  CParticleSystem system;system.alpha=(mask&256)?0.5f:1.0f;system.scale=(mask&4)?2.0f:1.0f;system.transform=(mask&8)!=0;system.scaled=system.transformed=0;
  LightEngine engine;engine.sets=0;CEnginePrimitivePrepareInfo info={&engine};CEnginePrimitiveHandle group={0};
  CParticleUpdateParameters<unsigned long,unsigned long> params;params.position.X=1;params.position.Y=2;params.position.Z=3;
  light.PreparePrimitives(0,info,system,params,group);
  bool active=light.LightIsAlive && light.LightEnabled;assert(light.PrimHandle.clears==int(!active) && engine.sets==int(active) && group.clears==0);
  if(!active){assert(!system.scaled && !system.transformed);continue;}
  assert(engine.handle==&light.PrimHandle && system.scaled==1 && system.transformed==1 && engine.pos.X==(system.transform?9:1) && engine.pos.Y==2 && engine.pos.Z==3);
  float life=light.LightUseLifeSecs?clamp01(timers[j]/8.0f):0.0f;float anim=life>1.0f-DELTA_NOTIONAL_ZERO?1.0f-DELTA_NOTIONAL_ZERO:life;
  float fade=system.alpha;if(life<0.25f)fade*=light.SmoothFadeValue(life/0.25f);if(life>0.75f)fade*=light.SmoothFadeValue((1.0f-life)/0.25f);
  float scale=(light.LightWorldRadiusFadeEnable && fade<1?(0.75f*fade+0.25f):1.0f)*system.scale;
  float radius=(8.0f+8.0f*anim)*scale;assert(std::fabs(engine.world-radius)<0.00002f && std::fabs(engine.spot-radius*0.5f)<0.00002f);
  CRGBColour start=light.LightUseStartColour?light.LightStartColour:light.LightUseMidColour?light.LightMidColour:light.LightUseEndColour?light.LightEndColour:white;
  CRGBColour end=light.LightUseEndColour?light.LightEndColour:light.LightUseMidColour?light.LightMidColour:light.LightUseStartColour?light.LightStartColour:white;
  for(int i=0;i<4;++i){
   int lo=channel(start,i),hi=channel(end,i),mid=light.LightUseMidColour?channel(light.LightMidColour,i):(lo+hi)/2;
   float c=life<0.5f?channelFraction((unsigned char)lo)+(channelFraction((unsigned char)mid)-channelFraction((unsigned char)lo))*(2*life):channelFraction((unsigned char)mid)+(channelFraction((unsigned char)hi)-channelFraction((unsigned char)mid))*(2*life-1);
   if(light.LightColourFadeEnable && fade<1 && i<3){float minimum=channelFraction(channel(light.LightColourFadeMinimum,i));c*=minimum+(1.0f-minimum)*fade;if(c<minimum)c=minimum;}
   float scaled=c*255;unsigned char actual=channel(engine.colour,i);
   // The independent formula can round on the opposite side of an exact half before the byte conversion.
   assert(std::fabs(actual-scaled)<=0.5001f);
  }
 }
 std::puts("00b19c60_TEST PASS");return 0;
}