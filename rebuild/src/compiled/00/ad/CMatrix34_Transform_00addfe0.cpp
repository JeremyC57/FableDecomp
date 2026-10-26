// CMatrix34::Transform(C4DVector& v, float w) -- transform a vector in place by a 3x4 row matrix: v.xyz = rows . (v.x, v.y, v.z, w), v.w = w
// Grade: MATCH -- rows 1 and 2 are `float y = R[1].Dot3(t); y += R[1].T * w; out.Y = y;` (the accumulator statement keeps the w term last, as in the twin 0x00987BF0)
// Twin of 0x00987BF0 with the output written over the input (the local copy makes it alias-safe).
// Leaf: Transform
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00ADDFE0, length 157 bytes (padded to 160), `ret 8`. No callees.
// Label note: the catalog's "CQuaternion::operator*" is propagated; `this` holds twelve floats (three rows of four, +0x00..+0x2c),
// the body reads a 16-byte vector through its first argument and writes four floats back through it, taking a float second
// argument as the homogeneous coordinate -- a row-matrix transform, modelled descriptively as CMatrix34_Retail::Transform.
// Shape: the input is copied into a 16-byte local first (`C4DVector t = out;`, its w copy is dead); each component is a sum
// of four `row * t` products in the source order retail evaluates (z, y first; the w and x terms swap in row 0); the
// out.w store is an integer move scheduled before the last fstp.
class C4DVector { public:
    float X, Y, Z, W;
};
struct CRow_Retail {                               // one matrix row: three direction terms and the translation term
    float X, Y, Z, T;
    float Dot3(const C4DVector& o) const { return Z * o.Z + Y * o.Y + X * o.X; }   // C3DVector::DotProduct order (landed 0x00C256A0)
};
struct CMatrix34_Retail {
    CRow_Retail R[3];
    void Transform(C4DVector& out, float w) const;
};

void CMatrix34_Retail::Transform(C4DVector& out, float w) const
{
    C4DVector t = out;
    out.X = R[0].Y * t.Y + R[0].Z * t.Z + R[0].T * w + R[0].X * t.X;
    float y = R[1].Dot3(t);
    y += R[1].T * w;
    out.Y = y;
    float z = R[2].Dot3(t);
    z += R[2].T * w;
    out.Z = z;
    out.W = w;
}