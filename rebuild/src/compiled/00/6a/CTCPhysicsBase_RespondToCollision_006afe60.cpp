// CTCPhysicsBase::RespondToCollision(const CPhysicsCollideResponse&,C3DVector&) const -- apply friction and bounce to collision velocity
// Retail: 0x006AFE60,145 bytes,thiscall
// Leaf: ?RespondToCollision@CTCPhysicsBase@@IBEXABVCPhysicsCollideResponse@@AAVC3DVector@@@Z
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTCPHYSICSBASE_COLLISION_RESPONSE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_C3DVECTOR
#include "engine/CTCPhysicsBase.h"
#include "engine/CPhysicsCollideResponse.h"
void CTCPhysicsBase::RespondToCollision(const CPhysicsCollideResponse& response_info,C3DVector& velocity) const
{
    const C3DVector& normal=response_info.CollideNormal;
    float friction=-GetFriction()*response_info.Friction;
    float elastic=GetElasticity();
    // Fidelity: value capture gives the original Z operand order at006afe7c.
    float velocity_z=velocity.Z;
    float dot=normal.Y*velocity.Y;
    dot+=normal.Z*velocity_z;
    dot+=normal.X*velocity.X;
    dot*=(-1.0f-friction-elastic);
    velocity+=velocity*friction+normal*dot;
}