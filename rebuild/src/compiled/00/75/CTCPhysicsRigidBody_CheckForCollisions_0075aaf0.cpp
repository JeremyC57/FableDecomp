// 0075aaf0 CTCPhysicsRigidBody::CheckForCollisions
// Leaf: ?CheckForCollisions@CTCPhysicsRigidBody@@AAE?AW4ECollideState@1@AAVCRigidBodyConfig@CRigidBody@@@Z
// Flags:
#define FABLE_RETAIL_RIGIDBODY_CHECK_COLLISIONS_PASS29
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTCPHYSICSRIGIDBODY
#define FABLE_RETAIL_CTCPHYSICSRIGIDBODY_BODY
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_CLEAR
#define FABLE_RETAIL_C3DVECTOR_ORIGINAL_COMPONENT_CTOR
#define FABLE_RETAIL_C3DVECTOR_ORIGINAL_COPY_FORMAL
#define FABLE_RETAIL_C3DVECTOR_PACKED
#define FABLE_RETAIL_C2DBOXI
#define FABLE_RETAIL_CTHINGSEARCHTOOLS
#define FABLE_RETAIL_CTCTARGETINGPLAYER_UPDATE_LIST
#define FABLE_RETAIL_CTCPHYSICSBASE_SEARCHTOOLS
#define FABLE_RETAIL_CTCPHYSICSBASE_WORLDMAP
#define FABLE_RETAIL_CWORLDMAP
#define FABLE_RETAIL_EXPLOSIVE_TRAIL_UPDATE_PASS28
#define FABLE_RETAIL_CWORLDMAP_GROUNDFACENORMAL
#define FABLE_RETAIL_CPLANE_NORMAL_CTOR_PASS23
#define FABLE_RETAIL_CPLANE_ACCESSORS

#define FABLE_RETAIL_CRIGIDBODYCONFIG_ASSIGN_OUTOFLINE
#define FABLE_RETAIL_STLP_VECTOR_OVERFLOW_DECL_ONLY
#define FABLE_RETAIL_STLP_INVENTORY_LONG_OWNERS_PASS27
#define FABLE_RETAIL_C3DVECTOR_CROSS_XZ_GROUP
#define FABLE_RETAIL_STLP_VECTOR_INDEX_START
#define FABLE_RETAIL_STLP_VECTOR_LONG_PUSHBACK_OWNER_PASS26
#define FABLE_RETAIL_STLP_VECTOR_PUSHBACK_NOINLINE
#include "engine/ETCInterfaceType.h"
#include "engine/C2DBoxI.h"
#include "engine/CTCPhysicsRigidBody.h"
#include "engine/CThingFilter_IsNot.h"
#include "engine/CThingFilter_CanCollideWithThing.h"
#include "engine/CFilter_Bind2.h"
#include "engine/CThingSearchTools.h"
#include "engine/CWorldMap.h"
#include "engine/CPlane.h"

inline float FABLE_FASTCALL GFDotProduct(const C3DVector& vector1,const C3DVector& vector2) {
    return vector1.X*vector2.X+vector1.Y*vector2.Y+vector1.Z*vector2.Z;
}

CTCPhysicsRigidBody::ECollideState CTCPhysicsRigidBody::CheckForCollisions(CRigidBody::CRigidBodyConfig& configuration) {
    ECollideState collide_state = CLEAR;
    float depth_epsilon = 0.001f;
    long no_vertices = Body.BodyBoundingVertices.size();
    bool collide_map = true;
    std::vector<CThing*> things;
    GetThingSearchTools().GetAllThingsInRoughArea<CThing>(GetPossibleCollidedWithArea(),
        GFFilter_Bind2(CThingFilter_IsNot(GetParentThing()),CThingFilter_CanCollideWithThing(*this)),things);
    for (long c1=0;c1<no_vertices;++c1) {
        C3DVector current_point = configuration.BoundingVertices[c1];
        C3DVector U = current_point-configuration.CMPosition;
        C3DVector velocity = configuration.CMVelocity+(configuration.AngularVelocity^U);
        if (collide_map) {
            float height_z = GetWorldMap().GetGroundSizeZAt(current_point);
            C3DVector normal = GetWorldMap().GetGroundFaceNormalAt(current_point);
            float distance = C3DVector(current_point.X,current_point.Y,height_z).Dot(normal);
            CPlane collide_plane(normal,-distance);
            float axbyczd = GFDotProduct(current_point,collide_plane.GetNormal()) + collide_plane.GetDistance();
            if (axbyczd < -depth_epsilon) return PENETRATING;
            else if (axbyczd < depth_epsilon) {
                float relative_velocity=GFDotProduct(collide_plane.GetNormal(),velocity);
                if (relative_velocity < 0.0f) {
                    collide_state=COLLIDING;
                    CollisionNormal.push_back(collide_plane.GetNormal());
                    CollidingVertexIndex.push_back(c1);
                }
            }
        }
    }
    return collide_state;
}