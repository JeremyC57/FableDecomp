// CCreatureAction_JumpOutFromCover::Initialise() -- normalise the jump direction; with a live combat target pick JUMP_OUT_RIGHT
// (direction turned -1/4) when the direction points along the target-to-the-left side (the normalised creature-to-target vector
// turned +1/4), else JUMP_OUT_LEFT (turned +1/4), and face the creature along the turned direction (atan2 as a 0..1 turn fraction
// wrapped with fmod); then the base Initialise
// Leaf: ?Initialise@CCreatureAction_JumpOutFromCover@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008B7040, 770 bytes, thiscall, plain `ret` (vtable 0x0129888C slot 12). Ego_r.pdb / FableWin.pdb
//   ?Initialise@CCreatureAction_JumpOutFromCover@@UAEXXZ.
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50 (PTarget +0xa8, PCreature +0x08), CThing::GetPos 0x004C73D0, C3DVector::RotateXY
//   0x00A14040 (Ego_r.pdb ?RotateXY@C3DVector@@QAEXM@Z), CCharString 0x0099EBF0 / ~ 0x0099EAE0, CCombatActionBase::SetCombatAnimation
//   0x008574D0 / ::Initialise 0x00857520, _CIfmod 0x00BFEB28 (the fmod intrinsic), CTCPhysicsBase::SetFacingAngleXY (slot 66, +0x108).
// Globals: GFastOneOverSqrtTable 0x013CE558 (the inline GFFastOneOverSqrt), 3.0f, 0.5f, 1/(2pi) 0x01237F08, 0.0f, 1.0f, double 1.0.
// Shape: the landed DuckAndCover::Initialise 0x008B6EE0 facing tail and the Q12 MidAttackSide::Initialise 0x008B6AD0 target maths, on
//   the shared headers. The member's Normalise reads X, Y, Z into locals first (NORMALISE_LOCALS_XYZ, chunk Q20 overlay: retail loads
//   X, Y, Z and pops the three copies after the store); the creature-to-target vector is normalised in place by the one-line
//   GFFastOneOverSqrt(GetSquaredMagnitude()) (SQUAREDMAGNITUDE_XYZ) and operator*= -- with the locals body there VC7.1 keeps the
//   factor in st(0) (`fst ; fmulp`) where retail stores and reloads it (0x008B71C0 `fstp [esp+14h] ; fmul [esp+14h]`). The dot is
//   the Z, Y, X running form (DOT_ORDERED_ZYX). Ported onto the shared headers this wave, the parked target / &PCreature ebp/ebx
//   swap is gone (named `target` pointer read first, as retail's GetPItem order).
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_SQUAREDMAGNITUDE_XYZ
#define FABLE_RETAIL_C3DVECTOR_NORMALISE_LOCALS_XYZ
#define FABLE_RETAIL_C3DVECTOR_DOT_ORDERED_ZYX
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE_PCREATURE
#define FABLE_RETAIL_CCOMBATACTIONBASE_MELEE
#define FABLE_RETAIL_CCOMBATACTIONBASE_INITIALISE
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CThing.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CDefPointer.h"
#include "engine/CCountedPointer.h"
#include "stlport/fable_containers.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CThingCreatureBase.h"
#include "engine/ThingAlive.h"
#include "engine/CCombatActionBase.h"
#include "engine/CCreatureAction_JumpOutFromCover.h"
#include <math.h>
#pragma intrinsic(atan2, fmod)

void CCreatureAction_JumpOutFromCover::Initialise()
{
    JumpDirection.Normalise();
    if (IS_THING_ALIVE(PTarget)) {
        CThing* target = PTarget.Get();
        C3DVector toTarget = target->GetPos() - PCreature->GetPos();
        float one_on_magnitude = GFFastOneOverSqrt(toTarget.GetSquaredMagnitude());
        toTarget *= one_on_magnitude;
        C3DVector side = toTarget;
        side.RotateXY(0.25f);
        C3DVector direction;
        if (side.Dot(JumpDirection) > 0.0f) {
            direction = JumpDirection;
            direction.RotateXY(-0.25f);
            SetCombatAnimation("JUMP_OUT_RIGHT");
        } else {
            direction = JumpDirection;
            direction.RotateXY(0.25f);
            SetCombatAnimation("JUMP_OUT_LEFT");
        }
        CTCPhysicsBase* physics = PCreature->GetPhysicsTC();
        float fraction = (float)atan2((double)direction.X, (double)direction.Y) * 0.15915493667125702f;
        if (fraction < 0.0f || fraction >= 1.0f) {
            fraction = (float)fmod((double)fraction, 1.0);
            if (fraction < 0.0f)
                fraction += 1.0f;
        }
        physics->SetFacingAngleXY(fraction);
    }
    CCombatActionBase::Initialise();
}