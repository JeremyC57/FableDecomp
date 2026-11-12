// NThingShot::CTCShotDefinitionMultiArrow::InitMovement() -- aim at the shoot-at position (scattered unless this is a straight arrow),
// Grade: DIFFER(808v808) -- Q20 port onto the shared shot headers (same size, 90 diff lines); residue: retail sums the shot length X*X first (ours Y*Y+Z*Z then X*X), one cross-product operand pair swapped, the velocity `fld dir.X ; fmul speed` (ours fld speed ; fmul X), and frame packing (retail puts fall/shortfall in the velocity slot ebp-28h, ours in the side slot ebp-40h). FP operand order did not move under any source spelling (VC canonicalises commutative operands).
// face and launch the arrow, wobble a scattered arrow by a random range-scaled angle about both side axes, and give it the constant
// acceleration that lands it on the target in TimeToReachTarget seconds; set the turn cone
// Leaf: ?InitMovement@CTCShotDefinitionMultiArrow@NThingShot@@MAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x007BCF7F, 808 bytes, ebp frame + `leave` (optimize("y", off) + ("s", on): the shot TU).
//   Ego_r.pdb ?InitMovement@CTCShotDefinitionMultiArrow@NThingShot@@MAEXXZ (protected virtual).
// Callees: CTCShotDefinitionBase::GetShootAtPos 0x007BEDD0 (twice), _ftol 0x00BFEA70, CTCBase::GetWorld 0x00686A70, CWorld::GetWorldSeed
//   0x0049D810, NThingShot::RandomiseShotDirection 0x007BEBE0, C3DVector::AccurateNormalise 0x00A14440 (x3), CRightHandedSet(fwd, up)
//   0x00487050, CTCPhysicsBase::ResetRHSet (slot 32, +0x80), CDefClassBase::GetSubDef<CThingMultiArrowShotDef> 0x007BE7EA (this TU,
//   out of line), GFFloatRandom 0x004978A0, C3DVector::Rotate(const C3DVector&, float) 0x00A13C60, CTCPhysicsBase::SetVelocity
//   0x0066F9B0, ::EnableGravity (slot 38, +0x98); the def released through its vtable slot 1.
// Globals: CMainGameComponent::ConstantFPS 0x01375550 (inline GetConstantFPS), 2.0f, 1.0f, 0.0f, 1/360 0x01238E00, 2*pi (double).
// Layout: Ego_r.pdb CTCShotDefinitionMultiArrow (Acceleration +0x9C, MaxCosAngleToTurn +0xA8, MaxAngleToTurn +0xAC, StraightArrow
//   +0xB0; new overlay header) and CThingMultiArrowShotDef (MaxDistance +0x2C, MaxAngle +0x30, TimeToReachTarget +0x34,
//   MaxAngleToTurnDegs +0x38).
// Shape: the landed ProjectilePredictive::InitMovement 0x007BC6FA aim/launch, ported onto the shared headers (Q20); GetSubDef's
//   body visible in the TU (CDefClassBase_GetSubDef.h, NOINLINE: the def pointer's `and [ebp-8],0` init and the null-test-free
//   release); the protected GetShotAccuracyPercentage / GetShotDef accessors (Q20 overlay SHOTACCESSORS).
#ifdef _MSC_VER
#pragma optimize("y", off)
#pragma optimize("s", on)
#endif
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
#define FABLE_RETAIL_C3DVECTOR_MAGNITUDE_SQRTF
#define FABLE_RETAIL_C3DVECTOR_SCALE_X_PAREN
#define FABLE_RETAIL_CRIGHTHANDEDSET
#define FABLE_RETAIL_CRIGHTHANDEDSET_CTOR
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTCPHYSICSBASE_SETVELOCITY_OUTOFLINE
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CTCSHOTDEFINITIONBASE_ONHITBYMELEE
#define FABLE_RETAIL_CTCSHOTDEFINITIONBASE_MOVEMENT
#define FABLE_RETAIL_CTCSHOTDEFINITIONBASE_COLLISION
#define FABLE_RETAIL_CTCSHOTDEFINITIONBASE_SHOTACCESSORS
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
#include "engine/CThingMultiArrowShotDef.h"
#include "engine/NThingShot_CTCShotDefinitionMultiArrow.h"
#ifdef _MSC_VER
#pragma intrinsic(cos)
#endif
namespace NThingShot {
C3DVector FABLE_FASTCALL RandomiseShotDirection(const C3DVector& direction, long accuracy, unsigned long* seed);   // 0x007BEBE0

void CTCShotDefinitionMultiArrow::InitMovement()
{
    CTCPhysicsBase* physics = ParentThing->GetPhysicsTC();
    C3DVector target = GetShootAtPos();
    C3DVector dir = target - physics->GetPosition();
    float distance = dir.GetMagnitude();
    if (!StraightArrow)
        dir = RandomiseShotDirection(dir, GetShotAccuracyPercentage(), GetWorld().GetWorldSeed());
    dir.AccurateNormalise();
    C3DVector side = C3DVector(dir.Y, -dir.X, 0.0f) ^ dir;
    C3DVector up = dir ^ side;
    side.AccurateNormalise();
    up.AccurateNormalise();
    physics->ResetRHSet(CRightHandedSet(side, dir));
    float speed = GetShotDef().Speed / CMainGameComponent::GetConstantFPS();
    CDefPointer<const CThingMultiArrowShotDef> def;
    GetShotDef().GetSubDef(&def);
    if (!StraightArrow) {
        if (distance > def->MaxDistance)
            distance = def->MaxDistance;
        distance = def->MaxAngle / def->MaxDistance * distance;
        dir.Rotate(side, (GFFloatRandom(distance * 2.0f, GetWorld().GetWorldSeed()) - distance) * (1.0f / 360.0f));
        dir.Rotate(up, (GFFloatRandom(distance, GetWorld().GetWorldSeed()) - distance) * (1.0f / 360.0f));
    }
    C3DVector velocity = dir * speed;
    physics->SetVelocity(velocity);
    if (!StraightArrow) {
        C3DVector fall = velocity * def->TimeToReachTarget * (float)CMainGameComponent::GetConstantFPS();
        C3DVector shortfall = (GetShootAtPos() - physics->GetPosition() - fall) * 2.0f;
        Acceleration = shortfall * (1.0f / ((float)(CMainGameComponent::GetConstantFPS() * CMainGameComponent::GetConstantFPS()) *
                                            def->TimeToReachTarget * def->TimeToReachTarget));
    }
    physics->EnableGravity(false);
    MaxAngleToTurn = def->MaxAngleToTurnDegs * (1.0f / 360.0f);
    MaxCosAngleToTurn = (float)cos(MaxAngleToTurn * 6.2831854820251465);
}
}
