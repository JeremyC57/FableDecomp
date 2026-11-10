// CCreatureAction_TeleportOut::FinishAction -- finish teleporting and queue the arrival action.
// Retail: 0x008BD3F0, 312 bytes; thiscall.
// Grade: VERIFIED_MATCH; see ROW.json and PROBES.md for verification or remaining residue.
// Leaf: ?FinishAction@CCreatureAction_TeleportOut@@UAEXXZ
// Flags: none
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CCREATUREACTIONBASE_FULL_MEMBERS
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCOMBATACTIONBASE_MELEE
#define FABLE_RETAIL_CCOMBATACTIONBASE_FINISHACTION
#define FABLE_RETAIL_CCREATUREACTION_TELEPORT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_ANGLE_BODIES
#define FABLE_RETAIL_IS_THING_ALIVE
#include "engine/C3DVector.h"
#include "engine/CDefPointer.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CCombatActionBase.h"
#include "engine/CCreatureAction_TeleportOut.h"
#include "engine/CCreatureAction_TeleportIn.h"
#include "engine/ThingAlive.h"
void CCreatureAction_TeleportOut::FinishAction()
{
 PCreature->GetPhysicsTC()->ResetPosition(TeleportPos);
 if(IS_THING_ALIVE(PTarget)) {
  C3DVector vector_to_target=PTarget->GetPos()-PCreature->GetPos();
  PCreature->GetPhysicsTC()->SetFacingAngleXY(vector_to_target.GetAngleXY());
 }
 CThingCreatureBase* pcreature=PCreature.Get();
 CCombatActionBase::FinishAction();
 pcreature->SetCurrentAction(CCreatureAction_TeleportIn(*pcreature,CreateEffects));
}