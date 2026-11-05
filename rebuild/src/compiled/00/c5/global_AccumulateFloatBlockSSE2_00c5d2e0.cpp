// AccumulateFloatBlockSSE2(const float* src, long* dst) -- add a 128-sample float block into a 32-bit accumulator block with SSE2, converting each sample to an integer under round-to-nearest (MXCSR rounding forced to nearest for the loop and restored afterwards)
// Leaf: ?AccumulateFloatBlockSSE2@@YAXPBMPAJ@Z
// Retail-asm: 0x00C5D310 `cvtps2dq xmm0,xmmword ptr [eax]` -- retail runs the conversion loop (eight passes of four cvtps2dq / paddd /
// Helper (2026-11-05 asm migration): the retail inline-asm block is the shared libpk/MixKernelsSIMD.h (byte-matching path + FABLE_PORTABLE twin); the row has
//   no asm of its own. C++/intrinsic attempts: the batch ATTEMPTS.tsv, family SIMD_LIBPK.
//   movdqa over 64 bytes, `mov ecx,8 ; dec ecx ; jne`) as inline asm; the MXCSR save / round-to-nearest / restore around it is the
//   _mm_getcsr / _mm_setcsr intrinsics (`stmxcsr [ebp-4]` / `ldmxcsr [ebp-4]` through the one frame word).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C5D2E0, length 141 bytes (+ int3 padding); `ret` (__cdecl, args at [ebp+8] /
//   [ebp+0Ch], ebp frame from the asm block). Callers: 0x00C5D3DA and the SSE2 mixer's MixAddFloat 0x00C5D490 (0x00C5D49A).
// Label: the manifest's "Math_AccumulateFloatArray" is close (propagated); descriptive name (GameCODA, no PDB).
// Shape: the caller's rounding bits are read and masked in one initializer (`and esi,6000h` keeps MXCSR.RC; masking in a later
//   statement lets VC7.1 sink the `and` past the asm block), RC is cleared for the loop
//   (`and eax,0FFFF9FFFh`), and restored by `(csr & 0FFFF9FFFh) | saved`.
#include "libpk/MixKernelsSIMD.h"
#ifdef _MSC_VER
#include <xmmintrin.h>
#endif
void AccumulateFloatBlockSSE2(const float* src, long* dst)
{
#ifdef _MSC_VER
    unsigned int saved = _mm_getcsr() & 0x6000;
    _mm_setcsr(_mm_getcsr() & 0xffff9fff);
    LIBPK_SSE2_ACCUMULATE_FLOAT128_BLOCK(src, dst);
    _mm_setcsr((_mm_getcsr() & 0xffff9fff) | saved);
#else
    for (int i = 0; i < 128; ++i) {
        float v = src[i];
        long r = (long)(v < 0.0f ? v - 0.5f : v + 0.5f);
        if ((v - (float)(long)v == 0.5f || (float)(long)v - v == 0.5f) && (r & 1)) r -= (v < 0.0f ? -1 : 1);
        dst[i] += r;
    }
#endif
}