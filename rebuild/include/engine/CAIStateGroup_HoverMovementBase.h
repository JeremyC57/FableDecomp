#ifndef FABLE_ENGINE_CAISTATEGROUP_HOVERMOVEMENTBASE_H
#define FABLE_ENGINE_CAISTATEGROUP_HOVERMOVEMENTBASE_H
// New header (chunk Q17, row 0x0095B820). RTTI .?AVCAIStateGroup_HoverMovementBase@@ : CAIStateGroup_EnemyCreatureBase (no own data:
// the derived CAIStateGroup_HoverHornetQueen's first Ego_r.pdb member is at +0x1C, right after PTargetThing). Member functions: the
// Ego_r.pdb name set (protected AIAction_MoveToAndFaceThingAtDistance / AIAction_MoveToPosProximityAndFacePos, protected virtual
// AIAction_MoveToPosAndFacePos), declarations only. Needs FABLE_RETAIL_CAISTATEGROUP_ENEMYCREATUREBASE.
#include "engine/CAIStateGroup_EnemyCreatureBase.h"
class C3DVector;
class CAIStateGroup_HoverMovementBase : public CAIStateGroup_EnemyCreatureBase {
};
FABLE_STATIC_ASSERT(sizeof(CAIStateGroup_HoverMovementBase) == 0x1c);
#endif
