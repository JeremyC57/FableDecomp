// CTCPhysicsStandard::Move() -- one physics step of a moving thing: apply the forces, measure the speed, gather the things in the area the move can reach, step the position (after the impassability check when the def asks for it), then while still moving restore the speed when the def preserves kinetic energy, settle on the surface, update the moving flag and commit the new position
// Leaf: ?Move@CTCPhysicsStandard@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) 0x008E5D40, 441 bytes, thiscall plain `ret` (public virtual; CTCPhysicsStandard vtable 0x01265F84).
// Names: Ego_r.pdb / FableWin.pdb ?Move@CTCPhysicsStandard@@UAEXXZ; Ego_d.pdb tc_physics_standard_movement.obj (first function) locals
//   area / preserve_ke / current_pos / speed / thing_list; flags PHYSICS_PRESERVE_KE (128) and
//   PHYSICS_STANDARD_COLLIDE_WITH_IMPASSABLE_LANDSCAPE (512) of Ego_d's EPhysicsInteractionType (written as their values: the enum is
//   not in a shared header yet). Callees: PerformForceUpdates 0x008E5670, GetPossibleInteractedWithList 0x008E57D0,
//   UpdateVelocityForImpassability 0x007243C0, PerformStepBasedMovement 0x008E5880, UpdateIsMoving 0x008E5270, virtual slots 98
//   GetPossibleCollidedWithArea (+0x188), 97 UpdateOnSurface (+0x184), 30 SetPosition (+0x78); free 0x00BFEA14 (vector storage).
// Shape: FableWin.pdb's inline accessors IsMoving / GetVelocity / GetPhysicsDef (with them the row is FableWin-size at /Od /EHsc,
//   120i/379b); the second moving test is an early return after the step -- FITTED: the FableWin.pdb / Ego_d.pdb debug size (379
//   bytes) is the `if (IsMoving()) { ... }` form, but that form packs current_pos below thing_list (0x008E5DAA / 0x008E5DD0 slots
//   swapped, also with an 11-instruction allocator::deallocate text); the early return adds a destructor reference to thing_list
//   and gives retail's frame. 4 /Od instructions longer than FableWin. x87 orders: FITTED C3DVector texts GetMagnitude X*X+(Y*Y+Z*Z) and Normalise
//   from locals summed Y, Z, X (overlay opt-ins FABLE_RETAIL_C3DVECTOR_MAGNITUDE_X_THEN_YZ / _NORMALISE_LOCALS_YZX).
#include <math.h>
#pragma intrinsic(sqrt)
#define FABLE_RETAIL_CTCPHYSICSSTANDARD
#define FABLE_RETAIL_CTCPHYSICSSTANDARD_IMPULSE
#define FABLE_RETAIL_CTCPHYSICSSTANDARD_MOVE
#define FABLE_RETAIL_CTCPHYSICSSTANDARD_MOVE_OVERRIDE
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTCPHYSICSBASE_COLLISION_RESPONSE
#define FABLE_RETAIL_CTCPHYSICSBASE_MOVE_ACCESSORS
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_MAGNITUDE_X_THEN_YZ
#define FABLE_RETAIL_C3DVECTOR_NORMALISE_LOCALS_YZX
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#include "engine/CThing.h"
#include "engine/C3DVector.h"
#include "engine/CPhysicsDef.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CTCPhysicsStandard.h"

void CTCPhysicsStandard::Move()
{
    if (!IsMoving())
        return;
    PerformForceUpdates();
    float speed = GetVelocity().GetMagnitude();
    bool preserve_ke = (GetPhysicsDef().InteractionFlags & 0x80) != 0;            // PHYSICS_PRESERVE_KE
    C2DBoxI area = GetPossibleCollidedWithArea();
    std::vector<CThing*> thing_list;
    GetPossibleInteractedWithList(area, thing_list, speed);
    C3DVector current_pos(Position);
    if (GetPhysicsDef().InteractionFlags & 0x200)                                 // PHYSICS_STANDARD_COLLIDE_WITH_IMPASSABLE_LANDSCAPE
        UpdateVelocityForImpassability(current_pos, Velocity);
    PerformStepBasedMovement(thing_list, current_pos);
    if (!IsMoving())
        return;
    if (preserve_ke) {
        Velocity.Normalise();
        Velocity *= speed;
    }
    UpdateOnSurface(thing_list, current_pos);
    UpdateIsMoving();
    SetPosition(current_pos);
}