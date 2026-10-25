// Atan2(float x, float y) -- angle of the vector (x, y) in (-pi, pi], computed octant by octant so the fpatan argument is always the smaller over the larger component
// Leaf: ?Atan2@@YGMMM@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C1CAB0, 200 bytes (`__stdcall`, `ret 8`, result in st0). Descriptive name, kept
//   from the landed caller GetRotationAngles 0x00C1CC10 (four calls); one more call at 0x00C1CBAC. The manifest's
//   "Math_Atan2Normalized" is propagated.
// Callees: none (atan is the /O2 intrinsic: `fdiv ; fld1 ; fpatan`). Constants pi/2 @ 0x012A332C, 0.0 @ 0x0122DEDC,
//   pi @ 0x0128F604, -pi/2 @ 0x012B3294 (all `__real@` pool floats).
// Shape: four guarded returns -- |y| <= x: atan(y/x); |x| <= y: pi/2 - atan(x/y); x <= -|y|: atan(y/x) -/+ pi by the
//   sign of y (the `y >= 0` compare is scheduled before the fpatan); otherwise -pi/2 - atan(x/y).
#include <math.h>

float __stdcall Atan2(float x, float y)
{
    if (x >= y && -y <= x) return atan(y / x);
    if (y >= x && -x <= y) return 1.5707964f - atan(x / y);
    if (-y >= x && x < y) {
        if (y >= 0.0f) return atan(y / x) + 3.1415927f;
        return atan(y / x) - 3.1415927f;
    }
    return -1.5707964f - atan(x / y);
}