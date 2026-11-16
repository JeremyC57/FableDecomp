// C3DBoundingBox::InitialiseFromOBB(const C3DBoundingBox& obb, const CMatrix3x4& transform) -- axis-aligned box around a transformed box
// Grade: DIFFER(2735v2735) -- wfA6m: real 34 / um 34, lea region exact, SSE arm exact; every X and Y row of the four TransformVector expansions and the centre TransformPoint = retail; left: the Z rows of all five transforms (retail y,z,x; ours x,z,y) = the E13/E23/E33 IND residues need +2 (NOTES wfA6m). Default TransformVector text (no fitted TV opt-in); TP = TRANSFORMPOINT_LTR (existing opt-in); `const CMatrix3x4& t = transform;` in the SSE arm (FITTED: separates the x87 arm's transform IND records); v1..v3 by constructor.
// Leaf: ?InitialiseFromOBB@C3DBoundingBox@@QAEXABV1@ABVCMatrix3x4@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00ADE080, 2735 bytes, `ret 8`.
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DBOUNDINGBOX
#define FABLE_RETAIL_C3DBOUNDINGBOX_OBB
#define FABLE_RETAIL_CMATRIX4X4_ALIGNEDTRANSFORMTRANSPOSEDASSUMEW
#define FABLE_RETAIL_CMATRIX4X4_INITIALISETRANSPOSED
#define FABLE_RETAIL_CMATRIX3X4
#define FABLE_RETAIL_CMATRIX3X4_TRANSFORMPOINT_LTR
#define FABLE_RETAIL_CMATRIX3X4_TRANSFORMVECTOR
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
        const CMatrix3x4& t = transform;
        matrix.InitialiseTransposed(t);
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
