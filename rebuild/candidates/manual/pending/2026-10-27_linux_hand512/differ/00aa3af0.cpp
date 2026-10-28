// C3DKeyframe::ToMatrix(CMatrix4x4& m, bool applyScaling) const -- keyframe (quaternion + position) to a 4x4 matrix: the inline quaternion-to-matrix with the position as the translation column and a 0,0,0,1 last row; this build does not read applyScaling
// Grade: DIFFER(346v346) -- instruction scheduling only: VC hoists the six integer stores (E34 = pos.Z, the 0,0,0 last row, E44 = 1.0f) to just after the E11 store, where retail emits them after the last x87 store (E33); every other instruction matches (4 grades)
// Leaf: ?ToMatrix@C3DKeyframe@@QBEXAAVCMatrix4x4@@_N@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AA3AF0, 346 bytes, thiscall `ret 8` (matrix reference + bool, the bool unread).
// Identity (probable): FableWin.pdb names exactly two 4x4 conversions, ?ToMatrix@C3DKeyframe@@QBEXAAVCMatrix4x4@@_N@Z and
//   the inline ?ToMatrix@CQuaternion@@QBEXAAVCMatrix4x4@@ABVC3DVector@@@Z; this body reads the 48-byte keyframe (Rotation +0,
//   Position +0x10) through `this`, takes two stack arguments and inlines the quaternion conversion -- the 4x4 twin of the
//   landed C3DKeyframe::ToMatrix(CMatrix3x4&, bool) 0x00A52450 without its scaling block. No callers in the retail image.
// Callees: none (CQuaternion::ToMatrix is inlined).
// Globals: 0.0f 0x0122DEDC, 2.0f 0x0122DCDC, 1.0f 0x0122DED8.
// Layout: Ego_r.pdb C3DKeyframe and CMatrix4x4 (anonymous union over E11..E44); no shared engine headers exist for them.
class C3DVector { public:
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
};
class C4DVector { public: float X, Y, Z, W; };
class CMatrix4x4 { public:
    union {
        struct { float E11, E12, E13, E14, E21, E22, E23, E24, E31, E32, E33, E34, E41, E42, E43, E44; };
    };
};
class CQuaternion { public:
    float X, Y, Z, W;
    __forceinline void ToMatrix(CMatrix4x4& m, const C3DVector& pos) const
    {
        float x = X, y = Y, z = Z, w = W;
        float n = w * w + z * z + y * y + x * x;
        float s = (n != 0.0f) ? 2.0f / n : 1.0f;
        float xs = X * s, ys = Y * s, zs = Z * s;
        float wx = W * xs, wy = W * ys, wz = W * zs;
        float xx = X * xs, xy = X * ys, xz = X * zs;
        float yy = Y * ys, yz = Y * zs, zz = Z * zs;
        m.E11 = 1.0f - (yy + zz);
        m.E12 = xy + wz;
        m.E13 = xz - wy;
        m.E14 = pos.X;
        m.E21 = xy - wz;
        m.E22 = 1.0f - (zz + xx);
        m.E23 = yz + wx;
        m.E24 = pos.Y;
        m.E31 = xz + wy;
        m.E32 = yz - wx;
        m.E33 = 1.0f - (xx + yy);
        m.E34 = pos.Z;
        m.E41 = 0.0f;
        m.E42 = 0.0f;
        m.E43 = 0.0f;
        m.E44 = 1.0f;
    }
};
class C3DKeyframe { public:
    CQuaternion Rotation;
    C4DVector Position;
    C4DVector Scaling;
    void ToMatrix(CMatrix4x4& m, bool applyScaling) const;
};
void C3DKeyframe::ToMatrix(CMatrix4x4& m, bool applyScaling) const
{
    Rotation.ToMatrix(m, C3DVector(Position.X, Position.Y, Position.Z));
}
