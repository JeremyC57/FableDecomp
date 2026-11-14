// C3DBoundingBox::InitialiseFromOBB(const C3DBoundingBox& obb, const CMatrix3x4& transform) -- axis-aligned box around a transformed box
// Grade: DIFFER(2733v2735) -- frame identical, SSE arm identical (asm helper); residue = x87 operand order only: the Y rows of the v1..v3 half-extent transforms and of the centre TransformPoint put z*E3k before y*E2k in retail (ours y first: the inner pair is reordered by VC7.1 whatever the source order), and the v0 X row is (x*E11 + y*E21) + z*E31 in retail (ours (x+z)+y from the fitted grouping); 43 masked diff lines
// Leaf: ?InitialiseFromOBB@C3DBoundingBox@@QAEXABV1@ABVCMatrix3x4@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00ADE080, 2735 bytes, `ret 8`.
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DBOUNDINGBOX
#define FABLE_RETAIL_C3DBOUNDINGBOX_OBB
#define FABLE_RETAIL_CMATRIX4X4_ALIGNEDTRANSFORMTRANSPOSEDASSUMEW
#define FABLE_RETAIL_CMATRIX4X4_INITIALISETRANSPOSED
#define FABLE_RETAIL_CMATRIX3X4
#define FABLE_RETAIL_CMATRIX3X4_TRANSFORMPOINT_GROUPED_XZY
#define FABLE_RETAIL_CMATRIX3X4_TRANSFORMVECTOR
#define FABLE_RETAIL_CMATRIX3X4_TRANSFORMVECTOR_GROUPED_XZY
#include "engine/C3DBoundingBox.h"
#include "engine/CMatrix3x4.h"
#include "engine/CMatrix4x4.h"
#include "engine/C3DBoundingBoxOBBSSE.h"
extern bool g_SSEAvailable;   // 0x013D2880

void C3DBoundingBox::InitialiseFromOBB(const C3DBoundingBox& obb, const CMatrix3x4& transform)
{
    if (g_SSEAvailable) {
        __declspec(align(16)) C4DVector centre(obb.GetCenter(), 0.0f);
        C3DVector half = (obb.MaxCoord - obb.MinCoord) * 0.5f;
        __declspec(align(16)) C4DVector corners[4] = {
            C4DVector(half.X, half.Y, half.Z, 0.0f),
            C4DVector(half.X, -half.Y, half.Z, 0.0f),
            C4DVector(-half.X, half.Y, half.Z, 0.0f),
            C4DVector(-half.X, -half.Y, half.Z, 0.0f) };
        __declspec(align(16)) CMatrix4x4 matrix;
        matrix.InitialiseTransposed(transform);
        matrix.TransformTransposedAssumeW(centre, centre, 1.0f);
        matrix.AlignedTransformTransposedAssumeW(corners, corners, 4, 0.0f);
        __declspec(align(16)) C4DVector min_v;
        __declspec(align(16)) C4DVector max_v;
        C4DVector* p = corners;
        GF_SSE_BOX_FROM_OBB_CORNERS(p, centre, min_v, max_v);
        MinCoord = C3DVector(min_v.X, min_v.Y, min_v.Z);
        MaxCoord = C3DVector(max_v.X, max_v.Y, max_v.Z);
        return;
    }
    C3DVector v0 = (obb.MaxCoord - obb.MinCoord) * 0.5f;
    C3DVector v1(v0.X, -v0.Y, v0.Z);
    C3DVector v2(-v0.X, v0.Y, v0.Z);
    C3DVector v3(-v0.X, -v0.Y, v0.Z);
    transform.TransformVector(v0);
    transform.TransformVector(v1);
    transform.TransformVector(v2);
    transform.TransformVector(v3);
    C3DVector centre = obb.GetCenter();
    transform.TransformPoint(centre);

    MaxCoord = centre + v0;
    MinCoord = MaxCoord;
    MinCoord.LimitUpper(centre - v0);
    MaxCoord.LimitLower(centre - v0);
    MinCoord.LimitUpper(centre + v1);
    MaxCoord.LimitLower(centre + v1);
    MinCoord.LimitUpper(centre - v1);
    MaxCoord.LimitLower(centre - v1);
    MinCoord.LimitUpper(centre + v2);
    MaxCoord.LimitLower(centre + v2);
    MinCoord.LimitUpper(centre - v2);
    MaxCoord.LimitLower(centre - v2);
    MinCoord.LimitUpper(centre + v3);
    MaxCoord.LimitLower(centre + v3);
    MinCoord.LimitUpper(centre - v3);
    MaxCoord.LimitLower(centre - v3);
}
