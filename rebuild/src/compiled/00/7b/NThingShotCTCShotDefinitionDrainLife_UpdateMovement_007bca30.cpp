#define FABLE_RETAIL_C3DVECTOR_SQUARE_GROUPED_PASS23
// Original full shared owner, names and named math/accessor calls from actual donor.
// Leaf: ?UpdateMovement@CTCShotDefinitionDrainLife@NThingShot@@MAEXXZ
// Flags: /O1 /Oy-
#define FABLE_RETAIL_SHOT_UPDATEMOVEMENT_PASS26
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTCPHYSICSBASE_COLLISION_RESPONSE
#define FABLE_RETAIL_CTCPHYSICSBASE_MOVE_ACCESSORS
#define FABLE_RETAIL_CTCPHYSICSBASE_SETVELOCITY_OUTOFLINE
#define FABLE_RETAIL_CTCPHYSICSBASE_APPLYACCELERATION
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_GET_ORIGINAL_CHAIN
#define FABLE_RETAIL_CTCSHOTDEFINITIONBASE_MOVEMENT
#define FABLE_RETAIL_CTCSHOTDEFINITIONBASE_SHOTACCESSORS
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_ORIGINAL_COMPONENT_CTOR
#define FABLE_RETAIL_C3DVECTOR_ORIGINAL_COPY_FORMAL
#define FABLE_RETAIL_C3DVECTOR_NORMALISE_SQUARE_HELPER
#define FABLE_RETAIL_C3DVECTOR_SQUARE_ORIGINAL_FORMAL
#define FABLE_RETAIL_GFFASTONEOVERSQRT_ORIGINAL_UNION
#define FABLE_RETAIL_C3DVECTOR_CROSS_ORIGINAL_FORMAL
#define FABLE_RETAIL_C3DVECTOR_DOT_ORIGINAL_SCOPE_PASS23
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_IS_THING_ALIVE_NOINLINE
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#include "engine/CThing.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/NThingShot_CTCShotDefinitionDrainLife.h"
#include "engine/NThingShotHelpers.h"
#include "engine/ThingAlive.h"
#include "engine/CMainGameComponent.h"
#pragma optimize("s",on)
#pragma optimize("y",off)
namespace NThingShot {
void CTCShotDefinitionDrainLife::UpdateMovement()
{
 const CThing* ptarget=GetPTargetThing();
 if(IS_THING_ALIVE(ptarget)) {
  CTCPhysicsBase* ptc_phys=GetParentThing().GetPhysicsTC();
        C3DVector velocity=ptc_phys->GetVelocity();
        velocity.Normalise();
        C3DVector position=ptc_phys->GetPosition();
        C3DVector fire_at_pos=GetCreatureFireAtPos(*ptarget);
        C3DVector to_target=fire_at_pos-position;
        to_target.Normalise();
        float cos_theta=velocity.Dot(to_target);
        if(cos_theta<MaxCosAngleToTurn) {
            C3DVector cross=to_target.Cross(velocity);
            velocity.Rotate(cross,MaxAngleToTurn);
        } else velocity=to_target;
        velocity*=Speed;
        ptc_phys->SetVelocity(velocity);
 }
}
}