// CTCPhysicsBase::ImpulseRespondToCollision(const CPhysicsCollideResponse&, C3DVector&) const -- remove motion into an impact plane
// Grade: DIFFER(612v612) -- The original scoped axis locals on the shared physics headers improve the shape, but the response-basis computations and x87 operands differ.
// Leaf: ?ImpulseRespondToCollision@CTCPhysicsBase@@IBEXABVCPhysicsCollideResponse@@AAVC3DVector@@@Z
// Retail: 0x006AF7D0, 612 bytes, const thiscall.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTCPHYSICSBASE_COLLISION_RESPONSE
#define FABLE_RETAIL_CTCPHYSICSBASE_IMPULSE_COLLISION
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_SUBTRACT_ASSIGN
#include "fable_string.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CPhysicsCollideResponse.h"

extern bool g_flipMeshCollideNormals_013b86ec;

void CTCPhysicsBase::ImpulseRespondToCollision(const CPhysicsCollideResponse& response_info, C3DVector& current_velocity) const
{
    if (g_flipMeshCollideNormals_013b86ec) {
        C3DVector ax_z = response_info.CollideNormal;
        C3DVector ax_x = ax_z ^ (ax_z.Y > ax_z.Z ? C3DVector(0.0f, 0.0f, 1.0f) : C3DVector(0.0f, 1.0f, 0.0f));
        ax_x.AccurateNormalise();
        C3DVector ax_y = ax_z ^ ax_x;
        ax_y.AccurateNormalise();
        C3DVector velocity_in_new_coords;
        velocity_in_new_coords.Z = ax_z.Dot(current_velocity);
        velocity_in_new_coords.X = ax_x.Dot(current_velocity);
        velocity_in_new_coords.Y = ax_y.Dot(current_velocity);
        if (velocity_in_new_coords.Z < 0.0f)
            velocity_in_new_coords.Z = 0.0f;
        current_velocity = ax_z * velocity_in_new_coords.Z + ax_x * velocity_in_new_coords.X + ax_y * velocity_in_new_coords.Y;
    } else {
        float dot = response_info.CollideNormal.Dot(current_velocity);
        current_velocity -= response_info.CollideNormal * dot;
    }
}
