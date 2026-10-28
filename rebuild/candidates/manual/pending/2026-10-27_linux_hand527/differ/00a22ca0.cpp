// CRightHandedSet::GetAngleXZ() -- the set's roll about its forward axis in turns: the signed angle between the normalised right vector and the horizontal perpendicular of the forward heading
// Grade: DIFFER(368v368) -- one operand swap: the cross product's Z term loads `fld [ecx+0Ch] ; fmul [ecx+4]` (Forward.X * Up.Y) in retail, ours `fld [ecx+4] ; fmul [ecx+0Ch]`; every factor order, a memberwise Cross, an explicit GetRight, a C2DVector forward copy and statement orders are inert (3 grades + cc probes).
// Leaf: ?GetAngleXZ@CRightHandedSet@@QBEMXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A22CA0, 368 bytes, thiscall, plain `ret`, float in st(0).
// Identity: Ego_r.pdb / FableWin.pdb `?GetAngleXZ@CRightHandedSet@@QBEMXZ`; Ego_r donor order CRightHandedSet(const
//   C3DAngles&), GetAngleXZ, MakePerpendicular, Rotate, Interpolate matches retail 0x00A229E0, this row, 0x00A22E10
//   (manifest MakePerpendicular), ..., 0x00A23190 (manifest Interpolate); the landed callers
//   CTCPhysicsStandard::GetFacingAngleXZ 0x00723750 and CTCPhysicsRigidBody::GetFacingAngleXZ 0x007599B0 bind it with
//   this spelling. The manifest label "VectorAngle_ComputeRotation" is behavioural.
// Callees: C3DVector::AccurateNormalise 0x00A14440 (twice), _CIfmod 0x00BFEB28, _CIacos 0x00BFEBA2. Constants: 1/(2pi)
//   0x01237F08, 0.0f 0x0122DEDC, 1.0f 0x0122DED8, -1.0f 0x0122DEE0, 1.0 / 0.0 doubles 0x01237F00 / 0x0122ED70,
//   (double)2pi 0x0124F2B8.
// Shape: right = GetRight() (Forward x Up, inline) normalised; the forward copied (X, Y) and its GetAngleXY() turned
//   into a unit heading with InitialiseFromAngles(angle, 0, 1) (cos(0.0) left to the FPU, Z dead); its CrossWithUp()
//   (Y, -X, 0) normalised; sign = right.Z < 0 ? -1 : 1 (an int select, converted early); the dot product clamped to
//   [-1, 1]; acos(dot) / 2pi * sign.
// Layout: Ego_r.pdb CRightHandedSet (Up +0x00, Forward +0x0C, 24 bytes); no engine header.
#include <math.h>
#pragma intrinsic(atan2, fmod, sin, cos, acos)

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
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) {}
    C3DVector& AccurateNormalise();                                     // 0x00A14440
    __forceinline float GetAngleXY() const { return WrapAngle((float)atan2(X, Y) * 0.15915494f); }
    __forceinline const C3DVector Cross(const C3DVector& v) const
    {
        return C3DVector(Y * v.Z - Z * v.Y, Z * v.X - X * v.Z, v.Y * X - Y * v.X);
    }
    __forceinline C3DVector CrossWithUp() const { return C3DVector(Y, -X, 0.0f); }
    __forceinline const float Dot(const C3DVector& v) const { return X * v.X + Y * v.Y + Z * v.Z; }
    __forceinline const C3DVector& InitialiseFromAngles(float angleXY, float angleYZ, float length)
    {
        float cosYZ = (float)cos(angleYZ * (double)TWO_PI);
        X = (float)(sin(angleXY * (double)TWO_PI) * (cosYZ * length));
        Y = (float)(cos(angleXY * (double)TWO_PI) * (cosYZ * length));
        Z = (float)(sin(angleYZ * (double)TWO_PI) * length);
        return *this;
    }
};

class CRightHandedSet {
public:
    C3DVector Up;                                                       // +0x00
    C3DVector Forward;                                                  // +0x0C
    const C3DVector& GetForward() const { return Forward; }
    C3DVector GetRight() const { return Forward.Cross(Up); }
    float GetAngleXZ() const;
};

float CRightHandedSet::GetAngleXZ() const
{
    C3DVector forward = GetForward();
    C3DVector right = GetRight();
    right.AccurateNormalise();
    C3DVector heading;
    heading.InitialiseFromAngles(forward.GetAngleXY(), 0.0f, 1.0f);
    C3DVector flatRight = heading.CrossWithUp();
    float sign = (float)(right.Z < 0.0f ? -1 : 1);
    flatRight.AccurateNormalise();
    float dot = flatRight.Dot(right);
    if (dot < -1.0f)
        dot = -1.0f;
    else if (dot > 1.0f)
        dot = 1.0f;
    return (float)acos(dot) * 0.15915494f * sign;
}
