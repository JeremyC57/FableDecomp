// 007bcc40 NThingShot::CTCShotDefinitionDragonFireball::InitMovement
// Leaf: ?InitMovement@CTCShotDefinitionDragonFireball@NThingShot@@MAEXXZ
// Flags:
#define FABLE_RETAIL_C3DVECTOR_MAGNITUDE_SQRTF
#define FABLE_RETAIL_FIREBALL_MAGNITUDE_HELPER_PASS30
#define FABLE_RETAIL_FIREBALL_NONZERO_HELPER_PASS30
#define FABLE_RETAIL_C3DVECTOR_NORMALISE_NAMED_SQUARED
#define FABLE_RETAIL_DRAGON_FIREBALL_MOVEMENT_PASS30
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CTCPHYSICSBASE_LASTPOSITION
#define FABLE_RETAIL_CTCPHYSICSBASE_APPLYACCELERATION
#define FABLE_RETAIL_CTHING_STATEACCESS
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_IS_THING_ALIVE_NOINLINE
#define FABLE_RETAIL_SHOT_UPDATEMOVEMENT_PASS26
#define FABLE_RETAIL_GFRANDOM_OUTOFLINE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_NOINLINE
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_SQUAREDMAGNITUDE_XYZ
#define FABLE_RETAIL_C3DVECTOR_ACCURATE_MAGNITUDE
#define FABLE_RETAIL_C3DVECTOR_ANGLE_BODIES
#define FABLE_RETAIL_C3DVECTOR_SCALE_X_PAREN
#define FABLE_RETAIL_CRIGHTHANDEDSET
#define FABLE_RETAIL_CRIGHTHANDEDSET_CTOR_INLINE
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTCPHYSICSBASE_SETVELOCITY_OUTOFLINE
#define FABLE_RETAIL_CTCPHYSICSBASE_SETASMOVING
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CTCSHOTDEFINITIONBASE_ONHITBYMELEE
#define FABLE_RETAIL_CTCSHOTDEFINITIONBASE_MOVEMENT
#define FABLE_RETAIL_CTCSHOTDEFINITIONBASE_COLLISION
#define FABLE_RETAIL_CTCSHOTDEFINITIONBASE_SHOTACCESSORS
#ifdef _MSC_VER
#pragma optimize("s", on)
#pragma intrinsic(sqrt)
#endif
#include <math.h>
#include "fable_string.h"
#include "engine/ETCInterfaceType.h"
#include "engine/C3DVector.h"
#include "engine/CThing.h"
#include "engine/CRightHandedSet.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CWorld.h"
#include "engine/CMainGameComponent.h"
#include "engine/GFFloatRandom.h"
#include "engine/CDefClassBase_GetSubDef.h"
#include "engine/ThingAlive.h"
#include "engine/GFRandom.h"
#include "engine/NThingShot_CTCShotDefinitionDragonFireball.h"
#include "engine/NThingShotHelpers.h"

namespace NThingShot {
void CTCShotDefinitionDragonFireball::InitMovement()
{
    CTCPhysicsBase* ptc_phys = GetParentThing().GetPhysicsTC();
    C3DVector fire_at_pos = GetShootAtPos();
    C3DVector start_pos = GetParentThing().GetPos();
    C3DVector line_to_target = fire_at_pos - start_pos;
    float shot_speed = GetShotDef().Speed / CMainGameComponent::GetConstantFPS();
    const CThing* ptarget_thing = GetPTargetThing();
    if (IS_THING_ALIVE(ptarget_thing)) {
        C3DVector target_pos = ptarget_thing->GetPos();
        C3DVector target_velocity = ptarget_thing->GetPhysicsTC()->GetPosition() - ptarget_thing->GetPhysicsTC()->GetLastPosition();
        float distance_to_target = (target_pos - start_pos).GetAccurateMagnitude();
        float time_to_get_to_target = 0.0f;
        if (GetShotDef().Speed > DELTA_NOTIONAL_ZERO)
            time_to_get_to_target = distance_to_target / shot_speed;
        if (!target_velocity.IsNonZero()) {
            target_velocity.Initialise(0.0f, 1.0f, 0.0f);
            target_velocity *= 0.1f;
        }
        float max_deviation = GetShotDef().ShotOffsetRadiusRange.HighValue;
        float min_deviation = GetShotDef().ShotOffsetRadiusRange.LowValue;
        float deviation = GFFloatRandom(max_deviation - min_deviation, GetWorld().GetWorldSeed()) + min_deviation;
        float max_angle_deviation = GetShotDef().ShotOffsetAngleRange.HighValue;
        float min_angle_deviation = GetShotDef().ShotOffsetAngleRange.LowValue;
        float angle_deviation = GFFloatRandom(max_angle_deviation - min_angle_deviation, GetWorld().GetWorldSeed()) + min_angle_deviation;
        float up_or_down = GFRandom(1, GetWorld().GetWorldSeed()) == 0 ? -1.0f : 1.0f;
        target_velocity.Rotate(C3DVector(0.0f,0.0f,1.0f), angle_deviation * up_or_down);
        target_velocity *= deviation;
        C3DVector projected_target_pos = target_pos + target_velocity * time_to_get_to_target;
        line_to_target = projected_target_pos - start_pos;
    }
    line_to_target = RandomiseShotDirection(line_to_target, GetShotAccuracyPercentage(), GetWorld().GetWorldSeed());
    line_to_target.AccurateNormalise();
    C3DVector up = line_to_target.CrossWithUp().Cross(line_to_target);
    up.AccurateNormalise();
    ptc_phys->ResetRHSet(CRightHandedSet(up,line_to_target));
    float speed = shot_speed;
    C3DVector applied_acceleration = line_to_target * speed;
    ptc_phys->ApplyAcceleration(applied_acceleration);
    ptc_phys->EnableGravity(false);
}
}