// C3DBoundingBox::InitialiseFromOBB(const C3DBoundingBox&, const CMatrix3x4&) -- axis-aligned box around a transformed box
// Grade: DIFFER(2696v2735) -- 1280 difflines (mostly stack-slot offsets): the SSE path is the engine's hand-SSE min/max kernel (retail 0x00ADE329 `lea eax,[esp+0B0h]` / 0x00ADE330 `mov [esp+24h],eax` / 0x00ADE334 `mov eax,[esp+24h]` then movaps/addps/subps/minps/maxps over four corners: a homed and reloaded pointer local, the inline-asm signature); intrinsics give a different frame (0xD4 v 0xE4) and schedule; GetCenter's X sum loads Min first (retail Max first) and the x87 arm reproduces retail's per-vector store/reload pattern (four named locals, v1..v3 from v0's components) but VC7.1 picks different slots and the obb pointer load is hoisted above the g_SSEAvailable branch; the row also carries a second function 0x00ADEB30..0x00ADEBBB (ret 4), C3DBoundingBox::InitialiseFromSphere(const C3DBoundingSphere&) (Ego_r name + C3DBoundingSphere layout), that the coordinator must split off
// Next idea: after the ASM_RULES budget (30 plain C++ + 10 intrinsic spellings; 4 intrinsic + 13 plain spellings so far) add a shared helper engine/C3DBoundingBoxSSE.h macro GF_SSE_OBB_MIN_MAX(corners_ptr, centre, min, max) with a FABLE_PORTABLE twin; then re-grade the x87 arm (its slot layout follows the SSE arm's frame)
// Leaf: ?InitialiseFromOBB@C3DBoundingBox@@QAEXABV1@ABVCMatrix3x4@@@Z
#include <xmmintrin.h>
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DBOUNDINGBOX
#define FABLE_RETAIL_C3DBOUNDINGBOX_OBB
#define FABLE_RETAIL_CMATRIX4X4_ALIGNEDTRANSFORM
#define FABLE_RETAIL_CMATRIX3X4_TRANSFORMVECTOR
#include "engine/C3DBoundingBox.h"
#include "engine/CMatrix3x4.h"
#include "engine/CMatrix4x4.h"
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
        matrix.E11 = transform.E11; matrix.E12 = transform.E21; matrix.E13 = transform.E31; matrix.E14 = transform.E41;
        matrix.E21 = transform.E12; matrix.E22 = transform.E22; matrix.E23 = transform.E32; matrix.E24 = transform.E42;
        matrix.E31 = transform.E13; matrix.E32 = transform.E23; matrix.E33 = transform.E33; matrix.E34 = transform.E43;
        matrix.E41 = 0.0f; matrix.E42 = 0.0f; matrix.E43 = 0.0f; matrix.E44 = 1.0f;
        matrix.TransformTransposedAssumeW(centre, centre, 1.0f);
        matrix.AlignedTransformTransposedAssumeW(corners, corners, 4, 0.0f);
        __m128 c = _mm_load_ps(&centre.X);
        __m128 lo, hi;
        {
            __m128 a = _mm_add_ps(c, _mm_load_ps(&corners[0].X));
            __m128 b = _mm_sub_ps(c, _mm_load_ps(&corners[0].X));
            lo = _mm_min_ps(a, b);
            hi = _mm_max_ps(a, b);
        }
        for (long i = 1; i < 4; ++i) {
            __m128 a = _mm_add_ps(c, _mm_load_ps(&corners[i].X));
            __m128 b = _mm_sub_ps(c, _mm_load_ps(&corners[i].X));
            lo = _mm_min_ps(lo, a);
            hi = _mm_max_ps(hi, a);
            lo = _mm_min_ps(lo, b);
            hi = _mm_max_ps(hi, b);
        }
        __declspec(align(16)) C4DVector min_v;
        __declspec(align(16)) C4DVector max_v;
        _mm_store_ps(&min_v.X, lo);
        _mm_store_ps(&max_v.X, hi);
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
    C3DVector p = centre - v0;
    MinCoord.LimitUpper(p); MaxCoord.LimitLower(p);
    p = centre + v1;
    MinCoord.LimitUpper(p); MaxCoord.LimitLower(p);
    p = centre - v1;
    MinCoord.LimitUpper(p); MaxCoord.LimitLower(p);
    p = centre + v2;
    MinCoord.LimitUpper(p); MaxCoord.LimitLower(p);
    p = centre - v2;
    MinCoord.LimitUpper(p); MaxCoord.LimitLower(p);
    p = centre + v3;
    MinCoord.LimitUpper(p); MaxCoord.LimitLower(p);
    p = centre - v3;
    MinCoord.LimitUpper(p); MaxCoord.LimitLower(p);
}
