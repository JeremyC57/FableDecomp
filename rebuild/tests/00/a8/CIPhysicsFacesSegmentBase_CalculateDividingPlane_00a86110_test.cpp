#include <stdio.h>
#include <math.h>
#define FABLE_RETAIL_PASS23_HOST_TEST_ABI
#define FABLE_RETAIL_C3DTRIANGLE2_SETVERTEXINDEX
#ifndef _MSC_VER
#define FABLE_RETAIL_STLP_HOST_TAGS_PASS23
#define __int64 long long
#define __fastcall
#define __cdecl
#define __stdcall
#define __fastcall
#endif
// 00a86110 CIPhysicsFacesSegmentBase::CalculateDividingPlane
// Flags: none
// Leaf: ?CalculateDividingPlane@CIPhysicsFacesSegmentBase@@KI?AVCPlane@@ABV?$CArray@VC3DVector@@@@ABV?$CArray@VC3DTriangle2@@@@ABV?$CArray@G@@J@Z
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
class PlaneTest : public CIPhysicsFacesSegmentBase {
public:
    using CIPhysicsFacesSegmentBase::CalculateDividingPlane;
};

static int check(const CArray<C3DVector>& points, const CArray<C3DTriangle2>& triangles,
                 const CArray<unsigned short>& indices)
{
    double sums[3] = {0.0, 0.0, 0.0};
    for (unsigned int i = 0; i < indices.size(); ++i) {
        const C3DTriangle2& tri = triangles[indices[i]];
        const C3DVector& second = points[tri.GetVertexIndex(1)];
        const C3DVector& third = points[tri.GetVertexIndex(2)];
        sums[0] += second.X + 2.0 * third.X;
        sums[1] += second.Y + 2.0 * third.Y;
        sums[2] += second.Z + 2.0 * third.Z;
    }
    for (long level = 0; level < 3; ++level) {
        const CPlane plane = PlaneTest::CalculateDividingPlane(points, triangles, indices, level);
        const int component = (level + 2) % 3;
        const double expected = sums[component] / (3.0 * indices.size());
        if (plane.Normal.X != float(component == 0) ||
            plane.Normal.Y != float(component == 1) ||
            plane.Normal.Z != float(component == 2) ||
            !(fabs(plane.Distance - expected) <= 0.0001)) return 1;
    }
    return 0;
}

int main()
{
    CArray<C3DVector> points(6);
    points[0] = C3DVector(0,0,0);
    points[1] = C3DVector(2,-8,4);
    points[2] = C3DVector(-6,4,-2);
    points[3] = C3DVector(12,2,6);
    points[4] = C3DVector(-4,-12,10);
    points[5] = C3DVector(8,6,-10);
    CArray<C3DTriangle2> triangles(4);
    const unsigned short corners[4][3] = {{0,1,2},{3,4,5},{2,3,1},{5,5,5}};
    for (unsigned int i = 0; i < 4; ++i)
        for (unsigned int j = 0; j < 3; ++j)
            triangles[i].SetVertexIndex(j, corners[i][j]);
    const unsigned short order[] = {3,0,1,2,1,0,2,3,1};
    for (unsigned int count = 1; count <= 9; ++count) {
        CArray<unsigned short> indices(count);
        for (unsigned int i = 0; i < count; ++i) indices[i] = order[i];
        if (check(points, triangles, indices)) return 1;
    }
    puts("00a86110_TEST PASS");
    return 0;
}