// NCameraModes::CPhysicsForceModelBase::FrameUpdate() -- sum the camera's force components (thing, ground, theme and creature evasion and velocity damping negated, the point attraction as is) and store the total as the linked physics state's acceleration
// Grade: DIFFER(ours v retail) -- 356v366 on engine/NCameraModes_CPhysicsForceModelBase.h (slot names DoThingEvasion..DoVelocityDamping): retail builds the first negated force as a temporary and copies it into the sum (Z through the frame, `fstp [esp+1Ch]; mov edx; mov [esp+10h],edx`, X/Y from the x87 stack), ours constructs the sum in place (copy elided); the assignment form copies all three words (386 bytes).
// Leaf: ?FrameUpdate@CPhysicsForceModelBase@NCameraModes@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008FA660, length 366 bytes, __thiscall, plain `ret` (vtable slot 0).
// Callees: the model's own virtual force getters, slots 2..7 (+0x08..+0x1C: DoThingEvasion, DoThemeEvasion, DoGroundEvasion,
//   DoCreatureEvasion, DoAttractToPoint, DoVelocityDamping), each returning a C3DVector through a hidden pointer.
#define FABLE_RETAIL_C3DVECTOR
#include "engine/NCameraModes_CPhysicsForceModelBase.h"
namespace NCameraModes {

void CPhysicsForceModelBase::FrameUpdate()
{
    C3DVector acceleration = -DoThingEvasion();
    acceleration += -DoGroundEvasion();
    acceleration += -DoThemeEvasion();
    acceleration += -DoCreatureEvasion();
    acceleration += DoAttractToPoint();
    acceleration += -DoVelocityDamping();
    LinkedState.Acceleration = acceleration;
}

}
