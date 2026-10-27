// MillibelsToAmplitude_00c5c130(long millibels) -- libpk: 0 at or below -10000 mB (silence), else 10^(mB / 2000) computed as 2^(mB * 0.0005 * log2 10) through the x87 base-2 instructions
// Leaf: ?MillibelsToAmplitude_00c5c130@@YAMJ@Z
// Retail-asm: 0x00C5C165..0x00C5C194 `fld [esp+4] ; fmul [esp] ; frndint ; fst [esp+8] ; wait ; fld1 ; fscale ; fxch st(1) ; fstp [esp+10h] ; fld [esp+4] ; fmul [esp] ; fsub [esp+8] ; f2xm1 ; fadd [esp+18h] ; fmulp st(1),st ; fstp [esp+0Ch] ; wait` -- libpk's inline-assembly 2^x block (the landed retail_asm twin MillibelsToAmplitude_00c40ee0 0x00C40EE0, byte-identical); frndint, fscale and f2xm1 have no VC7.1 intrinsic form and the `wait`s are asm-only; the prologue test, the exponent and the two constants are C++.
// Retail: Fable.exe (TLC, VC7.1) @ 0x00c5c130, 109 bytes, __cdecl, float in st(0).
// Identity (descriptive): a byte-identical copy of the landed 0x00C40EE0 in another libpk TU (retail keeps each copy as
//   its own function), named per address as the twin is; callers 0x00C5C1B0 and 0x00C5CA60 (twice). Constants: 0x01264AF0 = 0.0005f,
//   0x0122DEDC = 0.0f, the immediate 0x40549A78 = log2(10).
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
    __asm {
        fld   log2of10
        fmul  exponent
        frndint
        fst   whole
        wait
        fld1
        fscale
        fxch  st(1)
        fstp  scale
        fld   log2of10
        fmul  exponent
        fsub  whole
        f2xm1
        fadd  one
        fmulp st(1), st
        fstp  result
        wait
    }
    return result;
}