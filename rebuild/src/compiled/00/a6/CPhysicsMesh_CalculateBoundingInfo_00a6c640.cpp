// CPhysicsMesh::CalculateBoundingInfo() -- fit the mesh's bounds to its points: axis-aligned box (per-axis min / max), the sphere around the box centre (radius = half the diagonal) and the XY circle
// Leaf: ?CalculateBoundingInfo@CPhysicsMesh@@AAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A6C640, 517 bytes, thiscall plain `ret`.
// Ego_d.pdb: min_vector, max_vector, centre, radial_diagonal (C3DVector, function scope); { vertex_index (long) { loop_pos (const C3DVector&) } }.
// Helper bodies (C3DBoundingBox::SetMinMax, C3DBoundingSphere::Set, CCircle::Set) equal FableWin's /Od sizes; GetMagnitude2D is the
// FableWin-sized squared-sum text (opt-in FABLE_RETAIL_C3DVECTOR_MAGNITUDE2D_SQUARED_SUM): its parenthesised sum is the no-code tuple
// that puts `pop edi` after the circle radius `fsqrt` (0x00A6C839) via c2's 81-tuple scheduling regions.
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_MAGNITUDE2D
#define FABLE_RETAIL_C3DVECTOR_MAGNITUDE2D_SQUARED_SUM
#define FABLE_RETAIL_C3DBOUNDINGBOX
#define FABLE_RETAIL_C3DBOUNDINGBOX_SETMINMAX
#define FABLE_RETAIL_C3DBOUNDINGSPHERE_SET
#define FABLE_RETAIL_CCIRCLE_SET
#define FABLE_RETAIL_CPHYSICSMESH_BOUNDS
#define FABLE_RETAIL_CPHYSICSMESH_CALCULATEBOUNDINGINFO
#include "engine/C3DVector.h"
#include "engine/C2DVector.h"
#include "engine/C3DBoundingBox.h"
#include "engine/C3DBoundingSphere.h"
#include "engine/CCircle.h"
#include "engine/CPhysicsMesh.h"

void CPhysicsMesh::CalculateBoundingInfo()
{
    C3DVector min_vector = TotalPoints[0];
    C3DVector max_vector = TotalPoints[0];
    for (long vertex_index = 0; vertex_index < TotalPoints.size(); vertex_index++) {
        const C3DVector& loop_pos = TotalPoints[vertex_index];
        if (loop_pos.X < min_vector.X)
            min_vector.X = loop_pos.X;
        if (loop_pos.Y < min_vector.Y)
            min_vector.Y = loop_pos.Y;
        if (loop_pos.Z < min_vector.Z)
            min_vector.Z = loop_pos.Z;
        if (loop_pos.X > max_vector.X)
            max_vector.X = loop_pos.X;
        if (loop_pos.Y > max_vector.Y)
            max_vector.Y = loop_pos.Y;
        if (loop_pos.Z > max_vector.Z)
            max_vector.Z = loop_pos.Z;
    }
    C3DVector centre = (max_vector + min_vector) * 0.5f;
    C3DVector radial_diagonal = (max_vector - min_vector) * 0.5f;
    BoundingBox.SetMinMax(min_vector, max_vector);
    BoundingSphere.Set(centre, radial_diagonal.GetMagnitude());
    BoundingCircle.Set(C2DVector(centre.X, centre.Y), radial_diagonal.GetMagnitude2D());
}