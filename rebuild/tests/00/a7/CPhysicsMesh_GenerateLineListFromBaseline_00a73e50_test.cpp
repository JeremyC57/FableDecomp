#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __stdcall
#define __int64 long long
#define FABLE_RETAIL_PASS23_HOST_TEST_ABI
#define FABLE_PORTABLE
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C2DVECTOR_FROM3D
#define FABLE_RETAIL_C2DLINEF_FROM3D
#define FABLE_RETAIL_C3DPRIMITIVE_CORNER_ACCESSORS
#define FABLE_RETAIL_C3DTRIANGLE2_SETVERTEXINDEX
#define FABLE_RETAIL_CPLANE_NORMAL_CTOR_PASS23
#define FABLE_RETAIL_PHYSICS_MESH_BASELINE_PASS30
#define FABLE_RETAIL_CARRAY_ORIGINAL_COMPLETE_DECLARATIONS_PASS22
#define FABLE_RETAIL_CARRAY_ORIGINAL_INDEX_BASE_PASS22
#define FABLE_RETAIL_STLP_VECTOR
#include "fable_assert.h"
#include "engine/CPlane.h"
#include "engine/C2DLineF.h"
#include "engine/C3DTriangle2.h"
#include "engine/CArray.h"
#include "engine/GFAngles.h"

class CPhysicsMesh {
public:
    CArray<C3DTriangle2> TotalFaceIndices;
    CArray<C3DVector> TotalPoints;
    void GenerateLineListFromBaseline(float, CArray<C2DLineF>&, CArray<long>&) const;
};

static int shrinkCalls, callCount;
static bool permitIntersection;
static float expectedHeight;
static unsigned long expectedFace[12];
static unsigned int expectedCount;
static int caseNumber;
static void require(bool condition) {
    if (!condition) { fprintf(stderr, "geometry case %d failed\n", caseNumber); exit(1); }
}

template<> void CArray<C2DLineF>::DeallocateUnusedMemory() { ++shrinkCalls; }

bool CPlane::GetIntersectionWithTriangle(const C3DVector& a, const C3DVector& b, const C3DVector& c,
        C3DVector& first, C3DVector& second) const {
    require(Normal.X == 0.0f && Normal.Y == 0.0f && Normal.Z == 1.0f);
    require(Distance == expectedHeight && (unsigned)callCount < expectedCount);
    unsigned long face = expectedFace[callCount++];
    require(a.X == (float)(3 * face) && b.X == a.X + 1 && c.X == b.X + 1);
    first = C3DVector(a.X, b.Y, Distance);
    second = C3DVector(c.X, c.Y, Distance);
    return permitIntersection;
}

void CPhysicsMesh::GenerateLineListFromBaseline(float base_line_z,
    CArray<C2DLineF>& line_list, CArray<long>& used_face_list) const
{
    long no_triangles = TotalFaceIndices.size();
    float best_diff = 1e10f;
    float best_base_line_z = 0.0f;
    for (long c1 = 0; c1 < no_triangles; ++c1) {
        long index_a = TotalFaceIndices[c1].GetA();
        long index_b = TotalFaceIndices[c1].GetB();
        long index_c = TotalFaceIndices[c1].GetC();
        float triangle_z = (TotalPoints[index_a].Z + TotalPoints[index_b].Z + TotalPoints[index_c].Z) * 0.3333333f;
        float current_diff = (float)fabs(triangle_z - base_line_z);
        if (current_diff < best_diff) {
            best_diff = current_diff;
            best_base_line_z = triangle_z;
        }
    }
    FABLE_RETAIL_ASSERT(best_diff != 1e10f);
    long no_lines_found = 0;
    for (long c1 = 0; c1 < no_triangles; ++c1) {
        long sign_za = GFGetSign(TotalPoints[TotalFaceIndices[c1].GetA()].Z - best_base_line_z);
        long sign_zb = GFGetSign(TotalPoints[TotalFaceIndices[c1].GetB()].Z - best_base_line_z);
        long sign_zc = GFGetSign(TotalPoints[TotalFaceIndices[c1].GetC()].Z - best_base_line_z);
        if ((sign_za == 1 && sign_zb == 1 && sign_zc == -1) ||
            (sign_za == 1 && sign_zb == -1 && sign_zc == 1) ||
            (sign_za == -1 && sign_zb == 1 && sign_zc == 1) ||
            (sign_za == -1 && sign_zb == -1 && sign_zc == 1) ||
            (sign_za == -1 && sign_zb == 1 && sign_zc == -1) ||
            (sign_za == 1 && sign_zb == -1 && sign_zc == -1)) {
            C3DVector intersection1, intersection2;
            CPlane plane(C3DVector(0.0f, 0.0f, 1.0f), best_base_line_z);
            if (plane.GetIntersectionWithTriangle(TotalPoints[TotalFaceIndices[c1].GetA()],
                    TotalPoints[TotalFaceIndices[c1].GetB()], TotalPoints[TotalFaceIndices[c1].GetC()],
                    intersection1, intersection2)) {
                used_face_list.push_back(c1);
                ++no_lines_found;
                line_list.push_back(C2DLineF(intersection1, intersection2));
            } else {
                C3DVector tri_a = TotalPoints[TotalFaceIndices[c1].GetA()];
                C3DVector tri_b = TotalPoints[TotalFaceIndices[c1].GetB()];
                C3DVector tri_c = TotalPoints[TotalFaceIndices[c1].GetC()];
                FABLE_RETAIL_ASSERT(false);
            }
        }
    }
    line_list.DeallocateUnusedMemory();
}

static void exercise(const int* heights, unsigned faces, float baseline, bool hit) {
    ++caseNumber;
    CPhysicsMesh mesh;
    CArray<C2DLineF> lines;
    CArray<long> used;
    const C3DVector saved(64.0f, 32.0f, 16.0f);
    used.push_back(-9L);
    lines.push_back(C2DLineF(saved,saved));
    double smallest = 10000000000.0;
    expectedHeight = 0.0f;
    expectedCount = 0;
    callCount = shrinkCalls = 0;
    permitIntersection = hit;
    for (unsigned int f = 0; f < faces; ++f) {
        C3DTriangle2 tri;
        int total = 0;
        for (unsigned int v = 0; v != 3; ++v) {
            unsigned i = 3*f + v;
            tri.SetVertexIndex(v,(unsigned short)i);
            mesh.TotalPoints.push_back(C3DVector((float)i,(float)(3*i), (float)heights[i]));
            total += heights[i];
        }
        mesh.TotalFaceIndices.push_back(tri);
        // The denominator and numerator independently express the exact native coefficient.
        float height = (float)(((double)total * 5592405.0) / 16777216.0);
        float diff = (float)fabs((double)height - baseline);
        if (diff < smallest) { smallest = diff; expectedHeight = height; }
    }
    for (unsigned int f = 0; f < faces; ++f) {
        unsigned below = 0;
        for (unsigned int v=0; v<3; ++v) if (heights[3*f+v] < expectedHeight) ++below;
        if (below != 0 && below != 3) expectedFace[expectedCount++] = f;
    }
    mesh.GenerateLineListFromBaseline(baseline,lines,used);
    require(shrinkCalls == 1 && (unsigned)callCount == expectedCount);
    unsigned int out = 1 + (hit ? expectedCount : 0);
    require(lines.size() == out && used.size() == out && used[0] == -9);
    require(lines[0].Point1.X == 64 && lines[0].Point2.Y == 32);
    for (unsigned int pos=1;pos<out;++pos) {
        unsigned int face = expectedFace[pos-1];
        require(used[pos] == (long)face && lines[pos].Point1.X == 3*face);
        require(lines[pos].Point1.Y == 3*(3*face+1) && lines[pos].Point2.X == 3*face+2);
        require(lines[pos].Point2.Y == 3*(3*face+2));
    }
}
int main() {
    const int multi[] = {-1, 1, 3, 2, 2, 2, -3,-1,1, 0,0,0};
    exercise(0,0,0.0f,true);
    for (int b=-4;b<=4;++b) {
        exercise(multi,4,(float)b,true); exercise(multi,4,(float)b,false);
    }
    int values[3];
    for (values[0]=-2;values[0]<=2;++values[0])
        for (values[1]=-2;values[1]<=2;++values[1])
            for (values[2]=-2;values[2]<=2;++values[2]) {
                exercise(values,1,0.0f,true);
                exercise(values,1,0.0f,false);
            }
    puts("00a73e50_TEST PASS");
    return 0;
}