// NThingShot::CTCShotDefinitionProjectile::InitMovement() -- aim the projectile at its target, scattered by the shot's accuracy, and launch it.
// Leaf: ?InitMovement@CTCShotDefinitionProjectile@NThingShot@@MAEXXZ
// Flags: none
// Retail: 0x007BBB3B, 268 bytes, thiscall (vtable slot 33).
// What it does: line_to_target = shoot-at position - the physics position, scattered by RandomiseShotDirection(accuracy, world seed)
//   and normalised; up = line_to_target.CrossWithUp().Cross(line_to_target), normalised; the shot is oriented with
//   CRightHandedSet(up, line_to_target), accelerated by line_to_target * GetInitialVelocity() and has its gravity switched off.
// Shape: FableWin.pdb tc_shot_definition.cpp lines 196..215 (FableWin.exe 0x02708A62) and Ego_r.pdb record the locals ptc_phys,
//   fire_at_pos, line_to_target, up, speed (float) and applied_acceleration, the statement order above (ResetRHSet before
//   GetInitialVelocity) and the member calls CrossWithUp / Cross. The donor's line 201 ASSERT emits nothing here.
//   - FABLE_RETAIL_CRIGHTHANDEDSET_CTOR_INLINE: the CRightHandedSet temporary keeps its own slot (ebp-58h, frame 58h) while retail
//     still calls 0x00487050 -- the same finding as the same-TU sibling 0x007BBE34.
//   - FABLE_RETAIL_C3DVECTOR_SCALE_X_PAREN (sibling 0x007BC6FA's operator* text) and the new FABLE_RETAIL_C3DVECTOR_CROSS_MEMBER_XY_GROUP
//     (the member Cross in the CROSS_XY_GROUP grouping sibling 0x007BBE34 landed for operator^): no-code PAREN tuples that put
//     `lea eax,[ebp-34h]` before the Z product of applied_acceleration (0x007BBC21). Sweep: 3 tuples ahead of the Z product are needed;
//     X_PAREN alone, or with the 1- / 3-tuple member Cross texts (MEMBER_X_GROUP, CYLINDER, COMPONENTS_GROUPED), leave 4-6 instructions.
// Callees: CThing::GetPhysicsTC (via the inline CTCBase::GetParentThing), CTCShotDefinitionBase::GetShootAtPos 0x007BEDD0,
//   __ftol2 0x00BFEA70 (GetShotAccuracyPercentage, inline), CTCBase::GetWorld 0x00686A70, CWorld::GetWorldSeed 0x0049D810,
//   NThingShot::RandomiseShotDirection 0x007BEBE0, C3DVector::AccurateNormalise 0x00A14440 (twice), CRightHandedSet::CRightHandedSet
//   0x00487050, CTCPhysicsBase::ResetRHSet (virtual +0x80), CTCShotDefinitionBase::GetInitialVelocity 0x007BEFB0,
//   CTCPhysicsBase::ApplyAcceleration 0x006B0210, CTCPhysicsBase::EnableGravity (virtual +0x98). Pool float 0.0f 0x0122DEDC.
#ifdef _MSC_VER
#pragma optimize("y", off)
#pragma optimize("s", on)
#endif
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_CROSSWITHUP
#define FABLE_RETAIL_C3DVECTOR_SCALE_X_PAREN
#define FABLE_RETAIL_C3DVECTOR_CROSS_MEMBER_XY_GROUP
#define FABLE_RETAIL_CRIGHTHANDEDSET
#define FABLE_RETAIL_CRIGHTHANDEDSET_CTOR_INLINE
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTCPHYSICSBASE_APPLYACCELERATION
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CTCSHOTDEFINITIONBASE_ONHITBYMELEE
#define FABLE_RETAIL_CTCSHOTDEFINITIONBASE_MOVEMENT
#define FABLE_RETAIL_CTCSHOTDEFINITIONBASE_SHOTACCESSORS
#define FABLE_RETAIL_CTCSHOTDEFINITIONBASE_GETINITIALVELOCITY
#define FABLE_RETAIL_CTCSHOTDEFINITIONPROJECTILE_INITMOVEMENT
#include "engine/ETCInterfaceType.h"
#include "engine/C3DVector.h"
#include "engine/CThing.h"
#include "engine/CRightHandedSet.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CWorld.h"
#include "engine/NThingShot_CTCShotDefinitionProjectilePredictive.h"
namespace NThingShot {
C3DVector FABLE_FASTCALL RandomiseShotDirection(const C3DVector& direction, long accuracy, unsigned long* seed); // 0x007BEBE0
void CTCShotDefinitionProjectile::InitMovement()
{
    CTCPhysicsBase* ptc_phys = GetParentThing().GetPhysicsTC();
    C3DVector fire_at_pos = GetShootAtPos();
    C3DVector line_to_target = fire_at_pos - ptc_phys->GetPosition();
    line_to_target = RandomiseShotDirection(line_to_target, GetShotAccuracyPercentage(), GetWorld().GetWorldSeed());
    line_to_target.AccurateNormalise();
    C3DVector up = line_to_target.CrossWithUp().Cross(line_to_target);
    up.AccurateNormalise();
    ptc_phys->ResetRHSet(CRightHandedSet(up, line_to_target));
    float speed = GetInitialVelocity();
    C3DVector applied_acceleration = line_to_target * speed;
    ptc_phys->ApplyAcceleration(applied_acceleration);
    ptc_phys->EnableGravity(false);
}
}