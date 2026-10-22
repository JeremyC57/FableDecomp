// C3DKeyframe::InitialiseFromMatrix(const CMatrix3x4&) -- decompose into position, row-length scaling (z negated for a left-handed basis) and rotation; true when the rebuilt matrix equals the input
// Grade: DIFFER(839v904) -- first draft: frame 0x78 v 0x84 (retail spills the position copy and quaternion products to more frame words), this/argument in ebx/ebp swapped, and the inline quaternion-to-matrix x87 order
// Leaf: InitialiseFromMatrix
extern "C" double __cdecl sqrt(double);
#pragma intrinsic(sqrt)

class C3DVector {
public:
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
};

class C4DVector {
public:
    float X, Y, Z, W;
};

class CMatrix3x4 {
public:
    float E11, E12, E13;
    float E21, E22, E23;
    float E31, E32, E33;
    float E41, E42, E43;
    CMatrix3x4() {}
    bool Equals(const CMatrix3x4& m) const;
    __forceinline void ScaleRows(float x, float y, float z)
    {
        E11 *= x; E12 *= x; E13 *= x;
        E21 *= y; E22 *= y; E23 *= y;
        E31 *= z; E32 *= z; E33 *= z;
    }
};

class CQuaternion {
public:
    float X, Y, Z, W;
    void SetFromRotationMatrix(const CMatrix3x4& m);
    __forceinline void GetMatrix(CMatrix3x4& r) const
    {
        float n = X * X + Y * Y + Z * Z + W * W;
        float s = n != 0.0f ? 2.0f / n : 1.0f;
        float xs = X * s, ys = Y * s, zs = Z * s;
        float wx = W * xs, wy = W * ys, wz = W * zs;
        float xx = X * xs, xy = X * ys, xz = X * zs;
        float yy = Y * ys, yz = Y * zs, zz = Z * zs;
        r.E11 = 1.0f - (yy + zz); r.E12 = xy - wz; r.E13 = xz + wy;
        r.E21 = xy + wz; r.E22 = 1.0f - (xx + zz); r.E23 = yz - wx;
        r.E31 = xz - wy; r.E32 = yz + wx; r.E33 = 1.0f - (xx + yy);
    }
};

class C3DKeyframe {
public:
    CQuaternion Rotation;
    C4DVector Position;
    C4DVector Scaling;
    bool InitialiseFromMatrix(const CMatrix3x4& m);
};

bool C3DKeyframe::InitialiseFromMatrix(const CMatrix3x4& m)
{
    CMatrix3x4 rot = m;
    float sx = (float)sqrt(rot.E11 * rot.E11 + rot.E13 * rot.E13 + rot.E12 * rot.E12);
    Position.X = rot.E41;
    Position.Y = rot.E42;
    Position.Z = rot.E43;
    Position.W = 0.0f;
    float sy = (float)sqrt(rot.E23 * rot.E23 + rot.E22 * rot.E22 + rot.E21 * rot.E21);
    float sz = (float)sqrt(rot.E33 * rot.E33 + rot.E32 * rot.E32 + rot.E31 * rot.E31);
    float cx = rot.E12 * rot.E23 - rot.E13 * rot.E22;
    float cy = rot.E13 * rot.E21 - rot.E11 * rot.E23;
    float cz = rot.E11 * rot.E22 - rot.E12 * rot.E21;
    if (cx * rot.E31 + cy * rot.E32 + cz * rot.E33 < 0.0f)
        sz = -sz;
    Scaling.X = sx;
    Scaling.Y = sy;
    Scaling.Z = sz;
    Scaling.W = 0.0f;
    rot.E41 = 0.0f;
    rot.E42 = 0.0f;
    rot.E43 = 0.0f;
    rot.ScaleRows(1.0f / sx, 1.0f / sy, 1.0f / sz);
    Rotation.SetFromRotationMatrix(rot);
    CMatrix3x4 built;
    Rotation.GetMatrix(built);
    built.E41 = Position.X;
    built.E42 = Position.Y;
    built.E43 = Position.Z;
    built.ScaleRows(Scaling.X, Scaling.Y, Scaling.Z);
    if (built.Equals(m))
        return true;
    return false;
}
