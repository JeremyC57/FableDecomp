// NThingShot::CTCShotDefinitionNymphFun::InitMovement() -- aim the shot at the live target's ground point (0.5 above its position, dropped to the terrain height), randomise the direction by the accuracy, orient the physics along it with a level side axis, and push it at the def's speed per frame
// Grade: DIFFER(343v342) -- on the shared headers (new engine/NThingShot_CTCShotDefinitionNymphFun.h): retail copies the RandomiseShotDirection result through eax (`mov esi,eax`, 0x007BC0A4) from a temporary that keeps its own frame slot (0x5c frame); a named result keeps the slot but copies via `lea esi,[ebp-44h]`, the direct `direction = Randomise(...)` copies via eax but shares the slot with the CRightHandedSet temporary (0x50 frame); a named CRightHandedSet fixes the frame but pushes `lea` not the ctor result; plus the velocity scale: retail issues `fild [ConstantFPS]` before reloading this and `mov ecx,ebx` after `push eax` (named speed / parens / velocity temporary inert)
// Leaf: ?InitMovement@CTCShotDefinitionNymphFun@NThingShot@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x007BC004, 342 bytes (plain `ret`; size TU with frame: optimize("y", off) + ("s", on), as the landed
//   shot-definition siblings 0x007BE05C / 0x007BE3E0).
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50, IS_THING_ALIVE<const CThing*> 0x00436380 (__fastcall, out of line),
//   CThing::GetPos 0x004C73D0, CTCBase::GetWorldMap 0x00686D30, CWorldMap::GetHeightZBelowPos 0x0063E480, __ftol2, CTCBase::GetWorld
//   0x00686A70, CWorld::GetWorldSeed 0x0049D810, NThingShot::RandomiseShotDirection 0x007BEBE0, C3DVector::AccurateNormalise 0x00A14440,
//   CRightHandedSet(const C3DVector&, const C3DVector&) 0x00487050, CTCPhysicsBase::ResetRHSet (+0x80), ApplyAcceleration 0x006B0210,
//   EnableGravity (+0x98); CMainGameComponent::ConstantFPS 0x01375550.
// Layout (Ego_r.pdb names, retail offsets): PTargetThing +0x20, PShotDef +0x38, AccuracyPercentage +0x40; CThingShotDef Speed +0x3C.
#pragma optimize("y", off)
#pragma optimize("s", on)
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CRIGHTHANDEDSET
#define FABLE_RETAIL_CRIGHTHANDEDSET_CTOR
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTCPHYSICSBASE_APPLYACCELERATION
#define FABLE_RETAIL_CWORLDMAP
#define FABLE_RETAIL_CWORLDMAP_HEIGHTZ
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CTCSHOTDEFINITIONBASE_MOVEMENT
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CDefPointer.h"
#include "engine/CDefClassBase.h"
#include "engine/CRightHandedSet.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CWorldMap.h"
#include "engine/CWorld.h"
#include "engine/CMainGameComponent.h"
#include "engine/NThingShot_CTCShotDefinitionNymphFun.h"

template <class T> bool FABLE_FASTCALL IS_THING_ALIVE(const T& thing);   // <const CThing*> 0x00436380

namespace NThingShot {
C3DVector FABLE_FASTCALL RandomiseShotDirection(const C3DVector& direction, long accuracy, unsigned long* seed);   // 0x007BEBE0

void CTCShotDefinitionNymphFun::InitMovement()
{
    const CThing* target = PTargetThing.Get();
    if (IS_THING_ALIVE(target)) {
        CTCPhysicsBase* physics = GetParentThing().GetPhysicsTC();
        C3DVector aim = target->GetPos();
        aim.Z += 0.5f;
        aim.Z = GetWorldMap().GetHeightZBelowPos(aim);
        C3DVector direction = aim - physics->GetPosition();
        long accuracy = (long)AccuracyPercentage;
        C3DVector randomised = RandomiseShotDirection(direction, accuracy, GetWorld().GetWorldSeed());
        direction = randomised;
        direction.AccurateNormalise();
        C3DVector side = C3DVector(direction.Y, -direction.X, 0.0f) ^ direction;
        side.AccurateNormalise();
        physics->ResetRHSet(CRightHandedSet(side, direction));
        C3DVector velocity = direction * (PShotDef->Speed / CMainGameComponent::GetConstantFPS());
        physics->ApplyAcceleration(velocity);
        physics->EnableGravity(false);
    }
}
}
