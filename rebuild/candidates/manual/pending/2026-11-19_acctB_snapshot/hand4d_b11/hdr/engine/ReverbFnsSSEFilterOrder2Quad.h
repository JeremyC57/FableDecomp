// ReverbFnsSSEFilterOrder2Quad.h -- GameCODA 2.10 (Sensaura; the audio library linked into Fable.exe): the hand-written SSE kernel of
// CReverbFnsSSE::ApplyFilterOrder2_quad (0x00C88E60), four second-order filters run side by side on four 128-sample channel blocks,
// isolated as a macro. The macro names the caller's parameters and the local pfData directly (an `__asm` operand that names a C
// variable reads its frame slot): retail's block reads pfSrc / pfDest / uiSamples from the realigned argument frame (`mov esi,[ebx+0Ch]`,
// `mov edi,[ebx+8]`, `mov ecx,[ebx+18h]`) and pfData from its ebp slot (`mov edx,[ebp-4]`).
// Retail-asm-helper: CODA_SSE_APPLY_FILTER_ORDER2_QUAD
// Retail-asm: 0x00C88FEA `mov esi,[ebx+0Ch]` .. 0x00C89001 `subps xmm0,xmm0` + 0x00C89004 `subps xmm5,xmm5` + 0x00C89007 `subps xmm6,xmm6`
//   .. 0x00C890AA `dec ecx ; jne 0x00C8900A` .. 0x00C890B5 `movaps [edx+80h],xmm2` (CReverbFnsSSE::ApplyFilterOrder2_quad). Proof that the
//   original is inline asm: the GameCODA 2.10 CODAApiDbg.lib ReverbFns_SSE.obj /Od body of the same function carries the identical
//   51-instruction block (only pfData's frame slot differs: [ebp-0E4h] v [ebp-4]; same fixed esi / edi / ecx / edx, same xmm numbering, no /Od spill of any __m128 value), and VC7.1 never
//   zeroes with `subps x,x` (_mm_setzero_ps gives xorps) nor keeps a dead zeroing whose register a `movss` load overwrites.
// Semantics (per sample n = 0 .. uiSamples-1, lane l = 0..3; samples of lane l at src[l*128 + n] / dst[l*128 + n]); pfData is a
//   16-byte aligned float[40] laid out as ten four-lane rows: a1 (0), a2 (4), c2 (8), b0 (12), b1 (16), b2 (20), c6 (24), h0 (28),
//   h1 (32), g (36):
//     w = (src + c2) + (a2*h1 + a1*h0);  out = ((b0*w + (b2*h1 + b1*h0)) + c6) * g;  h1 = h0;  h0 = w;  dst = out
//   then h0 / h1 are written back to their rows. uiSamples > 0 (the loop is `dec ecx ; jne`). Every operation is one IEEE
//   single-precision operation (round to nearest under the default MXCSR).
// Clobbers: ecx, edx, esi, edi, xmm0..xmm6, the h0 / h1 rows of pfData.
// Portable twin (FABLE_PORTABLE): plain C++ in fable_portable, the kernel's operation order; tests in
//   tests/portable/test_ReverbFnsSSEFilterOrder2Quad.cpp. FPU state: the asm path rounds to nearest under the default MXCSR (no FTZ /
//   DAZ assumed). The twin is bit-identical to it where float is evaluated in single precision with no contraction (SSE builds:
//   x86-64 g++, -ffp-contract=off where FMA exists); under an x87 build (VC7.1 /DFABLE_PORTABLE) intermediates may stay in extended
//   precision, so inexact data can differ in the last bit there (exact data agrees bit for bit; the test checks both).
// Use: write the macro call as a statement on a line of its own (an `__asm` statement runs to the end of its source line).
#ifndef FABLE_ENGINE_REVERBFNSSSEFILTERORDER2QUAD_H
#define FABLE_ENGINE_REVERBFNSSSEFILTERORDER2QUAD_H

#if defined(FABLE_PORTABLE) || defined(FABLE_DUAL_PATH_TEST)
namespace fable_portable
{
    inline void ApplyFilterOrder2QuadSSE(float* pfDest, const float* pfSrc, unsigned int uiSamples, float* pfData)
    {
        float h0[4], h1[4];
        int l;
        for (l = 0; l < 4; ++l) { h0[l] = pfData[28 + l]; h1[l] = pfData[32 + l]; }
        do {
            for (l = 0; l < 4; ++l) {
                const float fb = pfData[4 + l] * h1[l] + pfData[0 + l] * h0[l];      // a2*h1 + a1*h0
                const float ff = pfData[20 + l] * h1[l] + pfData[16 + l] * h0[l];    // b2*h1 + b1*h0
                const float w = (pfSrc[l * 128] + pfData[8 + l]) + fb;
                const float out = ((pfData[12 + l] * w + ff) + pfData[24 + l]) * pfData[36 + l];
                h1[l] = h0[l];
                h0[l] = w;
                pfDest[l * 128] = out;
            }
            ++pfSrc;
            ++pfDest;
        } while (--uiSamples);
        for (l = 0; l < 4; ++l) { pfData[28 + l] = h0[l]; pfData[32 + l] = h1[l]; }
    }
}
#define FABLE_PORTABLE_CODA_SSE_APPLY_FILTER_ORDER2_QUAD(pfDest, pfSrc, uiSamples, pfData) \
    fable_portable::ApplyFilterOrder2QuadSSE((pfDest), (pfSrc), (uiSamples), (pfData))
#endif

#ifdef FABLE_PORTABLE
#define CODA_SSE_APPLY_FILTER_ORDER2_QUAD(pfDest, pfSrc, uiSamples, pfData) \
    FABLE_PORTABLE_CODA_SSE_APPLY_FILTER_ORDER2_QUAD(pfDest, pfSrc, uiSamples, pfData)
#else
#define CODA_SSE_APPLY_FILTER_ORDER2_QUAD(pfDest, pfSrc, uiSamples, pfData) \
    __asm mov esi, pfSrc \
    __asm mov edi, pfDest \
    __asm mov ecx, uiSamples \
    __asm mov edx, pfData \
    __asm movaps xmm1, [edx+70h] \
    __asm movaps xmm2, [edx+80h] \
    __asm subps xmm0, xmm0 \
    __asm subps xmm5, xmm5 \
    __asm subps xmm6, xmm6 \
    __asm coda_sse_apply_filter_order2_quad_next: \
    __asm movaps xmm3, [edx+10h] \
    __asm movaps xmm4, [edx+50h] \
    __asm mulps xmm3, xmm2 \
    __asm mulps xmm4, xmm2 \
    __asm movaps xmm5, [edx] \
    __asm movaps xmm6, [edx+40h] \
    __asm mulps xmm5, xmm1 \
    __asm mulps xmm6, xmm1 \
    __asm addps xmm3, xmm5 \
    __asm addps xmm4, xmm6 \
    __asm movss xmm0, dword ptr [esi] \
    __asm movss xmm5, dword ptr [esi+200h] \
    __asm unpcklps xmm0, xmm5 \
    __asm movss xmm6, dword ptr [esi+400h] \
    __asm movss xmm5, dword ptr [esi+600h] \
    __asm add esi, 4 \
    __asm unpcklps xmm6, xmm5 \
    __asm shufps xmm0, xmm6, 44h \
    __asm movaps xmm6, [edx+20h] \
    __asm addps xmm0, xmm6 \
    __asm addps xmm0, xmm3 \
    __asm movaps xmm5, [edx+30h] \
    __asm mulps xmm5, xmm0 \
    __asm addps xmm5, xmm4 \
    __asm movaps xmm4, [edx+60h] \
    __asm addps xmm5, xmm4 \
    __asm movaps xmm2, xmm1 \
    __asm movaps xmm1, xmm0 \
    __asm movaps xmm4, [edx+90h] \
    __asm mulps xmm5, xmm4 \
    __asm movss dword ptr [edi], xmm5 \
    __asm shufps xmm5, xmm5, 39h \
    __asm movss dword ptr [edi+200h], xmm5 \
    __asm shufps xmm5, xmm5, 39h \
    __asm movss dword ptr [edi+400h], xmm5 \
    __asm shufps xmm5, xmm5, 39h \
    __asm movss dword ptr [edi+600h], xmm5 \
    __asm add edi, 4 \
    __asm dec ecx \
    __asm jne coda_sse_apply_filter_order2_quad_next \
    __asm movaps [edx+70h], xmm1 \
    __asm movaps [edx+80h], xmm2
#endif

#endif
