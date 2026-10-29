// CEngineInternalPrimitiveMeshBase::Calc2DObjectMatrix(CMatrix3x4& m, const C3DVector& centre, const C3DVector& position, const CRightHandedSet& orientation, float scale) -- the 2D mesh object matrix: the scaled frame (-right, -forward, up) about the bounding-sphere centre, then moved to the position
// Grade: DIFFER(970v978) -- x87 term order / CSE scheduling in the two inlined translation multiplies (fadd st(1)/st(2) order, where the shared x*0.0f products spill), the zero-translation stores scheduled before the rotation stores, and the second snapshot's rep movsd setup not interleaved into the first multiply's translation row; prologue, frame (0x98), cross product, scaling and the rotation stores match.
// Leaf: ?Calc2DObjectMatrix@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BEB6C0, 978 bytes, static __fastcall (m in ecx, centre in edx; position,
//   orientation and scale on the stack, `ret 0Ch`). No callees; reads the 0.0f constant 0x0122DEDC.
// Identity: Ego_r.pdb ?Calc2DObjectMatrix@CEngineInternalPrimitiveMeshBase@@SIXAAVCMatrix3x4@@ABVC3DVector@@1ABVCRightHandedSet@@M@Z
//   (0x00AA3920, directly before CalcObjectMatrix 0x00AA3C80; FableWin.pdb 0x02EE3850 before 0x02EE3A00), as retail
//   0x00BEB6C0 sits directly before CalcObjectMatrix 0x00BEBAA0; its only caller is
//   CEngineInternalPrimitive2DAnimatedMesh::CalcInterpolatedObjectTransform 0x00BCF670 (centre = the bounding-sphere centre).
//   The manifest label Interpolate_Quaternion_Scale_Matrix is wrong (no quaternion, no interpolation).
// Math: right = Forward x Up (CRightHandedSet::GetRight); m = [-right*s; -forward*s; up*s; 0], then m = T(-centre) * m and
//   m = m * T(position) through inline CMatrix3x4 translation multiplies (x*1.0f folded, x*0.0f kept as fmul [0x0122DEDC]).
// Shape: the translation matrices are frame locals built first (T(-centre) stays in x87 registers, T(position) is the
//   frame object at +0x68 whose translation is an integer copy); the rotation goes through the 12-float Set (a vector Set
//   fed -right temporaries materialises them); the multiplies snapshot *this (rep movsd) -- the sum forms below are the
//   best of a per-element search (two separate multiply bodies).
class C3DVector { public:
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector operator-() const { return C3DVector(-X, -Y, -Z); }
    C3DVector operator*(float f) const { return C3DVector(X * f, Y * f, Z * f); }
};
inline C3DVector CrossProduct(const C3DVector& a, const C3DVector& b)
{
    return C3DVector(a.Y * b.Z - a.Z * b.Y,
                     a.Z * b.X - a.X * b.Z,
                     a.X * b.Y - a.Y * b.X);
}
class CRightHandedSet { public:
    C3DVector Up;
    C3DVector Forward;
    C3DVector GetRight() const { return CrossProduct(Forward, Up); }
};
class CMatrix3x4 { public:
    float E11, E12, E13;
    float E21, E22, E23;
    float E31, E32, E33;
    float E41, E42, E43;
    CMatrix3x4() {}
    __forceinline void Set(float e11, float e12, float e13, float e21, float e22, float e23,
                           float e31, float e32, float e33, float e41, float e42, float e43)
    {
        E11 = e11; E12 = e12; E13 = e13;
        E21 = e21; E22 = e22; E23 = e23;
        E31 = e31; E32 = e32; E33 = e33;
        E41 = e41; E42 = e42; E43 = e43;
    }
    __forceinline void SetTranslate(const C3DVector& t)
    {
        E11 = 1.0f; E12 = 0.0f; E13 = 0.0f;
        E21 = 0.0f; E22 = 1.0f; E23 = 0.0f;
        E31 = 0.0f; E32 = 0.0f; E33 = 1.0f;
        E41 = t.X; E42 = t.Y; E43 = t.Z;
    }
    __forceinline void SetMultiplyPre(const CMatrix3x4& a, const CMatrix3x4& b)
    {
        E11 = (a.E11 * b.E11 + a.E12 * b.E21) + a.E13 * b.E31;
        E12 = (a.E11 * b.E12 + a.E13 * b.E32) + a.E12 * b.E22;
        E13 = (a.E11 * b.E13 + a.E12 * b.E23) + a.E13 * b.E33;
        E21 = (a.E21 * b.E11 + a.E22 * b.E21) + a.E23 * b.E31;
        E22 = (a.E21 * b.E12 + a.E22 * b.E22) + a.E23 * b.E32;
        E23 = (a.E21 * b.E13 + a.E22 * b.E23) + a.E23 * b.E33;
        E31 = a.E31 * b.E11 + (a.E32 * b.E21 + a.E33 * b.E31);
        E32 = (a.E31 * b.E12 + a.E33 * b.E32) + a.E32 * b.E22;
        E33 = (a.E31 * b.E13 + a.E33 * b.E33) + a.E32 * b.E23;
        E41 = (a.E41 * b.E11 + (a.E42 * b.E21 + a.E43 * b.E31)) + b.E41;
        E42 = (a.E41 * b.E12 + (a.E42 * b.E22 + a.E43 * b.E32)) + b.E42;
        E43 = (a.E41 * b.E13 + (a.E42 * b.E23 + a.E43 * b.E33)) + b.E43;
    }
    __forceinline void SetMultiplyPost(const CMatrix3x4& a, const CMatrix3x4& b)
    {
        E11 = (a.E11 * b.E11 + a.E12 * b.E21) + a.E13 * b.E31;
        E12 = (a.E11 * b.E12 + a.E12 * b.E22) + a.E13 * b.E32;
        E13 = (a.E11 * b.E13 + a.E13 * b.E33) + a.E12 * b.E23;
        E21 = (a.E21 * b.E11 + a.E22 * b.E21) + a.E23 * b.E31;
        E22 = (a.E21 * b.E12 + a.E22 * b.E22) + a.E23 * b.E32;
        E23 = (a.E21 * b.E13 + a.E23 * b.E33) + a.E22 * b.E23;
        E31 = (a.E31 * b.E11 + a.E32 * b.E21) + a.E33 * b.E31;
        E32 = (a.E31 * b.E12 + a.E32 * b.E22) + a.E33 * b.E32;
        E33 = (a.E31 * b.E13 + a.E33 * b.E33) + a.E32 * b.E23;
        E41 = ((a.E41 * b.E11 + a.E42 * b.E21) + b.E41) + a.E43 * b.E31;
        E42 = (a.E41 * b.E12 + (a.E42 * b.E22 + b.E42)) + a.E43 * b.E32;
        E43 = a.E42 * b.E23 + ((a.E43 * b.E33 + b.E43) + a.E41 * b.E13);
    }
    __forceinline void PreMultiply(const CMatrix3x4& m) { CMatrix3x4 a(*this); SetMultiplyPre(m, a); }
    __forceinline void PostMultiply(const CMatrix3x4& m) { CMatrix3x4 a(*this); SetMultiplyPost(a, m); }
    __forceinline void PreTranslate(const C3DVector& v) { CMatrix3x4 t; t.SetTranslate(v); PreMultiply(t); }
    __forceinline void PostTranslate(const C3DVector& v) { CMatrix3x4 t; t.SetTranslate(v); PostMultiply(t); }
};
class CEngineInternalPrimitiveMeshBase { public:
    static void __fastcall Calc2DObjectMatrix(CMatrix3x4& m, const C3DVector& centre, const C3DVector& position, const CRightHandedSet& rhs, float scale);
};
void __fastcall CEngineInternalPrimitiveMeshBase::Calc2DObjectMatrix(CMatrix3x4& m, const C3DVector& centre, const C3DVector& position, const CRightHandedSet& rhs, float scale)
{
    CMatrix3x4 toCentre;
    toCentre.SetTranslate(-centre);
    CMatrix3x4 toPosition;
    toPosition.SetTranslate(position);
    C3DVector forward = rhs.Forward * scale;
    C3DVector right = rhs.GetRight() * scale;
    C3DVector up = rhs.Up * scale;
    m.Set(-right.X, -right.Y, -right.Z, -forward.X, -forward.Y, -forward.Z, up.X, up.Y, up.Z, 0.0f, 0.0f, 0.0f);
    m.PreMultiply(toCentre);
    m.PostMultiply(toPosition);
}
