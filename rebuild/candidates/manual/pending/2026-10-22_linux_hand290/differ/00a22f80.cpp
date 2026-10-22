// Grade: DIFFER(523v523) -- one commuted x87 product in the Up X row (fmul operand order); everything else matches
// CRightHandedSet::Rotate(const C3DVector& axis, float angle) -- rotate both basis vectors (Up, Forward) about an axis:
//   build the axis/angle quaternion, expand it into a 3x3 rotation matrix (Shoemake, s = 2/|q|^2, 1 for a zero
//   quaternion) and replace each vector by vector * matrix
// Leaf: ?Rotate@CRightHandedSet
// Retail: Fable.exe @ 0x00A22F80, 523 bytes, `ret 8`. Catalog name "CMatrix3x3::MultiplyByQuaternionRotation" is wrong
//   (labels_retry10n_corrections: called as rhset.Rotate(axis, angle) from CTCBob::FrameUpdate).
// Callees: CQuaternion::SetFromAxisAngle(const C3DVector&, float) 0x00A88D10. 0.0f 0x0122DEDC, 1.0f 0x0122DED8,
//   2.0f 0x0122DCDC.
// Layout: Ego_r.pdb CRightHandedSet (24 bytes: Up +0, Forward +0xc); CQuaternion (X, Y, Z, W).

struct C3DVector {
    float X, Y, Z;
};
struct CQuaternion {
    float X, Y, Z, W;
    void SetFromAxisAngle(const C3DVector& axis, float angle);   // 0x00A88D10
};
struct CMatrix3x4 {
    union {
        struct { float E11, E12, E13, E21, E22, E23, E31, E32, E33, E41, E42, E43; };
    };
};
__forceinline void QuaternionToMatrix(const CQuaternion& q, CMatrix3x4& m)
{
    float x = q.X, y = q.Y, z = q.Z, w = q.W;
    float n = w * w + z * z + y * y + x * x;
    float s = (n != 0.0f) ? 2.0f / n : 1.0f;
    float xs = q.X * s, ys = q.Y * s, zs = q.Z * s;
    float wx = q.W * xs, wy = q.W * ys, wz = q.W * zs;
    float xx = q.X * xs, xy = q.X * ys, xz = q.X * zs;
    float yy = q.Y * ys, yz = q.Y * zs, zz = q.Z * zs;
    m.E11 = 1.0f - (yy + zz);
    m.E12 = xy - wz;
    m.E13 = xz + wy;
    m.E21 = xy + wz;
    m.E22 = 1.0f - (zz + xx);
    m.E23 = yz - wx;
    m.E31 = xz - wy;
    m.E32 = yz + wx;
    m.E33 = 1.0f - (xx + yy);
}
__forceinline void TransformInPlace(C3DVector& v, const CMatrix3x4& m)
{
    float tx = v.X, ty = v.Y, tz = v.Z;
    v.X = m.E31 * tz + m.E21 * ty + m.E11 * tx;
    v.Y = m.E32 * tz + m.E22 * ty + m.E12 * tx;
    v.Z = m.E33 * tz + m.E23 * ty + m.E13 * tx;
}
struct CRightHandedSet {
    C3DVector Up;
    C3DVector Forward;
    void Rotate(const C3DVector& axis, float angle);
};

void CRightHandedSet::Rotate(const C3DVector& axis, float angle)
{
    CQuaternion q;
    q.SetFromAxisAngle(axis, angle);
    CMatrix3x4 m;
    QuaternionToMatrix(q, m);
    TransformInPlace(Up, m);
    TransformInPlace(Forward, m);
}
