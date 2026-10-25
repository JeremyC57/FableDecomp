// exp(float value) -- the C++ float overload of exp from <math.h>: widen to double, the /Oi true-intrinsic exp on the x87 stack (x * log2(e), split into integer and fraction, f2xm1 + 1, fscale), narrowed without a store
// Leaf: ?exp@@YAMM@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C595E0, length 27 bytes (+ int3 padding); `ret` (__cdecl), the
// result on the x87 stack.
// Callee: none (the intrinsic expands inline). Caller: 0x00C723C5 in the GameCODA source update (`exp(t * rate)` on a
// float, stored to +0x30); that TU called the overload out of line.
// Label: the manifest's "Math_Exp2" describes 2^x; the body is e^x (the `fldl2e ; fmulp` prologue): VC7.1 <math.h>
// `inline float __cdecl exp(float _X) { return (expf(_X)); }` over `inline float __cdecl expf(float _X)
// { return ((float)exp((double)_X)); }`, emitted out of line (same bytes as `_expf`; the precedent is the landed
// `?ceil@@YAMM@Z` 0x00689370, which also defines the overload itself instead of including <math.h>).
// Shape: under /O2 (no /Op) `exp(double)` is a true intrinsic: `fldl2e ; fmulp ; fld st0 ; frndint ; fxch ; fsub
// st,st(1) ; f2xm1 ; fld1 ; faddp ; fscale ; fstp st(1)`, no `_CIexp` call.
extern "C" double __cdecl exp(double value);
#pragma intrinsic(exp)

float __cdecl exp(float value)
{
    return (float)exp((double)value);
}