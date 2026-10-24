// ceil(float value) -- the C++ float overload from <math.h>: widen to double, call the CRT ceil and narrow the result
// Leaf: ?ceil@@YAMM@Z
//
// Retail 0x00689370, 19 bytes (+ int3 padding to 32), `ret` (__cdecl).
// Callee: MSVCR71 ceil(double) through the import thunk @ 0x00BFEB8A.
// Callers: 0x0068AC70, 0x0068B24D, 0x0068B73B, 0x0068BBE2, 0x0068C54B, 0x006913F7 (the input-process TUs).
// Shape: VC7.1 <math.h> `inline float __cdecl ceil(float _X) { return ((float)ceil((double)_X)); }` emitted out of
//   line: `fld dword [esp+4] ; sub esp,8 ; fstp qword [esp] ; call ceil ; add esp,8 ; ret` (the narrowing is left on
//   the x87 stack, no fstp/fld round-trip).  Ego_r.pdb names `?ceil@@YAMM@Z` as well.
extern "C" double __cdecl ceil(double value);               // 0x00BFEB8A (MSVCR71 import thunk)

float __cdecl ceil(float value)
{
    return (float)ceil((double)value);
}