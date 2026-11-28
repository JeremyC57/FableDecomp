// 00a73e50 CPhysicsMesh::GenerateLineListFromBaseline
// Flags: /Gr
// Leaf: ?GenerateLineListFromBaseline@CPhysicsMesh@@ABEXMAAV?$CArray@VC2DLineF@@@@AAV?$CArray@J@@@Z
#define FABLE_RETAIL_PHYSICS_MESH_BASELINE_PASS30
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CPHYSICSMESH_INTERSECTION_PASS23
#define FABLE_RETAIL_C3DPRIMITIVE_CORNER_ACCESSORS
#define FABLE_RETAIL_CPLANE_NORMAL_CTOR_PASS23
#define FABLE_RETAIL_C2DLINEF_FROM3D
#define FABLE_RETAIL_CARRAY_ORIGINAL_COMPLETE_DECLARATIONS_PASS22
#define FABLE_RETAIL_STLP_LONG_OVERFLOW_PASS30
#define FABLE_RETAIL_STLP_VECTOR_LONG_PUSHBACK_OWNER_PASS26
#define FABLE_RETAIL_C2DVECTOR_FROM3D
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_CARRAY_ORIGINAL_INDEX_BASE_PASS22
// Inline-budget: the line array grows through the out-of-line copy at 00a13490.
#ifdef _MSC_VER
#define FABLE_STLP_OUTOFLINE_UNINIT_COPY
#endif
#include "fable_assert.h"
#include "engine/C2DLineF.h"
#include "engine/CPlane.h"
#include "engine/CPhysicsMesh.h"
#include "engine/GFAngles.h"

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