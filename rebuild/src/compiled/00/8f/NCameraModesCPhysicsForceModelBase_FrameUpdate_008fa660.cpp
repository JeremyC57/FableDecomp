// Sum the evasion, point attraction and damping forces.
// Leaf: ?FrameUpdate@CPhysicsForceModelBase@NCameraModes@@UAEXXZ
// Flags:
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CPHYSICSFORCEMODELBASE_SET_ACCELERATION_PASS27
#include "engine/NCameraModes_CPhysicsForceModelBase.h"
namespace NCameraModes {

void CPhysicsForceModelBase::FrameUpdate()
{
    C3DVector acceleration(0.0f, 0.0f, 0.0f);
    acceleration += -DoThingEvasion();
    acceleration += -DoGroundEvasion();
    acceleration += -DoThemeEvasion();
    acceleration += -DoCreatureEvasion();
    acceleration += DoAttractToPoint();
    acceleration += -DoVelocityDamping() * 1.0f;
    SetAcceleration(acceleration);
}

}