// CThingPlayerCreature::ResolveFacingDirection() -- turn the hero's physics right-handed set towards where he should face this frame
// Grade: DIFFER(2328v2328) -- one scheduling swap in the z-target arm: retail 0x006ABB81 `mov edx,[esp+48h]` / 0x006ABB85 `fld qword [0.0]` / 0x006ABB8B `mov [esp+6Ch],edx` / 0x006ABB8F `fsin`, ours `fld` `fsin` `mov` `mov`; all other 652 instructions (operands, allocation, frame words) equal
// Leaf: ?ResolveFacingDirection@CThingPlayerCreature@@IAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006AB820, 2328 bytes, `ret` (int3 padding to 0x006AC140, the next row ResetThingReactions).
// Identity: Ego_r.pdb / FableWin.pdb ?ResolveFacingDirection@CThingPlayerCreature@@IAEXXZ (protected). Two sources: (1) Ego_r donor order
//   ResolveMovementAcceleration 0x005475B0 -> ResolveFacingDirection 0x00547690 = retail 0x006AB770 (landed) -> 0x006AB820; (2) its only
//   caller is the landed UpdateWalkingControlForces 0x006AD260, which ends `jmp 0x006AB820` after ResolveMovementAcceleration.
// Callees: CTCPhysicsBase slots 63 GetFacingAngleXY (+0xFC), 70 GetFacingVector() (+0x118), 75 SetRHSet (+0x12C); CCreatureActionBase
//   slot 49 IsPreventingNormalMovement (+0xC4); CVectorMap<..>::LowerBound const 0x004365B0 (PeekTC<const CTCStealth> / <const
//   CTCTargetingBase>); CThing::GetPlayer 0x004C7A30, GetWorld 0x004C7AA0, GetPos 0x004C73D0; CPlayer::IsFirstPersonTargeting 0x004874D0;
//   CAIGameCameraBase slot 10 GetCameraManager(const CGameCameraManager**) const (+0x28); C3DVector::AccurateNormalise 0x00A14440,
//   Rotate(const C3DVector&, float) 0x00A13C60; CTCTargetingBase slots 36 GetPTargetLockedThing (+0x90), 37 GetTargetedPos (+0x94);
//   NGraphicAppearance::CTCGraphicAppearance::GetFocalPos 0x004C1060; CThingPhysical::PeekPos 0x006A4D00; GFGetAngleXYTo 0x00661540;
//   CRightHandedSet(const C3DVector&, const C3DVector&) 0x00487050 (called at three cold sites, expanded at five: VC7.1's inline split,
//   inline trace sc 79 / 139 / 121 called, 43..69 expanded); CThingPlayerCreature::GetMaxTurnSpeed 0x006A99D0 (four calls);
//   GFSmoothTowards<float> 0x006ADC20 (same-TU template instance, called); _CIacos 0x00BFEBA2, _CIfmod 0x00BFEB28 (intrinsics).
// Globals: DELTA_NOTIONAL_ZERO 0x0129BA3C (1e-4f). Float pool: 1/(2pi) 0x01237F08, 1/360 0x01238E00, -1/360 0x0125D1E8, 1/90 0x0125D1E4,
//   0.45 0x0125D1E0, 0.05 0x0125A15C, (double) 2pi 0x0124F2B8 / 0.0 0x0122ED70 / 1.0 0x01237F00, +-0.5 / +-1.0 / 0.0.
// Layout: engine/CThingPlayerCreature.h retail branch (LeanAngle +0x1FC, MovementBandType +0x200, PPlayerMovementDef +0x1EC,
//   MovementAcceleration +0x180), engine/CPlayerMovementDef.h (overlay, NEW: LeanAngleTurnFactor / LimitAngle / StepPerFrame +0x70..+0x78).
// Shape (each measured; NOTES.md grade log):
//   - Every arm that leaves early ends `LeanAngle = 0.0f; return;` (the zero constant gets ebp as in retail); the z-target arm's found /
//     null if-else shares one such tail (separate returns let VC7.1 cross-jump the camera arm's tail into the z-target arm, which splits
//     that arm's scheduling region and keeps its up-vector stores late).
//   - GetPhysicsTC() at every physics call: the object is evaluated once (`mov esi,[esi+60h]` in the action arm).
//   - Clamps written out (`if (x < lo) x = lo; else if (x > hi) x = hi;`): GFLimit<float,float,float> (0x0048DAE0) is plain inline and
//     its trace scores here (83 / 93) would leave it called; the turn limit calls GetMaxTurnSpeed four times as retail does.
//   - GFGetRoundedSignedAngle is FABLE_FORCEINLINE (Inline-budget below); GFGetAngleXYTo's header body is visible (not expanded) so the
//     target position does not escape and shares its frame word with the z-target direction.
//   - Main arm: direction.Initialise(MovementAcceleration.X, MovementAcceleration.Y, 0.0f) and the sign from (facing ^ direction).Z: the
//     only spellings measured that give retail's x87 operand orders in the movement check, the squared magnitude, the dot and the cross
//     product (c2 record-id tie-breaks; free trace sweeps, NOTES.md). The facing angle is read before GetAngleXY (call first).
// Inline-budget: FABLE_FORCEINLINE GFGetRoundedSignedAngle (engine/GFAngles.h overlay) -- plain-inline trace c1 87, f 0.041, score 377;
//   87 * 6/7 > 74 at any frequency, retail expands it (0x006AC0B7..0x006AC10D; unreferenced out-of-line copy 0x006ADDB0).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGCREATUREBASE_ANIMATION
#define FABLE_RETAIL_CTHINGPLAYERCREATURE
#define FABLE_RETAIL_CTHINGPLAYERCREATURE_MAXTURNSPEED
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_ANGLE_BODIES
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTCPHYSICSBASE_LASTPOSITION
#define FABLE_RETAIL_CRIGHTHANDEDSET
#define FABLE_RETAIL_CRIGHTHANDEDSET_CTOR_INLINE
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE_INTERRUPTION
#define FABLE_RETAIL_CTCSTEALTH_SNEAKTOGGLE
#define FABLE_RETAIL_CTCSTEALTH_STATTYPE
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CAIGAMECAMERABASE
#define FABLE_RETAIL_CGAMECAMERAMANAGER
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_PLAYER_ISPCPROJECTILEAIMING
#define FABLE_RETAIL_GFANGLES_SIGNED_ROUNDING
#define FABLE_RETAIL_GFANGLES_ANGLEXYTO_BODY
#include "fable_string.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CRightHandedSet.h"
#include "engine/CCreatureActionBase.h"
#include "engine/CTCStealth.h"
#include "engine/CTCTargetingBase.h"
#include "engine/CWorld.h"
#include "engine/CAIGameCameraBase.h"
#include "engine/CGameCameraManager.h"
#include "engine/CCamera.h"
#include "engine/CPlayer.h"
#include "engine/NGraphicAppearance_CTCGraphicAppearance.h"
#include "engine/GFAngles.h"
#include "engine/CPlayerMovementDef.h"

// GFSmoothTowards<float> (Ego_r / FableWin ??$GFSmoothTowards@M@@YIXABM0AAM@Z): the landed body of 0x006ADC20, instantiated in this TU
// and called (trace score 565).
template <class T>
void FABLE_FASTCALL GFSmoothTowards(const T& target, const T& max_step, T& current)
{
    T step = target - current;
    if (step < -max_step)
        step = -max_step;
    else if (step > max_step)
        step = max_step;
    current += step;
    if (step > 0) {
        if (current > target)
            current = target;
    } else {
        if (current < target)
            current = target;
    }
}

void CThingPlayerCreature::ResolveFacingDirection()
{
    const CCreatureActionBase* action = PeekPCurrentAction();
    if (action && !action->IsFinished() && action->IsPreventingNormalMovement()) {
        LeanAngle = 0.0f;
        GetPhysicsTC()->SetRHSet(CRightHandedSet(C3DVector(0.0f, 0.0f, 1.0f), GetPhysicsTC()->GetFacingVector()));
        return;
    }
    const CTCStealth* stealth;
    bool sneaking = PeekTC(&stealth) && stealth->IsSneaking();
    if (GetPlayer().IsPCProjectileWeaponThirdPersonAimingMode()) {
        const CGameCameraManager* manager;
        if (GetWorld().GetGameCamera()->GetCameraManager(&manager)) {
            C3DVector camera_direction = manager->GetCamera().PeekViewDirection();
            camera_direction.Z = 0.0f;
            if (camera_direction.GetSquaredMagnitude2D() > DELTA_NOTIONAL_ZERO * DELTA_NOTIONAL_ZERO) {
                camera_direction.AccurateNormalise();
                GetPhysicsTC()->SetRHSet(CRightHandedSet(C3DVector(0.0f, 0.0f, 1.0f), camera_direction));
            }
        }
        LeanAngle = 0.0f;
        return;
    } else if (GetPlayer().IsFirstPersonTargeting()) {
        const CTCTargetingBase* targeting;
        PeekTC(&targeting);
        C3DVector targeted_pos;
        targeting->GetTargetedPos(targeted_pos, 1.0f);
        C3DVector direction = targeted_pos - GetPos();
        direction.Z = 0.0f;
        if (direction.GetSquaredMagnitude2D() > DELTA_NOTIONAL_ZERO * DELTA_NOTIONAL_ZERO) {
            direction.AccurateNormalise();
            GetPhysicsTC()->SetRHSet(CRightHandedSet(C3DVector(0.0f, 0.0f, 1.0f), direction));
        }
        LeanAngle = 0.0f;
        return;
    } else if (GetPlayer().IsZTargeting()) {
        const CTCTargetingBase* targeting;
        PeekTC(&targeting);
        CThing* target = targeting->GetPTargetLockedThing();
        if (target) {
            C3DVector target_pos = target->GetTCGraphicAppearance() ? target->GetTCGraphicAppearance()->GetFocalPos(1.0f) : target->GetPos();
            float angle = GFGetAngleXYTo(PeekPos(), target_pos);
            C3DVector direction;
            GetPhysicsTC()->SetRHSet(CRightHandedSet(C3DVector(0.0f, 0.0f, 1.0f), direction.InitialiseFromAngles(angle, 0.0f, 1.0f)));
        } else
            GetPhysicsTC()->SetRHSet(CRightHandedSet(C3DVector(0.0f, 0.0f, 1.0f), GetPhysicsTC()->GetFacingVector()));
        LeanAngle = 0.0f;
        return;
    } else {
        if (MovementAcceleration.GetSquaredMagnitude() > DELTA_NOTIONAL_ZERO * DELTA_NOTIONAL_ZERO) {
            C3DVector facing = GetPhysicsTC()->GetFacingVector();
            C3DVector direction;
            direction.Initialise(MovementAcceleration.X, MovementAcceleration.Y, 0.0f);
            if (direction.GetSquaredMagnitude() > DELTA_NOTIONAL_ZERO * DELTA_NOTIONAL_ZERO)
                direction.AccurateNormalise();
            float cos_angle = facing.Dot(direction);
            if (cos_angle < -1.0f)
                cos_angle = -1.0f;
            else if (cos_angle > 1.0f)
                cos_angle = 1.0f;
            float angle = GFArcCos(cos_angle);
            float turn = (((facing ^ direction).Z > 0.0f) ? -1.0f : 1.0f) * angle;
            float desired_turn = turn * -1.0f;
            if (turn < GetMaxTurnSpeed() * -(1.0f / 360.0f))
                turn = GetMaxTurnSpeed() * -(1.0f / 360.0f);
            else if (turn > GetMaxTurnSpeed() * (1.0f / 360.0f))
                turn = GetMaxTurnSpeed() * (1.0f / 360.0f);
            float actual_turn = turn * -1.0f;
            facing.Rotate(C3DVector(0.0f, 0.0f, 1.0f), turn);
            GetPhysicsTC()->SetRHSet(CRightHandedSet(C3DVector(0.0f, 0.0f, 1.0f), facing));
            float lean_target = 0.0f;
            // MovementBandType 3 / 4 (EMovementBandType: no enumerator names on disk) are the running / jogging bands of
            // GetCurrentMaxMovementSpeed 0x006AB5D0; 1 / 2 are the slow and walking bands.
            if (!sneaking && (MovementBandType == (EMovementBandType)4 || MovementBandType == (EMovementBandType)3)
                && desired_turn == actual_turn && fabs(actual_turn) > 4.0f / 360.0f)
                lean_target = MovementAcceleration.GetMagnitude() * actual_turn
                              / (PPlayerMovementDef->LeanAngleTurnFactor * (1.0f / 360.0f)) * 0.45f;
            float lean_step = PPlayerMovementDef->LeanAngleStepPerFrame * (1.0f / 360.0f);
            if ((GetPhysicsTC()->GetPosition() - GetPhysicsTC()->GetLastPosition()).GetSquaredMagnitude() < 0.05f) {
                LeanAngle = 0.0f;
                GetPhysicsTC()->SetRHSet(CRightHandedSet(C3DVector(0.0f, 0.0f, 1.0f), GetPhysicsTC()->GetFacingVector()));
            } else
                GFSmoothTowards(lean_target, lean_step, LeanAngle);
            float lean_limit = PPlayerMovementDef->LeanAngleLimitAngle * (1.0f / 360.0f);
            if (LeanAngle < -lean_limit)
                LeanAngle = -lean_limit;
            else if (LeanAngle > lean_limit)
                LeanAngle = lean_limit;
            if (fabs(LeanAngle) > DELTA_NOTIONAL_ZERO) {
                C3DVector up(0.0f, 0.0f, 1.0f);
                C3DVector facing_now = GetPhysicsTC()->GetFacingVector();
                up.Rotate(facing_now, LeanAngle);
                GetPhysicsTC()->SetRHSet(CRightHandedSet(up, facing_now));
            }
        } else {
            LeanAngle = 0.0f;
            GetPhysicsTC()->SetRHSet(CRightHandedSet(C3DVector(0.0f, 0.0f, 1.0f), GetPhysicsTC()->GetFacingVector()));
        }
        if (MovementBandType == (EMovementBandType)1)
            MovementAcceleration.X = MovementAcceleration.Y = MovementAcceleration.Z = 0.0f;
        else if (sneaking || MovementBandType == (EMovementBandType)2) {
            float facing_angle = GetPhysicsTC()->GetFacingAngleXY();
            if (fabs(GFGetRoundedSignedAngle(facing_angle - MovementAcceleration.GetAngleXY())) > 1.0f / 360.0f)
                MovementAcceleration.X = MovementAcceleration.Y = MovementAcceleration.Z = 0.0f;
        }
    }
}
