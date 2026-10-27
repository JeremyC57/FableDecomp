// MillibelsToAmplitude(long millibels) -- libpk: 0 at or below -10000 mB (silence), else 10^(mB / 2000) computed as 2^(mB * 0.0005 * log2 10) through the x87 base-2 instructions
// Leaf: ?MillibelsToAmplitude@@
// Retail-asm: 0x00C77735..0x00C77764 `fld [esp+4] ; fmul [esp] ; frndint ; fst [esp+8] ; wait ; fld1 ; fscale ; fxch st(1) ; fstp [esp+10h] ; fld [esp+4] ; fmul [esp] ; fsub [esp+8] ; f2xm1 ; fadd [esp+18h] ; fmulp st(1),st ; fstp [esp+0Ch] ; wait` -- libpk's inline-assembly 2^x block over named float locals (17 instructions, no branch: rasm1 family (e); the Pow10 helper 0x00C53B10 expanded in place, 1.0f homed in the dead `millibels` slot). frndint, fscale and f2xm1 have no VC7.1 intrinsic form and the `wait`s are asm-only (`#pragma intrinsic(exp)` gives fldl2e..fscale without fwait, `pow` becomes a `_CIpow` jump: rasm1 probe); the prologue test, the exponent and the two constants are C++.
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C77700, 109 bytes (+ int3 padding), __cdecl, float in st(0). Byte-identical twin of 0x00C40EE0
// (the same helper in a second libpk TU). Called from 0x00C77798 and the reverb dispatcher 0x00C7856F. Constants: 0x01264AF0 = 0.0005f,
// 0x0122DEDC = 0.0f, the immediate 0x40549A78 = log2(10). The catalog label "CAudioChannel_GetOutputGain" is descriptive.
float __cdecl MillibelsToAmplitude(long millibels)
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