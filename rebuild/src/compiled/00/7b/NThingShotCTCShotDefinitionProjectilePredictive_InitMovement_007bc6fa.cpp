// NThingShot::CTCShotDefinitionProjectilePredictive::InitMovement() -- lead the target, orient the shot and launch it.
// Leaf: ?InitMovement@CTCShotDefinitionProjectilePredictive@NThingShot@@MAEXXZ
// Flags:
// Retail: 007bc6fa, 514 bytes, thiscall.
#ifdef _MSC_VER
#pragma optimize("y", off)
#pragma optimize("s", on)
#pragma intrinsic(sqrt)
#endif
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_SCALE_X_PAREN
#define FABLE_RETAIL_C3DVECTOR_SQUAREDMAGNITUDE_XYZ
#define FABLE_RETAIL_CRIGHTHANDEDSET
#define FABLE_RETAIL_CRIGHTHANDEDSET_CTOR
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTCPHYSICSBASE_APPLYACCELERATION
#define FABLE_RETAIL_CTCPHYSICSBASE_LASTPOSITION
#define FABLE_RETAIL_CTCSHOTDEFINITIONBASE_ONHITBYMELEE
#define FABLE_RETAIL_CTCSHOTDEFINITIONBASE_MOVEMENT
#include "engine/ETCInterfaceType.h"
#include "engine/C3DVector.h"
#include "engine/CThing.h"
#include "engine/CRightHandedSet.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CMainGameComponent.h"
#include "engine/NThingShot_CTCShotDefinitionProjectilePredictive.h"
template<class T> bool FABLE_FASTCALL IS_THING_ALIVE(const T& thing);
namespace NThingShot {
void CTCShotDefinitionProjectilePredictive::InitMovement()
{
    CTCPhysicsBase* physics = ParentThing->GetPhysicsTC();
    C3DVector target = GetShootAtPos();
    if (IS_THING_ALIVE(PTargetThing.Get()) && IS_THING_ALIVE(PShooter.Get())) {
        C3DVector motion = PTargetThing.Get()->GetPos() - PTargetThing.Get()->GetPhysicsTC()->GetLastPosition();
        const CThing* targetThing = PTargetThing.Get();
        float time = (targetThing->GetPos() - PShooter.Get()->GetPos()).GetMagnitude() / PShotDef->Speed * 3.0f;
        C3DVector lead = motion * time;
        target += lead * ((long)AccuracyPercentage * 0.01f);
    }
    C3DVector dir = target - physics->GetPosition();
    dir.AccurateNormalise();
    C3DVector side = C3DVector(dir.Y, -dir.X, 0.0f) ^ dir;
    side.AccurateNormalise();
    physics->ResetRHSet(CRightHandedSet(side, dir));
    C3DVector velocity = dir * (PShotDef->Speed / CMainGameComponent::GetConstantFPS());
    physics->ApplyAcceleration(velocity);
    physics->EnableGravity(false);
}
}