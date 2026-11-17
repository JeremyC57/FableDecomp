// CTCPhysicsControlled::Move() -- one controlled physics step: honour a pending forced move (teleport, or fold it into the impulse), gather the things the impulse and velocity can reach, apply the impulse and step movement, then commit the position and update the additional impulse
// Leaf: ?Move@CTCPhysicsControlled@@UAEXXZ
// Plan phase 4 (2026-11-05), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12). Also: local GetLengthSquared respelled GetSquaredMagnitude (the header / FableWin.pdb method, same computation); local GetLength respelled GetMagnitude (the header / FableWin.pdb method, same computation).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00725C40, 499 bytes (+ int3 padding), plain `ret`.
// Callees: CTCPhysicsStandard::PerformForceUpdates 0x008E5670, GetPossibleInteractedWithList 0x008E57D0,
//   PerformStepBasedMovement 0x008E5880, UpdateIsMoving 0x008E5270, UpdateVelocityForImpassability 0x007243C0;
//   CTCPhysicsControlled::AdjustThingListForDebugHelpers 0x007254B0, PerformImpulseBasedMovement 0x00725500,
//   UpdateAdditionalImpulse 0x00725B70; virtual slots 30 SetPosition (+0x78), 97 UpdateOnSurface (+0x184),
//   98 GetPossibleCollidedWithArea (+0x188, C2DBoxI by hidden pointer) -- CTCPhysicsStandard vtable 0x01265F84;
//   free 0x00BFEA14 (the vector<CThing*> storage). Constants 0.5f 0x0122F59C, 4.5f 0x01266308.
// Layout: CTCPhysicsBase Position +0x0c, Velocity +0x28 (engine/CTCPhysicsBase.h); CTCPhysicsControlled ImpulseVelocity
//   +0x88, ForcedMoveToPos +0x94, ForcedMoveToPosSet/UsePhysics +0xa0 bits 0/1 (PDB offsets; retail CTCPhysicsStandard
//   base is the PDB 0x88). The owner thing's interface bits 0xA8/0xA9 are one `test ch,3` on m_presentBits[5].
// Shape: the forced teleport `goto`s the shared SetPosition tail (retail `jmp`); the impulse length is a squared local
//   with the sqrt in the call expression (retail spills the square, sqrt late); the margin is `0.5f` overridden by an
//   `if` (retail `fld 0.5; je; fstp; fld 4.5`); the owner is a named const local (otherwise every later scratch
//   register rotates by one).
// Name-fix (agentNF55, round 7): the local CTCPhysicsControlled model (one struct, every callee a member of it) replaced by the
//   shared retail headers engine/CTCPhysicsControlled.h : CTCPhysicsStandard : CTCPhysicsBase, so the CTCPhysicsStandard steps bind
//   their Ego_r.pdb owner (UpdateVelocityForImpassability ?..@CTCPhysicsStandard@@IAEXABVC3DVector@@AAV2@@Z 0x007243C0,
//   PerformForceUpdates / UpdateIsMoving / PerformStepBasedMovement / GetPossibleInteractedWithList) and Move itself is the
//   public virtual override ?Move@CTCPhysicsControlled@@UAEXXZ; the vector is std::vector<CThing*>, the owner ParentThing.
#include <math.h>
#pragma intrinsic(sqrt)
#define FABLE_RETAIL_CTCPHYSICSCONTROLLED
#define FABLE_RETAIL_CTCPHYSICSCONTROLLED_MOVE
#define FABLE_RETAIL_CTCPHYSICSSTANDARD
#define FABLE_RETAIL_CTCPHYSICSSTANDARD_IMPULSE
#define FABLE_RETAIL_CTCPHYSICSSTANDARD_MOVE
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#include "engine/CThing.h"
#include "engine/C3DVector.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CTCPhysicsStandard.h"
#include "engine/CTCPhysicsControlled.h"

void CTCPhysicsControlled::Move()
{
    PerformForceUpdates();
    C3DVector position(Position);
    if (ForcedMoveToPosSet) {
        ForcedMoveToPosSet = false;
        if (!ForcedMoveToPosUsePhysics) {
            position = ForcedMoveToPos;
            ImpulseVelocity.Z = 0.0f;
            ImpulseVelocity.Y = 0.0f;
            ImpulseVelocity.X = 0.0f;
            goto commit;
        }
        ImpulseVelocity += ForcedMoveToPos - Position;
    }
    {
        C2DBoxI area = GetPossibleCollidedWithArea();
        float impulseSpeedSquared = ImpulseVelocity.GetSquaredMagnitude();
        std::vector<CThing*> things;
        float speed = Velocity.GetMagnitude();
        float margin = 0.5f;
        const CThing* owner = ParentThing;
        if (owner->HasTC(TCI_BATTLE_CHARGE) || owner->HasTC(TCI_ASSASSIN_RUSH))
            margin = 4.5f;
        GetPossibleInteractedWithList(area, things, (float)sqrt(impulseSpeedSquared) + margin + speed);
        AdjustThingListForDebugHelpers(things);
        PerformImpulseBasedMovement(things, position);
        ImpulseVelocity.Z = 0.0f;
        ImpulseVelocity.Y = 0.0f;
        ImpulseVelocity.X = 0.0f;
        UpdateVelocityForImpassability(position, Velocity);
        PerformStepBasedMovement(things, position);
        UpdateOnSurface(things, position);
        UpdateIsMoving();
    }
commit:
    SetPosition(position);
    UpdateAdditionalImpulse();
}
