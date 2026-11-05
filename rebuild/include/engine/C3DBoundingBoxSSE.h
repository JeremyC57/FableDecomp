// GF_SSE_BOX_DISTANCE_SQUARED(box, point, result) -- the hand-written SSE kernel of
// C3DBoundingBox::CalcShortestDistanceToPointSquared (FableWin.pdb ?CalcShortestDistanceToPointSquared@C3DBoundingBox@@QBEMABVC3DVector@@@Z,
// an inline member expanded in its callers), isolated as a macro: the squared distance from a point to the box (zero inside).
// Expanded at 0x00B49810 (the out-of-line member), 0x00B9C500 (CalcShortestDistanceToPoint), 0x00B46280
// (CEngineLightingManager::GetLightsInArea), 0x00B4CED0 / 0x00B9C2E0 (CShadowClipVolume::ClipBoundingBox), 0x00B99BB0
// (CEngineInternalPrimitiveRepeatedStaticMesh::IsInsideFrustum) and 0x00BF7DF0 (CLandscapeBackgroundTreeNode::GetDesiredLODBand).
// Retail-asm-helper: GF_SSE_BOX_DISTANCE_SQUARED
// Retail-asm: 0x00B4981F `mov ecx,[esp]` + 0x00B49822 `mov edx,[esp+24h]` + 0x00B49826 `movss xmm0,[ecx+8]` .. 0x00B49842
//   `maxps xmm2,xmm0` / 0x00B49845 `minps xmm2,xmm1` .. 0x00B4985C `addss xmm2,xmm3` + 0x00B49860 `movss [esp+4],xmm2`
//   (C3DBoundingBox::CalcShortestDistanceToPointSquared 0x00B49810): `this` is homed (`mov [esp],ecx`) and re-read inside the
//   block, the frame is a plain `sub esp,20h`. The <xmmintrin.h> spelling of the same operations (_mm_load_ss + _mm_loadh_pi,
//   _mm_max_ps / _mm_min_ps / _mm_sub_ps / _mm_mul_ps / _mm_shuffle_ps / _mm_add_ps / _mm_add_ss / _mm_store_ss, two operand
//   orders) makes VC7.1 build an `and esp,-16` ebp frame, insert `movaps` copies for every two-operand op and never home or
//   reload `this` (the batch ATTEMPTS.tsv, family SSE_BOX): the original block is inline asm.
// Operands: box -- the C3DBoundingBox (MinCoord X,Y,Z at +0, MaxCoord at +0Ch; `this` in the member); point -- a
//   C3DVector reference (X,Y,Z); result -- a float lvalue. Clobbers ecx, edx, xmm0..xmm3.
// Semantics (IEEE single, round to nearest): lanes (Z, 0, X, Y); c = minps(maxps(p, min), max) per lane (maxps(a,b) = a > b
//   ? a : b, minps(a,b) = a < b ? a : b: the second operand on NaN or equal); d = (c - p)^2; result = (dX + dY) + dZ.
// Portable twin (FABLE_PORTABLE): plain C++ in fable_portable; test rebuild/tests/portable/test_C3DBoundingBoxSSE.cpp.
// Use: write each macro call as a statement on a line of its own: the retail branch expands to `__asm` statements, and an
//   `__asm` statement runs to the end of its source line (anything after it on that line is read as assembler).
// Proven (2026-11-05 round 2): no `and esp,-16` in any of the 7 retail functions, which read stack arguments; VC7.1 gives every function that holds an SSE / MMX value and touches its stack an `and esp,-16` (-8) frame, and never folds a load
//   from a C pointer into a SIMD operation (only its own spill slots) (rebuild/candidates/manual/naming/2026-11-05_asm_helpers_r2/ATTEMPTS_UNIVERSAL.tsv).
//   Also `this` homed to [esp] and reloaded with no call between (0x00B4981A / 0x00B4981F). 10 intrinsic and 90 plain-C++
//   attempts: rebuild/candidates/manual/naming/2026-11-05_asm_helpers_r2/ATTEMPTS_SIMD_HELPERS.tsv.
#ifndef FABLE_ENGINE_C3DBOUNDINGBOXSSE_H
#define FABLE_ENGINE_C3DBOUNDINGBOXSSE_H

#if defined(FABLE_PORTABLE) || defined(FABLE_DUAL_PATH_TEST)
namespace fable_portable
{
    inline float SseMaxps(float a, float b) { return a > b ? a : b; }
    inline float SseMinps(float a, float b) { return a < b ? a : b; }
    inline float BoxDistanceSquaredSSE(const float* box, const float* point)
    {
        float d[3];
        for (int l = 0; l < 3; ++l) {
            float c = SseMinps(SseMaxps(point[l], box[l]), box[3 + l]);
            float e = c - point[l];
            d[l] = e * e;
        }
        float xy = d[0] + d[1];
        return xy + d[2];
    }
}
#define FABLE_PORTABLE_SSE_BOX_DISTANCE_SQUARED(box, point, result) \
    ((result) = fable_portable::BoxDistanceSquaredSSE((const float*)(box), (const float*)&(point)))
#endif

#ifdef FABLE_PORTABLE
#define GF_SSE_BOX_DISTANCE_SQUARED(box, point, result) FABLE_PORTABLE_SSE_BOX_DISTANCE_SQUARED(box, point, result)
#else
#define GF_SSE_BOX_DISTANCE_SQUARED(box, point, result) \
    __asm mov ecx, box \
    __asm mov edx, point \
    __asm movss xmm0, [ecx+8] \
    __asm movhps xmm0, [ecx] \
    __asm movss xmm1, [ecx+14h] \
    __asm movhps xmm1, [ecx+0Ch] \
    __asm movss xmm2, [edx+8] \
    __asm movhps xmm2, [edx] \
    __asm movaps xmm3, xmm2 \
    __asm maxps xmm2, xmm0 \
    __asm minps xmm2, xmm1 \
    __asm subps xmm2, xmm3 \
    __asm mulps xmm2, xmm2 \
    __asm movaps xmm3, xmm2 \
    __asm shufps xmm2, xmm2, 0B0h \
    __asm addps xmm2, xmm3 \
    __asm shufps xmm2, xmm2, 3 \
    __asm addss xmm2, xmm3 \
    __asm movss result, xmm2
#endif

#endif  // FABLE_ENGINE_C3DBOUNDINGBOXSSE_H
