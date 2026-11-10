// 0096a260 CAIStateGroup_WhisperRetreat::FindHitEvent
// Leaf: ?FindHitEvent@CAIStateGroup_WhisperRetreat@@IAE_NXZ
// Flags: none
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CTCCREATUREMODEMANAGER
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CAIBRAIN_CREATURE_REFERENCE
#define FABLE_RETAIL_CAISTATEGROUP_ENEMYCREATUREBASE
#define FABLE_RETAIL_CAISTATEGROUP_WHISPERRETREAT
#define FABLE_RETAIL_CTCENTITYEVENTS
#define FABLE_RETAIL_STLP_REMOVE_COPY_IF
#define FABLE_RETAIL_STLP_NOT1
#define FABLE_RETAIL_FILTER_BIND3_FUNCTION
#include "fable_string.h"
#include "engine/CThingAICreature.h"
#include "engine/CAIStateGroup_WhisperRetreat.h"
#include "engine/CAIBrain.h"
#include "engine/CTCEntityEvents.h"
#include "engine/CFilter_Bind3.h"
#include "engine/NEntityEvents_EventFilters.h"
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