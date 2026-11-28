// Leaf: ?GetSphereIntersectionFromToUsingSegmentation@CPhysicsMesh@@ABE_NMABVC3DVector@@0AAVCMeshIntersectionData@@@Z
// Flags:
// Inline-budget: the retail traversal expands the shared swept-sphere test on each candidate face.
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CPHYSICSMESH_INTERSECTION_PASS23
#define FABLE_RETAIL_CPHYSICSMESH_SEGMENT_SPHERE_PASS30
#define FABLE_RETAIL_CPHYSICSMESH_FULL_SPHERE_PASS30
#define FABLE_RETAIL_C3DVECTOR_GETMAGNITUDEANDNORMALISE
#define FABLE_RETAIL_CPHYSICSMESH_SWEEP_TRIANGLE_PASS30
#define FABLE_RETAIL_C3DPRIMITIVE_CORNER_ACCESSORS
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_NORMALISE_SQUARE_HELPER
#define FABLE_RETAIL_C3DVECTOR_MAGNITUDE_SQRTF
#define FABLE_RETAIL_C3DVECTOR_CROSS_OPERATOR_PASS29
#define FABLE_RETAIL_C3DVECTOR_NORMALISE_GROUPED_SUM
#define FABLE_RETAIL_GFFASTONEOVERSQRT_NEWTON_GROUPED
#include <math.h>
#pragma intrinsic(sqrt)
#include "engine/CPhysicsMesh.h"

bool CPhysicsMesh::GetSphereIntersectionFromToUsingSegmentation(
    float sphere_radius,const C3DVector& from,const C3DVector& to,CMeshIntersectionData& data) const
{
    std::vector<long>& possible_set = GetSingleInstanceCollisionSet();
    RootSegment->GetCollidedWithTriangles(this,sphere_radius,from,to,possible_set);
    std::vector<long>::const_iterator index;
    C3DVector intersection_pos;
    C3DVector intersection_normal;
    float best_intersection_distance = 1.0e10f;
    bool found = false;
    C3DVector normalised_velocity = to - from;
    float speed = normalised_velocity.GetMagnitudeAndNormalise();
    for (index = possible_set.begin(); index != possible_set.end(); ++index) {
        const C3DTriangle2& test_triangle = TotalFaceIndices[*index];
        if (HasCollidedWithTriangleAt(test_triangle,sphere_radius,from,normalised_velocity,speed,
                                     intersection_pos,intersection_normal)) {
            float intersection_distance = (intersection_pos - from).GetSquaredMagnitude();
            if (intersection_distance < best_intersection_distance) {
                data.DistanceSquared = intersection_distance;
                data.IntersectionPosition = intersection_pos;
                data.Normal = intersection_normal;
                best_intersection_distance = intersection_distance;
                found = true;
            }
        }
    }
    return found;
}