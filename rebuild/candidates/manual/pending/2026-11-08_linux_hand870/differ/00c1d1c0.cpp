// InvertMatrix3x4(CMatrix3x4*, const CMatrix3x4&) -- write the inverse of an affine 3x4 matrix (adjugate / determinant, translation -p*R^-1)
// Grade: DIFFER(347v347) md 8 -- two x87 operand orders in the translation: retail loads m.E41 first in `m.E41 * out->E11` and `m.E41 * out->E12` (rows 1-2) but second in row 3; ours loads it second in all three. Probe (c2prio, probe/00c1d1c0.best.best.out): m.E41 is one c2 CSE temp across the three rows (id 24, lines 43/47/52, like E43 id 23 and E42 id 25), the out elements are store-forwarding temps 15-22; the operand order is decided on those temps, not on source text (512 textual operand orders, 72 term permutations in expression and `t +=` forms, __fastcall member / C3DVector / reference models: no change; 599 grades)
// Next: a source shape that changes the m.E41 temp history (e.g. an inline vector-by-matrix helper or a different adjugate order that leaves the E41 row unforwarded); otherwise a VC7.1 x87 commutation rule outside source control
// Leaf: InvertMatrix3x4
// Flags: none
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
    float det = m.E11 * (m.E22 * m.E33 - m.E23 * m.E32) + m.E21 * (m.E32 * m.E13 - m.E33 * m.E12) + m.E31 * (m.E23 * m.E12 - m.E22 * m.E13);
    if (fabs(det) < g_InverseMatrixEpsilon)
        det = (det < 0.0f) ? -g_InverseMatrixEpsilon : g_InverseMatrixEpsilon;
    float inv = 1.0f / det;
    out->E11 = (m.E22 * m.E33 - m.E23 * m.E32) * inv;
    out->E21 = (m.E23 * m.E31 - m.E21 * m.E33) * inv;
    out->E31 = (m.E21 * m.E32 - m.E22 * m.E31) * inv;
    out->E12 = (m.E32 * m.E13 - m.E33 * m.E12) * inv;
    out->E22 = (m.E33 * m.E11 - m.E13 * m.E31) * inv;
    out->E32 = (m.E31 * m.E12 - m.E32 * m.E11) * inv;
    out->E13 = (m.E23 * m.E12 - m.E22 * m.E13) * inv;
    out->E23 = (m.E21 * m.E13 - m.E23 * m.E11) * inv;
    out->E33 = (m.E22 * m.E11 - m.E21 * m.E12) * inv;
    float t = m.E43 * out->E31;
    t += m.E41 * out->E11;
    t += m.E42 * out->E21;
    out->E41 = -t;
    t = m.E42 * out->E22;
    t += m.E41 * out->E12;
    t += out->E32 * m.E43;
    out->E42 = -t;
    t = m.E43 * out->E33;
    t += m.E42 * out->E23;
    t += out->E13 * m.E41;
    out->E43 = -t;
}
