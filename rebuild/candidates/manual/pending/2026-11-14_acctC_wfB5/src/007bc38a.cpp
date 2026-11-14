// NThingShot::CTCShotDefinitionThrowPredictive::InitMovement() -- aim a lobbed throw: keep the target at least 5 units from the shooter (else lead a moving target), then launch at 45 degrees with the speed that lands on it
// Leaf: ?InitMovement@CTCShotDefinitionThrowPredictive@NThingShot@@MAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x007BC38A, 880 bytes, thiscall, plain `ret` (vtable 0x0123CE9C slot 33).
// Callees: CTCShotDefinitionBase::GetShootAtPos 0x007BEDD0, IS_THING_ALIVE 0x0040DCE0, CThing::GetPos 0x004C73D0,
//   CTCPhysicsBase::EnableGravity (virtual +0x98), CTCPhysicsBase::SetVelocity 0x0066F9B0.
#ifdef _MSC_VER
#pragma optimize("y", off)
#pragma optimize("s", on)
#pragma intrinsic(sqrt, sin, cos)
#endif
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_MAGNITUDE2D
#define FABLE_RETAIL_C3DVECTOR_MAGNITUDE2D_ONE_EXPRESSION
#define FABLE_RETAIL_C3DVECTOR_NORMALISE_LOCALS
#define FABLE_RETAIL_C3DVECTOR_SCALE_X_PAREN
#define FABLE_RETAIL_C3DVECTOR_SQUAREDMAGNITUDE_XYZ
#define FABLE_RETAIL_CRIGHTHANDEDSET
#define FABLE_RETAIL_CRIGHTHANDEDSET_CTOR
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTCPHYSICSBASE_APPLYACCELERATION
#define FABLE_RETAIL_CTCPHYSICSBASE_SETVELOCITY_OUTOFLINE
#define FABLE_RETAIL_CTCPHYSICSBASE_LASTPOSITION
#define FABLE_RETAIL_CTCSHOTDEFINITIONBASE_ONHITBYMELEE
#define FABLE_RETAIL_CTCSHOTDEFINITIONBASE_MOVEMENT
#include <math.h>
#include "engine/ETCInterfaceType.h"
#include "engine/C3DVector.h"
#include "engine/CThing.h"
#include "engine/CRightHandedSet.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CMainGameComponent.h"
#include "engine/NThingShot_CTCShotDefinitionThrowPredictive.h"
template<class T> bool FABLE_FASTCALL IS_THING_ALIVE(const T& thing);
static const float THROW_ANGLE = 0.7853982f;
namespace NThingShot {
void CTCShotDefinitionThrowPredictive::InitMovement()
{
    C3DVector target = GetShootAtPos();
    if (IS_THING_ALIVE(PShooter.Get())) {
        C3DVector from_shooter = target - PShooter.Get()->GetPos();
        if (from_shooter.GetMagnitude2D() < 5.0f) {
            C3DVector offset = from_shooter;
            offset.Normalise();
            target = PShooter.Get()->GetPos() + offset * 5.0f;
            *(C3DVector*)TargetPos = target;
        } else if (IS_THING_ALIVE(PTargetThing.Get())) {
            C3DVector motion = PTargetThing.Get()->GetPos() - PTargetThing.Get()->GetPhysicsTC()->GetLastPosition();
            const CThing* targetThing = PTargetThing.Get();
            float time = (targetThing->GetPos() - PShooter.Get()->GetPos()).GetMagnitude() / PShotDef->Speed * 3.0f;
            C3DVector lead = motion * time;
            target += lead * ((long)AccuracyPercentage * 0.01f);
            *(C3DVector*)TargetPos = target;
        }
    }
    C3DVector direction = target - ParentThing->GetPos();
    float height = direction.Z;
    direction.Z = 0.0f;
    float distance = direction.GetMagnitude2D();
    direction /= distance;
    double sin_squared = sin((double)THROW_ANGLE);
    sin_squared *= sin_squared;
    double level_range = (distance / sin((double)THROW_ANGLE)) * cos((double)THROW_ANGLE);
    float one_over_speed_squared = (float)((level_range - height) * (2.0f * sin_squared) / (distance * distance * 0.12f));
    CTCPhysicsBase* physics = ParentThing->GetPhysicsTC();
    physics->EnableGravity(true);
    if (one_over_speed_squared > DELTA_NOTIONAL_ZERO) {
        float speed = 1.0f / sqrt(one_over_speed_squared);
        C3DVector velocity = (direction * (float)sin((double)THROW_ANGLE) + C3DVector(0.0f, 0.0f, 1.0f) * (float)cos((double)THROW_ANGLE)) * speed;
        physics->SetVelocity(velocity);
    }
}
}
