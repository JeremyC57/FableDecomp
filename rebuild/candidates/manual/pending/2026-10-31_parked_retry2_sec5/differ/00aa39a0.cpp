// CQuaternion::ToMatrix(C4x4Matrix&, long) -- 4x4 matrix from the rotation quaternion plus the position at +0x10
// Grade: DIFFER(332v332) -- rows 0-2 written through an inline SetRow(r, a, b, c) followed by a separate `m[4r+3]`
//   store: rows 2 and 3 now store their ends where retail does; residue: row 0's int zero store (m[3]) is hoisted to
//   just after `xor ecx,ecx` (retail after m[2]) and the translation's pos.Z store lands earlier
struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
};
struct C4x4Matrix { float m[16]; void SetRow(int r, float a, float b, float c) { m[r*4] = a; m[r*4+1] = b; m[r*4+2] = c; } };
struct CQuat4 {
    float X, Y, Z, W;
    CQuat4(float x, float y, float z, float w) : X(x), Y(y), Z(z), W(w) {}
    float Norm() const { return X * X + Y * Y + Z * Z + W * W; }
};
struct CQuatPos {
    float X, Y, Z, W;
    C3DVector Position;
    void ToMatrix(C4x4Matrix& out, long unused) const;
};
void CQuatPos::ToMatrix(C4x4Matrix& out, long unused) const
{
    C3DVector pos(Position.X, Position.Y, Position.Z);
    float n = CQuat4(X, Y, Z, W).Norm();
    float s = (n != 0.0f) ? 2.0f / n : 1.0f;
    float xs = X * s, ys = Y * s, zs = Z * s;
    float wx = W * xs, wy = W * ys, wz = W * zs;
    float xx = X * xs, xy = X * ys, xz = X * zs;
    float yy = Y * ys, yz = Y * zs, zz = Z * zs;
    out.SetRow(0, 1.0f - (yy + zz), xy - wz, xz + wy); out.m[3] = 0.0f;
    out.SetRow(1, xy + wz, 1.0f - (xx + zz), yz - wx); out.m[7] = 0.0f;
    out.SetRow(2, xz - wy, yz + wx, 1.0f - (xx + yy)); out.m[11] = 0.0f;
    out.SetRow(3, pos.X, pos.Y, pos.Z); out.m[15] = 1.0f;
}
