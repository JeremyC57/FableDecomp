// 008de080 NCreatureMode::CModeDragonStrafeBurninate::FrameUpdate
// Leaf: ?FrameUpdate@CModeDragonStrafeBurninate@NCreatureMode@@UAEXXZ
// Flags: none
// Ego_d creature_modes.obj and the FableWin caller agree on the receiver, locals and calls.
// Inferred: the grouping of the three GFSqr calls is indistinguishable in the debug helper.
// The shared component lookup and the portable inverse-root seed give the retail arithmetic order.
#define FABLE_RETAIL_DRAGON_LOOK_INLINES_PASS30
#define FABLE_RETAIL_DRAGON_FLYING_PROTECTED_PASS30
#define FABLE_RETAIL_C3DVECTOR_SQUARE_ORIGINAL_FORMAL
#define FABLE_RETAIL_UPDATEAI_VECTOR_SUB_PASS29
#define FABLE_RETAIL_C3DVECTOR_NORMALISE_GROUPED_SUM
#define FABLE_RETAIL_GFFASTONEOVERSQRT_ORIGINAL_UNION
#define FABLE_RETAIL_GFFASTONEOVERSQRT_PORTABLE_SEED_FIELDS_PASS26
#define FABLE_RETAIL_CTCANIMATIONCOMPLEX_FULL
#define FABLE_RETAIL_CTCANIMATIONCOMPLEX_GETANIMATION_PASS26
#define FABLE_RETAIL_DRAGONBASE_LIFETIME_PASS29
#define FABLE_RETAIL_JACK_STAGE1_UPDATE
#define FABLE_RETAIL_GFDEBUGDYNAMICCAST
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING

#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_NORMALISE_SQUARE_HELPER
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_C3DVECTOR_NOT_EQUAL
#define FABLE_RETAIL_CMODEBASE_VIRTUALS
#define FABLE_RETAIL_CMODEBASE_PARENTTHING
#define FABLE_RETAIL_CTCCREATUREMODEMANAGER
#define FABLE_RETAIL_C3DVECTOR_ANGLE_BODIES
#define FABLE_RETAIL_CLOOKDEF
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CRIGHTHANDEDSET
#define FABLE_RETAIL_CCARDINALSPLINE_GETSPLINEPOSITION
#define FABLE_RETAIL_LOOK_HEAD_ANIM
#define FABLE_RETAIL_CANIMSET_TYPE
#define FABLE_RETAIL_CANIMSET_STRAFE_ACCESSORS_PASS24
#define FABLE_RETAIL_CANIMATETHING_MAINANIM
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGCREATUREBASE_SETCURRENTACTION_COUNTED
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CThing.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CTCCreatureModeManager.h"
#include "engine/NCreatureMode.h"
#include "engine/CRightHandedSet.h"
#include "engine/CAnimSet.h"
#include "engine/CAnimateThing.h"
#include "engine/CTCAnimationComplex.h"
#include "engine/CTCLook.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CCardinalSpline.h"
#include "engine/GFAngles.h"
#include <math.h>

#include "engine/NCreatureMode_CModeDragonStrafeBurninate.h"

namespace NCreatureMode {
void CModeDragonStrafeBurninate::FrameUpdate()
{
    CTCCreatureModeManager::CModeBase::FrameUpdate();
    CThingCreatureBase& creature = *GFDebugDynamicCast(&ParentThing, (CThingCreatureBase*)0);
    CTCAnimationComplex* ptc_animation = 0;
    GetParentThing().GetTC(&ptc_animation);
    CAnimateThing& anim = ptc_animation->GetAnimation();
    float anim_percent_done = anim.GetMainAnim().GetPercentageComplete();
    float percent_of_spline_travelled = (NumAnimLoops + anim_percent_done) / NumAnimLoopsToDo;
    SplinePercentageComplete = percent_of_spline_travelled;
    if (percent_of_spline_travelled >= FlameStartPercentage && !FlameActive)
        StartBeamEffect();
    else if (percent_of_spline_travelled >= FlameStopPercentage && FlameActive)
        KillBeamEffect();
    UpdateBeam(percent_of_spline_travelled);

    percent_of_spline_travelled *= 1.0f - 1.0f / (FlightSpline.GetControlPointCount() - 1.0f);
    percent_of_spline_travelled += 1.0f / (FlightSpline.GetControlPointCount() - 1.0f);
    if (percent_of_spline_travelled > 1.0f)
        percent_of_spline_travelled = 1.0f;
    C3DVector new_pos;
    FlightSpline.GetSplinePosition(new_pos, percent_of_spline_travelled);
    PreviousPos = GetParentThing().GetPos();
    GetParentThing().GetPhysicsTC()->SetPosition(new_pos);
    C3DVector ideal_movement_direction = new_pos - PreviousPos;
    ideal_movement_direction = ideal_movement_direction.Normalise();
    C3DVector movement_direction = (ideal_movement_direction + GetParentThing().GetPhysicsTC()->GetFacingVector()) / 2.0f;
    GetParentThing().GetPhysicsTC()->SetFacingAngleXY(movement_direction.GetAngleXY());
    float angle_delta = GFGetSignedAngleDifference(movement_direction.GetAngleXY(), GetParentThing().GetPhysicsTC()->GetFacingVector().GetAngleXY());
    float roll_angle = angle_delta * 20.0f;
    if (roll_angle > 0.125f)
        roll_angle = 0.125f;
    if (roll_angle < -0.125f)
        roll_angle = -0.125f;
    CAnimSet* pleg_anim_set = anim.GetFirstCombinationAnimFromType(0x22);
    if (pleg_anim_set) {
        float desired_alpha = (float)fabs(roll_angle) / 0.125f;
        if (anim_percent_done > 0.75f && NumAnimLoops == NumAnimLoopsToDo - 1)
            desired_alpha = 0.0f;
        float new_alpha = (desired_alpha - LegAnimAlpha) * 0.1f + LegAnimAlpha;
        pleg_anim_set->SetInternalAlpha(0, new_alpha, false);
        LegAnimAlpha = new_alpha;
    }
    CTCLook* ptc_look = 0;
    if (GetParentThing().GetTC(&ptc_look)) {
        C3DVector look_pos = GetParentThing().GetPos() + GetParentThing().GetPhysicsTC()->GetFacingVector() * 50.0f;
        ptc_look->LookAtPos(look_pos, 10);
    }
    if (anim_percent_done > 0.999f) {
        ++NumAnimLoops;
        if (NumAnimLoops >= NumAnimLoopsToDo) {
            CCountedPointer<CCreatureActionBase> p_end_action = PEndAction;
            CTCCreatureModeManager* ptc_mode_manager;
            GetParentThing().GetTC(&ptc_mode_manager);
            ptc_mode_manager->RemoveMode(MODE_DRAGON_STRAFE_BURNINATE);
            if (p_end_action.Get())
                creature.SetCurrentAction(p_end_action);
        }
    }
}
}