// C3DBoundingBox::InitialiseFromOBB -- Build axis-aligned bounds around a transformed box.
// Grade: DIFFER(2696v2735) -- Portable SSE intrinsics and the scalar four-diagonal path produce different operand scheduling and instruction selection starting in the SIMD arm.
// Leaf: ?InitialiseFromOBB@C3DBoundingBox@@QAEXABV1@ABVCMatrix3x4@@@Z
// Retail: 0x00ADE080, 2735 bytes, thiscall.
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
#include <xmmintrin.h>
#ifdef _MSC_VER
#define FABLE_OBB_ALIGN16 __declspec(align(16))
#else
#define FABLE_OBB_ALIGN16 __attribute__((aligned(16)))
#endif
extern bool GEnableSIMD;   // 0x013D2880

void C3DBoundingBox::InitialiseFromOBB(const C3DBoundingBox& src, const CMatrix3x4& transform)
{
    if (GEnableSIMD) {
        FABLE_OBB_ALIGN16 C4DVector center(src.GetCenter(), 0.0f);
        C3DVector half = (src.MaxCoord - src.MinCoord) * 0.5f;
        FABLE_OBB_ALIGN16 C4DVector diag[4] = {
            C4DVector(half.X, half.Y, half.Z, 0.0f),
            C4DVector(half.X, -half.Y, half.Z, 0.0f),
            C4DVector(-half.X, half.Y, half.Z, 0.0f),
            C4DVector(-half.X, -half.Y, half.Z, 0.0f) };
        FABLE_OBB_ALIGN16 CMatrix4x4 sse_transform;
        const CMatrix3x4& t = transform;
        sse_transform.InitialiseTransposed(t);
        sse_transform.TransformTransposedAssumeW(center, center, 1.0f);
        sse_transform.AlignedTransformTransposedAssumeW(diag, diag, 4, 0.0f);
        FABLE_OBB_ALIGN16 C4DVector min_coord;
        FABLE_OBB_ALIGN16 C4DVector max_coord;
        const C4DVector* diag_ptr = diag;
        const __m128 c = _mm_load_ps(&center.X);
        __m128 d = _mm_load_ps(&diag_ptr[0].X);
        __m128 plus = _mm_add_ps(c,d);
        __m128 minus = _mm_sub_ps(c,d);
        __m128 lower = _mm_min_ps(plus,minus);
        __m128 upper = _mm_max_ps(plus,minus);
        for (int i = 1; i < 4; ++i) {
            d = _mm_load_ps(&diag_ptr[i].X);
            plus = _mm_add_ps(c,d);
            minus = _mm_sub_ps(c,d);
            lower = _mm_min_ps(lower,plus);
            upper = _mm_max_ps(upper,plus);
            lower = _mm_min_ps(lower,minus);
            upper = _mm_max_ps(upper,minus);
        }
        _mm_store_ps(&min_coord.X, lower);
        _mm_store_ps(&max_coord.X, upper);
        MinCoord = C3DVector(min_coord.X, min_coord.Y, min_coord.Z);
        MaxCoord = C3DVector(max_coord.X, max_coord.Y, max_coord.Z);
        return;
    }
    C3DVector diag1 = (src.MaxCoord - src.MinCoord) * 0.5f;
    C3DVector diag2(diag1.X, -diag1.Y, diag1.Z);
    C3DVector diag3(-diag1.X, diag1.Y, diag1.Z);
    C3DVector diag4(-diag1.X, -diag1.Y, diag1.Z);
    transform.TransformVector(diag1);
    transform.TransformVector(diag2);
    transform.TransformVector(diag3);
    transform.TransformVector(diag4);
    C3DVector center = src.GetCenter();
    transform.TransformPoint(center);

    MaxCoord = center + diag1;
    MinCoord = MaxCoord;
    MinCoord.LimitUpper(center - diag1);
    MaxCoord.LimitLower(center - diag1);
    MinCoord.LimitUpper(center + diag2);
    MaxCoord.LimitLower(center + diag2);
    MinCoord.LimitUpper(center - diag2);
    MaxCoord.LimitLower(center - diag2);
    MinCoord.LimitUpper(center + diag3);
    MaxCoord.LimitLower(center + diag3);
    MinCoord.LimitUpper(center - diag3);
    MaxCoord.LimitLower(center - diag3);
    MinCoord.LimitUpper(center + diag4);
    MaxCoord.LimitLower(center + diag4);
    MinCoord.LimitUpper(center - diag4);
    MaxCoord.LimitLower(center - diag4);
}
