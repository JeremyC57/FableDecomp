// CScriptGameResourceObjectScriptedThing::FollowPreCalculatedRoute -- queue a route action, optionally waiting for completion.
// Grade: DIFFER(254v254) -- route and movement values use exchanged saved CPU slots.
// Leaf: ?FollowPreCalculatedRoute@CScriptGameResourceObjectScriptedThing@@UAEXABVCScriptThing@@W4EScriptEntityMoveType@@_N2@Z
// Retail: 00903f30,254 bytes,thiscall.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_ORIGINAL_CHAIN
#define FABLE_RETAIL_CVECTORMAP_ORIGINAL_FIND
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_GET_ORIGINAL_CHAIN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHING
#define FABLE_RETAIL_SCRIPTEDTHING_FOLLOW_ROUTE
#define FABLE_RETAIL_SCRIPT_MOVE_ENUM
#define FABLE_RETAIL_CTCSCRIPTEDCONTROL_ACTIONBASE
#define FABLE_RETAIL_CTCSCRIPTEDCONTROL_ADD_ACTION
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CScriptGameResourceObjectScriptedThing.h"
#include "engine/NTCScriptedControl_CActionMoveTo.h"
#include "engine/ThingAlive.h"
inline EMovementType FABLE_FASTCALL GFConvertScriptMoveTypeToAIMoveType(EScriptEntityMoveType move_type)
{
 switch(move_type){
 case ENTITY_MOVE_WALK:return AI_MOVEMENT_WALKING;
 case ENTITY_MOVE_RUN:return AI_MOVEMENT_RUNNING;
 default:return AI_MOVEMENT_SNEAKING;
 }
}
void CScriptGameResourceObjectScriptedThing::FollowPreCalculatedRoute(const CScriptThing& route,EScriptEntityMoveType move_type,bool wait,bool avoid_dynamic_obstacles)
{
 if(IS_THING_ALIVE(PThing)&&route.IsAlive()){
  const CThing* proute=route.GetPThing();
  const bool& is_route=proute->HasTC(TCI_PRE_CALCULATED_NAVIGATION_ROUTE);
  if(is_route){
   EMovementType ai_move_type=GFConvertScriptMoveTypeToAIMoveType(move_type);
   CTCScriptedControl* ptc_script_control;
   PThing->GetTC(&ptc_script_control);
   ptc_script_control->AddAction(new NTCScriptedControl::CActionMoveTo(*proute,ai_move_type,true,avoid_dynamic_obstacles));
   if(wait)WaitWhilePerformingTasks();
  }
 }
}
