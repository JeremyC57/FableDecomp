// GFGetSignedAngleDifference(float from, float to) -- signed difference to - from between two angles held as fractions of a full turn, wrapped into [-0.5, 0.5)
// Grade: DIFFER(106v98) -- the compares reload the homed `to - from` from [esp+8] (fld ; fcomp) where retail compares the live register copy (fcom, then fst after it); the wrap arm, the fmod tail and the 0.5 fold already match
// Leaf: ?GFGetSignedAngleDifference@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A441E0, length 98 bytes (+14 int3 pad), `__fastcall` with both floats on
//   the stack (`ret 8`), float in st(0).
// Name: Ego_r.pdb / FableWin.pdb `?GFGetSignedAngleDifference@@YIMMM@Z` (name set); the catalogue's "AngleDifference" is
//   descriptive. Callees: _CIfmod 0x00BFEB28 (the intrinsic fmod). Constants: 0.0f 0x0122DEDC, 1.0f 0x0122DED8,
//   0.5f 0x0122F59C, 1.0 (double) 0x01237F00.
// Tried: v1 `angle` reassigned in place (all of it through memory, 126v98); v2 `if (out of range) { w = fmod; return w<0 ? w+1 : w; } return angle;`
//   with the ternary 0.5 fold (tail matched, 106v98); v3 the `>= 0 && < 1` early return (test order flipped); v4 this
//   separate `wrapped = angle` result (only the two compare reloads left).
// Next: compare the result copy instead of the parameter (`if (wrapped < 0.0f || wrapped >= 1.0f) wrapped = fmod(angle..)`),
//   or the whole wrap as one ternary on `angle`.
// Shape: `to - from` is homed in `to`'s argument slot; a `__forceinline WrapAngle` into [0, 1) that differs from the landed
//   GFGetAngleXYTo / GFGetAngleYZTo one (retail keeps a register copy of the angle as the result on the in-range path and
//   reloads the slot for the fmod), then `return d >= 0.5f ? -(1.0f - d) : d;` (the `fsubr [1.0f] ; fchs` pair).
#include <math.h>
#pragma intrinsic(fmod)

__forceinline float WrapAngle(float angle)
{
    float wrapped = angle;
    if (angle < 0.0f || angle >= 1.0f) {
        wrapped = (float)fmod((double)angle, 1.0);
        if (wrapped < 0.0f)
            wrapped += 1.0f;
    }
    return wrapped;
}

float __fastcall GFGetSignedAngleDifference(float from, float to)
{
    float diff = WrapAngle(to - from);
    return diff >= 0.5f ? -(1.0f - diff) : diff;
}
