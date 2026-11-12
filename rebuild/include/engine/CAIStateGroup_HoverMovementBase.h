#ifndef FABLE_ENGINE_CAISTATEGROUP_HOVERMOVEMENTBASE_H
#define FABLE_ENGINE_CAISTATEGROUP_HOVERMOVEMENTBASE_H
// New header (chunk Q17, row 0x0095B820). RTTI .?AVCAIStateGroup_HoverMovementBase@@ : CAIStateGroup_EnemyCreatureBase (no own data:
// the derived CAIStateGroup_HoverHornetQueen's first Ego_r.pdb member is at +0x1C, right after PTargetThing). Member functions: the
// Ego_r.pdb name set (protected AIAction_MoveToAndFaceThingAtDistance / AIAction_MoveToPosProximityAndFacePos, protected virtual
// AIAction_MoveToPosAndFacePos), declarations only. Needs FABLE_RETAIL_CAISTATEGROUP_ENEMYCREATUREBASE.
#include "engine/CAIStateGroup_EnemyCreatureBase.h"
class C3DVector;
class CAIStateGroup_HoverMovementBase : public CAIStateGroup_EnemyCreatureBase {
#ifdef FABLE_RETAIL_CAISTATEGROUP_HOVERMOVEMENTBASE_MOVETOPOSANDFACEPOS
protected:
    // slot 22 (0x0091D7F0) ?AIAction_MoveToPosAndFacePos@CAIStateGroup_HoverMovementBase@@MAE?AW4EAIActionReturn@@W4EAIActionRepeat@@ABVC3DVector@@1AA_NW4EMovementType@@@Z
    virtual EAIActionReturn AIAction_MoveToPosAndFacePos(EAIActionRepeat repeat, const C3DVector& pos, const C3DVector& facePos, bool& reached, EMovementType movement);
#endif
};
FABLE_STATIC_ASSERT(sizeof(CAIStateGroup_HoverMovementBase) == 0x1c);
#endif
