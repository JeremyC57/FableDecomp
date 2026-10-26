// GFGetAngleYZTo(const C3DVector& from, const C3DVector& to) -- elevation from one position to another (atan2 of the rise over the XY distance) as a fraction of a full turn wrapped into [0, 1)
// Leaf: ?GFGetAngleYZTo@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00754E70, 117 bytes (+11 int3), `__fastcall` (from in ecx, to in edx, `ret`),
//   float in st(0).
// Name: Ego_r.pdb / FableWin.pdb `?GFGetAngleYZTo@@YIMABVC3DVector@@0@Z` (name set); the XY twin GFGetAngleXYTo is landed
//   at 0x00661540. Callers: CTCLook::GetAnglesFromBodyToPos 0x00751C70 (0x00751D45, 0x00751E64), CTCLook::SetEyesLooking
//   0x00752930 (0x00752AB1), 0x0075403D, 0x0086361F (both landed CTCLook callers declare it as the `(from, to)` YZ twin).
// Callees: _CIfmod @ 0x00BFEB28 (the intrinsic fmod). Constants: 1/(2*pi) 0x01237F08, 0.0f 0x0122DEDC, 1.0f 0x0122DED8,
//   1.0 (double) 0x01237F00.
// Label correction: the manifest's "Math_AngleFromPoints" is descriptive.
// Shape: the three differences are formed first (x and y as from - to, z as to - from), then
//   `atan2(dz, sqrt(dy*dy + dx*dx)) * (1 / 2pi)` (intrinsic atan2 / sqrt -> fpatan / fsqrt; the y square is formed
//   first), stored once to the `push ecx` frame float, then the landed `__forceinline WrapAngle` of the XY twin
//   (`if (a < 0 || a >= 1) { a = fmod(a, 1.0); if (a < 0) a += 1; }`, the fmod result returned off the x87 stack).
#include <math.h>
#pragma intrinsic(atan2, sqrt, fmod)

class C3DVector { public:
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

float __fastcall GFGetAngleYZTo(const C3DVector& from, const C3DVector& to)
{
    float dx = from.X - to.X;
    float dy = from.Y - to.Y;
    float dz = to.Z - from.Z;
    float angle = (float)atan2(dz, sqrt(dy * dy + dx * dx)) * 0.15915494f;
    return WrapAngle(angle);
}