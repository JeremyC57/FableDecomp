// GFGetAngleXYTo(const C3DVector& from, const C3DVector& to) -- heading from one position to another in the XY plane, as a fraction of a full turn wrapped into [0, 1)
// Leaf: ?GFGetAngleXYTo@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00661540, 91 bytes, `__fastcall` (from in ecx, to in edx, `ret`), float in st(0).
// Name: Ego_r.pdb `?GFGetAngleXYTo@@YIMABVC3DVector@@0@Z` (name set): it reads X/Y only, and the landed caller
// CTCLook::GetTurningAngles 0x00751E80 calls it as `(from, to)` (9 call sites in all).
// Callees: _CIfmod @ 0x00BFEB28 (the intrinsic fmod). Constants: 1/(2*pi) 0x01237F08, 0.0f 0x0122DEDC, 1.0f 0x0122DED8,
// 1.0 (double) 0x01237F00.
// Label correction: the catalog "AngleBetweenPositions" is descriptive.
// Shape: `atan2(to.X - from.X, to.Y - from.Y) * (1 / 2pi)` (intrinsic -> fpatan), stored once to the frame float,
// then the landed `__forceinline WrapAngle` (0x007FAF60 model): `if (a < 0 || a >= 1) { a = fmod(a, 1.0); if (a < 0)
// a += 1; }` -- the fmod result is returned straight off the x87 stack.
// No pragma (`#pragma intrinsic(atan2, fmod)`).
#include <math.h>
#pragma intrinsic(atan2, fmod)

struct C3DVector {
    float X, Y, Z;
};

__forceinline float WrapAngle(float angle)
{
    if (angle < 0.0f || angle >= 1.0f) {
        angle = (float)fmod((double)angle, 1.0);
        if (angle < 0.0f)
            angle += 1.0f;
    }
    return angle;
}

float __fastcall GFGetAngleXYTo(const C3DVector& from, const C3DVector& to)
{
    float angle = (float)atan2(to.X - from.X, to.Y - from.Y) * 0.15915494f;
    return WrapAngle(angle);
}