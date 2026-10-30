// CEngineInternalPrimitiveMeshBase::CalcObjectMatrix(CMatrix3x4& matrix, const C3DVector& pos, const CRightHandedSet& rhset, float scale) -- build a mesh object's world matrix: rows -Right*scale, -Forward*scale, Up*scale (Right = Forward x Up) and the position as the translation row
// Grade: DIFFER(255v257) -- every instruction matches but VC7.1 shrink-wraps esi: it emits `pop esi` right after the last esi copy (before the first fsubp at 0x00BEBACE) where retail keeps esi pushed to the epilogue (`pop esi` at 0x00BEBB9A), so every later [esp+N] is 4 lower. Tried (compile-only sweep, RTM and QFE 4035, /Oa /G6): the six local orders, CRightHandedSet copies (implicit and user copy ctor), cross product on the copies / on rhset / spelled per component, GetUp/GetForward accessors, a 12-float Set, Set(4 vectors) with operator- temporaries, explicit translation stores, scaling orders.
// Leaf: ?CalcObjectMatrix@CEngineInternalPrimitiveMeshBase@@SIXAAVCMatrix3x4@@ABVC3DVector@@ABVCRightHandedSet@@M@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BEBAA0, length 257 bytes, `__fastcall` (matrix in ecx, position in edx,
//   the set and the scale on the stack, `ret 8`). Seven callers: 0x00B9CFAF, 0x00B9D963 (CEngineInternalPrimitiveStaticMesh
//   Update), 0x00BCEA05, 0x00BCEA25, 0x00BCF02F (CEngineInternalPrimitiveAnimatedMesh::CalcInterpolatedObjectTransform),
//   0x00BCF7E0, 0x00BCF9B0 (CEngineInternalPrimitive2DAnimatedMesh GetTransformFrameStart / End).
// Identity: Ego_r.pdb and FableWin.pdb both name `?CalcObjectMatrix@CEngineInternalPrimitiveMeshBase@@SIXAAVCMatrix3x4@@ABVC3DVector@@ABVCRightHandedSet@@M@Z`
//   (public static __fastcall), whose signature is exactly retail's register use; every caller is a
//   CEngineInternalPrimitiveMeshBase derivative passing (matrix, WorldPos, RightHandedSet, Scale). The landed callers bind
//   five invented spellings of this address (NAME_FIXES.tsv).
// Layout (PDB): CRightHandedSet = Up +0x00, Forward +0x0C; CMatrix3x4 = E11..E43 (row 4 the translation).
// Shape: the Forward copy (X/Y kept on the x87 stack, Z homed), then the inline GetRight (Forward x Up, read from the
//   set itself: VC7.1 copy-propagates), then the Up copy (homed), all three scaled in place (`x * scale`), and the rows
//   stored member by member with the negation at the store (`fchs ; fstp`), the translation copied last.
class C3DVector { public:
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }   // the engine's memberwise copy constructor
    C3DVector& operator*=(float f) { X *= f; Y *= f; Z *= f; return *this; }
};
inline C3DVector CrossProduct(const C3DVector& a, const C3DVector& b)
{
    return C3DVector(a.Y * b.Z - a.Z * b.Y,
                     a.Z * b.X - a.X * b.Z,
                     a.X * b.Y - a.Y * b.X);
}
class CRightHandedSet { public:
    C3DVector Up;                                      // +0x00
    C3DVector Forward;                                 // +0x0c
    C3DVector GetRight() const { return CrossProduct(Forward, Up); }   // as landed 0x0049CE00
};
class CMatrix3x4 { public:
    float E11, E12, E13;
    float E21, E22, E23;
    float E31, E32, E33;
    float E41, E42, E43;
    void SetTranslateOnly(const C3DVector& t) { E41 = t.X; E42 = t.Y; E43 = t.Z; }
};
class CEngineInternalPrimitiveMeshBase { public:
    static void __fastcall CalcObjectMatrix(CMatrix3x4& matrix, const C3DVector& pos, const CRightHandedSet& rhset, float scale);
};

void __fastcall CEngineInternalPrimitiveMeshBase::CalcObjectMatrix(CMatrix3x4& matrix, const C3DVector& pos, const CRightHandedSet& rhset, float scale)
{
    C3DVector forward = rhset.Forward;
    C3DVector right = rhset.GetRight();
    C3DVector up = rhset.Up;
    right *= scale;
    forward *= scale;
    up *= scale;
    matrix.E11 = -right.X;
    matrix.E12 = -right.Y;
    matrix.E13 = -right.Z;
    matrix.E21 = -forward.X;
    matrix.E22 = -forward.Y;
    matrix.E23 = -forward.Z;
    matrix.E31 = up.X;
    matrix.E32 = up.Y;
    matrix.E33 = up.Z;
    matrix.SetTranslateOnly(pos);
}
