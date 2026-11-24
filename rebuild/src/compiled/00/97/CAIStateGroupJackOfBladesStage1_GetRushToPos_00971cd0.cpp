// Original locals, actual shared owner and original operand/evaluation order.
// Leaf: ?GetRushToPos@CAIStateGroup_JackOfBladesStage1@@IAE_NAAVC3DVector@@@Z
// Flags: (none)
#define FABLE_RETAIL_JACK_STAGE1_GETRUSHTOPOS_PASS26
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUP_JACKOFBLADESBASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_GET_ORIGINAL_CHAIN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_ANGLE_BODIES
#define FABLE_RETAIL_C3DVECTOR_MAGNITUDE2D
#define FABLE_RETAIL_C3DVECTOR_MAGNITUDE2D_ORIGINAL_SUM_PASS23
#define FABLE_RETAIL_C3DVECTOR_ORIGINAL_COMPONENT_CTOR
#define FABLE_RETAIL_C3DVECTOR_ORIGINAL_COPY_FORMAL
#include "engine/CThingAICreature.h"
#include "engine/CAIStateGroup_JackOfBladesStage1.h"
#include "engine/CAIBrain.h"
#include "engine/ThingAlive.h"
#pragma intrinsic(atan2,fmod,sqrt)
bool CAIStateGroup_JackOfBladesStage1::GetRushToPos(C3DVector& rush_to_pos)
{
 C3DVector centre_pos;
 if(IS_THING_ALIVE(PArenaCentre)) centre_pos=PArenaCentre->GetPos();
 else centre_pos=GetCreature().GetPos();
 if(IS_THING_ALIVE(PTargetThing)) {
  long count=0;
  C3DVector vector_to_target=PTargetThing->GetPos()-GetCreature().GetPos();
  if(vector_to_target.GetMagnitude2D()>8.0f*2.0f) {
   float angle_to_target=vector_to_target.GetAngleXY();
   C3DVector zig_vector(.5f,1.0f,0.0f),zag_vector(-.5f,1.0f,0.0f);
   C3DVector move_vector;
   if(ZigZagCounter%2)move_vector=zig_vector;else move_vector=zag_vector;
   move_vector.RotateXY(angle_to_target);
   move_vector*=8.0f;
   rush_to_pos=GetCreature().GetPos()+move_vector;
   return true;
  }
  return false;
 }
 return false;
}