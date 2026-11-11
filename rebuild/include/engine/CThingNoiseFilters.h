#ifndef FABLE_ENGINE_CTHINGNOISEFILTERS_H
#define FABLE_ENGINE_CTHINGNOISEFILTERS_H
#include "stlport/fable_containers.h"
#include "engine/C3DVector.h"
#include "engine/ThingAlive.h"
#include "engine/CTCNoise.h"
class CThing;
// Layout: complete original Ego23ef4/FW25f3a,size4, genuine unary-function base.
class CThingFilter_NoiseCameFromThing:public std::unary_function<CThing,bool>{
 const CThing&Thing;
public:CThingFilter_NoiseCameFromThing(const CThing&thing):Thing(thing){}
 bool operator()(const CThing&thing)const{const CTCNoise*ptc_noise=0;return thing.PeekTC(&ptc_noise)&&ptc_noise->GetCreator()==&Thing;}
};
// Layout: complete original Ego23f00/FW25f46,size16.
class CThingFilter_NoiseCanBeHeardFromPosition:public std::unary_function<CThing,bool>{
 C3DVector Position;float ListenersHearingRange;
public:CThingFilter_NoiseCanBeHeardFromPosition(const CThingFilter_NoiseCanBeHeardFromPosition&);
 CThingFilter_NoiseCanBeHeardFromPosition(const C3DVector&pos,float range):Position(pos),ListenersHearingRange(range){}
 bool operator()(const CThing&thing)const{const CTCNoise*ptc_noise;if(thing.PeekTC(&ptc_noise)){float distance_squared=(thing.GetPos()-Position).GetSquaredMagnitude();float noise_range=ptc_noise->GetDistanceCanBeHeardFrom();float range=std::min(noise_range,ListenersHearingRange);return distance_squared<=range*range;}return false;}
};
// Layout: complete original Ego38abb/FW3cbcd,size12; Pos is genuinely unused in this comparator.
class CThingCompare_GetMostRecentlyCreatedThing{
 C3DVector Pos;
public:CThingCompare_GetMostRecentlyCreatedThing(){}
 CThingCompare_GetMostRecentlyCreatedThing(const CThingCompare_GetMostRecentlyCreatedThing&);
 template<class T>T*operator()(T*first,T*second)const{
  if(IS_THING_DEAD(first)&&second)return second;
  if(IS_THING_DEAD(second))return first;
  return first->GetWorldFrameCreatedAt()>second->GetWorldFrameCreatedAt()?first:second;
 }
};
FABLE_STATIC_ASSERT(sizeof(CThingCompare_GetMostRecentlyCreatedThing)==12);
#endif
