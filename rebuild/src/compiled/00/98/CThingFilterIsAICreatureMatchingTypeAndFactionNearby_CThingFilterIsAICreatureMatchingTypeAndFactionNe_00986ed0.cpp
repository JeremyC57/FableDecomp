// CThingFilter_IsAICreatureMatchingTypeAndFactionNearby::CThingFilter_IsAICreatureMatchingTypeAndFactionNearby(const CThingAICreature& creature, float max_dist, bool restrict_angle) -- capture the search criteria: the creature's position, type and faction, the squared range and, when the angle is restricted, the creature's side vector (flipped for a child) and cos(45 degrees)
// Leaf: ??0CThingFilter_IsAICreatureMatchingTypeAndFactionNearby@@QAE@ABVCThingAICreature@@M_N@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00986ED0, 353 bytes, thiscall `ret 0xc`.
// Identity: Ego_r.pdb / FableWin.pdb (the leaf above); Ego_d opinion-reaction TU: parameters creature / max_dist / restrict_angle,
//   ptc_enemy (const CTCEnemy*) at function scope, ptc_physics (const CTCPhysicsBase*) inside the restrict_angle block.
// Shape: the TC lookups are the template PeekTC(&ptc) with FableWin's PeekTC text (FABLE_RETAIL_CTHING_PEEKTC_ORIGINAL_CHAIN: the
//   const GetTC(id) reference, then the checked cast; Find / LowerBound 0x004365B0 expanded, key in the restrict_angle slot);
//   MinCosine is the shared inline GFCos(0.125f) (both PDBs; cos(angle * (double)6.2831855f) folds to the retail double
//   0.78539818525314331 at 0x01261E50, then fcos); GetRight is Forward ^ Up with the grouped cross text (CROSS_XYZ_GROUP). Those
//   inline layers fix the x87 operand order of the three cross products (0x00986F9A..0x00986FCA) the draft's direct
//   PeekTC(id, &ptc) / cos() spelling could not.
// Callees: CThing::GetPos 0x004C73D0, CThingCreatureBase::GetCreatureType 0x00662000, CVectorMap::LowerBound const 0x004365B0,
//   CTCPhysicsBase::GetRHSet (virtual slot +0x120).
#define FABLE_RETAIL_C3DVECTOR_CROSS_XYZ_GROUP
#define FABLE_RETAIL_C3DVECTOR_PACKED
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CRIGHTHANDEDSET
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_PEEKTC_ORIGINAL_CHAIN
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGCREATUREBASE_CREATURETYPE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTCPHYSICSBASE_STATTYPE
#define FABLE_RETAIL_CTCENEMY_QUERIES
#include "engine/CThingAICreature.h"
#include "engine/CTCEnemy.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CRightHandedSet.h"
#include "engine/CThingFilter_IsAICreatureMatchingTypeAndFactionNearby.h"
#include "engine/GFCos.h"
CThingFilter_IsAICreatureMatchingTypeAndFactionNearby::CThingFilter_IsAICreatureMatchingTypeAndFactionNearby(const CThingAICreature& creature,float max_dist,bool restrict_angle)
{
 MinCosine=0.0f;
 RestrictAngle=restrict_angle;
 OurPos=creature.GetPos();
 MaxSqDist=max_dist*max_dist;
 OurCreatureType=creature.GetCreatureType();
 const CTCEnemy* ptc_enemy;
 creature.PeekTC(&ptc_enemy);
 PFaction=ptc_enemy->GetPFaction();
 if(restrict_angle) {
  const CTCPhysicsBase* ptc_physics;
  creature.PeekTC(&ptc_physics);
  SideVec=ptc_physics->GetRHSet().GetRight();
  if(OurCreatureType==HUMAN_CHILD) SideVec=-SideVec;
  MinCosine=GFCos(0.125f);
 }
}