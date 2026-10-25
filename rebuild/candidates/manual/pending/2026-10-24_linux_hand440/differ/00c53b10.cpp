// Pow10(float x) -- ten to the power x in single precision, through the x87 base-2 exponent instructions: 2^(x*log2 10) split into a rounded integer part (fscale) and a fractional part (f2xm1)
// Leaf: ?Pow10@@
// Grade: DIFFER(96v75) -- retail_asm candidate: retail computes 10^x with an inline-asm frndint / fscale / f2xm1 block; the C++ reference goes through the CRT pow and floor
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C53B10, length 75 bytes (+ int3 padding); cdecl, plain `ret`, the result
// in st(0); no direct callers.
// Retail-asm evidence: `sub esp,14h`; two C++ float locals log2(10) = 3.3219280f (0x40549A78) and 1.0f (0x3F800000) are
// stored, then an inline-asm block runs `fld [log2_10] ; fmul [x] ; frndint ; fst [int] ; fwait ; fld1 ; fscale ;
// fxch st(1) ; fstp [scratch] ; fld [log2_10] ; fmul [x] ; fsub [int] ; f2xm1 ; fadd [one] ; fmulp st(1),st ; fstp
// [result] ; fwait`, and C++ returns the result (`fld [result]`). frndint / fscale / f2xm1 have no VC7.1 intrinsic. The
// lane rules forbid inline asm here; this is the C++ reference body (the CRT pow on the same exponent split).
// The catalog label "Pow2_Float" is wrong: the base is 10 (the multiplier is log2 10).

extern "C" double __cdecl pow(double x, double y);
extern "C" double __cdecl floor(double x);

float Pow10(float x)
{
    float log2of10 = 3.32192802f;
    float one = 1.0f;
    float exponent = log2of10 * x;
    float whole = (float)floor(exponent + 0.5f);                 // frndint (round to nearest; even on ties)
    float scale = (float)pow(2.0, (double)whole);                // fld1 ; fscale
    float fraction = (float)pow(2.0, (double)(exponent - whole)) - 1.0f;   // f2xm1
    return (fraction + one) * scale;
}
