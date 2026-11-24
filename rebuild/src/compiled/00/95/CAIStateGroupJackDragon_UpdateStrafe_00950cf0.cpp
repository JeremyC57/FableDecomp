// CAIStateGroup_JackDragon::UpdateStrafe()
// Leaf: ?UpdateStrafe@CAIStateGroup_JackDragon@@AAEXXZ
// Flags: (none)
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_PACKED
#define FABLE_RETAIL_C3DVECTOR_ANGLE_BODIES
#define FABLE_RETAIL_JACK_STAGE1_UPDATE
#define FABLE_RETAIL_C3DVECTOR_ORIGINAL_COMPONENT_CTOR
#define FABLE_RETAIL_C3DVECTOR_ORIGINAL_COPY_FORMAL
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_GET_ORIGINAL_CHAIN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CCOUNTEDPOINTER_CALLED_CTOR
#define FABLE_RETAIL_STLP_VECTOR_PUSHBACK_OUTOFLINE
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CAISTATEGROUP_JACKDRAGON
#define FABLE_RETAIL_CAISTATEGROUP_JACKDRAGON_UPDATESTRAFE_PASS26
#define FABLE_RETAIL_CTCCREATUREMODEMANAGER_ADDMODEPOINTER
#define FABLE_RETAIL_CMODEBASE_VIRTUALS
#include "rebuild_abi.h"
#include "engine/C3DVector.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CThing.h"
#include "engine/CThingAICreature.h"
#include "engine/ThingAlive.h"
#include "engine/CAIStateGroup_JackDragon.h"
#include "engine/CAIBrain.h"
#include "engine/NCreatureMode_CModeDragonStrafeBurninate.h"

const float DRAGON_FLY_SPEED = 30.0f;

// Original same-TU body in FableWin 0x02BD9900 and retail 0x0094FDF0.
inline bool CAIStateGroup_JackDragon::IsStillRunnable()
{
    if (!IS_THING_ALIVE(PEnemy))
        return false;
    if (IsStopping())
        return false;
    return true;
}

void CAIStateGroup_JackDragon::UpdateStrafe()
{
    C3DVector offset;
    if (IS_THING_ALIVE(PStrafeMarker1) && IS_THING_ALIVE(PStrafeMarker2) &&
            PStrafeMarker1->GetPhysicsTC() && PStrafeMarker2->GetPhysicsTC()) {
        CThing* pmarker1;
        CThing* pmarker2;
        if (GoLeft) {
            pmarker1 = PStrafeMarker2.Get();
            pmarker2 = PStrafeMarker1.Get();
        }
        else {
            pmarker1 = PStrafeMarker1.Get();
            pmarker2 = PStrafeMarker2.Get();
        }
        C3DVector strafe_vector = pmarker2->GetPos() - pmarker1->GetPos();
        C3DVector target_pos_offset(0.0f, -60.0f, 60.0f);
        target_pos_offset.RotateXY(strafe_vector.GetAngleXY());
        C3DVector start_pos = pmarker1->GetPos() + target_pos_offset;
        C3DVector flame_start_pos;
        C3DVector flame_stop_pos;
        float fly_speed = DRAGON_FLY_SPEED;
        std::vector<C3DVector> movement_points;
        movement_points.push_back(GetCreature().GetPos());
        if (GoLeft) {
            movement_points.push_back(DragonPosIntoStrafeLeft);
            movement_points.push_back(DragonPosIntoStrafeRight);
            movement_points.push_back(DragonPosOutOfStrafeRight);
            flame_start_pos = DragonPosStrafeFlameLeft;
            flame_stop_pos = DragonPosStrafeFlameRight;
        }
        else {
            movement_points.push_back(DragonPosIntoStrafeRight);
            movement_points.push_back(DragonPosIntoStrafeLeft);
            movement_points.push_back(DragonPosOutOfStrafeLeft);
            flame_start_pos = DragonPosStrafeFlameRight;
            flame_stop_pos = DragonPosStrafeFlameLeft;
        }
        CTCCreatureModeManager* ptc_cmm;
        if (GetCreature().GetTC(&ptc_cmm)) {
            ptc_cmm->AddMode(new NCreatureMode::CModeDragonStrafeBurninate(GetCreature(), movement_points,
                PreviousFlightPos, 0.15f, 0.6f, flame_start_pos, flame_stop_pos));
            AIAction_DoNothing();
        }
    }
    if (!IsStillRunnable()) {
        Terminate();
    }
    else {
        if (GoLeft) {
            PreviousFlightPos = DragonPosIntoStrafeRight;
            GoLeft = 0;
        }
        else {
            PreviousFlightPos = DragonPosIntoStrafeLeft;
            GoLeft = 1;
        }
        CurrentState = ChooseIntoGroundState();
    }
}