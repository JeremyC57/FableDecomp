// NThingShot::CTCShotDefinitionNymphFun::InitMovement() -- aim the shot at the live target's ground point (0.5 above its position, dropped to the terrain height), randomise the direction by the accuracy, orient the physics along it with a level side axis, and push it at the def's speed per frame
// Grade: DIFFER(343v342) -- temporaries: retail gives the RandomiseShotDirection result, the CRightHandedSet and the velocity temporaries disjoint frame slots (0x5c) while copying the result through eax; a named result fixes the frame but copies via lea; `mov ecx,ebx` scheduled before the fdivr.
// Leaf: ?InitMovement@CTCShotDefinitionNymphFun@NThingShot@@UAEXXZ
// Flags: none
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

template <class T> bool __fastcall IS_THING_ALIVE(const T& thing);   // <const CThing*> 0x00436380

class CThingShotDef : public CDefClassBase { public:             // Ego_r.pdb CThingShotDef (retail offsets)
    unsigned char _pad_shot[0x3c - sizeof(CDefClassBase)];
    float Speed;                                                  // +0x3C
};

namespace NThingShot {
C3DVector __fastcall RandomiseShotDirection(const C3DVector& direction, long accuracy, unsigned long* seed);   // 0x007BEBE0

class CTCShotDefinitionBase : public CTCBase { public:
    unsigned char _pad0c[0x20 - 0x0c];                            // PEffect, HitThingsList
    CIntelligentPointer<const CThing> PTargetThing;               // +0x20
    CIntelligentPointer<CThing> PShooter;                         // +0x28
    CIntelligentPointer<CThing> PWeapon;                          // +0x30
    CDefPointer<const CThingShotDef> PShotDef;                    // +0x38
    unsigned char _pad3c[4];                                      // PShotAbilityDef
    float AccuracyPercentage;                                     // +0x40
};
class CTCShotDefinitionProjectile : public CTCShotDefinitionBase { };
class CTCShotDefinitionNymphFun : public CTCShotDefinitionProjectile { public:
    virtual void InitMovement();
};

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
