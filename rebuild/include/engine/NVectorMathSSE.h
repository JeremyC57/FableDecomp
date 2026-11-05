// NVectorMathSSE.h -- the hand-written SSE kernels of the NVectorMath block (FableWin.pdb, namespace NVectorMath, right after
// GFInitVectorMath 0x00A5B850), isolated as macros. Each function tests g_SSEAvailable and runs either this block or its
// C++ loop; the macro names the function's own parameters and aligned locals directly (an `__asm` operand that names a C
// variable reads its frame slot), as retail's blocks do.
//   GF_SSE_INTERPOLATE_VECTORS           ?GFInterpolateVectors@NVectorMath@@YAXPAVC4DVector@@KPBV2@KKM@Z    0x00A5B870
//   GF_SSE_INTERPOLATE_VECTORS_FROM_TO   ?GFInterpolateVectors@NVectorMath@@YAXPAVC4DVector@@KPBV2@K1KKM@Z  0x00A5BBA0
//   GF_SSE_INTERPOLATE_VECTORS_WEIGHTED  ?GFInterpolateVectors@NVectorMath@@YAXPAVC4DVector@@KPBV2@K1KPBMKK@Z 0x00A5BEF0
//   GF_SSE_FINISH_PARTIAL_SLERP          NVectorMath::GFFinishPartialSlerp                                  0x00A5C2B0
//   GF_SSE_CLEAR_4D_VECTORS              NVectorMath::GFClear4DVectors                                      0x00A5C300
// Retail-asm-helper: GF_SSE_INTERPOLATE_VECTORS, GF_SSE_INTERPOLATE_VECTORS_FROM_TO, GF_SSE_INTERPOLATE_VECTORS_WEIGHTED, GF_SSE_FINISH_PARTIAL_SLERP, GF_SSE_CLEAR_4D_VECTORS
// Retail-asm: 0x00A5B8AF `movaps xmm0,[ebp-30h]` + 0x00A5B8B7 `shufps xmm0,xmm0,0` .. the loop 0x00A5B8C3 `movaps xmm2,[eax]` ;
//   `mulps xmm2,xmm0` ; `addps xmm2,xmm3` ; `movaps [eax],xmm2` ; 0x00A5B8D5 `add eax,[ebx+0Ch]` ; 0x00A5B8D8 `add edx,[ebx+14h]`
//   ; `dec ecx ; jne` (GFInterpolateVectors 0x00A5B870; the same shape at 0x00A5BBA0 / 0x00A5BEF0 / 0x00A5C2B0) and 0x00A5C315
//   `movups xmm0,[eax]` + 0x00A5C31F `movaps [edx],xmm0` + 0x00A5C322 `add edx,10h` (GFClear4DVectors 0x00A5C300): each loop
//   adds its byte strides from the argument slots in memory while the esi / edi it saved stay unused, and re-reads every
//   pointer and the count from the frame. The <xmmintrin.h> spellings of the same loops (_mm_set_ps1 or aligned locals +
//   _mm_shuffle_ps, _mm_load_ps / _mm_mul_ps / _mm_add_ps / _mm_store_ps, for and do-while forms) make VC7.1 load both
//   strides into esi / edi before the loop and pick its own register and operand order (`mulps xmm2,xmm1 ; mulps xmm3,xmm0 ;
//   addps xmm3,xmm2`) (the batch ATTEMPTS.tsv, family SSE_NVECTORMATH): the original blocks are inline asm.
// Operands (all pointers 16-byte aligned C4DVector arrays, strides in bytes, count > = 0, clobbers eax, ecx, edx, esi, edi,
//   xmm0..xmm4 as each block shows):
//   INTERPOLATE_VECTORS(out, from, count, oneMinusT, tt, outStride, fromStride): out[i] = out[i]*oneMinusT[0] + from[i]*tt[0]
//   INTERPOLATE_VECTORS_FROM_TO(out, from, to, count, oneMinusW, w, outStride, fromStride, toStride):
//     out[i] = from[i]*oneMinusW[0] + to[i]*w[0]
//   INTERPOLATE_VECTORS_WEIGHTED(out, from, to, weights, count, one, outStride, fromStride, toStride, weightStride):
//     w = weights[i]; out[i].l = from[i].l*(one.l - w) + to[i].l*w per lane l
//   FINISH_PARTIAL_SLERP(out, in, weights, count, weightStride, inStride, outStride): out[i] = out[i]*weights[i][0] +
//     in[i]*weights[i][1]
//   CLEAR_4D_VECTORS(value, dst, count): dst[i] = *value (an unaligned 16-byte load, aligned stores)
// Semantics: each operation one IEEE single-precision op (round to nearest): a product of each side, then their sum.
// Portable twin (FABLE_PORTABLE): plain C++ in fable_portable; tests in rebuild/tests/portable/test_NVectorMathSSE.cpp.
// Use: write each macro call as a statement on a line of its own: the retail branch expands to `__asm` statements, and an
//   `__asm` statement runs to the end of its source line (anything after it on that line is read as assembler).
// Proven (2026-11-05 round 2): GFClear4DVectors / GFFinishPartialSlerp have no `and esp,-16` though they read stack
//   arguments; VC7.1 gives every function that holds an SSE / MMX value and touches its stack an `and esp,-16` (-8) frame, and never folds a load
//   from a C pointer into a SIMD operation (only its own spill slots) (rebuild/candidates/manual/naming/2026-11-05_asm_helpers_r2/ATTEMPTS_UNIVERSAL.tsv). The three GFInterpolateVectors keep the `push ebx ; mov ebx,esp` + ebp double frame of a
//   block naming aligned locals and re-read the strides from argument slots (`add eax,[ebx+0Ch]`) with esi / edi free; 10
//   intrinsic spellings each give the plain realigned ebp frame with strides in registers. 51 intrinsic and 180 plain-C++
//   attempts: rebuild/candidates/manual/naming/2026-11-05_asm_helpers_r2/ATTEMPTS_SIMD_HELPERS.tsv.
#ifndef FABLE_ENGINE_NVECTORMATHSSE_H
#define FABLE_ENGINE_NVECTORMATHSSE_H

#if defined(FABLE_PORTABLE) || defined(FABLE_DUAL_PATH_TEST)
namespace fable_portable
{
    inline float* VmStep(void* p, unsigned long bytes) { return (float*)((char*)p + bytes); }
    inline const float* VmStep(const void* p, unsigned long bytes) { return (const float*)((const char*)p + bytes); }
    inline void InterpolateVectorsSSE(void* out, const void* from, unsigned long count, const float* oneMinusT,
                                      const float* tt, unsigned long outStride, unsigned long fromStride)
    {
        float* o = (float*)out; const float* f = (const float*)from;
        for (; count; --count, o = VmStep(o, outStride), f = VmStep(f, fromStride))
            for (int l = 0; l < 4; ++l) { float a = o[l] * oneMinusT[0]; float b = f[l] * tt[0]; o[l] = a + b; }
    }
    inline void InterpolateVectorsFromToSSE(void* out, const void* from, const void* to, unsigned long count,
                                            const float* oneMinusW, const float* w, unsigned long outStride,
                                            unsigned long fromStride, unsigned long toStride)
    {
        float* o = (float*)out; const float* f = (const float*)from; const float* t = (const float*)to;
        for (; count; --count, o = VmStep(o, outStride), f = VmStep(f, fromStride), t = VmStep(t, toStride))
            for (int l = 0; l < 4; ++l) { float a = f[l] * oneMinusW[0]; float b = t[l] * w[0]; o[l] = a + b; }
    }
    inline void InterpolateVectorsWeightedSSE(void* out, const void* from, const void* to, const void* weights,
                                              unsigned long count, const float* one, unsigned long outStride,
                                              unsigned long fromStride, unsigned long toStride, unsigned long weightStride)
    {
        float* o = (float*)out; const float* f = (const float*)from; const float* t = (const float*)to;
        const float* wt = (const float*)weights;
        for (; count; --count, o = VmStep(o, outStride), f = VmStep(f, fromStride), t = VmStep(t, toStride),
                               wt = VmStep(wt, weightStride)) {
            float w = wt[0], r[4];
            for (int l = 0; l < 4; ++l) { float s = one[l] - w; float a = f[l] * s; float b = t[l] * w; r[l] = a + b; }
            for (int l = 0; l < 4; ++l) o[l] = r[l];
        }
    }
    inline void FinishPartialSlerpSSE(void* out, const void* in, const void* weights, unsigned long count,
                                      unsigned long weightStride, unsigned long inStride, unsigned long outStride)
    {
        float* o = (float*)out; const float* v = (const float*)in; const float* wt = (const float*)weights;
        for (; count; --count, o = VmStep(o, outStride), v = VmStep(v, inStride), wt = VmStep(wt, weightStride)) {
            float w0 = wt[0], w1 = wt[1];
            for (int l = 0; l < 4; ++l) { float a = o[l] * w0; float b = v[l] * w1; o[l] = a + b; }
        }
    }
    inline void Clear4DVectorsSSE(const void* value, void* dst, unsigned long count)
    {
        float v[4]; const float* s = (const float*)value; float* d = (float*)dst;
        for (int l = 0; l < 4; ++l) v[l] = s[l];
        for (; count; --count, d += 4)
            for (int l = 0; l < 4; ++l) d[l] = v[l];
    }
}
#define FABLE_PORTABLE_SSE_INTERPOLATE_VECTORS(out, from, count, oneMinusT, tt, outStride, fromStride) \
    fable_portable::InterpolateVectorsSSE((out), (from), (count), (const float*)&(oneMinusT), (const float*)&(tt), \
                                          (outStride), (fromStride))
#define FABLE_PORTABLE_SSE_INTERPOLATE_VECTORS_FROM_TO(out, from, to, count, oneMinusW, w, outStride, fromStride, toStride) \
    fable_portable::InterpolateVectorsFromToSSE((out), (from), (to), (count), (const float*)&(oneMinusW), \
                                                (const float*)&(w), (outStride), (fromStride), (toStride))
#define FABLE_PORTABLE_SSE_INTERPOLATE_VECTORS_WEIGHTED(out, from, to, weights, count, one, outStride, fromStride, toStride, weightStride) \
    fable_portable::InterpolateVectorsWeightedSSE((out), (from), (to), (weights), (count), (const float*)&(one), \
                                                  (outStride), (fromStride), (toStride), (weightStride))
#define FABLE_PORTABLE_SSE_FINISH_PARTIAL_SLERP(out, in, weights, count, weightStride, inStride, outStride) \
    fable_portable::FinishPartialSlerpSSE((out), (in), (weights), (count), (weightStride), (inStride), (outStride))
#define FABLE_PORTABLE_SSE_CLEAR_4D_VECTORS(value, dst, count) \
    fable_portable::Clear4DVectorsSSE((value), (dst), (count))
#endif

#ifdef FABLE_PORTABLE
#define GF_SSE_INTERPOLATE_VECTORS(out, from, count, oneMinusT, tt, outStride, fromStride) \
    FABLE_PORTABLE_SSE_INTERPOLATE_VECTORS(out, from, count, oneMinusT, tt, outStride, fromStride)
#define GF_SSE_INTERPOLATE_VECTORS_FROM_TO(out, from, to, count, oneMinusW, w, outStride, fromStride, toStride) \
    FABLE_PORTABLE_SSE_INTERPOLATE_VECTORS_FROM_TO(out, from, to, count, oneMinusW, w, outStride, fromStride, toStride)
#define GF_SSE_INTERPOLATE_VECTORS_WEIGHTED(out, from, to, weights, count, one, outStride, fromStride, toStride, weightStride) \
    FABLE_PORTABLE_SSE_INTERPOLATE_VECTORS_WEIGHTED(out, from, to, weights, count, one, outStride, fromStride, toStride, weightStride)
#define GF_SSE_FINISH_PARTIAL_SLERP(out, in, weights, count, weightStride, inStride, outStride) \
    FABLE_PORTABLE_SSE_FINISH_PARTIAL_SLERP(out, in, weights, count, weightStride, inStride, outStride)
#define GF_SSE_CLEAR_4D_VECTORS(value, dst, count) FABLE_PORTABLE_SSE_CLEAR_4D_VECTORS(value, dst, count)
#else
#define GF_SSE_INTERPOLATE_VECTORS(out, from, count, oneMinusT, tt, outStride, fromStride) \
    __asm mov eax, out \
    __asm mov edx, from \
    __asm mov ecx, count \
    __asm movaps xmm0, xmmword ptr oneMinusT \
    __asm movaps xmm1, xmmword ptr tt \
    __asm shufps xmm0, xmm0, 0 \
    __asm shufps xmm1, xmm1, 0 \
    __asm test ecx, ecx \
    __asm je gf_sse_iv_done \
    __asm gf_sse_iv_next: \
    __asm movaps xmm2, [eax] \
    __asm movaps xmm3, [edx] \
    __asm mulps xmm2, xmm0 \
    __asm mulps xmm3, xmm1 \
    __asm addps xmm2, xmm3 \
    __asm movaps [eax], xmm2 \
    __asm add eax, outStride \
    __asm add edx, fromStride \
    __asm dec ecx \
    __asm jne gf_sse_iv_next \
    __asm gf_sse_iv_done:
#define GF_SSE_INTERPOLATE_VECTORS_FROM_TO(out, from, to, count, oneMinusW, w, outStride, fromStride, toStride) \
    __asm mov eax, out \
    __asm mov esi, from \
    __asm mov edx, to \
    __asm mov ecx, count \
    __asm movaps xmm0, oneMinusW \
    __asm movaps xmm1, w \
    __asm shufps xmm0, xmm0, 0 \
    __asm shufps xmm1, xmm1, 0 \
    __asm test ecx, ecx \
    __asm je gf_sse_ivft_done \
    __asm gf_sse_ivft_next: \
    __asm movaps xmm2, [esi] \
    __asm movaps xmm3, [edx] \
    __asm mulps xmm2, xmm0 \
    __asm mulps xmm3, xmm1 \
    __asm addps xmm2, xmm3 \
    __asm movaps [eax], xmm2 \
    __asm add eax, outStride \
    __asm add esi, fromStride \
    __asm add edx, toStride \
    __asm dec ecx \
    __asm jne gf_sse_ivft_next \
    __asm gf_sse_ivft_done:
#define GF_SSE_INTERPOLATE_VECTORS_WEIGHTED(out, from, to, weights, count, one, outStride, fromStride, toStride, weightStride) \
    __asm mov edi, out \
    __asm mov esi, from \
    __asm mov edx, to \
    __asm mov eax, weights \
    __asm mov ecx, count \
    __asm movaps xmm4, one \
    __asm test ecx, ecx \
    __asm je gf_sse_ivw_done \
    __asm gf_sse_ivw_next: \
    __asm movss xmm1, [eax] \
    __asm shufps xmm1, xmm1, 0 \
    __asm movaps xmm0, xmm4 \
    __asm subps xmm0, xmm1 \
    __asm movaps xmm2, [esi] \
    __asm movaps xmm3, [edx] \
    __asm mulps xmm2, xmm0 \
    __asm mulps xmm3, xmm1 \
    __asm addps xmm2, xmm3 \
    __asm movaps [edi], xmm2 \
    __asm add edi, outStride \
    __asm add esi, fromStride \
    __asm add edx, toStride \
    __asm add eax, weightStride \
    __asm dec ecx \
    __asm jne gf_sse_ivw_next \
    __asm gf_sse_ivw_done:
#define GF_SSE_FINISH_PARTIAL_SLERP(out, in, weights, count, weightStride, inStride, outStride) \
    __asm mov eax, out \
    __asm mov edx, in \
    __asm mov esi, weights \
    __asm mov ecx, count \
    __asm test ecx, ecx \
    __asm je gf_sse_fps_done \
    __asm gf_sse_fps_next: \
    __asm movaps xmm0, [esi] \
    __asm movaps xmm2, [eax] \
    __asm movaps xmm3, [edx] \
    __asm add esi, weightStride \
    __asm add edx, inStride \
    __asm movaps xmm1, xmm0 \
    __asm shufps xmm0, xmm0, 0 \
    __asm shufps xmm1, xmm1, 55h \
    __asm mulps xmm2, xmm0 \
    __asm mulps xmm3, xmm1 \
    __asm addps xmm2, xmm3 \
    __asm movaps [eax], xmm2 \
    __asm add eax, outStride \
    __asm dec ecx \
    __asm jne gf_sse_fps_next \
    __asm gf_sse_fps_done:
#define GF_SSE_CLEAR_4D_VECTORS(value, dst, count) \
    __asm mov eax, value \
    __asm mov edx, dst \
    __asm movups xmm0, [eax] \
    __asm mov ecx, count \
    __asm test ecx, ecx \
    __asm je gf_sse_c4_done \
    __asm gf_sse_c4_next: \
    __asm movaps [edx], xmm0 \
    __asm add edx, 16 \
    __asm dec ecx \
    __asm jne gf_sse_c4_next \
    __asm gf_sse_c4_done:
#endif

#endif  // FABLE_ENGINE_NVECTORMATHSSE_H
