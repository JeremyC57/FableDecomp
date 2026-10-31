// C3DKeyframe::ToMatrix(CMatrix4x4& m, bool applyScaling) const -- keyframe (quaternion + position) to a 4x4 matrix: the inline quaternion-to-matrix with the position as translation and the projective column (0, 0, 0, 1); the flag is not read by the 4x4 form
// Grade: DIFFER(332v332) -- one instruction: the translation's integer pos.Z store (mov [eax+0x38],ecx) is scheduled before the pos.X/pos.Y fstp pair; retail keeps it after them
// Leaf: ?ToMatrix@C3DKeyframe@@QBEXAAVCMatrix4x4@@_N@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AA39A0, 332 bytes, `ret 8`. FableWin symbol ?ToMatrix@C3DKeyframe@@QBEXAAVCMatrix4x4@@_N@Z
//   (donor 0x0337FD30, 0x40 bytes ahead of the inlined ?ToMatrix@CQuaternion@@QBEXAAVCMatrix4x4@@ABVC3DVector@@@Z at 0x0337FD70);
//   the 3x4 sibling C3DKeyframe::ToMatrix(CMatrix3x4&, bool) is landed at 0x00A52450 with the same inline quaternion body.
//   The catalog label CQuaternion::ToMatrix is the inlined helper.
// Callees: none. Globals: 0.0f 0x0122DEDC, 2.0f 0x0122DCDC, 1.0f 0x0122DED8.
// Layout: Ego_r.pdb C3DKeyframe (Rotation +0, Position C4DVector +0x10, Scaling C4DVector +0x20) and CMatrix4x4 (anonymous
//   union over the floats E11..E44); no shared engine headers exist for these classes.
class C3DVector { public:
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
};
class C4DVector { public: float X, Y, Z, W; };
// Shape: rows 0-2 go through an inline row writer that takes the row's address (float* p = M[r]) and each row's 0.0f end is
//   a separate statement through the matrix: the two bases keep VC7.1 from hoisting the integer zero stores above the row's
//   fstps (the plain E11..E44 / M[r][c] forms hoist all four; a p-free SetRow hoists row 0's). SetRow is a descriptive shape
//   helper (no PDB symbol; it never reaches the object). Remaining: row 3's int pos.Z store, see line 2.
class CMatrix4x4 { public:
    union {
        struct { float E11, E12, E13, E14, E21, E22, E23, E24, E31, E32, E33, E34, E41, E42, E43, E44; };
        float M[4][4];
    };
    void SetRow(int r, float a, float b, float c) { float* p = M[r]; p[0] = a; p[1] = b; p[2] = c; }
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
        m.SetRow(0, 1.0f - (yy + zz), xy - wz, xz + wy); m.M[0][3] = 0.0f;
        m.SetRow(1, xy + wz, 1.0f - (zz + xx), yz - wx); m.M[1][3] = 0.0f;
        m.SetRow(2, xz - wy, yz + wx, 1.0f - (xx + yy)); m.M[2][3] = 0.0f;
        m.SetRow(3, pos.X, pos.Y, pos.Z); m.M[3][3] = 1.0f;
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
