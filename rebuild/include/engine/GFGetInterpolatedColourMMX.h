// GF_MMX_INTERPOLATE_COLOUR(a, b, inverse, weight, zero, blend) -- the MMX block of GFGetInterpolatedColour (0x00A44250,
// FableWin.pdb ?GFGetInterpolatedColour@@YA?AVCRGBColour@@V1@0K@Z), isolated as a macro: blend each byte channel of two
// colours with weights (256 - w) and w, >> 8.
// Retail-asm-helper: GF_MMX_INTERPOLATE_COLOUR
// Retail-asm: 0x00A4426A `movd mm0,[esp+18h]` .. 0x00A4427E `punpcklbw mm0,dword ptr [esp+8]` / 0x00A44283 `punpcklbw mm1,dword
//   ptr [esp+8]` (the zero widening operand is a C local in memory) .. 0x00A4429A `paddw mm0,mm1` + 0x00A4429D `psrlw mm0,8` +
//   0x00A442A1 `packuswb mm0,mm0` + 0x00A442A4 `movd [esp],mm0` + 0x00A442A8 `emms` (GFGetInterpolatedColour). The <mmintrin.h>
//   spelling (_mm_cvtsi32_si64 / _mm_unpacklo_pi8 with _mm_setzero_si64 / _mm_unpacklo_pi16 / _mm_mullo_pi16 / _mm_add_pi16 /
//   _mm_srli_pi16 / _mm_packs_pu16 / _mm_cvtsi64_si32 / _mm_empty) widens against a pxor'd register, never a memory dword
//   (the batch ATTEMPTS.tsv, family MMX_COLOUR): the original block is inline asm.
// Operands: a, b -- CRGBColour (4 bytes) by value; inverse, weight, zero -- unsigned long lvalues (256 - w, w, 0); blend -- the
//   CRGBColour result lvalue. Clobbers mm0..mm3 (emms at the end).
// Semantics: per byte channel c: ((a[c]*inverse16 mod 2^16) + (b[c]*weight16 mod 2^16)) mod 2^16 >> 8 (pmullw low words, a
//   wrapping paddw, psrlw 8; packuswb never saturates: the shifted value is at most 0xFF).
// Portable twin (FABLE_PORTABLE): plain C++; test rebuild/tests/portable/test_GFGetInterpolatedColourMMX.cpp.
// Use: write each macro call as a statement on a line of its own: the retail branch expands to `__asm` statements, and an
//   `__asm` statement runs to the end of its source line (anything after it on that line is read as assembler).
// Proven (2026-11-05 round 2): no `and esp,-8` though the function reads stack arguments, and `punpcklbw mm0,[esp+8]` reads
//   the C local `zero` from memory; VC7.1 gives every function that holds an SSE / MMX value and touches its stack an `and esp,-16` (-8) frame, and never folds a load
//   from a C pointer into a SIMD operation (only its own spill slots) (rebuild/candidates/manual/naming/2026-11-05_asm_helpers_r2/ATTEMPTS_UNIVERSAL.tsv). 10 intrinsic and 90 plain-C++ attempts:
//   rebuild/candidates/manual/naming/2026-11-05_asm_helpers_r2/ATTEMPTS_SIMD_HELPERS.tsv.
#ifndef FABLE_ENGINE_GFGETINTERPOLATEDCOLOURMMX_H
#define FABLE_ENGINE_GFGETINTERPOLATEDCOLOURMMX_H

#if defined(FABLE_PORTABLE) || defined(FABLE_DUAL_PATH_TEST)
namespace fable_portable
{
    inline void InterpolateColourMMX(const void* a, const void* b, unsigned long inverse, unsigned long weight, void* blend)
    {
        const unsigned char* pa = (const unsigned char*)a;
        const unsigned char* pb = (const unsigned char*)b;
        unsigned char* out = (unsigned char*)blend;
        unsigned int inv16 = (unsigned int)inverse & 0xFFFFu;
        unsigned int w16 = (unsigned int)weight & 0xFFFFu;
        for (int c = 0; c < 4; ++c)
            out[c] = (unsigned char)(((((pa[c] * inv16) & 0xFFFFu) + ((pb[c] * w16) & 0xFFFFu)) & 0xFFFFu) >> 8);
    }
}
#define FABLE_PORTABLE_MMX_INTERPOLATE_COLOUR(a, b, inverse, weight, zero, blend) \
    ((void)(zero), fable_portable::InterpolateColourMMX(&(a), &(b), (inverse), (weight), &(blend)))
#endif

#ifdef FABLE_PORTABLE
#define GF_MMX_INTERPOLATE_COLOUR(a, b, inverse, weight, zero, blend) \
    FABLE_PORTABLE_MMX_INTERPOLATE_COLOUR(a, b, inverse, weight, zero, blend)
#else
#define GF_MMX_INTERPOLATE_COLOUR(a, b, inverse, weight, zero, blend) \
    __asm movd mm0, a \
    __asm movd mm1, b \
    __asm movd mm2, inverse \
    __asm movd mm3, weight \
    __asm punpcklbw mm0, zero \
    __asm punpcklbw mm1, zero \
    __asm punpcklwd mm2, mm2 \
    __asm punpcklwd mm3, mm3 \
    __asm punpckldq mm2, mm2 \
    __asm punpckldq mm3, mm3 \
    __asm pmullw mm0, mm2 \
    __asm pmullw mm1, mm3 \
    __asm paddw mm0, mm1 \
    __asm psrlw mm0, 8 \
    __asm packuswb mm0, mm0 \
    __asm movd blend, mm0 \
    __asm emms
#endif

#endif  // FABLE_ENGINE_GFGETINTERPOLATEDCOLOURMMX_H
