// NTCScriptedControl::CActionPlayCombatAnimation::Update() -- start combat animation, preserve movement, then wait or finish
// Leaf: ?Update@CActionPlayCombatAnimation@NTCScriptedControl@@UAEXXZ
// Retail: 0x00712A80,216 bytes,virtual thiscall
#define FABLE_RETAIL_CSOUNDPAIR_ORIGINAL_CTORS
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCSCRIPTEDCONTROL_ACTIONBASE
#define FABLE_RETAIL_CTCSCRIPTEDCONTROL_CREATURE
#define FABLE_RETAIL_CCREATUREACTION_PLAYCOMBATANIMATION
#define FABLE_RETAIL_CCOMBATACTIONBASE_MELEE
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGCREATUREBASE_ACTIONQUERY
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_STLP_LIST
#include "fable_string.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CThingCreatureBase.h"
#include "engine/NTCScriptedControl_CActionPlayCombatAnimation.h"
#include "engine/CSoundPair.h"
#include "engine/CCreatureAction_PlayCombatAnimationWithLooking.h"
inline CThingCreatureBase& CTCScriptedControl::GetParentCreature() { CThingCreatureBase& t=static_cast<CThingCreatureBase&>(GetParentThing()); return t; }
inline CThingCreatureBase& CTCScriptedControl::CActionBase::GetParentCreature() { return PTCScript->GetParentCreature(); }
inline void CTCScriptedControl::CActionBase::Terminate() { Terminated=true; }
namespace NTCScriptedControl {
void CActionPlayCombatAnimation::Update(){
 CCreatureAction_PlayCombatAnimation* action=0;
 if(!PlayedAnim){
  if(AllowLooking)action=new CCreatureAction_PlayCombatAnimationWithLooking(GetParentCreature(),Animation,UsePhysics);
  else action=new CCreatureAction_PlayCombatAnimation(GetParentCreature(),Animation,UsePhysics);
  action->SetAlwaysUseMovementInAnim(true);
  if(AddAsQueuedAction){
   if(GetParentCreature().IsPerformingAnyAction()){
    GetParentCreature().AddQueuedAction(*action);PlayedAnim=true;
   }else PlayedAnim=GetParentCreature().SetCurrentAction(*action);
  }else PlayedAnim=GetParentCreature().SetCurrentAction(*action);
  if(!WaitForAnimationToFinish)Terminate();
 }else {
  if(!GetParentCreature().IsPerformingAnyAction())Terminate();
 }
 if(action)delete action;
}
}