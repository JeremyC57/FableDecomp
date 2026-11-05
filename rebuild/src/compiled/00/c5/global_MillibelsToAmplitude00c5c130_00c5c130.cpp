// MillibelsToAmplitude_00c5c130(long millibels) -- libpk: 0 at or below -10000 mB (silence), else 10^(mB / 2000) computed as 2^(mB * 0.0005 * log2 10) through the x87 base-2 instructions
// Leaf: ?MillibelsToAmplitude_00c5c130@@YAMJ@Z
// Retail-asm: 0x00C5C165..0x00C5C194 `fld [esp+4] ; fmul [esp] ; frndint ; fst [esp+8] ; wait ; fld1 ; fscale ; fxch st(1) ; fstp [esp+10h] ; fld [esp+4] ; fmul [esp] ; fsub [esp+8] ; f2xm1 ; fadd [esp+18h] ; fmulp st(1),st ; fstp [esp+0Ch] ; wait` -- libpk's inline-assembly 2^x block (the landed retail_asm twin MillibelsToAmplitude_00c40ee0 0x00C40EE0, byte-identical); frndint, fscale and f2xm1 have no VC7.1 intrinsic form and the `wait`s are asm-only; the prologue test, the exponent and the two constants are C++.
// Helper (2026-11-05 asm migration): the 17-instruction x87 2^x block is the shared libpk/Pow10.h (LIBPK_POW10_BLOCK; byte-matching
//   path + FABLE_PORTABLE twin); the row has no asm of its own. C++/intrinsic attempts: the batch ATTEMPTS.tsv, family POW10.
// Retail: Fable.exe (TLC, VC7.1) @ 0x00c5c130, 109 bytes, __cdecl, float in st(0).
// Identity (descriptive): a byte-identical copy of the landed 0x00C40EE0 in another libpk TU (retail keeps each copy as
//   its own function), named per address as the twin is; callers 0x00C5C1B0 and 0x00C5CA60 (twice). Constants: 0x01264AF0 = 0.0005f,
//   0x0122DEDC = 0.0f, the immediate 0x40549A78 = log2(10).
#include "libpk/Pow10.h"
float __cdecl MillibelsToAmplitude_00c5c130(long millibels)
{
    if (millibels <= -10000)
        return 0.0f;
    float scale;
    float result;
    float whole;
    float log2of10 = 3.32192802f;                  // 0x40549A78
    float exponent = millibels * 0.0005f;
    float one = 1.0f;
    LIBPK_POW10_BLOCK(log2of10, exponent, whole, scale, one, result);
    return result;
}