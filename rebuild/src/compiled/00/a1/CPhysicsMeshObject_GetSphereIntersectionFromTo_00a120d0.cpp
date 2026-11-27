// 00a120d0 CPhysicsMeshObject::GetSphereIntersectionFromTo
// Leaf: ?GetSphereIntersectionFromTo@CPhysicsMeshObject@@QBE_NMABVC3DVector@@0AAVCMeshIntersectionData@@@Z
// Flags: none
#define FABLE_RETAIL_CPHYSICSMESHOBJECT_SPHEREINTERSECTION
#define FABLE_RETAIL_GFFASTONEOVERSQRT_NEWTON_GROUPED
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CMATRIX3X4
#define FABLE_RETAIL_CMATRIX3X4_TRANSFORMPOINT_SOURCE_DEST_SUMS
#define FABLE_RETAIL_CMATRIX3X4_TRANSFORMVECTOR
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CPHYSICSMESHOBJECT
#define FABLE_RETAIL_CPHYSICSMESHOBJECT_MATRIX
#define FABLE_RETAIL_CPHYSICSMESH_CAST_PASS29
#define FABLE_RETAIL_C3DVECTOR_NORMALISE_SQUARE_HELPER
#include "engine/CMeshIntersectionData.h"
#include "engine/CPhysicsMesh.h"
#include "engine/CPhysicsMeshObject.h"
bool CPhysicsMeshObject::GetSphereIntersectionFromTo(float sphere_radius, const C3DVector& from, const C3DVector& to, CMeshIntersectionData& data) const
{
    CMatrix3x4 world_to_mesh;
    C3DVector ms_from, ms_to;
    float ms_sphere_radius = sphere_radius * OO_Scale;
    world_to_mesh.SetInverse(PositionMatrix);
    world_to_mesh.TransformPoint(from, ms_from);
    world_to_mesh.TransformPoint(to, ms_to);
    if (Mesh->GetSphereIntersectionFromTo(ms_sphere_radius, ms_from, ms_to, data)) {
        PositionMatrix.TransformVector(data.Normal);
        data.Normal.Normalise();
        PositionMatrix.TransformPoint(data.IntersectionPosition);
        data.DistanceSquared=(data.IntersectionPosition-from).GetSquaredMagnitude();
        return true;
    }
    return false;
}