// Pow10_00c77690(float x) -- libpk: ten to the power x in single precision through the x87 base-2 instructions: 2^(x*log2 10) split into a rounded integer part (fscale) and a fractional part (f2xm1)
// Leaf: ?Pow10_00c77690@@
// Retail-asm: 0x00C776A2..0x00C776D2 `fld [esp+4] ; fmul [esp+18h] ; frndint ; fst [esp+8] ; wait ; fld1 ; fscale ; fxch st(1) ; fstp [esp+10h] ; fld [esp+4] ; fmul [esp+18h] ; fsub [esp+8] ; f2xm1 ; fadd [esp] ; fmulp st(1),st ; fstp [esp+0Ch] ; wait` -- libpk's inline-assembly 2^x block over named float locals (17 instructions, no branch: rasm1 family (e)). frndint, fscale and f2xm1 have no VC7.1 intrinsic form and the `wait`s are asm-only (`#pragma intrinsic(exp)` gives fldl2e..fscale without fwait, `pow` becomes a `_CIpow` jump: rasm1 probe); the two constants are C++ stores and the result is read back by C++.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C77690, 75 bytes (+ int3 padding); __cdecl, plain `ret`, the result in st(0); no direct
// callers. One of seven byte-identical copies of libpk's Pow10 helper (one per TU: 0x00C53B10, 0x00C40E60, 0x00C42C30, 0x00C54A80, 0x00C5C0B0, 0x00C6BF60); the copy is named per
// address (one retail address, one name) because every copy is a separate retail function. The block is also inlined into
// 0x00C77700 / 0x00C40EE0 (MillibelsToAmplitude) and 0x00C6C000. The catalog label "Math_Pow2_Fast" is wrong: the multiplier is
// log2(10) = 3.3219280f (0x40549A78), so the base is 10.
float __cdecl Pow10_00c77690(float x)
{
    float scale;
    float result;
    float whole;
    float log2of10 = 3.32192802f;                  // 0x40549A78
    float one = 1.0f;
    __asm {
        fld   log2of10
        fmul  x
        frndint
        fst   whole
        wait
        fld1
        fscale
        fxch  st(1)
        fstp  scale
        fld   log2of10
        fmul  x
        fsub  whole
        f2xm1
        fadd  one
        fmulp st(1), st
        fstp  result
        wait
    }
    return result;
}