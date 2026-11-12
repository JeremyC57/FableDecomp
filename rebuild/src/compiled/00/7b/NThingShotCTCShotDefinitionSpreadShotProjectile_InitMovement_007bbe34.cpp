// NThingShot::CTCShotDefinitionSpreadShotProjectile::InitMovement() -- aim a spread shot at a point circling its live target and launch it.
// Leaf: ?InitMovement@CTCShotDefinitionSpreadShotProjectile@NThingShot@@MAEXXZ
// Flags:
// Retail: Fable.exe (TLC, VC7.1) @ 0x007BBE34, 464 bytes, __fastcall, no arguments (vtable 0x01240D84 slot 33).
// What it does: if the target thing is alive, take the target's facing vector times 1.5, turn it in the XY plane by a rotating
//   file-static angle, add the target's position and drop the point onto the ground (GetHeightZBelowPos); advance the angle by
//   0.333 and wrap it at 1.0. The shot direction is that point minus the shot's own position, scattered by
//   RandomiseShotDirection(accuracy, world seed) and normalised; the shot is oriented with CRightHandedSet(side, dir)
//   (side = (dir.Y, -dir.X, 0) x dir, normalised), accelerated to dir * (Speed / ConstantFPS) and has its gravity switched off.
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50 (through CIntelligentPointer::Get), IS_THING_ALIVE<const CThing*> 0x00436380,
//   CTCPhysicsBase::GetFacingVector (virtual +0x118), C3DVector::RotateXY 0x00A14040, CThing::GetPos 0x004C73D0,
//   CTCBase::GetWorldMap 0x00686D30, CWorldMap::GetHeightZBelowPos 0x0063E480, __ftol2 0x00BFEA70 (the (long) accuracy),
//   CTCBase::GetWorld 0x00686A70, CWorld::GetWorldSeed 0x0049D810, NThingShot::RandomiseShotDirection 0x007BEBE0,
//   C3DVector::AccurateNormalise 0x00A14440 (twice), CRightHandedSet::CRightHandedSet 0x00487050, CTCPhysicsBase::ResetRHSet
//   (virtual +0x80), CTCPhysicsBase::ApplyAcceleration 0x006B0210, CTCPhysicsBase::EnableGravity (virtual +0x98).
// Data: 0.333f 0x0126DF24 and 1.5f 0x0126DF28 are this TU's own .rdata constants (one reader each, not the shared float pool),
//   the angle is the TU's static float 0x013BAE7C; ConstantFPS 0x01375550; pool floats 1.0f 0x0122DED8, 0.0f 0x0122DEDC.
// Shape (why the bytes are what they are):
//   - frame pointer + size-optimised like every shot InitMovement (`optimize("y", off)` / `optimize("s", on)`);
//   - the target pointer lives in edi and a separate copy is passed by reference to IS_THING_ALIVE (0x007BBE4B `lea ecx,[ebp-4]` /
//     `mov [ebp-4],edi`): the argument is a converted temporary, not the local itself;
//   - the 1.5 scale is the plain inline operator*(float) (no X-product grouping): its parameter is loaded once after the
//     GetFacingVector call and held on the x87 stack for the three products (0x007BBE74 `fld [0126DF28h]`, `fmul st(1..3)`),
//     and the stores end `fst [ebp-18h] ; fstp st(0) ; fstp st(0)`;
//   - the CRightHandedSet temporary has its own frame slot (ebp-5Ch) instead of overlaying the C3DVector call temporary
//     (ebp-44h, shared by GetFacingVector and RandomiseShotDirection): retail saw the inline constructor's body and still called
//     its out-of-line copy 0x00487050 (FABLE_RETAIL_CRIGHTHANDEDSET_CTOR_INLINE);
//   - FABLE_RETAIL_C3DVECTOR_CROSS_XY_GROUP (two no-code PAREN tuples in operator^) puts VC7.1's scheduling-region cut where
//     retail has it: `push eax ; mov ecx,ebx` for ApplyAcceleration after the three velocity products (0x007BBFEB).
// Names: SPREAD_SHOT_ANGLE_STEP, SPREAD_SHOT_TARGET_OFFSET and s_SpreadShotAngle are descriptive (no PDB / string evidence).
#ifdef _MSC_VER
#pragma optimize("y", off)
#pragma optimize("s", on)
#endif
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_CROSS_XY_GROUP
#define FABLE_RETAIL_CRIGHTHANDEDSET
#define FABLE_RETAIL_CRIGHTHANDEDSET_CTOR_INLINE
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTCPHYSICSBASE_APPLYACCELERATION
#define FABLE_RETAIL_CTCSHOTDEFINITIONBASE_ONHITBYMELEE
#define FABLE_RETAIL_CTCSHOTDEFINITIONBASE_MOVEMENT
#define FABLE_RETAIL_CTCSHOTDEFINITIONBASE_SHOTACCESSORS
#define FABLE_RETAIL_CWORLDMAP
#define FABLE_RETAIL_CWORLDMAP_HEIGHTZ
#include "engine/ETCInterfaceType.h"
#include "engine/C3DVector.h"
#include "engine/CThing.h"
#include "engine/CRightHandedSet.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CMainGameComponent.h"
#include "engine/CWorldMap.h"
#include "engine/CWorld.h"
#include "engine/NThingShot_CTCShotDefinitionSpreadShotProjectile.h"
template<class T> bool FABLE_FASTCALL IS_THING_ALIVE(const T& thing);
namespace NThingShot {
C3DVector FABLE_FASTCALL RandomiseShotDirection(const C3DVector& direction, long accuracy, unsigned long* seed); // 0x007BEBE0
static const float SPREAD_SHOT_ANGLE_STEP = 0.333f; // 0x0126DF24
static const float SPREAD_SHOT_TARGET_OFFSET = 1.5f; // 0x0126DF28
static float s_SpreadShotAngle; // 0x013BAE7C
void CTCShotDefinitionSpreadShotProjectile::InitMovement()
{
    CThing* target = const_cast<CThing*>(PTargetThing.Get());
    if (IS_THING_ALIVE<const CThing*>((const CThing*)target)) {
        CTCPhysicsBase* physics = ParentThing->GetPhysicsTC();
        C3DVector aim = target->GetPhysicsTC()->GetFacingVector() * SPREAD_SHOT_TARGET_OFFSET;
        aim.RotateXY(s_SpreadShotAngle);
        aim += target->GetPos();
        aim.Z = GetWorldMap().GetHeightZBelowPos(aim);
        s_SpreadShotAngle += SPREAD_SHOT_ANGLE_STEP;
        if (s_SpreadShotAngle > 1.0f)
            s_SpreadShotAngle -= 1.0f;
        C3DVector dir = aim - physics->GetPosition();
        dir = RandomiseShotDirection(dir, GetShotAccuracyPercentage(), GetWorld().GetWorldSeed());
        dir.AccurateNormalise();
        C3DVector side = C3DVector(dir.Y, -dir.X, 0.0f) ^ dir;
        side.AccurateNormalise();
        physics->ResetRHSet(CRightHandedSet(side, dir));
        C3DVector velocity = dir * (PShotDef->Speed / CMainGameComponent::GetConstantFPS());
        physics->ApplyAcceleration(velocity);
        physics->EnableGravity(false);
    }
}
}