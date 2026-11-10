// CRightHandedSet::Rotate(const C3DVector& axis, float angle) -- rotate both basis vectors (Up, Forward) about an axis: build the axis/angle quaternion, expand it into a 3x3 rotation matrix (Shoemake, s = 2/|q|^2, 1 for a zero quaternion) and replace each vector by vector * matrix
// Grade: DIFFER(523v523) -- one commuted x87 product in the Up X row: retail `fld [esp+24h] ; fmul st(4)`, ours `fld st(3) ; fmul [esp+24h]` (E11 * tx operand order); shared C3DVector / CMatrix3x4, member ToMatrix, TransformVector; insensitive to term order and parentheses (16 grades)
// Leaf: ?Rotate@CRightHandedSet@@QAEXABVC3DVector@@M@Z
// Retail: Fable.exe @ 0x00A22F80, 523 bytes, `ret 8`. Catalog name "CMatrix3x3::MultiplyByQuaternionRotation" is wrong
//   (labels_retry10n_corrections: called as rhset.Rotate(axis, angle) from CTCBob::FrameUpdate).
// Callees: CQuaternion::SetFromAxisAngle(const C3DVector&, float) 0x00A88D10. 0.0f 0x0122DEDC, 1.0f 0x0122DED8,
//   2.0f 0x0122DCDC.
// Layout: Ego_r.pdb CRightHandedSet (24 bytes: Up +0, Forward +0xc); CQuaternion (X, Y, Z, W).
#include "fable_compiler.h"
#include "engine/C3DVector.h"
#include "engine/CMatrix3x4.h"
class CRightHandedSet {
public:
    C3DVector Up;
    C3DVector Forward;
    void Rotate(const C3DVector& axis, float angle);
};
class CQuaternion {
public:
    float X, Y, Z, W;
    void SetFromAxisAngle(const C3DVector& axis, float angle);   // 0x00A88D10
    FABLE_FORCEINLINE void ToMatrix(CMatrix3x4& m) const
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
    }
};
FABLE_FORCEINLINE void TransformVector(const CMatrix3x4& m, C3DVector& v)
{
    const float xx = v.X;
    const float yy = v.Y;
    const float zz = v.Z;
    v.X = m.E11 * xx + (m.E21 * yy + m.E31 * zz);
    v.Y = m.E12 * xx + (m.E22 * yy + m.E32 * zz);
    v.Z = m.E13 * xx + (m.E23 * yy + m.E33 * zz);
}
void CRightHandedSet::Rotate(const C3DVector& axis, float angle)
{
    CQuaternion q;
    q.SetFromAxisAngle(axis, angle);
    CMatrix3x4 m;
    q.ToMatrix(m);
    TransformVector(m, Up);
    TransformVector(m, Forward);
}
