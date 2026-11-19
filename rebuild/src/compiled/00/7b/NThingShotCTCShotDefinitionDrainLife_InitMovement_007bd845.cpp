// NThingShot::CTCShotDefinitionDrainLife::InitMovement() -- aim the drain-life bolt (randomised), face it, wobble it about up/right, launch it, set its turn cone and randomised speed
// Leaf: ?InitMovement@CTCShotDefinitionDrainLife@NThingShot@@MAEXXZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x007BD845, 642 bytes, thiscall `ret` (vtable slot 33; Ego_r + FableWin ?InitMovement@CTCShotDefinitionDrainLife@NThingShot@@MAEXXZ).
// Callees: CTCShotDefinitionBase::GetShootAtPos 0x007BEDD0, CTCPhysicsBase::SetInteractionFlags (virtual +0xAC), __ftol2 0x00BFEA70
//   (GetShotAccuracyPercentage, inline), CTCBase::GetWorld 0x00686A70, CWorld::GetWorldSeed 0x0049D810, NThingShot::RandomiseShotDirection
//   0x007BEBE0, C3DVector::AccurateNormalise 0x00A14440 (three times), CRightHandedSet::CRightHandedSet 0x00487050, CTCPhysicsBase::ResetRHSet
//   (virtual +0x80), CDefClassBase::GetSubDef<CThingDrainLifeShotDef> 0x007BE92E, GFFloatRandom 0x004978A0 (three times), C3DVector::Rotate
//   0x00A13C60 (twice), CTCPhysicsBase::SetVelocity 0x0066F9B0, CTCPhysicsBase::EnableGravity (virtual +0x98); ConstantFPS 0x01375550.
// Shape: the landed same-TU twin CTCShotDefinitionMultiArrow::InitMovement (0x007BCF7F) text and include / pragma set, in FableWin.pdb's /Od
//   statement order for this function (FableWin.exe 0x0270CF31: fire_at_pos, SetInteractionFlags(0), line_to_target, distance, Randomise,
//   up = CrossWithUp().Cross(line), right, the RH set, pt_shot_definition, CDefPointer + GetSubDef, speed, clamp, max_angle, two Rotate,
//   initial_velocity, ..., Speed = GetShotDef().Speed / FPS, speed_range, Speed += GFFloatRandom). The donor's two ASSERTs (line_to_target
//   IsNonZero, the GetSubDef result) emit nothing in retail.
//   - FABLE_RETAIL_CRIGHTHANDEDSET_CTOR_INLINE (as the same-TU siblings 0x007BBE34 / 0x007BBB3B): the CRightHandedSet temporary keeps its own
//     frame slot (ebp-5Ch, frame 5Ch) while retail still calls 0x00487050; with the declaration-only ctor it overlays the C3DVector call
//     temporary (ebp-50h, frame 50h).
// Bracket: FABLE_RETAIL_C3DVECTOR_SUBTRACT_XY_GROUP (operator- with the X and Y differences grouped, existing shared opt-in): puts
//   `lea ecx,[ebp-20h]` (up's AccurateNormalise) between the two products of up.Z (0x007BD90F). This function has no branch before
//   0x007BD9A1, so line_to_target's subtraction sits in the same block. Measured: ungrouped two x87 ops late, X-only grouped one late;
//   the member-Cross groupings are shadowed by CTCSHOTDEFINITIONBASE_COLLISION's Cross text (dropping it costs 6-8 lines); statement
//   brackets on the up / right lines change nothing.
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
#define FABLE_RETAIL_C3DVECTOR_SUBTRACT_XY_GROUP
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
#define FABLE_RETAIL_CTHINGDRAINLIFESHOTDEF_CLASS
#include <math.h>
#include "fable_string.h"
#include "engine/ETCInterfaceType.h"
#include "engine/C3DVector.h"
#include "engine/CThing.h"
#include "engine/CRightHandedSet.h"
#ifdef _MSC_VER
#pragma optimize("t", on)
#pragma optimize("y", on)
#endif
#include "engine/CTCPhysicsBase.h"
#ifdef _MSC_VER
#pragma optimize("s", on)
#pragma optimize("y", off)
#endif
#include "engine/CWorld.h"
#include "engine/CMainGameComponent.h"
#include "engine/GFFloatRandom.h"
#include "engine/CDefClassBase_GetSubDef.h"
#include "engine/CThingDrainLifeShotDef.h"
#include "engine/NThingShot_CTCShotDefinitionDrainLife.h"
#ifdef _MSC_VER
#pragma intrinsic(sqrt,cos)
#endif
#include "engine/GFCos.h"
namespace NThingShot {
C3DVector FABLE_FASTCALL RandomiseShotDirection(const C3DVector& direction,long accuracy,unsigned long* seed);
void CTCShotDefinitionDrainLife::InitMovement()
{
    CTCPhysicsBase* ptc_phys=ParentThing->GetPhysicsTC();
    C3DVector fire_at_pos=GetShootAtPos();
    ptc_phys->SetInteractionFlags(0);
    C3DVector line_to_target=fire_at_pos-ptc_phys->GetPosition();
    float distance=line_to_target.GetAccurateMagnitude();
    line_to_target=RandomiseShotDirection(line_to_target,GetShotAccuracyPercentage(),GetWorld().GetWorldSeed());
    line_to_target.AccurateNormalise();
    C3DVector up=line_to_target.CrossWithUp().Cross(line_to_target);
    C3DVector right=line_to_target.Cross(up);
    up.AccurateNormalise();
    right.AccurateNormalise();
    ptc_phys->ResetRHSet(CRightHandedSet(up,line_to_target));
    const CThingShotDef* pt_shot_definition=&GetShotDef();
    CDefPointer<const CThingDrainLifeShotDef> pt_drain_shot_definition;
    pt_shot_definition->GetSubDef(&pt_drain_shot_definition);
    float speed=pt_drain_shot_definition->InitialSpeed/CMainGameComponent::GetConstantFPS();
    if(distance>pt_drain_shot_definition->MaxDistance)distance=pt_drain_shot_definition->MaxDistance;
    float max_angle=pt_drain_shot_definition->MaxAngle*distance/pt_drain_shot_definition->MaxDistance;
    float angle=-max_angle+GFFloatRandom(max_angle*2.0f,GetWorld().GetWorldSeed());
    line_to_target.Rotate(up,angle/360.0f);
    angle=-max_angle+GFFloatRandom(max_angle,GetWorld().GetWorldSeed());
    line_to_target.Rotate(right,angle/360.0f);
    C3DVector initial_velocity=line_to_target*speed;
    ptc_phys->SetVelocity(initial_velocity);
    ptc_phys->EnableGravity(false);
    MaxAngleToTurn=pt_drain_shot_definition->MaxAngleToTurnDegs/360.0f;
    MaxCosAngleToTurn=(float)GFCos(MaxAngleToTurn);
    Speed=GetShotDef().Speed/CMainGameComponent::GetConstantFPS();
    float speed_range=pt_drain_shot_definition->SpeedRange/CMainGameComponent::GetConstantFPS();
    Speed+=GFFloatRandom(speed_range,GetWorld().GetWorldSeed());
}
}