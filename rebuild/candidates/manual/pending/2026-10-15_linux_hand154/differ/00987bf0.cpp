// CQuaternion::operator*(const C4DVector& in, C4DVector& out, float w) -- transform a vector by a 3x4 row matrix: out.xyz = rows . (in.x, in.y, in.z, w), out.w = w
// Grade: DIFFER(156v156) -- FP sum order only: rows 1 and 2 evaluate z,y,w,x (retail z,y,x,w); row 0 matches with the written order y,z,w,x. Tried z,y,x,w / y,z,x,w / x,y,z,w (VC7.1 reassociates the chains) and an inline Dot3 + w (closest)
// Leaf: Transform
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00987BF0, length 156 bytes, `ret 0Ch`. No callees.
// Label note: the catalog's "CQuaternion::operator*" is propagated; `this` holds twelve floats (three rows of four, +0x00..+0x2c),
// the body reads a 16-byte vector from the first argument, writes four floats through the second and takes a float third
// argument as the homogeneous coordinate -- a row-matrix transform, modelled descriptively as CMatrix34_Retail::Transform.
// Shape: the input is copied into a 16-byte local first (`C4DVector t = in;`, its w copy is dead); each component is a sum
// of four `row * t` products in the source order retail evaluates (z, y first; the w and x terms swap in row 0); the
// out.w store is an integer move scheduled before the last fstp.
struct C4DVector {
    float X, Y, Z, W;
};
struct CRow_Retail {                               // one matrix row: three direction terms and the translation term
    float X, Y, Z, T;
    float Dot3(const C4DVector& o) const { return Z * o.Z + Y * o.Y + X * o.X; }   // C3DVector::DotProduct order (landed 0x00C256A0)
};
struct CMatrix34_Retail {
    CRow_Retail R[3];
    void Transform(const C4DVector& in, C4DVector& out, float w) const;
};

void CMatrix34_Retail::Transform(const C4DVector& in, C4DVector& out, float w) const
{
    C4DVector t = in;
    out.X = R[0].Y * t.Y + R[0].Z * t.Z + R[0].T * w + R[0].X * t.X;
    out.Y = R[1].Dot3(t) + R[1].T * w;
    out.Z = R[2].Dot3(t) + R[2].T * w;
    out.W = w;
}
