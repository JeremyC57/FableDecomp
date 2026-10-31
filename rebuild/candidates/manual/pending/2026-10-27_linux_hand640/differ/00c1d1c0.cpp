// CMatrix3x4::SetInverse(const CMatrix3x4& m) -- the inverse of an affine 3x4 matrix: determinant by cofactors along the first column (clamped to +-epsilon), the adjugate times 1/det, translation = -(p * R^-1)
// Grade: DIFFER(347v347) -- x87 operand order of 7 commutative fmul pairs (det E23*E12 and E32*E23, cofactors E32*E23, E23*E12, E11*E23, E11*E22/E21*E12, translation E32*m.E43 and E13*m.E41); source order is inert | this session: inline GetDeterminant() (det identical), `this` members instead of a reference (translation worse), reference to *this (back to the 7)
// Leaf: SetInverse
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C1D1C0, 347 bytes, `ret`; destination in ecx, source in edx, no stack arguments
//   (the LTCG custom convention, reproduced with __fastcall: GOTCHAS chunk 230).
// Identity: FableWin.pdb ?SetInverse@CMatrix3x4@@QAEXABV1@@Z (with inline ?GetDeterminant@CMatrix3x4@@QBEMXZ); Ego_r.pdb has no
//   out-of-line copy (inlined there). The body is a generic two-matrix inverse with translation (not NoTranslate, not the
//   float-returning UniformScale form) called from two classes' code: C3DMeshObject's quadric update 0x00C1BF00 (`mov edx,esi
//   ; mov ecx,[esp+14h] ; call`) and CMatrix::UpdateInversePerspective 0x00C0B570. The catalog label
//   C3DMeshObject::SetInversePositionMatrixFromPositionMatrix (IBEXXZ, `this` only) does not fit the two-pointer interface.
//   The __fastcall member mangles ?SetInverse@CMatrix3x4@@QIAEXABV1@@Z; the PDB spelling is QAE (review at landing).
// Globals: inversion epsilon 0x0139C978 (1e-10f, descriptive); 0.0f / 1.0f from the float pool.
#include <math.h>
#pragma intrinsic(fabs)
class CMatrix3x4 {
public:
    float E11, E12, E13;
    float E21, E22, E23;
    float E31, E32, E33;
    float E41, E42, E43;
    float GetDeterminant() const
    {
        float det = (E13 * E32 - E33 * E12) * E21;
        det += (E23 * E12 - E13 * E22) * E31;
        det += (E33 * E22 - E32 * E23) * E11;
        return det;
    }
    void __fastcall SetInverse(const CMatrix3x4& m);
};
extern float g_InverseMatrixEpsilon;    // 0x0139C978

void __fastcall CMatrix3x4::SetInverse(const CMatrix3x4& m)
{
    float det = m.GetDeterminant();
    if (fabs(det) < g_InverseMatrixEpsilon)
        det = (det < 0.0f) ? -g_InverseMatrixEpsilon : g_InverseMatrixEpsilon;
    CMatrix3x4& r = *this;
    float inv = 1.0f / det;
    r.E11 = (m.E33 * m.E22 - m.E32 * m.E23) * inv;
    r.E21 = (m.E31 * m.E23 - m.E33 * m.E21) * inv;
    r.E31 = (m.E32 * m.E21 - m.E31 * m.E22) * inv;
    r.E12 = (m.E13 * m.E32 - m.E33 * m.E12) * inv;
    r.E22 = (m.E33 * m.E11 - m.E31 * m.E13) * inv;
    r.E32 = (m.E31 * m.E12 - m.E11 * m.E32) * inv;
    r.E13 = (m.E23 * m.E12 - m.E13 * m.E22) * inv;
    r.E23 = (m.E13 * m.E21 - m.E11 * m.E23) * inv;
    r.E33 = (m.E11 * m.E22 - m.E21 * m.E12) * inv;
    float t = m.E43 * r.E31;
    t += m.E41 * r.E11;
    t += m.E42 * r.E21;
    r.E41 = -t;
    t = m.E42 * r.E22;
    t += m.E41 * r.E12;
    t += r.E32 * m.E43;
    r.E42 = -t;
    t = m.E43 * r.E33;
    t += m.E42 * r.E23;
    t += r.E13 * m.E41;
    r.E43 = -t;
}
