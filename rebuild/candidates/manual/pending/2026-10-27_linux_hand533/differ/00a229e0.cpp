// CRightHandedSet::CRightHandedSet(const C3DAngles& angles) -- build the forward vector from the XY/YZ angles, rotate the matching up vector about forward by the XZ angle (inline axis-angle matrix, CMatrix3x4::SetFromVectorAndAngle shape), then fast-normalise both
// Grade: DIFFER(691v689) -- frame 0x50 vs retail 0x4c: retail homes up.X in the dead angles parameter slot and evaluates s*z before (xy - xy*c), spilling s*z and s*y; ours keeps a separate slot
// Leaf: ??0CRightHandedSet@@QAE@ABVC3DAngles@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A229E0, 689 bytes, thiscall `ret 4`.
// Identity: Ego_r.pdb `??0CRightHandedSet@@QAE@ABVC3DAngles@@@Z` (donor order: GFFastArcCos 0x00A22530, CQuaternion::SetSlerp 0x00A22590,
//   this ctor, GetAngleXZ 0x00A22CA0, Rotate 0x00A22F80); sin/cos of angle*(double)2pi (0x0124F2B8), inv-sqrt table 0x013CE558.
#include <math.h>
#pragma intrinsic(sin, cos)

extern unsigned long g_InvSqrtMantissaTable[128];   // 0x013CE558

static const float TWO_PI = 6.2831855f;

class C3DVector { public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }
    __forceinline C3DVector& Normalise()
    {
        float x = X;
        float y = Y;
        float z = Z;
        float lengthSquared = x * x + y * y + z * z;
        unsigned long bits = *(unsigned long*)&lengthSquared;
        unsigned long estimate = (((0x17c - ((bits >> 23) & 0xff)) >> 1) << 23) |
                                 (g_InvSqrtMantissaTable[(bits >> 17) & 0x7f] << 15);
        float r = *(float*)&estimate;
        r = (3.0f - r * r * lengthSquared) * r * 0.5f;
        X *= r;
        Y *= r;
        Z *= r;
        return *this;
    }
};

class C3DAngles { public:
    float AngleXY;   // +0
    float AngleYZ;   // +4
    float AngleXZ;   // +8
};

class CMatrix3x4 { public:
    C3DVector XAxis, YAxis, ZAxis, Position;
    CMatrix3x4() { }
    __forceinline CMatrix3x4(const C3DVector& axis, float angle)
    {
        float s = (float)sin(angle * (double)TWO_PI);
        float c = (float)cos(angle * (double)TWO_PI);
        float xx = axis.X * axis.X;
        float yy = axis.Y * axis.Y;
        float zz = axis.Z * axis.Z;
        float xy = axis.Y * axis.X;
        float xz = axis.Z * axis.X;
        float yz = axis.Z * axis.Y;
        float sx = s * axis.X;
        float sy = s * axis.Y;
        float sz = s * axis.Z;
        XAxis.X = xx + c - xx * c;
        YAxis.X = xy - xy * c + sz;
        ZAxis.X = xz - xz * c - sy;
        XAxis.Y = xy - xy * c - sz;
        YAxis.Y = yy + c - yy * c;
        ZAxis.Y = yz - yz * c + sx;
        XAxis.Z = xz - xz * c + sy;
        YAxis.Z = yz - yz * c - sx;
        ZAxis.Z = zz + c - zz * c;
        Position.X = 0.0f;
        Position.Y = 0.0f;
        Position.Z = 0.0f;
    }
};

class CRightHandedSet { public:
    C3DVector Up;        // +0x00
    C3DVector Forward;   // +0x0c
    CRightHandedSet(const C3DAngles& angles);
};

CRightHandedSet::CRightHandedSet(const C3DAngles& angles)
{
    float sinYZ = (float)sin(angles.AngleYZ * (double)TWO_PI);
    float cosYZ = (float)cos(angles.AngleYZ * (double)TWO_PI);
    float sinXY = (float)sin(angles.AngleXY * (double)TWO_PI);
    float cosXY = (float)cos(angles.AngleXY * (double)TWO_PI);
    float s = (float)sin(angles.AngleXZ * (double)TWO_PI);
    float c = (float)cos(angles.AngleXZ * (double)TWO_PI);
    Forward.X = sinXY * cosYZ;
    Forward.Y = cosXY * cosYZ;
    Forward.Z = sinYZ;
    CMatrix3x4 rotation;
    rotation.XAxis.X = Forward.X * Forward.X + c - Forward.X * Forward.X * c;
    rotation.YAxis.X = Forward.Y * Forward.X - Forward.Y * Forward.X * c + s * Forward.Z;
    rotation.ZAxis.X = Forward.Z * Forward.X - Forward.Z * Forward.X * c - s * Forward.Y;
    rotation.XAxis.Y = Forward.Y * Forward.X - Forward.Y * Forward.X * c - s * Forward.Z;
    rotation.YAxis.Y = Forward.Y * Forward.Y + c - Forward.Y * Forward.Y * c;
    rotation.ZAxis.Y = Forward.Z * Forward.Y - Forward.Z * Forward.Y * c + s * Forward.X;
    rotation.XAxis.Z = Forward.Z * Forward.X - Forward.Z * Forward.X * c + s * Forward.Y;
    rotation.YAxis.Z = Forward.Z * Forward.Y - Forward.Z * Forward.Y * c - s * Forward.X;
    rotation.ZAxis.Z = Forward.Z * Forward.Z + c - Forward.Z * Forward.Z * c;
    rotation.Position.X = 0.0f;
    rotation.Position.Y = 0.0f;
    rotation.Position.Z = 0.0f;
    C3DVector up(-sinXY * sinYZ, -cosXY * sinYZ, cosYZ);
    Up.X = up.X * rotation.XAxis.X + up.Y * rotation.YAxis.X + up.Z * rotation.ZAxis.X;
    Up.Y = up.X * rotation.XAxis.Y + up.Y * rotation.YAxis.Y + up.Z * rotation.ZAxis.Y;
    Up.Z = up.X * rotation.XAxis.Z + up.Y * rotation.YAxis.Z + up.Z * rotation.ZAxis.Z;
    Forward.Normalise();
    Up.Normalise();
}
