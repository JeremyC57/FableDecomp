// 00a12450 CPhysicsMeshObject::GetPointIntersectionFromTo
// Leaf: ?GetPointIntersectionFromTo@CPhysicsMeshObject@@QBE_NABVC3DVector@@0AAVCMeshIntersectionData@@@Z
// Flags: none
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
bool CPhysicsMeshObject::GetPointIntersectionFromTo(const C3DVector& from, const C3DVector& to, CMeshIntersectionData& data) const
{
    CMatrix3x4 world_to_mesh;
    C3DVector ms_from, ms_to;
    world_to_mesh.SetInverse(PositionMatrix);
    world_to_mesh.TransformPoint(from, ms_from);
    world_to_mesh.TransformPoint(to, ms_to);
    if (Mesh->GetPointIntersectionFromTo(ms_from, ms_to, data)) {
        PositionMatrix.TransformVector(data.Normal);
        data.Normal.Normalise();
        PositionMatrix.TransformPoint(data.IntersectionPosition);
        data.DistanceSquared=(data.IntersectionPosition-from).GetSquaredMagnitude();
        return true;
    }
    return false;
}