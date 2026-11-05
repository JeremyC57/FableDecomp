// LIBPK_POW10_BLOCK(log2of10, x, whole, scale, one, result) -- libpk (the GameCODA audio middleware): ten to the power x in
// single precision through the x87 base-2 instructions, 2^(x*log2 10) split into a rounded integer part (frndint, fscale) and
// a fractional part (f2xm1). libpk's inline-assembly block over named float locals, out of line in each libpk TU as
// Pow10 (0x00C53B10 and its byte-identical copies 0x00C40E60, 0x00C42C30, 0x00C54A80, 0x00C5C0B0, 0x00C6BF60, 0x00C77690)
// and expanded in MillibelsToAmplitude (0x00C40EE0, 0x00C42C90, 0x00C53B60, 0x00C54AE0, 0x00C5C130, 0x00C77700),
// CAudioObject::SetMillibels 0x00C42FB0 and COnePoleLowPass::SetLevel 0x00C6C000 (libpk has no PDB: descriptive names).
// Retail-asm-helper: LIBPK_POW10_BLOCK
// Retail-asm: 0x00C53B22..0x00C53B52 `fld [esp+4] ; fmul [esp+18h] ; frndint ; fst [esp+8] ; wait ; fld1 ; fscale ; fxch st(1)
//   ; fstp [esp+10h] ; fld [esp+4] ; fmul [esp+18h] ; fsub [esp+8] ; f2xm1 ; fadd [esp] ; fmulp st(1),st ; fstp [esp+0Ch] ;
//   wait` (Pow10 0x00C53B10; the same 17 instructions in every copy listed above). frndint, fscale and f2xm1 have no VC7.1
//   intrinsic form and the `wait`s are asm-only: `#pragma intrinsic(exp)` gives fldl2e..fscale without fwait, `pow` becomes a
//   `_CIpow` jump, `floor` a call (the batch ATTEMPTS.tsv, family POW10).
// Operands: float lvalues; log2of10 = 3.32192802f (0x40549A78) and one = 1.0f are C++ locals; whole (the rounded exponent as
//   a float), scale (the same value, the fxch'd copy) and result are written.
// Semantics, under the CRT default control word 0x027F (precision 53 bits, round to nearest): t = log2of10 * x (exact: a
//   24 x 24-bit product); w = nearbyint(t) (ties to even, the sign kept: -0.4 gives -0); whole = scale = (float)w; f = t - whole (exact); result =
//   (float)((2^f - 1 + 1) * 2^w), the sum rounded to 53 bits, the power-of-two product exact (fscale scales by trunc(w) = w).
// Portable twin (FABLE_PORTABLE): plain C++ in double (exp2 for f2xm1's 2^f, ldexp for fscale); f2xm1 is accurate to the
//   64-bit significand before the 53-bit rounding, so the twin can differ from the asm only where (2^f) lands within one
//   double ulp of a float rounding boundary; the dual-path test probes 2,001 exponents over [-10, 10] mB-scaled plus the
//   edges and requires every result equal. Test: rebuild/tests/portable/test_Pow10.cpp.
// Use: write each macro call as a statement on a line of its own: the retail branch expands to `__asm` statements, and an
//   `__asm` statement runs to the end of its source line (anything after it on that line is read as assembler).
#ifndef FABLE_LIBPK_POW10_H
#define FABLE_LIBPK_POW10_H

#if defined(FABLE_PORTABLE) || defined(FABLE_DUAL_PATH_TEST)
#include <math.h>
namespace fable_portable
{
    inline double Nearbyint53(double t)                 // frndint under RC = nearest: ties to even
    {
        double r = floor(t + 0.5);
        if (r - t == 0.5 && fmod(r, 2.0) != 0.0)
            r -= 1.0;
        if (r == 0.0 && t < 0.0)
            r = -r;                                     // frndint keeps the sign: -0.4 rounds to -0 (not a -0.0 literal: VC7.1 folds it to +0)
        return r;
    }
    inline void Pow10Block(float log2of10, float x, float& whole, float& scale, float one, float& result)
    {
        double t = (double)log2of10 * (double)x;
        double w = Nearbyint53(t);
        whole = (float)w;
        scale = (float)w;
        double f = t - (double)whole;
        double m = (pow(2.0, f) - 1.0) + (double)one;
        result = (float)ldexp(m, (int)w);
    }
}
#endif

#ifdef FABLE_PORTABLE
#define LIBPK_POW10_BLOCK(log2of10, x, whole, scale, one, result) \
    fable_portable::Pow10Block((log2of10), (x), (whole), (scale), (one), (result))
#else
#define LIBPK_POW10_BLOCK(log2of10, x, whole, scale, one, result) \
    __asm fld log2of10 \
    __asm fmul x \
    __asm frndint \
    __asm fst whole \
    __asm wait \
    __asm fld1 \
    __asm fscale \
    __asm fxch st(1) \
    __asm fstp scale \
    __asm fld log2of10 \
    __asm fmul x \
    __asm fsub whole \
    __asm f2xm1 \
    __asm fadd one \
    __asm fmulp st(1), st \
    __asm fstp result \
    __asm wait
#endif

#endif  // FABLE_LIBPK_POW10_H
