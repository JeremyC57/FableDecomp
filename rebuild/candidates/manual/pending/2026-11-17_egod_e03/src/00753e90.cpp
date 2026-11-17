// CTCLook::LookAtPos(const C3DVector& pos_to_look_at, long look_flags) -- start looking at a position: unless it is (nearly) where the parent stands, re-arm the look timer, drop the body flag when the parent overrides look turning, store flags / target / direction, set the head turn state (2 with flag 2, else 1) and the body turn state (2 with flag 1, possibly starting a turning animation when the creature is not navigating, else 1), then the recovery angles
// Leaf: ?LookAtPos@CTCLook@@QAEXABVC3DVector@@J@Z
// Retail: Fable.exe (TLC, VC7.1) 0x00753E90, 447 bytes, thiscall `ret 8`.
// Names: Ego_r.pdb ?LookAtPos@CTCLook@@QAEXABVC3DVector@@J@Z (+ FableWin); Ego_d.pdb tc_look.obj locals pos_to_look_at / look_flags /
//   dir_to_look_in and the block's ptc_creature_nav / naving. Callees: CThing::GetPos 0x004C73D0, CTCBase::PeekComponent 0x00686D60,
//   CMainGameComponent::GetCurrentWorldFrame 0x004167C8, NCreatureMode::IsLookTurningOverridenByParent 0x0085F200,
//   IsAbleToTurnViaAnimation 0x00752480, IsCurrentlyTurningViaAnimation 0x00753B40, IsTurningBody 0x00753020,
//   InitialiseTurningViaAnimation 0x00753630, CTCCreatureNavigation::IsCurrentlyNavigating 0x006CB8F0, GFGetAngleXYTo 0x00661540,
//   GFGetAngleYZTo 0x00754E70 (all Ego_r / FableWin spellings).
// Same-TU bodies (Ego_d tc_look.obj, defined after the row in Ego_d's order, each graded at its own retail address): SetHeadTurnState
//   0x007517B0 (MATCH; the TimeToStopLooking store LAST -- with it first the expanded copies hoist it above the head-state test,
//   0x00753F7E), GFGetAngleYZTo 0x00754E70 (the landed row's arithmetic with the wrap written out as in the shared GFGetAngleXYTo body; the landed row
//   puts the wrap in a made-up forced-inline WrapAngle -- coordinator decision), IsOverriddenByParent 0x00751A70, SetRecoveryAngles (expanded; no retail copy),
//   UpdateLookTimer 0x00751AD0 (all RELOCATION_MATCH). GFGetAngleXYTo's body is the existing shared opt-in
//   FABLE_RETAIL_GFANGLES_ANGLEXYTO_BODY (retail keeps the parent thing in edi across the two angle calls only with it visible).
#define FABLE_RETAIL_C3DVECTOR_PACKED
#define FABLE_RETAIL_CLOOKDEF
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_GFANGLES_ANGLEXYTO_BODY
#define FABLE_RETAIL_C3DVECTOR_NOT_EQUAL
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCBASE_COMPONENT
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_WORLDFRAME
#define FABLE_RETAIL_CTCCREATURENAVIGATION
#define FABLE_RETAIL_CTCCREATURENAVIGATION_ISNAVIGATING
#define FABLE_RETAIL_CTCCREATURENAVIGATION_NAVIGABLE
#include <math.h>
#pragma intrinsic(atan2, sqrt, fmod)
#include "engine/CTCLook.h"
#include "engine/CMainGameComponent.h"
#include "engine/CTCCreatureNavigation.h"
#include "engine/C2DVector.h"

namespace NCreatureMode {
bool FABLE_FASTCALL IsLookTurningOverridenByParent(const CThing& thing);   // 0x0085F200
}
float FABLE_FASTCALL GFGetAngleYZTo(const C3DVector& from, const C3DVector& to);   // 0x00754E70

void CTCLook::LookAtPos(const C3DVector& pos_to_look_at, long look_flags)
{
    C3DVector dir_to_look_in = pos_to_look_at - GetParentThing().GetPos();
    if (dir_to_look_in.GetSquaredMagnitude() > DELTA_NOTIONAL_ZERO * DELTA_NOTIONAL_ZERO) {
        UpdateLookTimer();
        if (IsOverriddenByParent() && (look_flags & 1))
            look_flags ^= 1;
        LookFlags = look_flags;
        PosToLookAt = pos_to_look_at;
        DirectionToLookIn = dir_to_look_in;
        UsingOldStyleTurning = true;
        if (look_flags & 2)
            SetHeadTurnState(2);
        else
            SetHeadTurnState(1);
        if (look_flags & 1) {
            SetBodyTurnState(2);
            CanTurnBody = true;
            if (IsAbleToTurnViaAnimation() && dir_to_look_in.GetSquaredMagnitude() > DELTA_NOTIONAL_ZERO * DELTA_NOTIONAL_ZERO) {
                CTCCreatureNavigation* ptc_creature_nav;
                bool naving = GetParentThing().GetTC(&ptc_creature_nav) && ptc_creature_nav->IsCurrentlyNavigating();
                if (!naving && !IsCurrentlyTurningViaAnimation() && IsTurningBody())
                    InitialiseTurningViaAnimation(pos_to_look_at, look_flags);
            }
        } else
            SetBodyTurnState(1);
        SetRecoveryAngles(PosToLookAt);
    }
}

void CTCLook::SetHeadTurnState(long state)
{
    if (HeadTurnState != state) {
        HeadTurnState = state;
        OneFrameDelay = true;
    }
    TimeToStopLooking = -1;
}

float FABLE_FASTCALL GFGetAngleYZTo(const C3DVector& from, const C3DVector& to)
{
    float dx = from.X - to.X;
    float dy = from.Y - to.Y;
    float dz = to.Z - from.Z;
    float angle = (float)atan2(dz, sqrt(dy * dy + dx * dx)) * 0.15915494f;
    if (angle < 0.0f || angle >= 1.0f) {
        angle = (float)fmod((double)angle, 1.0);
        if (angle < 0.0f)
            angle += 1.0f;
    }
    return angle;
}

bool CTCLook::IsOverriddenByParent() const
{
    return NCreatureMode::IsLookTurningOverridenByParent(GetParentThing()) ? true : false;
}

void CTCLook::SetRecoveryAngles(const C3DVector& pos_to_look_at)
{
    RecoveryAngleToLookAtXY = GFGetAngleXYTo(GetParentThing().GetPhysicsTC()->GetPosition(), pos_to_look_at);
    RecoveryAngleToLookAtYZ = GFGetAngleYZTo(GetParentThing().GetPhysicsTC()->GetPosition(), pos_to_look_at);
}

void CTCLook::UpdateLookTimer()
{
    LookTimer = PeekComponent().GetCurrentWorldFrame() + 90;
}
