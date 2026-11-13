// CThingFilter_IsAICreatureMatchingTypeAndFactionNearby::CThingFilter_IsAICreatureMatchingTypeAndFactionNearby(const CThingAICreature&,float,bool) -- capture the creature search criteria.
// Grade: DIFFER(353v353) -- three commutative cross-product multiply operand pairs; correct all field offsets and copy schedule
// Retail: 0x00986ED0, 353 bytes; thiscall.
#define FABLE_RETAIL_C3DVECTOR_PACKED
// Leaf: ??0CThingFilter_IsAICreatureMatchingTypeAndFactionNearby@@QAE@ABVCThingAICreature@@M_N@Z
// Flags: none
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CRIGHTHANDEDSET
#define FABLE_RETAIL_CRIGHTHANDEDSET_RIGHT_COMPONENTS
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGCREATUREBASE_CREATURETYPE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTCENEMY_FACTION
#include "engine/CThingAICreature.h"
#include "engine/CTCEnemy.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CRightHandedSet.h"
#include "engine/CThingFilter_IsAICreatureMatchingTypeAndFactionNearby.h"
#include <math.h>
#ifdef _MSC_VER
#pragma intrinsic(cos)
#endif
CThingFilter_IsAICreatureMatchingTypeAndFactionNearby::CThingFilter_IsAICreatureMatchingTypeAndFactionNearby(const CThingAICreature& creature,float max_dist,bool restrict_angle)
{
 MinCosine=0.0f;
 RestrictAngle=restrict_angle;
 OurPos=creature.GetPos();
 MaxSqDist=max_dist*max_dist;
 OurCreatureType=creature.GetCreatureType();
 const CTCEnemy* ptc_enemy;
 creature.PeekTC(TCI_ENEMY,&ptc_enemy);
 PFaction=ptc_enemy->GetPFaction();
 if(restrict_angle) {
  const CTCPhysicsBase* ptc_physics;
  creature.PeekTC(TCI_PHYSICS,&ptc_physics);
  SideVec=ptc_physics->GetRHSet().GetRight();
  if(OurCreatureType==HUMAN_CHILD) SideVec=-SideVec;
  MinCosine=(float)cos(0.78539818525314331);
 }
}
