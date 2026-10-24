// C3DMeshObject::GetPositionMatrixFromRHSet(const CRightHandedSet& set) -- static: the position matrix of an orientation frame, rows -Right, -Forward, Up and a zero translation
// Grade: DIFFER(185v185) -- one x87 operand order: the z product F.x*U.y loads Up.Y first (`fld [edx+4] ; fmul [edx+0Ch]`) where retail loads Forward.X first
// Leaf: GetPositionMatrixFromRHSet
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A358F0, 185 bytes (+ int3 padding), plain `ret` (`SI` = static __fastcall:
//   hidden result in ecx, the set in edx). Ego_r.pdb `?GetPositionMatrixFromRHSet@C3DMeshObject@@SI?AVCMatrix3x4@@ABVCRightHandedSet@@@Z`;
//   callers 0x00724726 / 0x0075A5ED / 0x0075AF3C / 0x0075B287. SetPositionMatrix 0x00A357B0 inlines the same row setup.
// Layout: CRightHandedSet (Ego_r.pdb, 24 bytes) = Up +0, Forward +0xC; CMatrix3x4 = E11..E43 (rows of three floats).
// Shape: a named local matrix (block-copied to the hidden result with `rep movsd`, VC7.1 has no NRVO) filled by an inline
//   setter that takes the three axes BY VALUE: the arguments evaluate right to left, so Up and Forward are copied first
//   (x/y kept on the FPU, z as a dword copy) and the cross product (CRightHandedSet::GetRight = CrossProduct(Forward, Up))
//   is computed from the set's memory last; the negations sit in the setter, so each `fchs` lands right before its store;
//   the zero translation is a chained `E41 = E42 = E43 = 0.0f` (descending stores).
// Tried: (A) a four-row by-value constructor with `-set.GetRight(), -set.Forward` (Forward negated up front);
//   (B/C) named up/forward/right locals into a const-ref / by-value constructor (right spilled to the frame); (D) this form.
// Next: write the z component's first product as `b.Y * a.X` (right operand loaded first) or use a member CrossProduct.
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }
    C3DVector operator-() const { return C3DVector(-X, -Y, -Z); }
};

struct CRightHandedSet {
    C3DVector Up;                            // +0x00
    C3DVector Forward;                       // +0x0C
    C3DVector GetRight() const;
};

inline C3DVector CrossProduct(const C3DVector& a, const C3DVector& b)
{
    return C3DVector(a.Y * b.Z - a.Z * b.Y,
                     a.Z * b.X - a.X * b.Z,
                     a.X * b.Y - a.Y * b.X);
}

inline C3DVector CRightHandedSet::GetRight() const
{
    return CrossProduct(Forward, Up);
}

struct CMatrix3x4 {
    float E11, E12, E13;
    float E21, E22, E23;
    float E31, E32, E33;
    float E41, E42, E43;
    CMatrix3x4() { }
    void SetFromAxes(C3DVector right, C3DVector forward, C3DVector up)
    {
        E11 = -right.X; E12 = -right.Y; E13 = -right.Z;
        E21 = -forward.X; E22 = -forward.Y; E23 = -forward.Z;
        E31 = up.X; E32 = up.Y; E33 = up.Z;
        E41 = E42 = E43 = 0.0f;
    }
};

struct C3DMeshObject {
    static CMatrix3x4 __fastcall GetPositionMatrixFromRHSet(const CRightHandedSet& set);
};

CMatrix3x4 __fastcall C3DMeshObject::GetPositionMatrixFromRHSet(const CRightHandedSet& set)
{
    CMatrix3x4 m;
    m.SetFromAxes(set.GetRight(), set.Forward, set.Up);
    return m;
}
