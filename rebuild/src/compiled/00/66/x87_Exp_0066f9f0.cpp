// GFExp(double x) -- e raised to x through the x87 (inline `exp` intrinsic)
// Retail 0x0066F9F0, 27 bytes: fldl2e ; fmul [x] ; fld st0 ; frndint ; fxch ; fsub st1 ; f2xm1 ;
// fld1 ; faddp ; fscale ; fstp st1 ; ret 8. This is exactly VC7.1's inline expansion of `exp()`
// under /O2 (which implies /Oi; GOTCHAS: bakes of this sequence are plain `exp`). The callee
// pops its 8-byte argument and never touches ecx, so it is a __stdcall wrapper (a __thiscall
// member with an unused `this` compiles to the same bytes). Fable.exe holds no reference to
// this address (an unreferenced copy left in the input-type TU), so the name is descriptive in
// the engine's GF* global-function style.
// Leaf: GFExp

#include <math.h>
#pragma intrinsic(exp)

double __stdcall GFExp(double x)
{
    return exp(x);
}
