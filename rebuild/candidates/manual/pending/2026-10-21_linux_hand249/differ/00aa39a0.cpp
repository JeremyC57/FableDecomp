// CQuaternion::ToMatrix(C4x4Matrix&, long) -- 4x4 matrix from the rotation quaternion plus the position at +0x10
// Grade: DIFFER(ours v retail) -- 332v332: only the int zero / 1.0f stores of the row ends and translation are hoisted by our scheduler (the norm load order is fixed by the CQuat4(X,Y,Z,W).Norm() temporary)
struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
};
struct C4x4Matrix { float m[16]; };
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
    out.m[0] = 1.0f - (yy + zz); out.m[1] = xy - wz; out.m[2] = xz + wy; out.m[3] = 0.0f;
    out.m[4] = xy + wz; out.m[5] = 1.0f - (xx + zz); out.m[6] = yz - wx; out.m[7] = 0.0f;
    out.m[8] = xz - wy; out.m[9] = yz + wx; out.m[10] = 1.0f - (xx + yy); out.m[11] = 0.0f;
    out.m[12] = pos.X; out.m[13] = pos.Y; out.m[14] = pos.Z; out.m[15] = 1.0f;
}
