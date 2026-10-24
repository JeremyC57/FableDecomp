// C2DVector::Rotate(float angle) -- turn the vector by a fraction of a full turn, keeping its length
// Leaf: ?Rotate@C2DVector@@QAEXM@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A141D0, 142 bytes, `ret 4`. 11 callers. PDB name (Ego_r) ?Rotate@C2DVector@@QAEXM@Z.
// Callees: none (atan2/sqrt/fmod/sin/cos are x87 intrinsics: fpatan, fsqrt, fprem, fsin, fcos).
// Shape: GetAngle = WrapAngle(atan2(X, Y) / 2pi) (both forced inline: a plain inline GetAngle stays a call),
//   length = sqrt(X*X + Y*Y), then X = sin(angle*2pi)*len, Y = cos(angle*2pi)*len with 2pi as the double
//   6.2831854820251465 (the float 2pi widened).
#include <math.h>
#pragma intrinsic(atan2, sqrt, fmod, sin, cos)

__forceinline float WrapAngle(float angle)
{
    if (angle < 0.0f || angle >= 1.0f) {
        angle = (float)fmod((double)angle, 1.0);
        if (angle < 0.0f)
            angle += 1.0f;
    }
    return angle;
}

class C2DVector {
public:
    float X, Y;
    __forceinline float GetAngle() const { return WrapAngle((float)atan2(X, Y) * 0.15915494f); }
    float GetLength() const { return (float)sqrt(X * X + Y * Y); }
    void Rotate(float angle);
};

void C2DVector::Rotate(float angle)
{
    float newAngle = GetAngle() + angle;
    float length = GetLength();
    X = (float)sin(newAngle * 6.2831854820251465) * length;
    Y = (float)cos(newAngle * 6.2831854820251465) * length;
}