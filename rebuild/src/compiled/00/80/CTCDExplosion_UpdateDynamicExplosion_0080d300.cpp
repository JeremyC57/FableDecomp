// 0080d300 CTCDExplosion::UpdateDynamicExplosion
#define FABLE_RETAIL_DYNAMIC_EXPLOSION_PASS29
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCBASE_COMPONENT
#define FABLE_RETAIL_CTCBASE_GETPARENTTHING
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_GET_ORIGINAL_CHAIN
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLDMAP_CYLINDER_QUERY
#define FABLE_RETAIL_CTCDPARTICLEEMITTER_METHODS_ONLY
#define FABLE_RETAIL_CEXPLOSIONDEF_MEMBERS
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_STLP_VECTOR_OVERFLOW_DECL_ONLY
#define FABLE_RETAIL_STLP_VECTOR_INSERT_OUTOFLINE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY_ORIGINAL_CHAIN
// Leaf: ?UpdateDynamicExplosion@CTCDExplosion@@AAEXXZ
// Flags: /O2 /Gr
#include "engine/C3DVector.h"
#include "engine/CThing.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CTCDExplosion.h"
#include "engine/CTCDParticleEmitter.h"
#include "engine/CMainGameComponent.h"
#include "engine/CWorld.h"
#include "engine/CWorldMap.h"
#define FABLE_RETAIL_MATH_TRIG_TWO_PI
#include "engine/GFSin.h"
#include "engine/GFCos.h"
#include "engine/ThingAlive.h"
extern const float DELTA_NOTIONAL_ZERO;
namespace {
float FABLE_FASTCALL GetExplosionEffectInterpolate(const CExplosionDef& def, float radius)
{
    float result = 0.0f;
    if (def.ExplosionRadiusLarge - def.ExplosionRadiusSmall > DELTA_NOTIONAL_ZERO) {
        if (radius < def.ExplosionRadiusSmall)
            radius = def.ExplosionRadiusSmall;
        else if (radius > def.ExplosionRadiusLarge)
            radius = def.ExplosionRadiusLarge;
        result = (radius - def.ExplosionRadiusSmall) / (def.ExplosionRadiusLarge - def.ExplosionRadiusSmall); if(result<0.0f)result=0.0f; else if(result>1.0f)result=1.0f;
    }
    return result;
}
}

#ifdef _MSC_VER
// Both STLport owner copies at 00788d88 and 00789515 come from a size-optimized TU.
#pragma optimize("s",on)
#endif
namespace std {
template<> inline pair<CIntelligentPointer<CThing>,long>::pair(const pair<CIntelligentPointer<CThing>,long>& p):first(p.first),second(p.second){}
}
namespace std {
template<> inline void vector<pair<CIntelligentPointer<CThing>,long> >::push_back(const pair<CIntelligentPointer<CThing>,long>& x) {
 if(this->_M_finish!=this->_M_end_of_storage._M_data){new (this->_M_finish) pair<CIntelligentPointer<CThing>,long>(x);++this->_M_finish;}
 else _M_insert_overflow(this->_M_finish,x,_IsPODType(),1,true);
}
}
#ifdef _MSC_VER
#pragma optimize("t",on)
#endif
void CTCDExplosion::UpdateDynamicExplosion()
{
 float wanted_rad=DamageRadius;
 CWorldMap& wmap=GetWorld().GetWorldMap();
 const long const_fps=CMainGameComponent::GetConstantFPS();
 ++ExplosionEvolutionTimer;
 long ring_number=0,next_time=0;
 if(false) {} else for (;ring_number<PExplosionDef->ExplosionRings.size()&&next_time<ExplosionEvolutionTimer;++ring_number) {
  const CExplosionRing& explosion_ring=PExplosionDef->ExplosionRings[ring_number];
  long frames_for_ring=(long)(const_fps*explosion_ring.SecondsBeforeSpawnNewRing);
  next_time+=frames_for_ring;
 }
 --ring_number;
 if(ring_number!=CurrentExplosionRing){
  CurrentExplosionRing=ring_number;CreatedEmitter=true;
  C3DVector line_x(1,0,0),line_y(0,1,0);
  long num_explosions_on_ring=PExplosionDef->ExplosionRings[ring_number].NumExplosions;
  float ring_radius=PExplosionDef->ExplosionRings[ring_number].RingRadius;
  float theta=PExplosionDef->ExplosionRings[ring_number].AngleOffset;
  float dtheta=1.0f/num_explosions_on_ring;
  if(false) {} else for(long c2=0;c2<num_explosions_on_ring;++c2){
   C3DVector pos=GetParentThing().GetPos()+(line_x*(float)GFSin(theta)+line_y*(float)GFCos(theta))*ring_radius;
   pos.Z=wmap.GetHeightZBelowPos(pos);
   float interp=GetExplosionEffectInterpolate(*PExplosionDef,wanted_rad);
   CThing* peffect=CTCDParticleEmitter::Create(PExplosionDef->ExplosionBlendSmall,PExplosionDef->ExplosionBlendLarge,interp,pos,false);
   if(IS_THING_ALIVE(peffect))Emitters.push_back(std::make_pair(CIntelligentPointer<CThing>(peffect),GetWorld().GetFrame()));
   theta+=dtheta;
  }
  float inner_rad=ring_number>0?wanted_rad+PExplosionDef->ExplosionRings[ring_number-1].RingRadius:0.0f;
  float outer_rad=ring_radius+wanted_rad;UpdateDynamicHitThings(inner_rad,outer_rad);
 }
 if(PExplosionDef->ExplosionReplacementBlendLarge>0&&PExplosionDef->ExplosionReplacementBlendSmall>0){
  if(false) {} else for(std::vector<std::pair<CIntelligentPointer<CThing>,long> >::iterator it=Emitters.begin();it!=Emitters.end();){
   CThing* pold_effect_thing=it->first.Get();long frame_created=it->second;bool moved_on=false;
   if(IS_THING_ALIVE(pold_effect_thing)&&frame_created>0){
    long frame_for_replacement=(long)(const_fps*PExplosionDef->SecondsBeforeEffectReplacement+frame_created);
    if(GetWorld().GetFrame()>frame_for_replacement){
     it->second=-1;
     float interp=GetExplosionEffectInterpolate(*PExplosionDef,wanted_rad);
     CThing* pnew_effect_thing=CTCDParticleEmitter::Create(PExplosionDef->ExplosionReplacementBlendSmall,PExplosionDef->ExplosionReplacementBlendLarge,interp,pold_effect_thing->GetPos(),false);
     if(IS_THING_ALIVE(pnew_effect_thing)){it=Emitters.insert(Emitters.end(),std::make_pair(CIntelligentPointer<CThing>(pnew_effect_thing),-1));moved_on=true;}
    }
   }
   if(!moved_on)++it;
  }
 }
}