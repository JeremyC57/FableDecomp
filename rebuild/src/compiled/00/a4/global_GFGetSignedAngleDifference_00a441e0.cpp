// GFGetSignedAngleDifference(float from, float to) -- signed difference to - from between two angles held as fractions of a full turn, wrapped into [-0.5, 0.5)
// Grade: RELOCATION_MATCH (agent902, 2nd grade) -- `if ((diff = to - from) < 0.0f || diff >= 1.0f)` keeps the compared value on the x87 stack (fcom, fst after it) and the re-spelled `to - from` in the fmod reloads its frame home, as retail.
// Leaf: ?GFGetSignedAngleDifference@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A441E0, length 98 bytes (+14 int3 pad), `__fastcall` with both floats on
//   the stack (`ret 8`), float in st(0).
// Name: Ego_r.pdb / FableWin.pdb `?GFGetSignedAngleDifference@@YIMMM@Z` (name set); the catalogue's "AngleDifference" is
//   descriptive. Callees: _CIfmod 0x00BFEB28 (the intrinsic fmod). Constants: 0.0f 0x0122DEDC, 1.0f 0x0122DED8,
//   0.5f 0x0122F59C, 1.0 (double) 0x01237F00.
// Tried: v1 `angle` reassigned in place (all of it through memory, 126v98); v2 `if (out of range) { w = fmod; return w<0 ? w+1 : w; } return angle;`
//   with the ternary 0.5 fold (tail matched, 106v98); v3 the `>= 0 && < 1` early return (test order flipped); v4 this
//   separate `wrapped = angle` result (only the two compare reloads left).
// Shape: one function: `float diff; if ((diff = to - from) < 0.0f || diff >= 1.0f) { diff = fmod(to - from, 1.0);
//   if (diff < 0.0f) diff += 1.0f; } return diff >= 0.5f ? -(1.0f - diff) : diff;` -- the assignment-in-condition value is
//   compared from the register (and stored to `to`'s dead argument slot after the first fcom); VC7.1 CSEs the second
//   `to - from` into that frame home, so the out-of-range path pops the register copy and reloads the slot for the
//   intrinsic fmod; the `fsubr [1.0f] ; fchs` pair is the -(1 - d) fold.
#include <math.h>
#pragma intrinsic(fmod)

float __fastcall GFGetSignedAngleDifference(float from, float to)
{
    float diff;
    if ((diff = to - from) < 0.0f || diff >= 1.0f) {
        diff = (float)fmod((double)(to - from), 1.0);
        if (diff < 0.0f)
            diff += 1.0f;
    }
    return diff >= 0.5f ? -(1.0f - diff) : diff;
}