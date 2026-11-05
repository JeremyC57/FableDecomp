// GF_SSE_ADD_WEIGHTED_VECTOR(source, target, weight) -- the hand-written SSE step of CParticleProgram::TransformVertices
// (0x00AE0E60): target += source * weight.X on one 16-byte aligned C4DVector, isolated as a macro.
// Retail-asm-helper: GF_SSE_ADD_WEIGHTED_VECTOR
// Retail-asm: 0x00AE0F91 `movaps xmm1,[edx]` + 0x00AE0F94 `movaps xmm2,[esp+30h]` + 0x00AE0F99 `shufps xmm2,xmm2,0` +
//   0x00AE0F9D `mulps xmm0,xmm2` + 0x00AE0FA0 `addps xmm1,xmm0` + 0x00AE0FA3 `movaps [edx],xmm1` (CParticleProgram::
//   TransformVertices): the weight goes through a 16-byte frame local and is broadcast with shufps, the pointers are
//   reloaded into eax / edx inside the loop. The <xmmintrin.h> spelling (_mm_load_ps / _mm_set_ps1 or _mm_load_ss +
//   _mm_shuffle_ps / _mm_mul_ps / _mm_add_ps / _mm_store_ps) has VC7.1 keep the pointers in its own registers and broadcast
//   from a register (the batch ATTEMPTS.tsv, family SSE_NVECTORMATH): the original block is inline asm.
// Operands: source, target -- C4DVector pointers (16-byte aligned); weight -- a 16-byte aligned C4DVector local whose X is
//   the blend weight. Clobbers eax, edx, xmm0..xmm2.
// Semantics: target.l = target.l + source.l * weight.X per lane, the product and the sum one IEEE single-precision op each.
// Portable twin (FABLE_PORTABLE): plain C++; test rebuild/tests/portable/test_CParticleProgramSSE.cpp.
// Use: write each macro call as a statement on a line of its own: the retail branch expands to `__asm` statements, and an
//   `__asm` statement runs to the end of its source line (anything after it on that line is read as assembler).
// Proven (2026-11-05 round 2): source and target are stored to frame slots before the SSE test (0x00AE0F76 / 0x00AE0F7A)
//   and reloaded in the block (0x00AE0F86 / 0x00AE0F8A) while the x87 path uses the registers; 13 intrinsic attempts (address-
//   taken pointers and a pointer array included) never store them (best 445 v 457 bytes; rebuild/candidates/manual/naming/2026-11-05_asm_helpers_r2/ATTEMPTS_SIMD_HELPERS.tsv).
#ifndef FABLE_ENGINE_CPARTICLEPROGRAMSSE_H
#define FABLE_ENGINE_CPARTICLEPROGRAMSSE_H

#if defined(FABLE_PORTABLE) || defined(FABLE_DUAL_PATH_TEST)
namespace fable_portable
{
    inline void AddWeightedVectorSSE(const float* source, float* target, float w)
    {
        for (int l = 0; l < 4; ++l) { float p = source[l] * w; target[l] = target[l] + p; }
    }
}
#define FABLE_PORTABLE_SSE_ADD_WEIGHTED_VECTOR(source, target, weight) \
    fable_portable::AddWeightedVectorSSE((const float*)(source), (float*)(target), ((const float*)&(weight))[0])
#endif

#ifdef FABLE_PORTABLE
#define GF_SSE_ADD_WEIGHTED_VECTOR(source, target, weight) FABLE_PORTABLE_SSE_ADD_WEIGHTED_VECTOR(source, target, weight)
#else
#define GF_SSE_ADD_WEIGHTED_VECTOR(source, target, weight) \
    __asm mov eax, source \
    __asm mov edx, target \
    __asm movaps xmm0, [eax] \
    __asm movaps xmm1, [edx] \
    __asm movaps xmm2, weight \
    __asm shufps xmm2, xmm2, 0 \
    __asm mulps xmm0, xmm2 \
    __asm addps xmm1, xmm0 \
    __asm movaps [edx], xmm1
#endif

#endif  // FABLE_ENGINE_CPARTICLEPROGRAMSSE_H
