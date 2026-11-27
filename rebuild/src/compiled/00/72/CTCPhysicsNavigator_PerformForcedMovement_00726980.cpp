// 00726980 CTCPhysicsNavigator::PerformForcedMovement
// Leaf: ?PerformForcedMovement@CTCPhysicsNavigator@@QAEXAAV?$vector@PAVCThing@@V?$allocator@PAVCThing@@@std@@@std@@ABVC3DVector@@AAV4@@Z
// Flags:
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTCPHYSICSSTANDARD
#define FABLE_RETAIL_CTCPHYSICSSTANDARD_MOVE
#define FABLE_RETAIL_CTCPHYSICSSTANDARD_IMPULSE
#define FABLE_RETAIL_CTCPHYSICSNAVIGATOR
#define FABLE_RETAIL_CTCPHYSICSNAVIGATOR_MOVE_PASS27
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_PACKED
#define FABLE_RETAIL_C3DVECTOR_ORIGINAL_COMPONENT_CTOR
#define FABLE_RETAIL_C3DVECTOR_ORIGINAL_COPY_FORMAL
#define FABLE_RETAIL_C3DVECTOR_ISNONZERO_ORIGINAL_YXZ
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C2DVECTOR_FROM3D
#define FABLE_RETAIL_C2DVECTOR_ORIGINAL_COPIES_PASS26
#define FABLE_RETAIL_C3DVECTOR_NORMALISE_LOCALS_XYZ
#define FABLE_RETAIL_CTCPHYSICSBASE_IMPULSE
#define FABLE_RETAIL_CPHYSICSCOLLIDERESPONSE_CTOR
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "fable_assert.h"
#include "engine/CThing.h"
#include "engine/CPhysicsCollideResponse.h"
#include "engine/C3DVector.h"
#include "engine/C2DVector.h"
#include "engine/CWorld.h"
#include "engine/CTCPhysicsNavigator.h"
#include "engine/GFSqr.h"


// FableWin's debug body loads this exact read-only public for its speed test.
extern const float DELTA_NOTIONAL_ZERO;

void CTCPhysicsNavigator::PerformForcedMovement(std::vector<CThing*>& thing_list, const C3DVector& impulse, C3DVector& current_pos)
{
    bool hit_ground = false;
    if (!thing_list.empty() || CanCollideWithLandscape()) {
        float speed = impulse.GetMagnitude();
        C3DVector velocity = impulse;
        C3DVector velocity_step = impulse;
        CPhysicsCollideResponse response;
        long count = 0;
        long no_interaction_steps = static_cast<long>(speed * 12.0f);
        if (no_interaction_steps < 1) no_interaction_steps = 1;
        velocity_step /= static_cast<float>(no_interaction_steps);
        bool still_active = speed > DELTA_NOTIONAL_ZERO;
        while (still_active) {
            UpdateVelocityForImpassability(current_pos, velocity_step);
            C3DVector desired_pos = current_pos + velocity_step;
            bool collided = false;
            std::vector<CThing*>::iterator test_thing_it;
            for (test_thing_it = thing_list.begin(); test_thing_it != thing_list.end(); ++test_thing_it) {
                CThing& test_thing = **test_thing_it;
                CTCPhysicsBase& physics_tc = *test_thing.GetPhysicsTC();
                if (HasCollidedWithThingFromTo(physics_tc,current_pos,desired_pos,response)) {
                    if (IsCollisionWithThingToCauseResponse(physics_tc)) {
                        if (physics_tc.IsPhysicsMeshTypeCylinder()) {
                            collided = ImpulseRespondToCollisonWithCylinder(response,velocity_step);
                        } else {
                            collided = true;
                            ModifyResponseForNearVerticalSlopes(response);
                            ImpulseRespondToCollision(response,velocity_step);
                        }
                    }
                    UpdateCollisionGameInteractions(test_thing,response);
                }
            }
            if (CanCollideWithLandscape() && HasCollidedWithLandscapeAt(desired_pos,response)) {
                desired_pos = response.CollidePos;
                ImpulseRespondToCollision(response,velocity_step);
                hit_ground = true;
            }
            ++count;
            if (collided) {
                if (count == 1 && no_interaction_steps == 1) no_interaction_steps = 3;
            } else {
                current_pos = desired_pos;
            }
            if (count >= no_interaction_steps) still_active = false;
        }
    }
    if (hit_ground) UpdateCollisionGroundIneractions(current_pos);
}