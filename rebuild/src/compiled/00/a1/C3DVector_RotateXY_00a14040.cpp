// C3DVector::RotateXY(float angle) -- turn the vector about the vertical axis by a fraction of a full turn, keeping its elevation and length
// Leaf: ?RotateXY@C3DVector@@QAEXM@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A14040, 395 bytes, thiscall, `ret 4`.
// Identity: Ego_r.pdb / FableWin.pdb `?RotateXY@C3DVector@@QAEXM@Z`. The vector-math TU's donor order (Ego_r) is
//   Rotate(const C3DVector&, float), C2DVector::ToString, C3DVector::ToString, C2DVector::GetRandomUnitVector,
//   C2DVector::GetRandomPointWithinUnitCircle, RotateXY, C2DVector::Rotate, Rotate(float, float), AccurateNormalise,
//   GetMagnitudeAndNormalise ...; retail has the landed C3DVector::Rotate 0x00A13C60, C2DVector::ToString 0x00A13CE0,
//   C3DVector::ToString 0x00A13DA0, C2DVector::GetRandomUnitVector 0x00A13EB0, this row, C2DVector::Rotate 0x00A141D0,
//   0x00A14260 (two float arguments, `ret 8`) and AccurateNormalise 0x00A14440 in the same order, and this row takes one
//   float (`ret 4`). The manifest label "Math_CartesianToSpherical" is behavioural.
// Callees: _CIfmod 0x00BFEB28 (intrinsic fmod). Constants: 1/(2pi) 0x01237F08, 0.0f 0x0122DEDC, 1.0f 0x0122DED8,
//   1.0 (double) 0x01237F00, (double)2pi 0x0124F2B8.
// Shape: GetAngleXY() (atan2(X, Y) in turns, wrapped) + angle, GetAngleYZ() (atan2(Z, sqrt(Y*Y + X*X)), wrapped), the
//   sum wrapped again, then InitialiseFromAngles(angleXY, angleYZ, GetMagnitude()) (the landed 0x00CBE693 form), all
//   inline in the TU.
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

static const float TWO_PI = 6.2831855f;

class C3DVector {
public:
    float X, Y, Z;
    __forceinline float GetAngleXY() const { return WrapAngle((float)atan2(X, Y) * 0.15915494f); }
    __forceinline float GetAngleYZ() const { return WrapAngle((float)atan2(Z, sqrt(Y * Y + X * X)) * 0.15915494f); }
    __forceinline float GetMagnitude() const { return (float)sqrt(X * X + Y * Y + Z * Z); }
    __forceinline const C3DVector& InitialiseFromAngles(float angleXY, float angleYZ, float length)
    {
        float cosYZ = (float)cos(angleYZ * (double)TWO_PI);
        X = (float)(sin(angleXY * (double)TWO_PI) * (cosYZ * length));
        Y = (float)(cos(angleXY * (double)TWO_PI) * (cosYZ * length));
        Z = (float)(sin(angleYZ * (double)TWO_PI) * length);
        return *this;
    }
    void RotateXY(float angle);
};

void C3DVector::RotateXY(float angle)
{
    float angleXY = GetAngleXY() + angle;
    float angleYZ = GetAngleYZ();
    angleXY = WrapAngle(angleXY);
    InitialiseFromAngles(angleXY, angleYZ, GetMagnitude());
}