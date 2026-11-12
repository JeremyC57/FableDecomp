// NThingShot::CTCShotDefinitionDrainLife::InitMovement() -- aim the drain-life bolt at its target with accuracy scatter, face it, wobble it
// Grade: DIFFER(642v642) -- Q20 port onto the shared shot headers (same size and frame, 28 diff lines); residue: the MultiArrow magnitude-order / cross-operand / velocity-operand residues, `lea ecx,[ebp-20h]` scheduled late, the RandomiseShotDirection temp at ebp-50h v retail ebp-44h (shared with the velocity there; an unnamed velocity temp fixes the slot but shrinks the frame to 0x50), one `mov ecx,eax` slot.
// by a random range-scaled angle about both side axes, launch it at the def's initial speed and set its turn cone and a randomised speed
// Leaf: ?InitMovement@CTCShotDefinitionDrainLife@NThingShot@@MAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x007BD845, 642 bytes, ebp frame + `leave` (optimize("y", off) + ("s", on): the shot TU).
//   Ego_r.pdb ?InitMovement@CTCShotDefinitionDrainLife@NThingShot@@MAEXXZ (protected virtual).
// Callees: CTCShotDefinitionBase::GetShootAtPos 0x007BEDD0, CTCPhysicsBase::SetInteractionFlags (slot 43, +0xAC), _ftol 0x00BFEA70,
//   CTCBase::GetWorld 0x00686A70, CWorld::GetWorldSeed 0x0049D810, NThingShot::RandomiseShotDirection 0x007BEBE0,
//   C3DVector::AccurateNormalise 0x00A14440 (x3), CRightHandedSet(fwd, up) 0x00487050, CTCPhysicsBase::ResetRHSet (slot 32, +0x80),
//   CDefClassBase::GetSubDef<CThingDrainLifeShotDef> 0x007BE92E (this TU, out of line), GFFloatRandom 0x004978A0 (x3),
//   C3DVector::Rotate(const C3DVector&, float) 0x00A13C60, CTCPhysicsBase::SetVelocity 0x0066F9B0, ::EnableGravity (slot 38, +0x98);
//   the def released through its vtable slot 1.
// Globals: CMainGameComponent::ConstantFPS 0x01375550 (inline GetConstantFPS), 0.0f, 1/360 0x01238E00, 2*pi (double) 0x0124F2B8.
// Layout: Ego_r.pdb CTCShotDefinitionDrainLife (MaxCosAngleToTurn +0x9C, MaxAngleToTurn +0xA0, Speed +0xA8; new overlay header) and
//   CThingDrainLifeShotDef (MaxDistance +0x28, MaxAngle +0x2C, MaxAngleToTurnDegs +0x30, InitialSpeed +0x34, SpeedRange +0x38).
// Shape: the MultiArrow sibling 0x007BCF7F without the straight-arrow tests and the lob; the bolt's own Speed member is the shot
//   def's speed per frame plus a GFFloatRandom of the def's SpeedRange per frame.
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
#define FABLE_RETAIL_CTHINGDRAINLIFESHOTDEF_CLASS
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
#include "engine/CThingDrainLifeShotDef.h"
#include "engine/NThingShot_CTCShotDefinitionDrainLife.h"
#ifdef _MSC_VER
#pragma intrinsic(cos)
#endif
namespace NThingShot {
C3DVector FABLE_FASTCALL RandomiseShotDirection(const C3DVector& direction, long accuracy, unsigned long* seed);   // 0x007BEBE0

void CTCShotDefinitionDrainLife::InitMovement()
{
    CTCPhysicsBase* physics = ParentThing->GetPhysicsTC();
    C3DVector target = GetShootAtPos();
    physics->SetInteractionFlags(0);
    C3DVector dir = target - physics->GetPosition();
    float distance = dir.GetMagnitude();
    dir = RandomiseShotDirection(dir, GetShotAccuracyPercentage(), GetWorld().GetWorldSeed());
    dir.AccurateNormalise();
    C3DVector side = C3DVector(dir.Y, -dir.X, 0.0f) ^ dir;
    C3DVector up = dir ^ side;
    side.AccurateNormalise();
    up.AccurateNormalise();
    physics->ResetRHSet(CRightHandedSet(side, dir));
    CDefPointer<const CThingDrainLifeShotDef> def;
    GetShotDef().GetSubDef(&def);
    float speed = def->InitialSpeed / CMainGameComponent::GetConstantFPS();
    if (distance > def->MaxDistance)
        distance = def->MaxDistance;
    distance = def->MaxAngle / def->MaxDistance * distance;
    dir.Rotate(side, (GFFloatRandom(distance * 2.0f, GetWorld().GetWorldSeed()) - distance) * (1.0f / 360.0f));
    dir.Rotate(up, (GFFloatRandom(distance, GetWorld().GetWorldSeed()) - distance) * (1.0f / 360.0f));
    C3DVector velocity = dir * speed;
    physics->SetVelocity(velocity);
    physics->EnableGravity(false);
    MaxAngleToTurn = def->MaxAngleToTurnDegs * (1.0f / 360.0f);
    MaxCosAngleToTurn = (float)cos(MaxAngleToTurn * 6.2831854820251465);
    Speed = GetShotDef().Speed / CMainGameComponent::GetConstantFPS();
    float range = def->SpeedRange / CMainGameComponent::GetConstantFPS();
    Speed += GFFloatRandom(range, GetWorld().GetWorldSeed());
}
}
