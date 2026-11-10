#include <cstdio>
#include <functional>
namespace NEntityEvents {
class CEventBase {public:long WorldFrame;};
class CEventHitBase:public CEventBase {public:bool Shot,Blocked;};
class CEventHitBy:public CEventHitBase {};
class CEventHitBySpecialAbility:public CEventBase {};
class CIsEventCreatedAfter {public:explicit CIsEventCreatedAfter(long frame):WorldFrame(frame){} bool operator()(const CEventBase* event)const{return event->WorldFrame>WorldFrame;}private:long WorldFrame;};
class CIsHitEventHitByShot {public:CIsHitEventHitByShot(){} bool operator()(const CEventHitBase* event)const{return event->Shot;}};
class CIsHitEventHitByMelee {public:CIsHitEventHitByMelee(){} bool operator()(const CEventHitBase* event)const{return !event->Shot;}};
class CIsHitEventBlocked:public std::unary_function<const CEventHitBase*,bool> {public:CIsHitEventBlocked(){} bool operator()(const CEventHitBase* event)const{return event->Blocked;}};
}
template<class A,class B,class C>class CFilter_Bind3 {protected:const A& F1;const B& F2;const C& F3;public:CFilter_Bind3(const A& a,const B& b,const C& c):F1(a),F2(b),F3(c){}template<class T>bool operator()(const T& t)const{return F1(t)&&F2(t)&&F3(t);}};
template<class A,class B,class C>inline CFilter_Bind3<A,B,C> GFFilter_Bind3(const A& a,const B& b,const C& c){return CFilter_Bind3<A,B,C>(a,b,c);}
class CTCEntityEvents {public:NEntityEvents::CEventHitBy Hits[2];NEntityEvents::CEventHitBySpecialAbility Special;mutable int Calls;CTCEntityEvents():Calls(0){}template<class F>bool GetEvent(NEntityEvents::CEventHitBy** out,const F& filter)const{++Calls;for(int i=1;i>=0;--i)if(filter(&Hits[i])){*out=const_cast<NEntityEvents::CEventHitBy*>(&Hits[i]);return true;}return false;}template<class F>bool GetEvent(NEntityEvents::CEventHitBySpecialAbility** out,const F& filter)const{++Calls;if(filter(&Special)){*out=const_cast<NEntityEvents::CEventHitBySpecialAbility*>(&Special);return true;}return false;} };
class CThingAICreature {public:const CTCEntityEvents* Events;template<class T>bool PeekTC(T** out)const{if(!Events)return false;*out=Events;return true;}};
class CAIStateGroup_WhisperRetreat {public:CThingAICreature Creature;bool bIsRangeAttack;long FrameOfLastHit;CThingAICreature& GetCreature(){return Creature;}bool FindHitEvent();};
bool CAIStateGroup_WhisperRetreat::FindHitEvent()
{
 using namespace NEntityEvents;
 bIsRangeAttack=false;
 const CTCEntityEvents* pEntityEventTC;
 if(GetCreature().PeekTC(&pEntityEventTC)){
  {
  CEventHitBy* phit_event;
  if(pEntityEventTC->GetEvent(&phit_event,GFFilter_Bind3(CIsHitEventHitByShot(),std::not1(CIsHitEventBlocked()),CIsEventCreatedAfter(FrameOfLastHit)))){
   bIsRangeAttack=true;
   return true;
  }else if(pEntityEventTC->GetEvent(&phit_event,GFFilter_Bind3(CIsHitEventHitByMelee(),std::not1(CIsHitEventBlocked()),CIsEventCreatedAfter(FrameOfLastHit)))){
   return true;
  }
  }
  {
  CEventHitBySpecialAbility* pwill_hit_event;
  if(pEntityEventTC->GetEvent(&pwill_hit_event,CIsEventCreatedAfter(FrameOfLastHit))){
   return true;
  }else return false;
  }
 }
 return false;
}

int main(){int ok=1,cases=0;long frames[3]={9,10,11};for(int present=0;present<2;++present)for(int ranged=0;ranged<3;++ranged)for(int melee=0;melee<3;++melee)for(int special=0;special<3;++special)for(int blocked=0;blocked<4;++blocked)for(int oldflag=0;oldflag<2;++oldflag){CTCEntityEvents events;events.Hits[0].Shot=true;events.Hits[0].Blocked=(blocked&1)!=0;events.Hits[0].WorldFrame=frames[ranged];events.Hits[1].Shot=false;events.Hits[1].Blocked=(blocked&2)!=0;events.Hits[1].WorldFrame=frames[melee];events.Special.WorldFrame=frames[special];CAIStateGroup_WhisperRetreat group;group.Creature.Events=present?&events:0;group.FrameOfLastHit=10;group.bIsRangeAttack=oldflag!=0;bool hitR=present&&ranged==2&&!(blocked&1),hitM=present&&melee==2&&!(blocked&2),hitS=present&&special==2;bool result=group.FindHitEvent();ok &= result==(hitR||hitM||hitS)&&group.bIsRangeAttack==hitR;ok &= events.Calls==(!present?0:hitR?1:hitM?2:3)&&group.FrameOfLastHit==10;ok &= events.Hits[0].WorldFrame==frames[ranged]&&events.Hits[1].WorldFrame==frames[melee]&&events.Special.WorldFrame==frames[special];++cases;}ok &= cases==432;std::printf(ok?"0096a260_TEST PASS\n":"0096a260_TEST FAIL\n");return ok?0:1;}