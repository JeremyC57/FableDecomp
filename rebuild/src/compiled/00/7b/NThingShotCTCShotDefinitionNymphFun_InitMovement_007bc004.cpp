// NThingShot::CTCShotDefinitionNymphFun::InitMovement() -- aim the shot at the live target's ground point (0.5 above its position, dropped to the terrain height), scatter the direction by the shot's accuracy, orient the physics along it with a level up axis and accelerate it by the def's speed per frame
// Leaf: ?InitMovement@CTCShotDefinitionNymphFun@NThingShot@@UAEXXZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x007BC004, 342 bytes, thiscall, plain `ret` (vtable slot 33; public in this class: Ego_r.pdb /
//   FableWin.pdb ?InitMovement@CTCShotDefinitionNymphFun@NThingShot@@UAEXXZ). Grade: RELOCATION_MATCH (342/342).
// Shape: Ego_d.pdb tc_shot_definition.cpp lines 85..111 and its locals ptargetthing, { fire_at_pos, ptc_phys, applied_acceleration,
//   line_to_target, up, speed }; the statement order follows the line table. Lines 97..99 (176 B at /Od) emit nothing in retail: the
//   debug-only ASSERT the same-TU sibling CTCShotDefinitionProjectile::InitMovement 0x007BBB3B has at its line 201 (179 B). Line 100
//   (50 B, the RandomiseShotDirection statement) and lines 101..104 (8 / 27 / 8 / 34 B) have exactly the sibling's sizes (its 204..208),
//   so `up = line_to_target.CrossWithUp().Cross(line_to_target)` and the accessor GetShotAccuracyPercentage() are the sibling's text.
//   Line 106 (36 B) is `PShotDef->Speed / GetConstantFPS()` (VC7.1 /Od 30 B + VC10's 6-byte float spill; `GetShotDef().Speed` gives 27).
// Same-TU recipe (landed sibling 0x007BBB3B, same pragmas): FABLE_RETAIL_CRIGHTHANDEDSET_CTOR_INLINE (the CRightHandedSet temporary keeps
//   its own slot ebp-5Ch next to the RandomiseShotDirection result at ebp-44h; retail still calls 0x00487050) and
//   FABLE_RETAIL_C3DVECTOR_CROSS_MEMBER_XY_GROUP (the member Cross text that sibling landed with: puts `fild ConstantFPS` ahead of the
//   PShotDef reload and `mov ecx,ebx` after `push eax`, 0x007BC115..0x007BC128).
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50 (CIntelligentPointer::Get), IS_THING_ALIVE<const CThing*> 0x00436380
//   (__fastcall, out of line), CThing::GetPos 0x004C73D0, CTCBase::GetWorldMap 0x00686D30, CWorldMap::GetHeightZBelowPos 0x0063E480,
//   __ftol2 0x00BFEA70 (GetShotAccuracyPercentage, inline), CTCBase::GetWorld 0x00686A70, CWorld::GetWorldSeed 0x0049D810,
//   NThingShot::RandomiseShotDirection 0x007BEBE0, C3DVector::AccurateNormalise 0x00A14440 (twice), CRightHandedSet::CRightHandedSet
//   0x00487050, CTCPhysicsBase::ResetRHSet (virtual +0x80), CTCPhysicsBase::ApplyAcceleration 0x006B0210, CTCPhysicsBase::EnableGravity
//   (virtual +0x98); CMainGameComponent::ConstantFPS 0x01375550 (GetConstantFPS, inline). Pool floats 0.5f 0x0122F59C, 0.0f 0x0122DEDC.
#ifdef _MSC_VER
#pragma optimize("y", off)
#pragma optimize("s", on)
#endif
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_CROSSWITHUP
#define FABLE_RETAIL_C3DVECTOR_CROSS_MEMBER_XY_GROUP
#define FABLE_RETAIL_CRIGHTHANDEDSET
#define FABLE_RETAIL_CRIGHTHANDEDSET_CTOR_INLINE
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTCPHYSICSBASE_APPLYACCELERATION
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLDMAP
#define FABLE_RETAIL_CWORLDMAP_HEIGHTZ
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CTCSHOTDEFINITIONBASE_MOVEMENT
#define FABLE_RETAIL_CTCSHOTDEFINITIONBASE_SHOTACCESSORS
#include "engine/ETCInterfaceType.h"
#include "engine/C3DVector.h"
#include "engine/CThing.h"
#include "engine/CRightHandedSet.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CWorld.h"
#include "engine/CWorldMap.h"
#include "engine/CMainGameComponent.h"
#include "engine/NThingShot_CTCShotDefinitionNymphFun.h"
template <class T> bool FABLE_FASTCALL IS_THING_ALIVE(const T& thing);   // <const CThing*> 0x00436380
namespace NThingShot {
C3DVector FABLE_FASTCALL RandomiseShotDirection(const C3DVector& direction, long accuracy, unsigned long* seed); // 0x007BEBE0
void CTCShotDefinitionNymphFun::InitMovement()
{
    const CThing* ptargetthing = PTargetThing.Get();
    if (IS_THING_ALIVE(ptargetthing)) {
        CTCPhysicsBase* ptc_phys = GetParentThing().GetPhysicsTC();
        C3DVector fire_at_pos = ptargetthing->GetPos();
        fire_at_pos.Z += 0.5f;
        fire_at_pos.Z = GetWorldMap().GetHeightZBelowPos(fire_at_pos);
        C3DVector line_to_target = fire_at_pos - ptc_phys->GetPosition();
        line_to_target = RandomiseShotDirection(line_to_target, GetShotAccuracyPercentage(), GetWorld().GetWorldSeed());
        line_to_target.AccurateNormalise();
        C3DVector up = line_to_target.CrossWithUp().Cross(line_to_target);
        up.AccurateNormalise();
        ptc_phys->ResetRHSet(CRightHandedSet(up, line_to_target));
        float speed = PShotDef->Speed / CMainGameComponent::GetConstantFPS();
        C3DVector applied_acceleration = line_to_target * speed;
        ptc_phys->ApplyAcceleration(applied_acceleration);
        ptc_phys->EnableGravity(false);
    }
}
}