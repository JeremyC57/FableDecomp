// C3DMeshObject::GetPositionMatrixFromRHSet(const CRightHandedSet& set) -- static: the position matrix of an orientation frame, rows -Right, -Forward, Up and a zero translation
// Grade: DIFFER(185v185) -- 4 lines: the z term F.X*U.Y of GetRight loads Up.Y first (`fld [edx+4] ; fmul [edx+0Ch]`), retail Forward.X first. chunk 895: rebased on the shared engine/C3DVector.h / CRightHandedSet.h (FABLE_RETAIL_C3DVECTOR operator^, FABLE_RETAIL_CRIGHTHANDEDSET GetRight = Forward ^ Up, GetForward / GetUp) -- same residue; a named right local reshuffles the frame (56 lines).
// Leaf: ?GetPositionMatrixFromRHSet@C3DMeshObject@@SI?AVCMatrix3x4@@ABVCRightHandedSet@@@Z
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
// Tried later (sweep + chunk 629): `b.Y * a.X`, (float)(double) z, `b.X * a.Y`, the landed CrossProduct; raw float pointer
//   spellings of the z operands (`(&a.X)[0]`, `*(&a.Y - 1)`, `pb[1]` through pointer locals, all six terms through pointers),
//   `-(a.Y*b.X) + a.X*b.Y`, named x/y/z locals, a member C3DVector::CrossProduct -- the z product never flips.
// Next: lane gap: x87 operand order (the Up.Y / Forward.X loads are also the by-value copy loads).
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CRIGHTHANDEDSET
#include "engine/CRightHandedSet.h"

class CMatrix3x4 { public:
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

class C3DMeshObject { public:
    static CMatrix3x4 __fastcall GetPositionMatrixFromRHSet(const CRightHandedSet& set);
};

CMatrix3x4 __fastcall C3DMeshObject::GetPositionMatrixFromRHSet(const CRightHandedSet& set)
{
    CMatrix3x4 m;
    m.SetFromAxes(set.GetRight(), set.GetForward(), set.GetUp());
    return m;
}
