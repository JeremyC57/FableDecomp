// C3DVector::Rotate(float angleXY, float angleYZ) -- turn the vector by a fraction of a full turn about the vertical axis and in elevation, keeping its length
// Leaf: ?Rotate@C3DVector@@QAEXMM@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A14260, 467 bytes, thiscall, `ret 8`.
// Identity: Ego_r.pdb / FableWin.pdb `?Rotate@C3DVector@@QAEXMM@Z`. The vector-math TU's donor order (Ego_r) is
//   Rotate(const C3DVector&, float), C2DVector::ToString, C3DVector::ToString, C2DVector::GetRandomUnitVector,
//   C2DVector::GetRandomPointWithinUnitCircle, RotateXY, C2DVector::Rotate, Rotate(float, float), AccurateNormalise,
//   GetMagnitudeAndNormalise ...; retail has the landed C3DVector::Rotate 0x00A13C60, C2DVector::ToString 0x00A13CE0,
//   C3DVector::ToString 0x00A13DA0, C2DVector::GetRandomUnitVector 0x00A13EB0, RotateXY 0x00A14040 (one float, `ret 4`),
//   C2DVector::Rotate 0x00A141D0, this row and AccurateNormalise 0x00A14440 in the same order, and this row takes two
//   floats (`ret 8`). The manifest label "C3DVector_FromSpherical" is behavioural; the landed 0x0090BED0 binds this
//   address as `?RotateAboutZ@C3DVector@@QAEXM_N@Z`, a name in neither PDB (NAME_FIXES.tsv).
// Callees: _CIfmod 0x00BFEB28 (intrinsic fmod). Constants: 1/(2pi) 0x01237F08, 0.0f 0x0122DEDC, 1.0f 0x0122DED8,
//   1.0 (double) 0x01237F00, (double)2pi 0x0124F2B8.
// Shape: RotateXY's (0x00A14040) with both angles: GetAngleXY() + angleXY and GetAngleYZ() + angleYZ, each sum
//   wrapped again (XY first), then InitialiseFromAngles(xy, yz, GetMagnitude()), all inline.
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
    void Rotate(float angleXY, float angleYZ);
};

void C3DVector::Rotate(float angleXY, float angleYZ)
{
    float newXY = GetAngleXY() + angleXY;
    float newYZ = GetAngleYZ() + angleYZ;
    newXY = WrapAngle(newXY);
    newYZ = WrapAngle(newYZ);
    InitialiseFromAngles(newXY, newYZ, GetMagnitude());
}