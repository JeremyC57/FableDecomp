// GF_SSE_BOX_FROM_OBB_CORNERS(corners, centre, minimum, maximum) -- the hand-written SSE kernel of
// C3DBoundingBox::InitialiseFromOBB (Ego_r.pdb / FableWin.pdb ?InitialiseFromOBB@C3DBoundingBox@@QAEXABV1@ABVCMatrix3x4@@@Z,
// retail 0x00ADE080), isolated as a macro: the per-lane minimum and maximum of centre +/- corners[i] over four transformed
// half-extent corners (the eight corners of the oriented box).
// Retail-asm-helper: GF_SSE_BOX_FROM_OBB_CORNERS
// Retail-asm: 0x00ADE329 `lea eax,[esp+0B0h]` / 0x00ADE330 `mov [esp+24h],eax` (the corner array address stored to a C pointer
//   local) + 0x00ADE334 `mov eax,[esp+24h]` (re-read from its frame word with nothing between) + 0x00ADE338 `movaps xmm2,[esp+40h]`
//   / 0x00ADE33D `movaps xmm3,[eax]` .. 0x00ADE3A9 `maxps xmm1,xmm5` (four passes, fixed xmm0..xmm5 schedule, loads folded from
//   the pointer register) + 0x00ADE3AC `movaps [esp+10h],xmm0` / 0x00ADE3B1 `movaps [esp+60h],xmm1`. No <xmmintrin.h> spelling
//   homes and re-reads a pointer local or keeps this register schedule (VC7.1 never folds a load through a C pointer into a
//   SIMD op and inserts its own spill copies: engine/C3DBoundingBoxSSE.h "Proven").
// Operands: corners -- a pointer local holding the address of four 16-byte aligned 4-float vectors; centre, minimum, maximum --
//   16-byte aligned 4-float locals (minimum / maximum written). Clobbers eax, xmm0..xmm5.
// Semantics (IEEE single, round to nearest): a_i = centre + corners[i], b_i = centre - corners[i] per lane;
//   minimum = minps chain, maximum = maxps chain in the order (a0 min b0), then a1, b1, a2, b2, a3, b3, where minps(x, y) =
//   x < y ? x : y and maxps(x, y) = x > y ? x : y (the second operand on NaN or equal).
// Portable twin (FABLE_PORTABLE): plain C++ in fable_portable; dual-path test test_C3DBoundingBoxOBBSSE.cpp.
// Use: write the macro call as a statement on a line of its own (an `__asm` statement runs to the end of its source line).
#ifndef FABLE_ENGINE_C3DBOUNDINGBOXOBBSSE_H
#define FABLE_ENGINE_C3DBOUNDINGBOXOBBSSE_H

#if defined(FABLE_PORTABLE) || defined(FABLE_DUAL_PATH_TEST)
namespace fable_portable
{
    inline float SseObbMinps(float a, float b) { return a < b ? a : b; }
    inline float SseObbMaxps(float a, float b) { return a > b ? a : b; }
    inline void BoxFromOBBCornersSSE(const float* corners, const float* centre, float* minimum, float* maximum)
    {
        for (int l = 0; l < 4; ++l) {
            float a = centre[l] + corners[l];
            float b = centre[l] - corners[l];
            float lo = SseObbMinps(a, b);
            float hi = SseObbMaxps(a, b);
            for (int i = 1; i < 4; ++i) {
                a = centre[l] + corners[i * 4 + l];
                b = centre[l] - corners[i * 4 + l];
                lo = SseObbMinps(lo, a);
                hi = SseObbMaxps(hi, a);
                lo = SseObbMinps(lo, b);
                hi = SseObbMaxps(hi, b);
            }
            minimum[l] = lo;
            maximum[l] = hi;
        }
    }
}
#define FABLE_PORTABLE_SSE_BOX_FROM_OBB_CORNERS(corners, centre, minimum, maximum) \
    fable_portable::BoxFromOBBCornersSSE((const float*)(corners), (const float*)&(centre), (float*)&(minimum), \
        (float*)&(maximum))
#endif

#ifdef FABLE_PORTABLE
#define GF_SSE_BOX_FROM_OBB_CORNERS(corners, centre, minimum, maximum) \
    FABLE_PORTABLE_SSE_BOX_FROM_OBB_CORNERS(corners, centre, minimum, maximum)
#else
#define GF_SSE_BOX_FROM_OBB_CORNERS(corners, centre, minimum, maximum) \
    __asm mov eax, corners \
    __asm movaps xmm2, centre \
    __asm movaps xmm3, [eax] \
    __asm movaps xmm4, xmm2 \
    __asm movaps xmm5, xmm2 \
    __asm addps xmm4, xmm3 \
    __asm subps xmm5, xmm3 \
    __asm movaps xmm0, xmm4 \
    __asm movaps xmm1, xmm4 \
    __asm minps xmm0, xmm5 \
    __asm maxps xmm1, xmm5 \
    __asm movaps xmm3, [eax+10h] \
    __asm movaps xmm4, xmm2 \
    __asm movaps xmm5, xmm2 \
    __asm addps xmm4, xmm3 \
    __asm subps xmm5, xmm3 \
    __asm minps xmm0, xmm4 \
    __asm maxps xmm1, xmm4 \
    __asm minps xmm0, xmm5 \
    __asm maxps xmm1, xmm5 \
    __asm movaps xmm3, [eax+20h] \
    __asm movaps xmm4, xmm2 \
    __asm movaps xmm5, xmm2 \
    __asm addps xmm4, xmm3 \
    __asm subps xmm5, xmm3 \
    __asm minps xmm0, xmm4 \
    __asm maxps xmm1, xmm4 \
    __asm minps xmm0, xmm5 \
    __asm maxps xmm1, xmm5 \
    __asm movaps xmm3, [eax+30h] \
    __asm movaps xmm4, xmm2 \
    __asm movaps xmm5, xmm2 \
    __asm addps xmm4, xmm3 \
    __asm subps xmm5, xmm3 \
    __asm minps xmm0, xmm4 \
    __asm maxps xmm1, xmm4 \
    __asm minps xmm0, xmm5 \
    __asm maxps xmm1, xmm5 \
    __asm movaps minimum, xmm0 \
    __asm movaps maximum, xmm1
#endif

#endif  // FABLE_ENGINE_C3DBOUNDINGBOXOBBSSE_H
