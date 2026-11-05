// GF_SSE_MATRIX_SET_INTERPOLATE(a, b, self, oneMinusT, weight) -- the hand-written SSE kernel of CMatrix4x4::AlignedSetInterpolate
// (0x00A5CEE0, ?AlignedSetInterpolate@CMatrix4x4@@...; 16-byte aligned matrices), isolated as a macro: self = a*(1-t) + b*t
// element by element.
// Retail-asm-helper: GF_SSE_MATRIX_SET_INTERPOLATE
// Retail-asm: 0x00A5CF10 `mov eax,[ebp+8]` + 0x00A5CF16 `mov ecx,[esp+0Ch]` (the `self` copy of `this` re-read from its frame word)
//   + 0x00A5CF1A `movaps xmm0,[esp+20h]` + 0x00A5CF24 `shufps xmm0,xmm0,0` .. 0x00A5CF6E `movaps [ecx],xmm2` .. 0x00A5CF79
//   `movaps [ecx+30h],xmm5`: all eight xmm registers in a fixed schedule over pointers reloaded from the frame. The
//   <xmmintrin.h> spelling (_mm_load_ps / _mm_mul_ps / _mm_add_ps / _mm_store_ps over the four rows) has VC7.1 use `this`
//   in ecx without a frame copy and insert `movaps` copies for the two-operand ops (the batch ATTEMPTS.tsv, family
//   SSE_NVECTORMATH): the original block is inline asm.
// Operands: a, b -- const CMatrix4x4& (16 floats, 16-byte aligned); self -- CMatrix4x4* (the destination); oneMinusT, weight --
//   16-byte aligned four-float locals whose X is (1 - t) and t. Clobbers eax, ecx, edx, xmm0..xmm7.
// Semantics: self[k] = a[k]*oneMinusT.X + b[k]*weight.X, each product and the sum one IEEE single-precision op.
// Portable twin (FABLE_PORTABLE): plain C++; test rebuild/tests/portable/test_CMatrix4x4InterpolateSSE.cpp.
// Use: write each macro call as a statement on a line of its own: the retail branch expands to `__asm` statements, and an
//   `__asm` statement runs to the end of its source line (anything after it on that line is read as assembler).
// Proven (2026-11-05 round 2): `this` is stored to [esp+0Ch] and reloaded (`mov ecx,[esp+0Ch]`) with no call or alias
//   between, and the block keeps its source order on fixed xmm0..xmm7; 17 intrinsic attempts (one statement per retail
//   instruction included) never home `this` and VC7.1 reorders the loads and products (rebuild/candidates/manual/naming/2026-11-05_asm_helpers_r2/ATTEMPTS_SIMD_HELPERS.tsv).
#ifndef FABLE_ENGINE_CMATRIX4X4INTERPOLATESSE_H
#define FABLE_ENGINE_CMATRIX4X4INTERPOLATESSE_H

#if defined(FABLE_PORTABLE) || defined(FABLE_DUAL_PATH_TEST)
namespace fable_portable
{
    inline void MatrixSetInterpolateSSE(const float* a, const float* b, float* self, float oneMinusT, float t)
    {
        float r[16];
        for (int k = 0; k < 16; ++k) { float x = a[k] * oneMinusT; float y = b[k] * t; r[k] = x + y; }
        for (int k = 0; k < 16; ++k) self[k] = r[k];
    }
}
#define FABLE_PORTABLE_SSE_MATRIX_SET_INTERPOLATE(a, b, self, oneMinusT, weight) \
    fable_portable::MatrixSetInterpolateSSE((const float*)&(a), (const float*)&(b), (float*)(self), \
                                            ((const float*)&(oneMinusT))[0], ((const float*)&(weight))[0])
#endif

#ifdef FABLE_PORTABLE
#define GF_SSE_MATRIX_SET_INTERPOLATE(a, b, self, oneMinusT, weight) \
    FABLE_PORTABLE_SSE_MATRIX_SET_INTERPOLATE(a, b, self, oneMinusT, weight)
#else
#define GF_SSE_MATRIX_SET_INTERPOLATE(a, b, self, oneMinusT, weight) \
    __asm mov eax, a \
    __asm mov edx, b \
    __asm mov ecx, self \
    __asm movaps xmm0, oneMinusT \
    __asm movaps xmm1, weight \
    __asm shufps xmm0, xmm0, 0 \
    __asm shufps xmm1, xmm1, 0 \
    __asm movaps xmm2, [eax] \
    __asm movaps xmm3, [eax+10h] \
    __asm movaps xmm4, [eax+20h] \
    __asm movaps xmm5, [eax+30h] \
    __asm mulps xmm2, xmm0 \
    __asm mulps xmm3, xmm0 \
    __asm mulps xmm4, xmm0 \
    __asm mulps xmm5, xmm0 \
    __asm movaps xmm6, [edx] \
    __asm movaps xmm7, [edx+10h] \
    __asm mulps xmm6, xmm1 \
    __asm mulps xmm7, xmm1 \
    __asm addps xmm2, xmm6 \
    __asm addps xmm3, xmm7 \
    __asm movaps xmm6, [edx+20h] \
    __asm movaps xmm7, [edx+30h] \
    __asm mulps xmm6, xmm1 \
    __asm mulps xmm7, xmm1 \
    __asm addps xmm4, xmm6 \
    __asm addps xmm5, xmm7 \
    __asm movaps [ecx], xmm2 \
    __asm movaps [ecx+10h], xmm3 \
    __asm movaps [ecx+20h], xmm4 \
    __asm movaps [ecx+30h], xmm5
#endif

#endif  // FABLE_ENGINE_CMATRIX4X4INTERPOLATESSE_H
