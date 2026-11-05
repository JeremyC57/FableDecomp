// MixKernelsSIMD.h -- libpk (the GameCODA audio middleware linked into Fable.exe, 0x00C08AE0..0x00C7xxxx): the hand-written
// MMX / SSE / SSE2 mixer kernels, isolated as macros. Each libpk TU keeps its own copy of the function that runs a kernel (the
// mixer voices' accumulate and ramp paths, the reverb / filter history clears and copies, the int <-> float block
// conversions); the macro names the function's own parameters and locals directly (an asm operand that names a C variable
// reads its frame slot), as retail's blocks do. libpk has no PDB: the names describe the bodies.
//   LIBPK_MMX_SCALE_ACCUMULATE16[_RAMP]       0x00C5BE20 0x00C6AA10 0x00C6F0F0 0x00C6F730 0x00C6FBB0 / ramp 0x00C5BEF0 0x00C6AAE0
//                                             0x00C6F1A0 0x00C6F800 0x00C6F990 0x00C6FC80 0x00C6FE10
//   LIBPK_SSE2_SCALE_ACCUMULATE16[_RAMP]      0x00C5D150 0x00C6E1F0 0x00C6ECD0 / ramp 0x00C5D1F0 0x00C6E290 0x00C6E410 0x00C6ED70
//   LIBPK_SSE2_SCALE_ACCUMULATE16[_RAMP]_UNALIGNED   0x00C6D370 / ramp 0x00C6D410 0x00C6D630
//   LIBPK_SSE2_ADD_INT128_BLOCK               0x00C5D370 0x00C5D3F0 0x00C5D4B0
//   LIBPK_SSE2_ACCUMULATE_FLOAT128_BLOCK      0x00C5D2E0
//   LIBPK_SSE2 / SSE_CONVERT_INT128_TO_FLOAT[_IN_PLACE]   0x00C6B3A0 / 0x00C6B5D0
//   LIBPK_SSE2 / SSE_CONVERT_FLOAT128_TO_INT_IN_PLACE     0x00C6B490
//   LIBPK_SSE2_COPY64 / COPY128, LIBPK_MMX_COPY128        0x00C6D630 / 0x00C70A60, 0x00C70040
//   LIBPK_SSE_ZERO64 / ZERO128, LIBPK_MMX_ZERO64 / ZERO128 0x00C6D560 / 0x00C70BC0, 0x00C6F300 / 0x00C700D0 0x00C701E0
// Retail-asm-helper: LIBPK_MMX_SCALE_ACCUMULATE16_RAMP, LIBPK_MMX_SCALE_ACCUMULATE16, LIBPK_SSE2_SCALE_ACCUMULATE16_RAMP, LIBPK_SSE2_SCALE_ACCUMULATE16, LIBPK_SSE2_SCALE_ACCUMULATE16_RAMP_UNALIGNED, LIBPK_SSE2_SCALE_ACCUMULATE16_UNALIGNED, LIBPK_SSE2_ADD_INT128_BLOCK, LIBPK_SSE2_ACCUMULATE_FLOAT128_BLOCK, LIBPK_SSE2_CONVERT_INT128_TO_FLOAT, LIBPK_SSE_CONVERT_INT128_TO_FLOAT, LIBPK_SSE2_CONVERT_INT128_TO_FLOAT_IN_PLACE, LIBPK_SSE_CONVERT_INT128_TO_FLOAT_IN_PLACE, LIBPK_SSE2_CONVERT_FLOAT128_TO_INT_IN_PLACE, LIBPK_SSE_CONVERT_FLOAT128_TO_INT_IN_PLACE, LIBPK_SSE2_COPY64, LIBPK_SSE2_COPY128, LIBPK_MMX_COPY128, LIBPK_SSE_ZERO64, LIBPK_MMX_ZERO64, LIBPK_SSE_ZERO128, LIBPK_MMX_ZERO128
// Retail-asm: 0x00C6AA45 `movq mm7,[gains]` + 0x00C6AA50 `movq mm0,[eax]` + 0x00C6AA53 `pmulhw mm0,mm7` .. `psrad mm1,15 ; paddd
//   mm1,[edx]` .. `emms` (0x00C6AA10), 0x00C6AB32 / 0x00C6AB36 `movq mm7 / mm6` + `paddsw mm7,mm6` (the ramp, 0x00C6AAE0), 0x00C5D195
//   `movdqa xmm7,[gains]` + 0x00C5D1A4 `pmulhw xmm0,xmm7` (0x00C5D150), 0x00C6D3C4 `pmulhw xmm0,xmm7` after `movdqu xmm0,[eax]`
//   (0x00C6D370), 0x00C5D310 `cvtps2dq xmm0,[eax]` (0x00C5D2E0), 0x00C6B3D0 `cvtdq2ps xmm0,[eax]` (0x00C6B3A0), 0x00C6B600
//   `cvtdq2ps xmm0,[eax]` (0x00C6B5D0), 0x00C6B4AB `stmxcsr` .. `ldmxcsr` around `cvtps2dq` / `cvtps2pi` (0x00C6B490), 0x00C6D573
//   / 0x00C70BD5 `xorps xmm0,xmm0` + `movdqa [edi],xmm0` (an integer store of a float-zeroed register: 0x00C6D560, 0x00C70BC0),
//   0x00C6F313 / 0x00C700D5 `pxor mm0,mm0` + `movq [edi],mm0` .. `emms` (0x00C6F300, 0x00C700D0), 0x00C7004A `movq mm0,[esi]` +
//   0x00C7004D `movq [edi],mm0` over mm0..mm7 in turn (0x00C70040). Fixed registers (the loop count in ecx, the pointers in
//   eax / edx or esi / edi), kernels in functions with no `and esp,-8 / -16` frame, a float-domain zero stored through an
//   integer move: the <mmintrin.h> / <xmmintrin.h> / <emmintrin.h> spellings of the same loops (_mm_mulhi_pi16 / _epi16,
//   _mm_unpacklo / hi_pi16 against _mm_setzero, _mm_srai_pi32, _mm_add_pi32 / _epi32, _mm_cvtps_epi32, _mm_cvtepi32_ps,
//   _mm_setzero_si64 / _si128 + stores, _mm_empty) make VC7.1 realign the stack (`push ebp ; mov ebp,esp ; and esp,-8 / -16`)
//   for every __m64 / __m128 value, pick its own registers and copy through register moves, and turn a 16-store zero loop
//   into `rep movs` (the batch ATTEMPTS.tsv, family SIMD_LIBPK): the original blocks are inline asm.
// Semantics (each per the instruction set; asm and twin compared input by input in the test):
//   SCALE_ACCUMULATE16(count, src, dst, gains[, increments]): for each group of 4 (MMX) / 8 (SSE2) shorts s[l] of src, lane
//     gain g[l] (shorts): p = (s*g) >> 16 (pmulhw, the signed high word), dst[l] += 2p (psrad 15 of p << 16; wrapping 32-bit
//     paddd); the ramp form then g[l] = saturate16(g[l] + inc[l]) (paddsw) after every group. MMX: 2 groups per count (8
//     samples, 8 dwords), SSE2: 1 group per count. gains / increments are aligned 8 / 16-byte locals; the ramp form leaves the
//     final gains in the register only (the locals are not written back).
//   ADD_INT128_BLOCK(src, dst): 128 dwords, dst += src (wrapping). ACCUMULATE_FLOAT128_BLOCK(src, dst): 128 floats, dst +=
//     cvtps2dq(src) under the current MXCSR rounding (round to nearest by default; NaN / out of range give 0x80000000).
//   CONVERT_INT128_TO_FLOAT[_IN_PLACE]: 128 dwords to floats (cvtdq2ps / cvtpi2ps, rounding per MXCSR: nearest by default).
//   CONVERT_FLOAT128_TO_INT_IN_PLACE(csr, block): save MXCSR.RC, force round to nearest, 128 floats to dwords in place
//     (cvtps2dq / cvtps2pi; NaN / out of range 0x80000000), restore RC; csr is a scratch dword local.
//   COPY64 / COPY128: 64 / 128 bytes (16-byte aligned for the SSE2 forms); ZERO64 / ZERO128: 64 / 128 zero bytes.
//   The MMX forms end with emms.
// Portable twin (FABLE_PORTABLE): plain C++ in fable_portable (explicit 32-bit arithmetic); tests in
//   rebuild/tests/portable/test_MixKernelsSIMD.cpp.
// Use: write each macro call as a statement on a line of its own: the retail branch expands to `__asm` statements, and an
//   `__asm` statement runs to the end of its source line (anything after it on that line is read as assembler).
#ifndef FABLE_LIBPK_MIXKERNELSSIMD_H
#define FABLE_LIBPK_MIXKERNELSSIMD_H

#if defined(FABLE_PORTABLE) || defined(FABLE_DUAL_PATH_TEST)
#include <string.h>
#include <math.h>
namespace fable_portable
{
    typedef int MixInt32;                               // a 32-bit lane on every supported host (ILP32 / LP64 / LLP64)
    inline int Saturate16(int v) { return v > 32767 ? 32767 : (v < -32768 ? -32768 : v); }
    inline int MulHigh16(int a, int b)                  // pmulhw: the high 16 bits of the signed 32-bit product (floor)
    {
        int p = a * b;
        return (p - (p & 0xFFFF)) / 65536;
    }
    inline MixInt32 Wrap32(unsigned int v) { return v >= 0x80000000u ? -(MixInt32)(~v) - 1 : (MixInt32)v; }
    inline void ScaleAccumulate16(unsigned long count, const void* src, void* dst, const void* gains,
                                  const void* increments, int lanes, int groupsPerCount)
    {
        const short* s = (const short*)src;
        MixInt32* d = (MixInt32*)dst;
        int g[8], inc[8];
        for (int l = 0; l < lanes; ++l) {
            g[l] = ((const short*)gains)[l];
            inc[l] = increments ? ((const short*)increments)[l] : 0;
        }
        for (unsigned long n = 0; n < count * (unsigned long)groupsPerCount; ++n, s += lanes, d += lanes) {
            for (int l = 0; l < lanes; ++l)
                d[l] = Wrap32((unsigned int)d[l] + (unsigned int)(MulHigh16(s[l], g[l]) * 2));
            if (increments)
                for (int l = 0; l < lanes; ++l)
                    g[l] = Saturate16(g[l] + inc[l]);
        }
    }
    inline void AddInt128Block(const void* src, void* dst)
    {
        const MixInt32* s = (const MixInt32*)src; MixInt32* d = (MixInt32*)dst;
        for (int i = 0; i < 128; ++i) d[i] = Wrap32((unsigned int)d[i] + (unsigned int)s[i]);
    }
    inline MixInt32 CvtFloatToIntNearest(float f)       // cvtps2dq / cvtps2pi under RC = nearest-even
    {
        if (!(f >= -2147483648.0f && f < 2147483648.0f)) return (MixInt32)0x80000000u;   // NaN, out of range
        double r = (double)f, fl = floor(r), frac = r - fl;  // exact: |f| < 2^31
        if (frac > 0.5 || (frac == 0.5 && fmod(fl, 2.0) != 0.0)) fl += 1.0;
        return (MixInt32)fl;
    }
    inline void AccumulateFloat128Block(const void* src, void* dst)
    {
        const float* s = (const float*)src; MixInt32* d = (MixInt32*)dst;
        for (int i = 0; i < 128; ++i) d[i] = Wrap32((unsigned int)d[i] + (unsigned int)CvtFloatToIntNearest(s[i]));
    }
    inline void ConvertInt128ToFloat(const void* src, void* dst)
    {
        MixInt32 v[128]; memcpy(v, src, sizeof v);
        float f[128];
        for (int i = 0; i < 128; ++i) f[i] = (float)v[i];
        memcpy(dst, f, sizeof f);
    }
    inline void ConvertFloat128ToIntNearest(void* block)
    {
        float f[128]; memcpy(f, block, sizeof f);
        MixInt32 v[128];
        for (int i = 0; i < 128; ++i) v[i] = CvtFloatToIntNearest(f[i]);
        memcpy(block, v, sizeof v);
    }
    inline void CopyBytes(void* dst, const void* src, unsigned int bytes) { memmove(dst, src, bytes); }
    inline void ZeroBytes(void* dst, unsigned int bytes) { memset(dst, 0, bytes); }
}
#endif
#ifdef FABLE_PORTABLE
#define LIBPK_MMX_SCALE_ACCUMULATE16_RAMP(count, src, dst, gains, increments) \
    fable_portable::ScaleAccumulate16((count), (src), (dst), &(gains), &(increments), 4, 2)
#define LIBPK_MMX_SCALE_ACCUMULATE16(count, src, dst, gains) \
    fable_portable::ScaleAccumulate16((count), (src), (dst), &(gains), 0, 4, 2)
#define LIBPK_SSE2_SCALE_ACCUMULATE16_RAMP(count, src, dst, gains, increments) \
    fable_portable::ScaleAccumulate16((count), (src), (dst), &(gains), &(increments), 8, 1)
#define LIBPK_SSE2_SCALE_ACCUMULATE16(count, src, dst, gains) \
    fable_portable::ScaleAccumulate16((count), (src), (dst), &(gains), 0, 8, 1)
#define LIBPK_SSE2_SCALE_ACCUMULATE16_RAMP_UNALIGNED(groupCount, source, accumulator, gains, increments) \
    fable_portable::ScaleAccumulate16((groupCount), (source), (accumulator), &(gains), &(increments), 8, 1)
#define LIBPK_SSE2_SCALE_ACCUMULATE16_UNALIGNED(count, src, dst, gains) \
    fable_portable::ScaleAccumulate16((count), (src), (dst), &(gains), 0, 8, 1)
#define LIBPK_SSE2_ADD_INT128_BLOCK(src, dst) \
    fable_portable::AddInt128Block((src), (dst))
#define LIBPK_SSE2_ACCUMULATE_FLOAT128_BLOCK(src, dst) \
    fable_portable::AccumulateFloat128Block((src), (dst))
#define LIBPK_SSE2_CONVERT_INT128_TO_FLOAT(src, dst) \
    fable_portable::ConvertInt128ToFloat((src), (dst))
#define LIBPK_SSE_CONVERT_INT128_TO_FLOAT(src, dst) \
    fable_portable::ConvertInt128ToFloat((src), (dst))
#define LIBPK_SSE2_CONVERT_INT128_TO_FLOAT_IN_PLACE(block) \
    fable_portable::ConvertInt128ToFloat((block), (block))
#define LIBPK_SSE_CONVERT_INT128_TO_FLOAT_IN_PLACE(block) \
    fable_portable::ConvertInt128ToFloat((block), (block))
#define LIBPK_SSE2_CONVERT_FLOAT128_TO_INT_IN_PLACE(csr, block) \
    ((void)(csr), fable_portable::ConvertFloat128ToIntNearest((block)))
#define LIBPK_SSE_CONVERT_FLOAT128_TO_INT_IN_PLACE(csr, block) \
    ((void)(csr), fable_portable::ConvertFloat128ToIntNearest((block)))
#define LIBPK_SSE2_COPY64(source, history) \
    fable_portable::CopyBytes((history), (source), 0x40)
#define LIBPK_SSE2_COPY128(src, dst) \
    fable_portable::CopyBytes((dst), (src), 0x80)
#define LIBPK_MMX_COPY128(src, dst) \
    fable_portable::CopyBytes((dst), (src), 0x80)
#define LIBPK_SSE_ZERO64(history) \
    fable_portable::ZeroBytes((history), 0x40)
#define LIBPK_MMX_ZERO64(history) \
    fable_portable::ZeroBytes((history), 0x40)
#define LIBPK_SSE_ZERO128(tail) \
    fable_portable::ZeroBytes((tail), 0x80)
#define LIBPK_MMX_ZERO128(tail) \
    fable_portable::ZeroBytes((tail), 0x80)
#else
#define LIBPK_MMX_SCALE_ACCUMULATE16_RAMP(count, src, dst, gains, increments) \
    __asm mov ecx,count \
    __asm mov eax,src \
    __asm mov edx,dst \
    __asm movq mm7,gains \
    __asm movq mm6,increments \
    __asm align 16 \
    __asm lpk_mr_next_block: \
    __asm movq mm0,[eax] \
    __asm pmulhw mm0,mm7 \
    __asm paddsw mm7,mm6 \
    __asm pxor mm1,mm1 \
    __asm pxor mm2,mm2 \
    __asm punpcklwd mm1,mm0 \
    __asm punpckhwd mm2,mm0 \
    __asm psrad mm1,15 \
    __asm psrad mm2,15 \
    __asm paddd mm1,[edx] \
    __asm paddd mm2,[edx+8] \
    __asm movq [edx],mm1 \
    __asm movq [edx+8],mm2 \
    __asm movq mm3,[eax+8] \
    __asm pmulhw mm3,mm7 \
    __asm paddsw mm7,mm6 \
    __asm pxor mm4,mm4 \
    __asm pxor mm5,mm5 \
    __asm punpcklwd mm4,mm3 \
    __asm punpckhwd mm5,mm3 \
    __asm psrad mm4,15 \
    __asm psrad mm5,15 \
    __asm paddd mm4,[edx+10h] \
    __asm paddd mm5,[edx+18h] \
    __asm movq [edx+10h],mm4 \
    __asm add eax,10h \
    __asm movq [edx+18h],mm5 \
    __asm add edx,20h \
    __asm dec ecx \
    __asm jne lpk_mr_next_block \
    __asm emms
#define LIBPK_MMX_SCALE_ACCUMULATE16(count, src, dst, gains) \
    __asm mov ecx, count \
    __asm mov eax, src \
    __asm mov edx, dst \
    __asm movq mm7, gains \
    __asm align 16 \
    __asm lpk_ma_next: \
    __asm movq mm0, [eax] \
    __asm pmulhw mm0, mm7 \
    __asm pxor mm1, mm1 \
    __asm pxor mm2, mm2 \
    __asm punpcklwd mm1, mm0 \
    __asm punpckhwd mm2, mm0 \
    __asm psrad mm1, 15 \
    __asm psrad mm2, 15 \
    __asm paddd mm1, [edx] \
    __asm paddd mm2, [edx + 8] \
    __asm movq [edx], mm1 \
    __asm movq [edx + 8], mm2 \
    __asm movq mm3, [eax + 8] \
    __asm pmulhw mm3, mm7 \
    __asm pxor mm4, mm4 \
    __asm pxor mm5, mm5 \
    __asm punpcklwd mm4, mm3 \
    __asm punpckhwd mm5, mm3 \
    __asm psrad mm4, 15 \
    __asm psrad mm5, 15 \
    __asm paddd mm4, [edx + 10h] \
    __asm paddd mm5, [edx + 18h] \
    __asm movq [edx + 10h], mm4 \
    __asm add eax, 10h \
    __asm movq [edx + 18h], mm5 \
    __asm add edx, 20h \
    __asm dec ecx \
    __asm jne lpk_ma_next \
    __asm emms
#define LIBPK_SSE2_SCALE_ACCUMULATE16_RAMP(count, src, dst, gains, increments) \
    __asm mov ecx,count \
    __asm mov eax,src \
    __asm mov edx,dst \
    __asm movdqa xmm7,gains \
    __asm movdqa xmm6,increments \
    __asm align 16 \
    __asm lpk_sr_next_block: \
    __asm movdqa xmm0,[eax] \
    __asm pmulhw xmm0,xmm7 \
    __asm paddsw xmm7,xmm6 \
    __asm pxor xmm1,xmm1 \
    __asm pxor xmm2,xmm2 \
    __asm punpcklwd xmm1,xmm0 \
    __asm punpckhwd xmm2,xmm0 \
    __asm psrad xmm1,15 \
    __asm psrad xmm2,15 \
    __asm paddd xmm1,[edx] \
    __asm paddd xmm2,[edx+10h] \
    __asm movdqa [edx],xmm1 \
    __asm movdqa [edx+10h],xmm2 \
    __asm add eax,10h \
    __asm add edx,20h \
    __asm dec ecx \
    __asm jne lpk_sr_next_block
#define LIBPK_SSE2_SCALE_ACCUMULATE16(count, src, dst, gains) \
    __asm mov ecx,count \
    __asm mov eax,src \
    __asm mov edx,dst \
    __asm movdqa xmm7,gains \
    __asm align 16 \
    __asm lpk_sa_next_block: \
    __asm movdqa xmm0,[eax] \
    __asm pmulhw xmm0,xmm7 \
    __asm pxor xmm1,xmm1 \
    __asm pxor xmm2,xmm2 \
    __asm punpcklwd xmm1,xmm0 \
    __asm punpckhwd xmm2,xmm0 \
    __asm psrad xmm1,15 \
    __asm psrad xmm2,15 \
    __asm paddd xmm1,[edx] \
    __asm paddd xmm2,[edx+10h] \
    __asm movdqa [edx],xmm1 \
    __asm movdqa [edx+10h],xmm2 \
    __asm add eax,10h \
    __asm add edx,20h \
    __asm dec ecx \
    __asm jne lpk_sa_next_block
#define LIBPK_SSE2_SCALE_ACCUMULATE16_RAMP_UNALIGNED(groupCount, source, accumulator, gains, increments) \
    __asm mov ecx,groupCount \
    __asm mov eax,source \
    __asm mov edx,accumulator \
    __asm movdqa xmm7,gains \
    __asm movdqa xmm6,increments \
    __asm align 16 \
    __asm lpk_sru_next_block: \
    __asm movdqu xmm0,[eax] \
    __asm pmulhw xmm0,xmm7 \
    __asm paddsw xmm7,xmm6 \
    __asm pxor xmm1,xmm1 \
    __asm pxor xmm2,xmm2 \
    __asm punpcklwd xmm1,xmm0 \
    __asm punpckhwd xmm2,xmm0 \
    __asm psrad xmm1,15 \
    __asm psrad xmm2,15 \
    __asm paddd xmm1,[edx] \
    __asm paddd xmm2,[edx+10h] \
    __asm movdqa [edx],xmm1 \
    __asm movdqa [edx+10h],xmm2 \
    __asm add eax,10h \
    __asm add edx,20h \
    __asm dec ecx \
    __asm jne lpk_sru_next_block
#define LIBPK_SSE2_SCALE_ACCUMULATE16_UNALIGNED(count, src, dst, gains) \
    __asm mov ecx, count \
    __asm mov eax, src \
    __asm mov edx, dst \
    __asm movdqa xmm7, gains \
    __asm align 16 \
    __asm lpk_sau_next: \
    __asm movdqu xmm0, [eax] \
    __asm pmulhw xmm0, xmm7 \
    __asm pxor xmm1, xmm1 \
    __asm pxor xmm2, xmm2 \
    __asm punpcklwd xmm1, xmm0 \
    __asm punpckhwd xmm2, xmm0 \
    __asm psrad xmm1, 15 \
    __asm psrad xmm2, 15 \
    __asm paddd xmm1, [edx] \
    __asm paddd xmm2, [edx + 10h] \
    __asm movdqa [edx], xmm1 \
    __asm movdqa [edx + 10h], xmm2 \
    __asm add eax, 10h \
    __asm add edx, 20h \
    __asm dec ecx \
    __asm jne lpk_sau_next
#define LIBPK_SSE2_ADD_INT128_BLOCK(src, dst) \
    __asm mov ecx, 8 \
    __asm mov eax, src \
    __asm mov edx, dst \
    __asm align 16 \
    __asm lpk_ai_next: \
    __asm movdqa xmm0, [eax] \
    __asm movdqa xmm1, [eax + 10h] \
    __asm movdqa xmm2, [eax + 20h] \
    __asm movdqa xmm3, [eax + 30h] \
    __asm paddd xmm0, [edx] \
    __asm paddd xmm1, [edx + 10h] \
    __asm paddd xmm2, [edx + 20h] \
    __asm paddd xmm3, [edx + 30h] \
    __asm movdqa [edx], xmm0 \
    __asm movdqa [edx + 10h], xmm1 \
    __asm movdqa [edx + 20h], xmm2 \
    __asm movdqa [edx + 30h], xmm3 \
    __asm add eax, 40h \
    __asm add edx, 40h \
    __asm dec ecx \
    __asm jne lpk_ai_next
#define LIBPK_SSE2_ACCUMULATE_FLOAT128_BLOCK(src, dst) \
    __asm mov ecx,8 \
    __asm mov eax,src \
    __asm mov edx,dst \
    __asm lpk_af_next_pass: \
    __asm cvtps2dq xmm0,[eax] \
    __asm cvtps2dq xmm1,[eax+10h] \
    __asm cvtps2dq xmm2,[eax+20h] \
    __asm cvtps2dq xmm3,[eax+30h] \
    __asm paddd xmm0,[edx] \
    __asm paddd xmm1,[edx+10h] \
    __asm paddd xmm2,[edx+20h] \
    __asm paddd xmm3,[edx+30h] \
    __asm movdqa [edx],xmm0 \
    __asm movdqa [edx+10h],xmm1 \
    __asm movdqa [edx+20h],xmm2 \
    __asm movdqa [edx+30h],xmm3 \
    __asm add eax,40h \
    __asm add edx,40h \
    __asm dec ecx \
    __asm jne lpk_af_next_pass
#define LIBPK_SSE2_CONVERT_INT128_TO_FLOAT(src, dst) \
    __asm mov ecx, 8 \
    __asm mov eax, src \
    __asm mov edx, dst \
    __asm align 16 \
    __asm lpk_ci2_next2: \
    __asm cvtdq2ps xmm0, [eax] \
    __asm cvtdq2ps xmm1, [eax + 10h] \
    __asm cvtdq2ps xmm2, [eax + 20h] \
    __asm cvtdq2ps xmm3, [eax + 30h] \
    __asm movaps [edx], xmm0 \
    __asm movaps [edx + 10h], xmm1 \
    __asm movaps [edx + 20h], xmm2 \
    __asm movaps [edx + 30h], xmm3 \
    __asm add eax, 40h \
    __asm add edx, 40h \
    __asm dec ecx \
    __asm jne lpk_ci2_next2
#define LIBPK_SSE_CONVERT_INT128_TO_FLOAT(src, dst) \
    __asm mov ecx, 16 \
    __asm mov eax, src \
    __asm mov edx, dst \
    __asm align 16 \
    __asm lpk_ci1_next1: \
    __asm cvtpi2ps xmm0, [eax] \
    __asm cvtpi2ps xmm1, [eax + 8] \
    __asm cvtpi2ps xmm2, [eax + 10h] \
    __asm cvtpi2ps xmm3, [eax + 18h] \
    __asm movlps [edx], xmm0 \
    __asm movlps [edx + 8], xmm1 \
    __asm movlps [edx + 10h], xmm2 \
    __asm movlps [edx + 18h], xmm3 \
    __asm add eax, 20h \
    __asm add edx, 20h \
    __asm dec ecx \
    __asm jne lpk_ci1_next1 \
    __asm emms
#define LIBPK_SSE2_CONVERT_INT128_TO_FLOAT_IN_PLACE(block) \
    __asm mov ecx, 8 \
    __asm mov eax, block \
    __asm align 16 \
    __asm lpk_cp2_next2: \
    __asm cvtdq2ps xmm0, [eax] \
    __asm cvtdq2ps xmm1, [eax + 10h] \
    __asm cvtdq2ps xmm2, [eax + 20h] \
    __asm cvtdq2ps xmm3, [eax + 30h] \
    __asm movaps [eax], xmm0 \
    __asm movaps [eax + 10h], xmm1 \
    __asm movaps [eax + 20h], xmm2 \
    __asm movaps [eax + 30h], xmm3 \
    __asm add eax, 40h \
    __asm dec ecx \
    __asm jne lpk_cp2_next2
#define LIBPK_SSE_CONVERT_INT128_TO_FLOAT_IN_PLACE(block) \
    __asm mov ecx, 16 \
    __asm mov eax, block \
    __asm align 16 \
    __asm lpk_cp1_next1: \
    __asm cvtpi2ps xmm0, [eax] \
    __asm cvtpi2ps xmm1, [eax + 8] \
    __asm cvtpi2ps xmm2, [eax + 10h] \
    __asm cvtpi2ps xmm3, [eax + 18h] \
    __asm movlps [eax], xmm0 \
    __asm movlps [eax + 8], xmm1 \
    __asm movlps [eax + 10h], xmm2 \
    __asm movlps [eax + 18h], xmm3 \
    __asm add eax, 20h \
    __asm dec ecx \
    __asm jne lpk_cp1_next1 \
    __asm emms
#define LIBPK_SSE2_CONVERT_FLOAT128_TO_INT_IN_PLACE(csr, block) \
    __asm stmxcsr csr \
    __asm mov edx, csr \
    __asm stmxcsr csr \
    __asm mov eax, csr \
    __asm and eax, 0FFFF9FFFh \
    __asm mov csr, eax \
    __asm ldmxcsr csr \
    __asm and edx, 6000h \
    __asm mov ecx, 8 \
    __asm mov eax, block \
    __asm align 16 \
    __asm lpk_cf2_next2: \
    __asm cvtps2dq xmm0, [eax] \
    __asm cvtps2dq xmm1, [eax + 10h] \
    __asm cvtps2dq xmm2, [eax + 20h] \
    __asm cvtps2dq xmm3, [eax + 30h] \
    __asm movdqa [eax], xmm0 \
    __asm movdqa [eax + 10h], xmm1 \
    __asm movdqa [eax + 20h], xmm2 \
    __asm movdqa [eax + 30h], xmm3 \
    __asm add eax, 40h \
    __asm dec ecx \
    __asm jne lpk_cf2_next2 \
    __asm stmxcsr csr \
    __asm mov ecx, csr \
    __asm and ecx, 0FFFF9FFFh \
    __asm or ecx, edx \
    __asm mov csr, ecx \
    __asm ldmxcsr csr
#define LIBPK_SSE_CONVERT_FLOAT128_TO_INT_IN_PLACE(csr, block) \
    __asm stmxcsr csr \
    __asm mov edx, csr \
    __asm stmxcsr csr \
    __asm mov eax, csr \
    __asm and eax, 0FFFF9FFFh \
    __asm mov csr, eax \
    __asm ldmxcsr csr \
    __asm and edx, 6000h \
    __asm mov ecx, 16 \
    __asm mov eax, block \
    __asm align 16 \
    __asm lpk_cf1_next1: \
    __asm cvtps2pi mm0, [eax] \
    __asm cvtps2pi mm1, [eax + 8] \
    __asm cvtps2pi mm2, [eax + 10h] \
    __asm cvtps2pi mm3, [eax + 18h] \
    __asm movq [eax], mm0 \
    __asm movq [eax + 8], mm1 \
    __asm movq [eax + 10h], mm2 \
    __asm movq [eax + 18h], mm3 \
    __asm add eax, 20h \
    __asm dec ecx \
    __asm jne lpk_cf1_next1 \
    __asm emms \
    __asm stmxcsr csr \
    __asm mov ecx, csr \
    __asm and ecx, 0FFFF9FFFh \
    __asm or ecx, edx \
    __asm mov csr, ecx \
    __asm ldmxcsr csr
#define LIBPK_SSE2_COPY64(source, history) \
    __asm mov esi, source \
    __asm mov edi, history \
    __asm movdqa xmm0, [esi] \
    __asm movdqa xmm1, [esi + 10h] \
    __asm movdqa xmm2, [esi + 20h] \
    __asm movdqa xmm3, [esi + 30h] \
    __asm movdqa [edi], xmm0 \
    __asm movdqa [edi + 10h], xmm1 \
    __asm movdqa [edi + 20h], xmm2 \
    __asm movdqa [edi + 30h], xmm3
#define LIBPK_SSE2_COPY128(src, dst) \
    __asm mov esi, src \
    __asm mov edi, dst \
    __asm movdqa xmm0, [esi] \
    __asm movdqa xmm1, [esi + 10h] \
    __asm movdqa xmm2, [esi + 20h] \
    __asm movdqa xmm3, [esi + 30h] \
    __asm movdqa xmm4, [esi + 40h] \
    __asm movdqa xmm5, [esi + 50h] \
    __asm movdqa xmm6, [esi + 60h] \
    __asm movdqa xmm7, [esi + 70h] \
    __asm movdqa [edi], xmm0 \
    __asm movdqa [edi + 10h], xmm1 \
    __asm movdqa [edi + 20h], xmm2 \
    __asm movdqa [edi + 30h], xmm3 \
    __asm movdqa [edi + 40h], xmm4 \
    __asm movdqa [edi + 50h], xmm5 \
    __asm movdqa [edi + 60h], xmm6 \
    __asm movdqa [edi + 70h], xmm7
#define LIBPK_MMX_COPY128(src, dst) \
    __asm mov esi, src \
    __asm mov edi, dst \
    __asm movq mm0, [esi] \
    __asm movq [edi], mm0 \
    __asm movq mm1, [esi + 8h] \
    __asm movq [edi + 8h], mm1 \
    __asm movq mm2, [esi + 10h] \
    __asm movq [edi + 10h], mm2 \
    __asm movq mm3, [esi + 18h] \
    __asm movq [edi + 18h], mm3 \
    __asm movq mm4, [esi + 20h] \
    __asm movq [edi + 20h], mm4 \
    __asm movq mm5, [esi + 28h] \
    __asm movq [edi + 28h], mm5 \
    __asm movq mm6, [esi + 30h] \
    __asm movq [edi + 30h], mm6 \
    __asm movq mm7, [esi + 38h] \
    __asm movq [edi + 38h], mm7 \
    __asm movq mm0, [esi + 40h] \
    __asm movq [edi + 40h], mm0 \
    __asm movq mm1, [esi + 48h] \
    __asm movq [edi + 48h], mm1 \
    __asm movq mm2, [esi + 50h] \
    __asm movq [edi + 50h], mm2 \
    __asm movq mm3, [esi + 58h] \
    __asm movq [edi + 58h], mm3 \
    __asm movq mm4, [esi + 60h] \
    __asm movq [edi + 60h], mm4 \
    __asm movq mm5, [esi + 68h] \
    __asm movq [edi + 68h], mm5 \
    __asm movq mm6, [esi + 70h] \
    __asm movq [edi + 70h], mm6 \
    __asm movq mm7, [esi + 78h] \
    __asm movq [edi + 78h], mm7 \
    __asm emms
#define LIBPK_SSE_ZERO64(history) \
    __asm mov edi, history \
    __asm xorps xmm0, xmm0 \
    __asm movdqa [edi], xmm0 \
    __asm movdqa [edi + 10h], xmm0 \
    __asm movdqa [edi + 20h], xmm0 \
    __asm movdqa [edi + 30h], xmm0
#define LIBPK_MMX_ZERO64(history) \
    __asm mov edi, history \
    __asm pxor mm0, mm0 \
    __asm movq [edi], mm0 \
    __asm movq [edi + 8h], mm0 \
    __asm movq [edi + 10h], mm0 \
    __asm movq [edi + 18h], mm0 \
    __asm movq [edi + 20h], mm0 \
    __asm movq [edi + 28h], mm0 \
    __asm movq [edi + 30h], mm0 \
    __asm movq [edi + 38h], mm0 \
    __asm emms
#define LIBPK_SSE_ZERO128(tail) \
    __asm mov edi, tail \
    __asm xorps xmm0, xmm0 \
    __asm movdqa [edi], xmm0 \
    __asm movdqa [edi + 10h], xmm0 \
    __asm movdqa [edi + 20h], xmm0 \
    __asm movdqa [edi + 30h], xmm0 \
    __asm movdqa [edi + 40h], xmm0 \
    __asm movdqa [edi + 50h], xmm0 \
    __asm movdqa [edi + 60h], xmm0 \
    __asm movdqa [edi + 70h], xmm0
#define LIBPK_MMX_ZERO128(tail) \
    __asm mov edi, tail \
    __asm pxor mm0, mm0 \
    __asm movq [edi], mm0 \
    __asm movq [edi + 8h], mm0 \
    __asm movq [edi + 10h], mm0 \
    __asm movq [edi + 18h], mm0 \
    __asm movq [edi + 20h], mm0 \
    __asm movq [edi + 28h], mm0 \
    __asm movq [edi + 30h], mm0 \
    __asm movq [edi + 38h], mm0 \
    __asm movq [edi + 40h], mm0 \
    __asm movq [edi + 48h], mm0 \
    __asm movq [edi + 50h], mm0 \
    __asm movq [edi + 58h], mm0 \
    __asm movq [edi + 60h], mm0 \
    __asm movq [edi + 68h], mm0 \
    __asm movq [edi + 70h], mm0 \
    __asm movq [edi + 78h], mm0 \
    __asm emms
#endif

#endif  // FABLE_LIBPK_MIXKERNELSSIMD_H
