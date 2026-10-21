// InvertMatrix3x4(CMatrix3x4*, const CMatrix3x4&) -- write the inverse of an affine 3x4 matrix (adjugate / determinant, translation -p*R^-1)
// Grade: DIFFER(347v347) -- 7 commutative fmul pairs emit the other operand first (E23*E12, E32*E23, E11*E23, E11*E22, E21*E12, two translation terms); source operand order has no effect
// Leaf: InvertMatrix3x4
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C1D1C0, length 347 bytes (AI steering library, LTCG register
// convention: result matrix in ecx, source in edx, no stack arguments -> __fastcall).
// Label: the manifest's "C3DMeshObject::SetInversePositionMatrixFromPositionMatrix" is a weak BSim port (0.53);
//        the real member would address PositionMatrix at this+0x10 and InversePositionMatrix at this+0x44, while
//        this body reads and writes both matrices at offset 0 through two register arguments. Both callers
//        (0x00C0B67A, 0x00C1BFA2) sit in the steering library at 0xC0xxxx-0xC2xxxx.
// Globals: inversion epsilon @ 0x0139C978 (.data, 1e-10f); 0.0f / 1.0f literals from the float pool.
// Shape: determinant by cofactor expansion along the first column, clamped to +-epsilon when |det| < epsilon
//        (`fabs ; fcomp ; test ah,5 ; jp`, then the sign select), 1/det kept on the FPU stack for the nine
//        cofactors, then the translation as the negated rotated position.
#include <math.h>
#pragma intrinsic(fabs)

struct CMatrix3x4 {                      // PDB CMatrix3x4 (48 bytes): rows E1x..E3x, translation E4x
    float E11, E12, E13;
    float E21, E22, E23;
    float E31, E32, E33;
    float E41, E42, E43;
};

extern float g_InverseMatrixEpsilon;    // 0x0139C978

void __fastcall InvertMatrix3x4(CMatrix3x4* out, const CMatrix3x4& m)
{
    CMatrix3x4& r = *out;
    float det = (m.E13 * m.E32 - m.E33 * m.E12) * m.E21;
    det += (m.E23 * m.E12 - m.E13 * m.E22) * m.E31;
    det += (m.E33 * m.E22 - m.E32 * m.E23) * m.E11;
    if (fabs(det) < g_InverseMatrixEpsilon)
        det = (det < 0.0f) ? -g_InverseMatrixEpsilon : g_InverseMatrixEpsilon;
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
