// CPhysicsMesh::GetSphereIntersectionFromToUsingFullCollision -- test all faces and retain the nearest swept-sphere hit.
// Leaf: ?GetSphereIntersectionFromToUsingFullCollision@CPhysicsMesh@@ABE_NMABVC3DVector@@0AAVCMeshIntersectionData@@@Z
// Flags: /O2 /Oy /W3
// Inline-budget: the native caller expands its original sphere-triangle helper.
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CPHYSICSMESH_INTERSECTION_PASS23
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

bool CPhysicsMesh::GetSphereIntersectionFromToUsingFullCollision(
    float sphere_radius, const C3DVector& from, const C3DVector& to, CMeshIntersectionData& data) const
{
    C3DVector intersection_pos;
    C3DVector intersection_normal;
    float best_intersection_distance = 1.0e10f;
    long total_faces = TotalFaceIndices.size();
    bool found = false;
    C3DVector normalised_velocity = to - from;
    float speed = normalised_velocity.GetMagnitudeAndNormalise();
    for (long index = 0; index < total_faces; ++index) {
        const C3DTriangle2& test_triangle = TotalFaceIndices[index];
        if (HasCollidedWithTriangleAt(test_triangle, sphere_radius, from, normalised_velocity, speed,
                                      intersection_pos, intersection_normal)) {
            float intersection_distance = (intersection_pos - from).GetSquaredMagnitude();
            if (intersection_distance < best_intersection_distance) {
                found = true;
                data.Normal = intersection_normal;
                data.DistanceSquared = intersection_distance;
                data.IntersectionPosition = intersection_pos;
                best_intersection_distance = intersection_distance;
            }
        }
    }
    return found;
}