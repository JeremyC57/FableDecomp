// Actual shared owner and original locals; unsupported creature/from/offset aliases removed.
// Leaf: ?Update@CAIStateGroup_MinionFindPersonalSpace@@UAEXXZ
// Flags: (none)
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_AIACTIONS
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEALACTIONS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_GET_ORIGINAL_CHAIN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_DEREFERENCE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTHINGCREATUREBASE_PERSONALSPACE_PASS24
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCCREATURENAVIGATION
#define FABLE_RETAIL_CTCCREATURENAVIGATION_NAVIGABLE
#define FABLE_RETAIL_KID_EVADE_PASS23
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_ORIGINAL_COMPONENT_CTOR
#define FABLE_RETAIL_C3DVECTOR_ORIGINAL_COPY_FORMAL
#define FABLE_RETAIL_C3DVECTOR_NORMALISE_SQUARE_HELPER
#define FABLE_RETAIL_C3DVECTOR_SQUARE_ORIGINAL_FORMAL
#define FABLE_RETAIL_C3DVECTOR_SQUARE_GROUPED_PASS23
#define FABLE_RETAIL_GFFASTONEOVERSQRT_ORIGINAL_UNION
#include "engine/CThingAICreature.h"
#include "engine/CAIStateGroup_MinionFindPersonalSpace.h"
#include "engine/CAIBrain.h"
#include "engine/CTCCreatureNavigation.h"
void CAIStateGroup_MinionFindPersonalSpace::Update()
{
 if(IsSpaceInvader(*PThingCreature)) {
  float space_radius=GetCreature().GetPersonalSpaceRadius();
  C3DVector away_dir=GetCreature().PeekPos()-PThingCreature->PeekPos();
  away_dir.Normalise();
  C3DVector move_to_pos=GetCreature().PeekPos()+away_dir*space_radius;
  C3DVector wander_pos;
  CTCCreatureNavigation* p_navigation;
  GetCreature().GetTC(&p_navigation);
  if(p_navigation->GetNearestFleePosition(move_to_pos,.5f,wander_pos)) {
   bool nav_route_calculated;
   AIAction_MoveToPos((EAIActionRepeat)1,wander_pos,nav_route_calculated,(EMovementType)3,true,true,0,true,0.0f);
  }
 }
 Terminate();
}