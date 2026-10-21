// C3DKeyframe::ToMatrix(CMatrix3x4* out, bool applyScaling) const -- keyframe (quat+pos+scale) to 3x4 matrix; catalog label CQuaternion::ToMatrix (this is the 48-byte C3DKeyframe: Rotation +0, Position +0x10, Scaling +0x20)
// Grade: DIFFER(435v447) -- pos.Z store fld/fstp vs retail int mov via edx (and bool-test placement); m0/m1 frame slots swapped; scale rows: retail keeps each scale in st(1) and pops it (fld m; fmul st1 ... fstp st0), ours folds the last use as fmul [mem]
struct CQuaternion { float X, Y, Z, W; };
struct C4DVector { float X, Y, Z, W; };
struct C3DVector { float X, Y, Z; C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {} };
struct CMatrix3x4 { float M[12]; };
struct C3DKeyframe {
    CQuaternion Rotation;
    C4DVector Position;
    C4DVector Scaling;
    void ToMatrix(CMatrix3x4* out, bool applyScaling) const;
};
void C3DKeyframe::ToMatrix(CMatrix3x4* out, bool applyScaling) const
{
    C3DVector pos(Position.X, Position.Y, Position.Z);
    float qx = Rotation.X, qy = Rotation.Y, qz = Rotation.Z, qw = Rotation.W;
    float n = qw * qw + qz * qz + qy * qy + qx * qx;
    float s = (n != 0.0f) ? 2.0f / n : 1.0f;
    float xs = Rotation.X * s, ys = Rotation.Y * s, zs = Rotation.Z * s;
    float wx = Rotation.W * xs, wy = Rotation.W * ys, wz = Rotation.W * zs;
    float xx = Rotation.X * xs, xy = Rotation.X * ys, xz = Rotation.X * zs;
    float yy = Rotation.Y * ys, yz = Rotation.Y * zs, zz = Rotation.Z * zs;
    out->M[11] = pos.Z;
    float m0, m1, m2, m3, m4, m5, m6, m7, m8;
    out->M[0] = m0 = 1.0f - (yy + zz);
    out->M[1] = m1 = xy - wz;
    out->M[2] = m2 = xz + wy;
    out->M[3] = m3 = xy + wz;
    out->M[4] = m4 = 1.0f - (zz + xx);
    out->M[5] = m5 = yz - wx;
    out->M[6] = m6 = xz - wy;
    out->M[7] = m7 = yz + wx;
    out->M[8] = m8 = 1.0f - (xx + yy);
    out->M[9] = pos.X;
    out->M[10] = pos.Y;
    if (applyScaling) {
        float sz = Scaling.Z, sy = Scaling.Y, sx = Scaling.X;
        out->M[0] = m0 * sx; out->M[1] = m1 * sx; out->M[2] = m2 * sx;
        out->M[3] = m3 * sy; out->M[4] = m4 * sy; out->M[5] = m5 * sy;
        out->M[6] = m6 * sz; out->M[7] = m7 * sz; out->M[8] = m8 * sz;
    }
}
