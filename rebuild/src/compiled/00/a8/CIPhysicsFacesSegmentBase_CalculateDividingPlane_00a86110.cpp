// 00a86110 CIPhysicsFacesSegmentBase::CalculateDividingPlane
// Flags: none
// Leaf: ?CalculateDividingPlane@CIPhysicsFacesSegmentBase@@KI?AVCPlane@@ABV?$CArray@VC3DVector@@@@ABV?$CArray@VC3DTriangle2@@@@ABV?$CArray@G@@J@Z
// Retail: 0x00A86110, 1967 bytes, static fastcall.
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_SUBTRACT_ASSIGN
#define FABLE_RETAIL_CPLANE_NORMAL_CTOR_PASS23
#define FABLE_RETAIL_CIPHYSICS_DIVIDING_PLANE_PASS24
#include "engine/CArray.h"
#include "engine/C3DVector.h"
#include "engine/C3DTriangle2.h"
#include "engine/CPlane.h"
#include "engine/CIPhysicsFacesSegmentBase.h"

namespace {
C3DVector SDividingPlaneNormalTable[3] = {
    C3DVector(0.0f, 0.0f, 1.0f),
    C3DVector(1.0f, 0.0f, 0.0f),
    C3DVector(0.0f, 1.0f, 0.0f)
};
}

CPlane FABLE_FASTCALL CIPhysicsFacesSegmentBase::CalculateDividingPlane(const CArray<C3DVector>& point_list, const CArray<C3DTriangle2>& faces_list, const CArray<unsigned short>& triangle_indices, long level)
{
    const C3DVector& first = point_list[faces_list[triangle_indices[0]].GetVertexIndex(0)];
    C3DVector min_vector(first);
    C3DVector max_vector(first);
    C3DVector plane_normal;
    plane_normal = SDividingPlaneNormalTable[level];
    long index_size = triangle_indices.size();

    for (long c1 = 0; c1 < index_size; ++c1) {
        const long tri_index = triangle_indices[c1];
        const C3DTriangle2& triangle = faces_list[tri_index];
        const long vert_a_index = triangle.GetVertexIndex(0);
        const long vert_b_index = triangle.GetVertexIndex(1);
        const long vert_c_index = triangle.GetVertexIndex(2);
        const C3DVector face_coords[3] = { point_list[vert_a_index], point_list[vert_b_index], point_list[vert_c_index] };
        for (const C3DVector* loop_pos = face_coords; loop_pos < face_coords + 3; ++loop_pos) {
            if (loop_pos->X < min_vector.X) min_vector.X = loop_pos->X;
            if (loop_pos->Y < min_vector.Y) min_vector.Y = loop_pos->Y;
            if (loop_pos->Z < min_vector.Z) min_vector.Z = loop_pos->Z;
            if (loop_pos->X > max_vector.X) max_vector.X = loop_pos->X;
            if (loop_pos->Y > max_vector.Y) max_vector.Y = loop_pos->Y;
            if (loop_pos->Z > max_vector.Z) max_vector.Z = loop_pos->Z;
        }
    }
    C3DVector to_centre = (min_vector + max_vector) * 0.5f;
    C3DVector average_pos(0.0f, 0.0f, 0.0f);
    for (long c1 = 0; c1 < index_size; ++c1) {
        const long tri_index = triangle_indices[c1];
        const C3DTriangle2& triangle = faces_list[tri_index];
        const long vert_b_index = triangle.GetVertexIndex(1);
        const long vert_c_index = triangle.GetVertexIndex(2);
        C3DVector face_coord_b = point_list[vert_b_index];
        C3DVector face_coord_c = point_list[vert_c_index];
        face_coord_b -= to_centre;
        face_coord_c -= to_centre;
        average_pos += (face_coord_c + face_coord_b + face_coord_c) * 0.333333f;
    }
    average_pos /= (float)index_size;
    average_pos += to_centre;
    const float plane_distance = (average_pos.Dot(plane_normal));
    return CPlane(plane_normal, plane_distance);
}