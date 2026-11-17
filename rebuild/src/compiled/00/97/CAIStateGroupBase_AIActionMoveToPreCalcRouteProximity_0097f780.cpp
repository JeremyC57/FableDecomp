// CAIStateGroupBase::AIAction_MoveToPreCalcRouteProximity(float proximity, const CThing& routeThing, bool& speed, EMovementType anim, const CAIFilterBase& condition, bool extra) -- walk to the route thing, then along its precalculated route until within `proximity` of the destination, polling the condition each step
// Leaf: ?AIAction_MoveToPreCalcRouteProximity@CAIStateGroupBase@@IAE?AW4EAIActionReturn@@MABVCThing@@AA_NW4EMovementType@@ABVCAIFilterBase@1@_N@Z
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0097F780, length 233 bytes (`ret 18h`).
// Callees: CThing::GetPos @ 0x004C73D0, CAIStateGroupBase::AIAction_MoveToPos @ 0x0097EC00 (mode 2, then 1,0,1,0),
// the condition's virtual slot 0, CAIStateGroupBase::SetUpPreCalcNav @ 0x0097F5B0, CAIStateGroupBase::
// AIAction_MoveToPosProximity @ 0x0097E890 (mode 2, then 0,1,0,1,0).
// Shape: as 0x0097F690 with the second leg through the proximity mover.
// Signature: Ego_r.pdb `?AIAction_MoveToPreCalcRouteProximity@CAIStateGroupBase@@IAE?AW4EAIActionReturn@@MABVCThing@@AA_N
// W4EMovementType@@ABVCAIFilterBase@1@_N@Z`; AIAction_MoveToPosProximity(EAIActionRepeat, C3DVector, float, bool&,
// EMovementType, bool, bool, bool, const CThing*, bool, float). These parameter types give retail's register assignment.
// All-`long` parameters permute ebx/ebp/edi.
// Name fix (agentNF5 2026-11-10): CAIStateGroupBase::AIAction_MoveToPos 0x0097EC00 is protected, EAIActionReturn(EAIActionRepeat, C3DVector, bool&, EMovementType, bool, bool, const CThing*, bool, float) (Ego_r.pdb / FableWin.pdb).
// NF51 (round 7): the AI state-group base comes from the shared engine/CAIStateGroupBase.h (was a local CAIStateGroupBase model with
// a local nested filter, a made-up EMovementType and long returns): the callee 0x0097E890 is the protected
// ?AIAction_MoveToPosProximity@CAIStateGroupBase@@IAE?AW4EAIActionReturn@@W4EAIActionRepeat@@VC3DVector@@MAA_NW4EMovementType@@_N44PBVCThing@@4M@Z
// (Ego_r.pdb / FableWin.pdb; _MEMBERS + _AIACTIONS + _MOVETOPOSPROXIMITY, _MEALACTIONS for AIAction_MoveToPos 0x0097EC00) with the Ego_r
// enums of engine/fable_ai_action_enums.h; the filter is the header's nested CAIFilterBase (_FILTER), whose slot-0 virtual is
// ??RCAIFilterBase@CAIStateGroupBase@@UBE_NXZ (was a local IsStillValid); SetUpPreCalcNav 0x0097F5B0 and this function are the
// protected members of the overlay opt-in FABLE_RETAIL_CAISTATEGROUPBASE_PRECALCROUTEPROXIMITY (Ego_r.pdb).
class CThing;
#include "engine/C3DVector.h"

#include "engine/CThing.h"

#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_AIACTIONS
#define FABLE_RETAIL_CAISTATEGROUPBASE_MOVETOPOSPROXIMITY
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEALACTIONS
#define FABLE_RETAIL_CAISTATEGROUPBASE_FILTER
#define FABLE_RETAIL_CAISTATEGROUPBASE_PRECALCROUTEPROXIMITY
#include "engine/fable_ai_action_enums.h"
#include "engine/CAIStateGroupBase.h"

EAIActionReturn CAIStateGroupBase::AIAction_MoveToPreCalcRouteProximity(float proximity, const CThing& routeThing, bool& speed,
                                                             EMovementType anim, const CAIFilterBase& condition, bool extra)
{
    long result;
    do
    {
        result = AIAction_MoveToPos(ACTION_REPEAT_CALLBACK, routeThing.GetPos(), speed, anim, extra, true, 0, true, 0.0f);
        if (!condition())
            return AI_ACTION_FAILED;
    } while (result == AI_ACTION_IN_PROGRESS);
    if (result != AI_ACTION_FINISHED)
        return (EAIActionReturn)result;
    C3DVector destination = SetUpPreCalcNav(routeThing);
    do
    {
        result = AIAction_MoveToPosProximity(ACTION_REPEAT_CALLBACK, destination, proximity, speed, anim, extra, false, true, 0, true, 0.0f);
        if (!condition())
            return AI_ACTION_FAILED;
    } while (result == AI_ACTION_IN_PROGRESS);
    return (EAIActionReturn)result;
}