// CAIStateGroup_MeleeCombatNearbyAttackWhenVeryClose::IsStillRunnable(bool) -- require a runnable melee state and an enemy within the height and radius limits.
// Leaf: ?IsStillRunnable@CMeleeCloseMethods@@QAE_N_N@Z
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Grade: RELOCATION_MATCH -- named squared-coordinate locals retain the retail x87 evaluation order (192 bytes).
// NF137 (2026-11-20, q5030): moved onto the retail engine/CAIStateGroup_MeleeCombatNearbyBase.h (_MOVE, the landed 0x0096EC10 block): the base call
//   0x0093D800 now binds the PDB protected virtual const ?IsStillRunnable@CAIStateGroup_MeleeCombatNearbyBase@@MBE_N_N@Z (Ego_r.pdb 0x009613E0 +
//   FableWin.pdb 0x02B8E5B0), was the CMeleeNearbyBaseMethods stand-in. The local C3DVector / CThing / CThingCombatGeometry / CIntelligentTargetView
//   models are gone for engine/C3DVector.h, CThing.h (GetPhysicsTC) and CTCPhysicsBase.h (RADIUS_ACCESS); the enemy is the base's PEnemy.
//   The stand-in CMeleeCloseMethods (no header; HEAD token) keeps its own leaf and now derives from the shared base (the PDB
//   ?IsStillRunnable@CAIStateGroup_MeleeCombatNearbyAttackWhenVeryClose@@MBE_N_N@Z needs that class's retail mode: NAME_FIXES).
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTCPHYSICSBASE_RADIUS_ACCESS
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CAIBRAIN_CREATURE_REFERENCE
#define FABLE_RETAIL_CAISTATEGROUP_MELEECOMBATNEARBY_MOVE
#define FABLE_RETAIL_CWORLD
#include <math.h>
#include "engine/fable_ai_action_enums.h"
#include "engine/C3DVector.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CThingAICreature.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CWorld.h"
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"
#include "engine/CAIStateGroup_MeleeCombatNearbyBase.h"

struct CMeleeCloseMethods : CAIStateGroup_MeleeCombatNearbyBase {
 bool IsStillRunnable(bool);
};
bool CMeleeCloseMethods::IsStillRunnable(bool initial) {
 if(CAIStateGroup_MeleeCombatNearbyBase::IsStillRunnable(initial)) {
  CThingCreatureBase* creature=&GetCreature();
  CThingCreatureBase* enemy=PEnemy.Get();
  const C3DVector& mine=creature->GetPos();
  C3DVector delta=enemy->GetPos()-mine;
  if(fabs(delta.Z)<2.0f) {
   CThingCreatureBase* other=PEnemy.Get();
   CThingCreatureBase* me=&GetCreature();
   float radius=(me->GetPhysicsTC()->Radius+other->GetPhysicsTC()->Radius)*1.2f+2.0f;
   float xx=delta.X*delta.X; float yy=delta.Y*delta.Y;
   if(yy+xx<radius*radius) return true;
  }
 }
 return false;
}
