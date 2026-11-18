// CAIStateGroup_SitRoundCampfire::OnReplace -- Release the campfire seat, leave the sitting mode and choose a new idle action.
// Grade: DIFFER(569v571) -- The shared random rotation restores the native seed update before the external call. Around 009398C2 the seed pointer enters EDX where retail retains it in EDI.
// Leaf: ?OnReplace@CAIStateGroup_SitRoundCampfire@@UAEXXZ
// Retail: 0x009397E0, 571 bytes, thiscall.
#define FABLE_RETAIL_CSOUNDPAIR_ORIGINAL_CTORS
#define FABLE_RETAIL_CCREATUREACTION_GETUPFROMSITTING_SOUNDARG
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_ISDEAD
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE_PEEK
#define FABLE_RETAIL_CAIBRAIN_SETOUTPUT
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CCOMBATACTIONBASE_MELEE
#define FABLE_RETAIL_CCOMBATACTIONBASE_SOUNDCTOR
#define FABLE_RETAIL_CCOMBATACTIONBASE_INITIALISE
#define FABLE_RETAIL_CTCCREATUREMODEMANAGER
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#include "fable_game.h"
#include "fable_string.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CThingAICreature.h"
#include "engine/campfire_types.h"
#include "engine/CAIBrain.h"
#include "engine/CTCCreatureModeManager.h"
#include "engine/CWorld.h"
extern CCharString GET_UP_FROM_FLOOR_ANIM_NAME;
static const long MIN_TIME_TO_LEAVE_BETWEEN_SITTINGS=CMainGameComponent::GetConstantFPS()*10;
static const long RANGE_TIME_TO_LEAVE_BETWEEN_SITTINGS=CMainGameComponent::GetConstantFPS()*50;
#include "engine/GFROR13.h"
// Retail-asm: GFROR13 at 0x00497890 applies GF_ROR13 to its argument home; the state calls that function at 0x009398D4.
long FABLE_FASTCALL GFROR13(unsigned long value)
{
    GF_ROR13(value);
    return static_cast<long>(value);
}

#include "engine/GFRandom.h"
template<class T> inline bool FABLE_FASTCALL operator!=(const CIntelligentPointer<T>& p,int) {
 return p.GetPItem()!=0;
}
template<class T> inline bool FABLE_FASTCALL IS_THING_ALIVE(const T& thing) {
 return thing!=0 && !thing->IsDead();
}
void CAIStateGroup_SitRoundCampfire::OnReplace() {
 CTCCreatureModeManager* ptc_mode_manager;
 if(GotSeatPos && IS_THING_ALIVE(PCampfire)) {
  CTCSingleUserActionPoints* ptc_action_points;
  PCampfire->GetTC(&ptc_action_points);
  ptc_action_points->SetActionPointAsFree(SeatPosIndex);
 }
 GetCreature().GetTC(&ptc_mode_manager);
 if(Sitting || ptc_mode_manager->GetCurrentMode().GetType()==10) {
  FrameToNextSitOnSeat=PeekWorld().GetFrame()+MIN_TIME_TO_LEAVE_BETWEEN_SITTINGS+GFRandom(RANGE_TIME_TO_LEAVE_BETWEEN_SITTINGS,GetPWorldSeed());
  CBrainOutput output;
  const CCreatureActionBase& action=CCreatureAction_GetUpFromSitting(GetCreature(),GET_UP_FROM_FLOOR_ANIM_NAME,0,false);
  if(GetCreature().IsFreeToPerformAction(action)) {
   output.SetAsPerformAction(action);
   PBrain->SetOutput(output);
  }
  ptc_mode_manager->RemoveMode(10);
  Sitting=false;
 }
 CTCIdleScheduler* scheduler;
 if(GetCreature().GetTC(&scheduler))scheduler->ChooseNewIdle();
}
