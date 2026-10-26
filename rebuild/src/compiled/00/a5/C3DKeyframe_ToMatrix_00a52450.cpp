// C3DKeyframe::ToMatrix(CMatrix3x4& m, bool applyScaling) const -- keyframe (quaternion + position + scale) to a 3x4 matrix: the inline quaternion-to-matrix with the position as translation, then each rotation row scaled by the matching scale component
// Leaf: ?ToMatrix@C3DKeyframe
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A52450, 447 bytes, `ret 8`. FableWin symbol ?ToMatrix@C3DKeyframe@@QBEXAAVCMatrix3x4@@_N@Z
//   (the catalog label CQuaternion::ToMatrix is the inlined ?ToMatrix@CQuaternion@@QBEXAAVCMatrix3x4@@ABVC3DVector@@@Z).
// Callees: none (CQuaternion::ToMatrix is inlined).
// Globals: 0.0f 0x0122DEDC, 2.0f 0x0122DCDC, 1.0f 0x0122DED8.
// Layout: Ego_r.pdb C3DKeyframe (Rotation +0, Position C4DVector +0x10, Scaling C4DVector +0x20) and CMatrix3x4 (anonymous
//   union over the floats E11..E43); no shared engine headers exist for these classes.
// Shape: the quaternion-to-matrix is a __forceinline member taking the translation by const reference; it copies the four
//   components into locals x, y, z, w first (loads in X..W order), and the norm sums w*w + z*z + y*y + x*x. The translation
//   is a C3DVector temporary built from the position (its Z travels as an integer copy). The scale components are float
//   locals declared Z, Y, X, and the rows are scaled with `*=` (the rotation values are forwarded from the frame).
class C3DVector { public:
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
};
class C4DVector { public: float X, Y, Z, W; };
class CMatrix3x4 { public:
    union {
        struct { float E11, E12, E13, E21, E22, E23, E31, E32, E33, E41, E42, E43; };
    };
};
class CQuaternion { public:
    float X, Y, Z, W;
    __forceinline void ToMatrix(CMatrix3x4& m, const C3DVector& pos) const
    {
        float x = X, y = Y, z = Z, w = W;
        float n = w * w + z * z + y * y + x * x;
        float s = (n != 0.0f) ? 2.0f / n : 1.0f;
        float xs = X * s, ys = Y * s, zs = Z * s;
        float wx = W * xs, wy = W * ys, wz = W * zs;
        float xx = X * xs, xy = X * ys, xz = X * zs;
        float yy = Y * ys, yz = Y * zs, zz = Z * zs;
        m.E11 = 1.0f - (yy + zz);
        m.E12 = xy - wz;
        m.E13 = xz + wy;
        m.E21 = xy + wz;
        m.E22 = 1.0f - (zz + xx);
        m.E23 = yz - wx;
        m.E31 = xz - wy;
        m.E32 = yz + wx;
        m.E33 = 1.0f - (xx + yy);
        m.E41 = pos.X;
        m.E42 = pos.Y;
        m.E43 = pos.Z;
    }
};
class C3DKeyframe { public:
    CQuaternion Rotation;
    C4DVector Position;
    C4DVector Scaling;
    void ToMatrix(CMatrix3x4& m, bool applyScaling) const;
};
void C3DKeyframe::ToMatrix(CMatrix3x4& m, bool applyScaling) const
{
    Rotation.ToMatrix(m, C3DVector(Position.X, Position.Y, Position.Z));
    if (applyScaling) {
        float sz = Scaling.Z, sy = Scaling.Y, sx = Scaling.X;
        m.E11 *= sx; m.E12 *= sx; m.E13 *= sx;
        m.E21 *= sy; m.E22 *= sy; m.E23 *= sy;
        m.E31 *= sz; m.E32 *= sz; m.E33 *= sz;
    }
}