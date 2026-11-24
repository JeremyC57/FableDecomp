// Leaf: ?Move@CTCPhysicsNavigator@@UAEXXZ
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
#include "fable_assert.h"
#include "engine/C3DVector.h"
#include "engine/C2DVector.h"
#include "engine/CWorld.h"
#include "engine/CTCPhysicsNavigator.h"
#include "engine/GFSqr.h"

namespace
{
extern float DISTANCE_AWAY_TO_ALLOW_ONSURFACE_UPDATE_FOR_NAVIGATORS;
}

namespace NGlobalConsole
{
extern bool GJossTestPhysics;
}

void CTCPhysicsNavigator::Move()
{
    if (IsNavigationEnabled() && !ForcedMoveToPosSet && !AdditionalImpulse.IsNonZero()) {
        C3DVector lod_centre = GetWorld().GetGameLODCentre();
        C2DVector between_vector_2d(lod_centre - PositionToNavigateTo);
        if (between_vector_2d.GetSquaredMagnitude() < GFSqr(DISTANCE_AWAY_TO_ALLOW_ONSURFACE_UPDATE_FOR_NAVIGATORS)) {
            C2DBoxI area = GetPossibleCollidedWithArea();
            std::vector<CThing*> thing_list;
            GetPossibleInteractedWithList(area,thing_list,0.0f);
            UpdateOnSurface(thing_list,PositionToNavigateTo);
        }
        CTCPhysicsBase::SetPosition(PositionToNavigateTo);
    } else if (ForcedMoveToPosSet && !ForcedMoveToPosUsePhysics) {
        SetPosition(ForcedMoveToPos);
        ForcedMoveToPosSet = false;
    } else {
        C3DVector current_pos = GetPosition();
        float add_mag = AdditionalImpulse.GetMagnitude();
        if (add_mag > 0.2f) {
            AdditionalImpulse.Normalise();
            AdditionalImpulse *= 0.2f;
        }
        C3DVector impulse = AdditionalImpulse;
        if (ForcedMoveToPosSet) {
            FABLE_RETAIL_ASSERT(ForcedMoveToPosUsePhysics);
            impulse += ForcedMoveToPos - current_pos;
            ForcedMoveToPosSet = false;
        }
        PerformForceUpdates();
        C2DBoxI area = GetPossibleCollidedWithAreaWithImpulse(impulse);
        std::vector<CThing*> thing_list;
        float speed = impulse.GetMagnitude();
        GetPossibleInteractedWithList(area,thing_list,speed);
        if (!NGlobalConsole::GJossTestPhysics)
            RemoveIntersectingCreaturesFromList(thing_list);
        current_pos = Position;
        UpdateVelocityForImpassability(current_pos,impulse);
        PerformForcedMovement(thing_list,impulse,current_pos);
        UpdateVelocityForImpassability(current_pos,Velocity);
        PerformStepBasedMovement(thing_list,current_pos);
        UpdateOnSurface(thing_list,current_pos);
        SetPosition(current_pos);
    }
    UpdateAdditionalImpulse();
}