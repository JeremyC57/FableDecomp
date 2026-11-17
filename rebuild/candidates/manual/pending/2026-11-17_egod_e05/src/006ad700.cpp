// CThingPlayerCreature::ControlledMove() -- update the walking control forces; while the game camera manager is not under scripted control and the hero has hit locations, push the hero (and drop the matching part of its movement acceleration) away from the camera when the camera sits inside the hero (projectile-weapon mode inside the bounding sphere, or inside a combat hit location); then hand the movement acceleration to ApplyImpulseVelocity (physics controlled impulse, or the impulse velocity) unless the current action prevents normal movement, and clear it when the action clears the movement vector
// Leaf: ?ControlledMove@CThingPlayerCreature@@IAEXXZ
// Retail: Fable.exe (TLC, VC7.1) 0x006AD700, 615 bytes, thiscall `ret`. Grade: RELOCATION_MATCH (e05 egod lane).
// Names: Ego_r.pdb 0x0054A890 / FableWin.pdb 0x01DF4A90 ?ControlledMove@CThingPlayerCreature@@IAEXXZ (protected, Ego_d class record);
//   Ego_d thing_player_creature.obj locals verbatim: function scope paction (const CCreatureActionBase*), pcam_man (const
//   CGameCameraManager*), ptc_hit_loc (const CTCHitLocations*); block cam_pos (const C3DVector&), centre_pos, radius; inner block
//   line_out2d (C3DVector), IMPULSE_MAG (const float).
// Callees: UpdateWalkingControlForces 0x006AD260 (landed; the old draft's "ComputeAcceleration"), CThing::GetWorld 0x004C7AA0,
//   CWorld::PCurrentGameCamera +0x34 -> CAIGameCameraBase slot 10 GetCameraManager(const CGameCameraManager**) const,
//   CVectorMap::LowerBound const 0x004365B0 (PeekTC<const CTCHitLocations>, TCI_HIT_LOCATIONS 127), CTCHitLocations::GetBoundingSphere
//   0x00815420, CGameCameraManager::GetCameraModeType 0x00697AB0 (8 = Ego_d NCameraModes::CAMERA_MODE_SCRIPTED_CONTROL; the enum's
//   enumerators are in no repo header, so the value is written as the literal), CThing::GetPlayer 0x004C7A30, CPlayer::GetCurrentMode
//   0x00633BE0 (PLAYER_MODE_USE_PROJECTILE_WEAPON 25), CTCHitLocations::GetPCombatFirstHitLocationPosInside const 0x00815870 (`jmp` to
//   the non-const 0x008153C0), C3DVector::AccurateNormalise 0x00A14440, CTCPhysicsBase slot 81 (+0x144) ApplyControlledImpulse,
//   CCreatureActionBase slots 49 (+0xC4) IsPreventingNormalMovement / 35 (+0x8C) IsClearingMovementVector. DELTA_NOTIONAL_ZERO
//   0x0129BA3C; IMPULSE_MAG's 0.1f is the .rdata constant 0x0122EE58.
// Same-TU body: ApplyImpulseVelocity (Ego_r 0x00546450 / FableWin 0x01DED830, Ego_d param `impulse`, defined before the row in Ego_d
//   order; expanded in retail 0x006AD8F8..0x006AD936): the physics branch lives in it, which is why retail forms
//   `lea eax,[esi+180h]` once ahead of the PhysicsTC test (0x006AD8FD) and reads impulse through eax in the else arm. FITTED text:
//   `if (GetPhysicsTC()) GetPhysicsTC()->ApplyControlledImpulse(impulse); else ImpulseVelocity += impulse;` (/Od 26i/76b against
//   FableWin 27i/78b; the plain `ImpulseVelocity += impulse` is 12i).
// Headers: hdr/ opt-ins FABLE_RETAIL_CTHINGPLAYERCREATURE_CONTROLLEDMOVE (the three declarations), FABLE_RETAIL_CTCHITLOCATIONS_POSINSIDE
//   (the two PosInside overloads), FABLE_RETAIL_CTHINGCREATUREBASE_PEEKPCURRENTACTION_GET (PeekPCurrentAction through
//   CCountedPointer::Get: FableWin 10i/25b = this text at /Od, the `.Data` text 9i/20b; it closed the last residue, the operand order
//   of `fld [eax] ; fadd [esi+1F0h]` at 0x006AD910, an x87 record-id order); existing FABLE_RETAIL_C3DVECTOR_SQUARED2D_ONE_EXPRESSION_PASS23
//   (retail squares Y then X at 0x006AD803) and FABLE_RETAIL_C3DVECTOR_SUBTRACT_ASSIGN.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGCREATUREBASE_ANIMATION
#define FABLE_RETAIL_CTHINGCREATUREBASE_PEEKPCURRENTACTION_GET
#define FABLE_RETAIL_CTHINGPLAYERCREATURE
#define FABLE_RETAIL_CTHINGPLAYERCREATURE_CONTROLLEDMOVE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CVECTORMAP_FORCEINLINE_FIND
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_SQUARED2D_ONE_EXPRESSION_PASS23
#define FABLE_RETAIL_C3DVECTOR_SUBTRACT_ASSIGN
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CAIGAMECAMERABASE
#define FABLE_RETAIL_CGAMECAMERAMANAGER
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE_INTERRUPTION
#define FABLE_RETAIL_CTCHITLOCATIONS_POSINSIDE
#define FABLE_RETAIL_PLAYER
#include "fable_string.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CTCHitLocations.h"
#include "engine/CCreatureActionBase.h"
#include "engine/CWorld.h"
#include "engine/CAIGameCameraBase.h"
#include "engine/CGameCameraManager.h"
#include "engine/CCamera.h"
#include "engine/CPlayer.h"

void CThingPlayerCreature::ApplyImpulseVelocity(const C3DVector& impulse)
{
    if (GetPhysicsTC())
        GetPhysicsTC()->ApplyControlledImpulse(impulse);
    else
        ImpulseVelocity += impulse;
}

void CThingPlayerCreature::ControlledMove()
{
    UpdateWalkingControlForces();
    const CCreatureActionBase* paction;
    const CGameCameraManager* pcam_man;
    const CTCHitLocations* ptc_hit_loc;
    if (GetWorld().GetGameCamera()->GetCameraManager(&pcam_man) && PeekTC(&ptc_hit_loc)) {
        const C3DVector& cam_pos = pcam_man->GetCamera().PeekWorldPos();
        C3DVector centre_pos;
        float radius;
        ptc_hit_loc->GetBoundingSphere(centre_pos, radius, true);
        if (pcam_man->GetCameraModeType() != 8) {
            if ((GetPlayer().GetCurrentMode() == PLAYER_MODE_USE_PROJECTILE_WEAPON && (cam_pos - centre_pos).GetSquaredMagnitude() < radius * radius)
                || ptc_hit_loc->GetPCombatFirstHitLocationPosInside(cam_pos)) {
                C3DVector line_out2d = centre_pos - cam_pos;
                line_out2d.Z = 0.0f;
                if (line_out2d.GetSquaredMagnitude2D() > DELTA_NOTIONAL_ZERO * DELTA_NOTIONAL_ZERO)
                    line_out2d.AccurateNormalise();
                const float IMPULSE_MAG = 0.1f;
                PhysicsTC->ApplyControlledImpulse(line_out2d * IMPULSE_MAG);
                MovementAcceleration -= line_out2d * line_out2d.Dot(MovementAcceleration);
            }
        }
    }
    paction = PeekPCurrentAction();
    if (!paction || paction->IsFinished() || !paction->IsPreventingNormalMovement()) {
        ApplyImpulseVelocity(MovementAcceleration);
    }
    if (paction && paction->IsClearingMovementVector())
        MovementAcceleration.X = MovementAcceleration.Y = MovementAcceleration.Z = 0.0f;
}
