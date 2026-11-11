// NCreatureMode::GetStrafingAnimationName(const CThing&, EMovementType) -- strafe animation name for the thing's current movement and facing ("STRAFE_IDLE" in player mode 0x19)
// Leaf: ?GetStrafingAnimationName@NCreatureMode@@YI?AVCCharString@@ABVCThing@@W4EMovementType@@@Z
// Retail 0x008E8FF0, 0x87 bytes, __fastcall (hidden CCharString result in ecx, thing in edx, movement type on the stack,
//   `ret 4`). Manifest name matches the FableWin PDB name set
//   (`?GetStrafingAnimationName@NCreatureMode@@YI?AVCCharString@@ABVCThing@@W4EMovementType@@@Z`, the overload beside
//   the (C3DVector, C3DVector, EMovementType) one at 0x008E8E50).
// Callees: CThing::GetPlayer @ 0x004C7A10, CPlayer::GetCurrentMode @ 0x00633BE0, CCharString(const char*, long) @
//   0x0099EBF0 ("STRAFE_IDLE" @ 0x01287C64), the physics TC's virtual GetFacingVector() const (slot 70, +0x118, hidden
//   C3DVector result; CTCPhysicsBase's own body returns (0, 1, 0)), GetStrafingAnimationName(const C3DVector&,
//   const C3DVector&, EMovementType) @ 0x008E8E50.
// Layout: CThing +0x10 thing type byte (2), +0x60 the physics component; CTCPhysicsBase Position +0x0C, OldPosition +0x18
//   (Ego_r.pdb).
// Shape (chunk 892): the TU's own (C3DVector, C3DVector, EMovementType) overload 0x008E8E50 is defined first, as in retail's
//   TU (it sits just below this row). With its body visible c2 knows it only READS `movement`, so the three delta stores no
//   longer order the physics vtable load: the list scheduler hoists `mov eax,[ecx]` between the first `fld` and `fsub`
//   (0x008E9030) as retail does. Declared only, the callee may capture `movement` and the load waits for the stores.
// Header debt: CThing's type byte (+0x10), PhysicsTC (+0x60) and CTCPhysicsBase's Position/OldPosition and slot-70
//   GetFacingVector are local models (the Ego_r-generated engine/CThing.h is 0x10 off for TLC at +0x10; landed CTCStealth
//   0x00747DA0 / IsAwareOfHero 0x008D0BA0 model the type byte the same way).
#include "rebuild_abi.h"
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_C3DVECTOR
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/EPlayerMode.h"
#include <math.h>

enum EMovementType { MOVEMENT_TYPE_0 };

class CTCPhysicsBase {
public:
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09();
    virtual void v10();
    virtual void v11();
    virtual void v12();
    virtual void v13();
    virtual void v14();
    virtual void v15();
    virtual void v16();
    virtual void v17();
    virtual void v18();
    virtual void v19();
    virtual void v20();
    virtual void v21();
    virtual void v22();
    virtual void v23();
    virtual void v24();
    virtual void v25();
    virtual void v26();
    virtual void v27();
    virtual void v28();
    virtual void v29();
    virtual void v30();
    virtual void v31();
    virtual void v32();
    virtual void v33();
    virtual void v34();
    virtual void v35();
    virtual void v36();
    virtual void v37();
    virtual void v38();
    virtual void v39();
    virtual void v40();
    virtual void v41();
    virtual void v42();
    virtual void v43();
    virtual void v44();
    virtual void v45();
    virtual void v46();
    virtual void v47();
    virtual void v48();
    virtual void v49();
    virtual void v50();
    virtual void v51();
    virtual void v52();
    virtual void v53();
    virtual void v54();
    virtual void v55();
    virtual void v56();
    virtual void v57();
    virtual void v58();
    virtual void v59();
    virtual void v60();
    virtual void v61();
    virtual void v62();
    virtual void v63();
    virtual void v64();
    virtual void v65();
    virtual void v66();
    virtual void v67();
    virtual void v68();
    virtual void v69();
    virtual C3DVector GetFacingVector() const;  // slot 70 (+0x118)
    unsigned char _pad04[0x8];
    C3DVector Position;                         // +0x0c
    C3DVector OldPosition;                      // +0x18
};

class CPlayer {
public:
    EPlayerMode GetCurrentMode() const;   /* ?GetCurrentMode@CPlayer@@QBE?AW4EPlayerMode@@XZ */                // 0x00633BE0 (EPlayerMode)
};

class CThing {
public:
    unsigned char   _pad00[0x10];
    unsigned char   ThingType;                  // +0x10 (2 = a creature that can be player-controlled)
    unsigned char   _pad11[0x60 - 0x11];
    CTCPhysicsBase* PhysicsTC;                  // +0x60
    const CPlayer& PeekPlayer() const;   // 0x004C7A10 ?PeekPlayer@CThing@@QBEABVCPlayer@@XZ
};

namespace NCreatureMode {
// 0x008E8E50 (FableWin.pdb / Ego_r.pdb ?GetStrafingAnimationName@NCreatureMode@@YI?AVCCharString@@ABVC3DVector@@0W4EMovementType@@@Z):
// the strafe animation for a movement delta seen from a facing: idle below 0.005 units, else forward / back when the
// flattened direction lies within 45 degrees of the flattened facing, left / right otherwise. Retail's own logic, out of
// line (auto_inline off) as in retail.
#pragma auto_inline(off)
CCharString FABLE_FASTCALL GetStrafingAnimationName(const C3DVector& movement, const C3DVector& facing,
                                                EMovementType movementType)
{
    float speed = movement.GetMagnitude();
    C3DVector direction(movement.X, movement.Y, 0.0f);
    if (speed > DELTA_NOTIONAL_ZERO)
        direction /= speed;
    C3DVector flatFacing(facing.X, facing.Y, 0.0f);
    if (flatFacing.IsNonZero())
        flatFacing.AccurateNormalise();                           // 0x00A14440
    float side = direction.X * flatFacing.Y - direction.Y * flatFacing.X;
    float forward = direction.Dot(flatFacing);
    CCharString name("STRAFE_IDLE");
    if (speed > 0.005f) {
        if (fabs(forward) > 1.0f / sqrt(2.0))
            name = forward < 0.0f ? "STRAFE_MOVEMENT_BACK" : "STRAFE_MOVEMENT_FORWARD";
        else
            name = side < 0.0f ? "STRAFE_MOVEMENT_LEFT" : "STRAFE_MOVEMENT_RIGHT";
    }
    return name;
}
#pragma auto_inline(on)

CCharString FABLE_FASTCALL GetStrafingAnimationName(const CThing& thing, EMovementType movementType)
{
    if (thing.ThingType == 2 && thing.PeekPlayer().GetCurrentMode() == 0x19)
        return CCharString("STRAFE_IDLE");
    const CTCPhysicsBase* physics = thing.PhysicsTC;
    C3DVector movement;
    movement.X = physics->Position.X - physics->OldPosition.X;
    movement.Y = physics->Position.Y - physics->OldPosition.Y;
    movement.Z = physics->Position.Z - physics->OldPosition.Z;
    C3DVector facing = physics->GetFacingVector();
    return GetStrafingAnimationName(movement, facing, movementType);
}
}