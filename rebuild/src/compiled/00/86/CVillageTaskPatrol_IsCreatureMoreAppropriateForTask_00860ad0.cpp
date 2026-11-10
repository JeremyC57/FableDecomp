// CVillageTaskPatrol::IsCreatureMoreAppropriateForTask -- compare creature suitability for a patrol task.
// Retail: 0x00860AD0, 302 bytes; thiscall.
// Grade: VERIFIED_MATCH; see ROW.json and PROBES.md for verification or remaining residue.
// Leaf: ?IsCreatureMoreAppropriateForTask@CVillageTaskPatrol@@QBE_NABVCThing@@@Z
// Flags: none
#define FABLE_RETAIL_CTHINGTRACKNODE_PEEK_NEXT
#define FABLE_RETAIL_CVILLAGETASKPATROL_GET_START
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CVILLAGETASKPATROL
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGTRACKNODE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_GET_ORIGINAL_CHAIN
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_NORMALISE_SQUARE_HELPER
#define FABLE_RETAIL_C3DVECTOR_SQUAREDMAGNITUDE_XYZ
#include "engine/CVillageTaskPatrol.h"
#include "engine/CThingTrackNode.h"
// Original body: FableWin029bece0 and native00860a50; read-only position input.
#ifdef _MSC_VER
#pragma auto_inline(off)
#endif
const CThingTrackNode* CVillageTaskPatrol::GetNearestNode(const C3DVector& to_pos)const
{
 const CThingTrackNode* ret=0;
 const CThingTrackNode* cur_node=GetPatrolStart();
 if(cur_node){
  float best_distance_sq=3.402823466e+38f;
  do{
   float cur_distance_sq=(cur_node->GetPos()-to_pos).GetSquaredMagnitude();
   if(cur_distance_sq<best_distance_sq){best_distance_sq=cur_distance_sq;ret=cur_node;}
   cur_node=cur_node->PeekNextNode();
  }while(cur_node&&!cur_node->IsTrackEnd());
 }
 return ret;
}
#ifdef _MSC_VER
#pragma auto_inline(on)
#endif
bool CVillageTaskPatrol::IsCreatureMoreAppropriateForTask(const CThing& creature)const
{
 if(TaskLocked)return false;
 const float FACTOR_HAVE_TO_BE_NEARER_BY=1.25f;
 C3DVector current_creature_pos=CreatureAssignedToTask->GetPos();
 C3DVector new_creature_pos=creature.GetPos();
 const CThingTrackNode* pnode=GetNearestNode(current_creature_pos);
 C3DVector current_creature_nearest_node_pos=pnode->GetPos();
 pnode=GetNearestNode(new_creature_pos);
 C3DVector new_creature_nearest_node_pos=pnode->GetPos();
 float dist_from_current_creature_to_patrol_start_sq=(current_creature_nearest_node_pos-current_creature_pos).GetSquaredMagnitude();
 float dist_from_new_creature_to_patrol_start_sq=(new_creature_nearest_node_pos-new_creature_pos).GetSquaredMagnitude();
 return (dist_from_new_creature_to_patrol_start_sq*GFSqr(FACTOR_HAVE_TO_BE_NEARER_BY))<dist_from_current_creature_to_patrol_start_sq;
}