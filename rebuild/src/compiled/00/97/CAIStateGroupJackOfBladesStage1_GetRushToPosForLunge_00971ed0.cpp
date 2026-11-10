// CAIStateGroup_JackOfBladesStage1::GetRushToPosForLunge -- find a clear rush destination for the lunge.
// Retail: 0x00971ED0, 359 bytes; thiscall.
// Grade: VERIFIED_MATCH; see ROW.json and PROBES.md for verification or remaining residue.
// Leaf: ?GetRushToPosForLunge@CAIStateGroup_JackOfBladesStage1@@IAE_NAAVC3DVector@@@Z
// Flags: none
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GOTOBED_WORLD
#define FABLE_RETAIL_CAISTATEGROUP_JACKOFBLADESBASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_GET_ORIGINAL_CHAIN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_STLP_REMOVE_COPY_IF
#define FABLE_RETAIL_STLP_NOT1
#include "engine/CThingCreatureBase.h"
#include "engine/CAIStateGroup_JackOfBladesStage1.h"
#include "engine/CWorld.h"
#include "engine/CVectorFilter_FurtherFromPointThan.h"
#include "engine/NCreatureHiding.h"
#include "engine/ThingAlive.h"
#include "engine/GFRandom.h"
#include "stlport/position_algorithms.h"
bool CAIStateGroup_JackOfBladesStage1::GetRushToPosForLunge(C3DVector& rush_to_pos)
{
 if(IS_THING_ALIVE(PTargetThing)){
  std::vector<C3DVector> validPositions;
  NCreatureHiding::GetValidPositions(validPositions,GetWorld().GetWorldMap(),GetWorld().PeekThingSearchTools(),PTargetThing->GetPos(),11.0f,false,0);
  if(validPositions.size()==0)return false;
  std::vector<C3DVector>::iterator it=std::remove_if(validPositions.begin(),validPositions.end(),std::not1(CVectorFilter_FurtherFromPointThan(PTargetThing->GetPos(),10.0f)));
  rush_to_pos=validPositions[GFRandom(std::distance(validPositions.begin(),it),GetWorld().GetWorldSeed())];
  return true;
 }
 return false;
}