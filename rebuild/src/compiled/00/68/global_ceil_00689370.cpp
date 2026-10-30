// floor(float value) -- the C++ float overload from <math.h>: widen to double, call the CRT floor and narrow the result
// Leaf: ?floor@@YAMM@Z
//
// Retail 0x00689370, 19 bytes (+ int3 padding to 32), `ret` (__cdecl).
// Callee: MSVCR71 floor(double) through the import thunk @ 0x00BFEB8A (IAT slot 0x01440100 = MSVCR71.dll!floor,
//   lib_verify MATCH). The row was first landed as the `ceil` overload; the true float ceil is 0x0057A2D0, the same
//   19-byte shape calling the ceil thunk 0x00BFEB9C (IAT slot 0x014400F4). Ego_r.pdb carries both ?floor@@YAMM@Z and
//   ?ceil@@YAMM@Z.
// Callers: 0x0068AC70, 0x0068B24D, 0x0068B73B, 0x0068BBE2, 0x0068C54B, 0x006913F7 (the input-process TUs).
// Shape: VC7.1 <math.h> `inline float __cdecl floor(float _X) { return ((float)floor((double)_X)); }` emitted out of
//   line: `fld dword [esp+4] ; sub esp,8 ; fstp qword [esp] ; call floor ; add esp,8 ; ret` (the narrowing is left on
//   the x87 stack, no fstp/fld round-trip).
extern "C" double __cdecl floor(double value);              // 0x00BFEB8A (MSVCR71 import thunk)

float __cdecl floor(float value)
{
    return (float)floor((double)value);
}