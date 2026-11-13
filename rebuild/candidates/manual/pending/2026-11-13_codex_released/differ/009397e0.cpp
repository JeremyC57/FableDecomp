// CAIStateGroup_SitRoundCampfire::OnReplace -- release the seat, delay sitting again and request a get-up action.
// Inline-budget: Plain and size-budget builds020/021 expand the rotate call;022 retains it with a scoped limit.
// Grade: DIFFER(569v571) -- the pre-call world-seed store is absent.
// Retail: 0x009397e0,571 bytes,virtual thiscall.
// Inline-budget note: plain and size-budget trials020/021 expand rotate unlike native; scope depth around actual random helper.
// Leaf: ?OnReplace@CAIStateGroup_SitRoundCampfire@@UAEXXZ
// Flags: none
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
inline long FABLE_FASTCALL GFROR13(unsigned long x) {
 unsigned int bits=static_cast<unsigned int>(x);
 return static_cast<long>(static_cast<int>((bits >> 13) | (bits << 19)));
}
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
inline unsigned long FABLE_FASTCALL GFRandom(unsigned long range,unsigned long* seed) {
 *seed=*seed*0x24a1U+0x24dfU;
 *seed=GFROR13(*seed);
 return range ? *seed%range : 0;
}
#ifdef _MSC_VER
#pragma inline_depth()
#endif
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
