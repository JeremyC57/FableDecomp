// C3DMeshObject::GetRotationMatrixForTransformFromTo(const CRightHandedSet& from, const CRightHandedSet& to) -- rotation matrix taking the from basis to the to basis: inverse of (FxU, Up, Forward) post-multiplied by (FxU, Forward, Up)
// Grade: DIFFER(1012v1022) -- operand order of memory x memory products (retail loads the lower-address operand first) and the cofactor evaluation order of the inline SetInverse (retail computes c11 first); structure and frame match
// Leaf: ?GetRotationMatrixForTransformFromTo@C3DMeshObject@@SI?AVCMatrix3x4@@ABVCRightHandedSet@@0@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A35B70, 1022 bytes, __fastcall static, hidden result pointer.
// Identity: Ego_r.pdb `?GetRotationMatrixForTransformFromTo@C3DMeshObject@@SI?AVCMatrix3x4@@ABVCRightHandedSet@@0@Z`
//   (donor order after GetPositionMatrixFromRHSet 0x00A358F0, rows (-R,-F,U)). Retail processes `to` first.
class C3DVector { public:
    float X, Y, Z;
};

class CRightHandedSet { public:
    C3DVector Up;        // +0x00
    C3DVector Forward;   // +0x0c
};

class CMatrix3x4 { public:
    float E11, E12, E13;
    float E21, E22, E23;
    float E31, E32, E33;
    float E41, E42, E43;
    CMatrix3x4() { }
    __forceinline void Set(const C3DVector& x, const C3DVector& y, const C3DVector& z)
    {
        E11 = x.X; E12 = x.Y; E13 = x.Z;
        E21 = y.X; E22 = y.Y; E23 = y.Z;
        E31 = z.X; E32 = z.Y; E33 = z.Z;
        E41 = 0.0f;
        E42 = 0.0f;
        E43 = 0.0f;
    }
    __forceinline void SetInverse(const CMatrix3x4& m)
    {
        float det = m.E11 * (m.E22 * m.E33 - m.E23 * m.E32) + m.E21 * (m.E32 * m.E13 - m.E33 * m.E12) + m.E31 * (m.E23 * m.E12 - m.E22 * m.E13);
        float inv = 1.0f / det;
        E11 = (m.E22 * m.E33 - m.E23 * m.E32) * inv;
        E21 = (m.E23 * m.E31 - m.E21 * m.E33) * inv;
        E31 = (m.E21 * m.E32 - m.E22 * m.E31) * inv;
        E12 = (m.E32 * m.E13 - m.E33 * m.E12) * inv;
        E22 = (m.E33 * m.E11 - m.E13 * m.E31) * inv;
        E32 = (m.E31 * m.E12 - m.E32 * m.E11) * inv;
        E13 = (m.E23 * m.E12 - m.E22 * m.E13) * inv;
        E23 = (m.E21 * m.E13 - m.E23 * m.E11) * inv;
        E33 = (m.E22 * m.E11 - m.E21 * m.E12) * inv;
        E41 = -(E11 * m.E41 + E21 * m.E42 + E31 * m.E43);
        E42 = -(E12 * m.E41 + E22 * m.E42 + E32 * m.E43);
        E43 = -(E13 * m.E41 + E23 * m.E42 + E33 * m.E43);
    }
    __forceinline void PostMultiply(const CMatrix3x4& matrix)
    {
        CMatrix3x4 a = *this;
        E11 = a.E11 * matrix.E11 + a.E12 * matrix.E21 + a.E13 * matrix.E31;
        E12 = a.E11 * matrix.E12 + a.E12 * matrix.E22 + a.E13 * matrix.E32;
        E13 = a.E11 * matrix.E13 + a.E12 * matrix.E23 + a.E13 * matrix.E33;
        E21 = a.E21 * matrix.E11 + a.E22 * matrix.E21 + a.E23 * matrix.E31;
        E22 = a.E21 * matrix.E12 + a.E22 * matrix.E22 + a.E23 * matrix.E32;
        E23 = a.E21 * matrix.E13 + a.E22 * matrix.E23 + a.E23 * matrix.E33;
        E31 = a.E31 * matrix.E11 + a.E32 * matrix.E21 + a.E33 * matrix.E31;
        E32 = a.E31 * matrix.E12 + a.E32 * matrix.E22 + a.E33 * matrix.E32;
        E33 = a.E31 * matrix.E13 + a.E32 * matrix.E23 + a.E33 * matrix.E33;
        E41 = a.E41 * matrix.E11 + a.E42 * matrix.E21 + a.E43 * matrix.E31 + matrix.E41;
        E42 = a.E41 * matrix.E12 + a.E42 * matrix.E22 + a.E43 * matrix.E32 + matrix.E42;
        E43 = a.E41 * matrix.E13 + a.E42 * matrix.E23 + a.E43 * matrix.E33 + matrix.E43;
    }
};

class C3DMeshObject { public:
    static CMatrix3x4 __fastcall GetRotationMatrixForTransformFromTo(const CRightHandedSet& from, const CRightHandedSet& to);
};

CMatrix3x4 __fastcall C3DMeshObject::GetRotationMatrixForTransformFromTo(const CRightHandedSet& from, const CRightHandedSet& to)
{
    C3DVector toRight;
    toRight.X = to.Up.Z * to.Forward.Y - to.Up.Y * to.Forward.Z;
    toRight.Y = to.Up.X * to.Forward.Z - to.Up.Z * to.Forward.X;
    toRight.Z = to.Forward.X * to.Up.Y - to.Forward.Y * to.Up.X;
    CMatrix3x4 toMatrix;
    toMatrix.Set(toRight, to.Forward, to.Up);
    C3DVector fromRight;
    fromRight.X = from.Up.Z * from.Forward.Y - from.Up.Y * from.Forward.Z;
    fromRight.Y = from.Up.X * from.Forward.Z - from.Up.Z * from.Forward.X;
    fromRight.Z = from.Forward.X * from.Up.Y - from.Forward.Y * from.Up.X;
    CMatrix3x4 fromMatrix;
    fromMatrix.Set(fromRight, from.Up, from.Forward);
    CMatrix3x4 result;
    result.SetInverse(fromMatrix);
    result.PostMultiply(toMatrix);
    return result;
}
