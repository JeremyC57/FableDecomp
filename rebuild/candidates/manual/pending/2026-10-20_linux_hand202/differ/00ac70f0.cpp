// QuadricOptimiserInternals::CQuadric::GetMatrixPart() const -- the inverse of the quadric's symmetric 3x3 part (AA AB AC / AB BB BC / AC BC CC) by cofactors, as the rotation rows of a CMatrix3x4 (translation row left unset)
// Grade: DIFFER(231v237) -- operand order: retail loads AB before BC in the c23 product and BB before AA in E33 (the lane picks the other operand first for both source orders, as in 0x852220), and parks E31 through a frame float with an integer copy to E13 where the lane keeps it on the x87 stack; the sum chain and the mirror copies already match
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AC70F0, length 237 bytes, `ret 4` (hidden result pointer).
// Constant: 1.0 (double) 0x01237F00. Layout: the eleven doubles of the landed Initialise (0x00AC7050) model; CMatrix3x4
// is E11..E43 floats (PDB). Used by the mesh simplifier to solve for the optimal collapse position.
// Shape: three cofactors kept as double locals (the frame is 8-aligned: `and esp,-8`), the determinant expanded along the
// first column and inverted with `1.0 / det`; the symmetric entries are chained `a = b = x` stores; the remaining
// cofactors are written inline in their assignments; the result is a named local returned by the implicit copy
// (`rep movsd` of 12 dwords, so E41..E43 carry whatever the frame held).
struct CMatrix3x4 {
    float E11, E12, E13;
    float E21, E22, E23;
    float E31, E32, E33;
    float E41, E42, E43;
};
namespace QuadricOptimiserInternals {

struct CQuadric {
    double AA, AB, AC, AD;
    double BB, BC, BD;
    double CC, CD;
    double DD;
    double Weight;
    CMatrix3x4 GetMatrixPart() const;
};

CMatrix3x4 CQuadric::GetMatrixPart() const
{
    CMatrix3x4 m;
    double c13 = AC * BC - CC * AB;
    double c23 = BC * AB - AC * BB;
    double c33 = CC * BB - BC * BC;
    double det = c33 * AA;
    det += c23 * AC;
    det += c13 * AB;
    double inv = 1.0 / det;
    m.E11 = (float)(c33 * inv);
    m.E21 = (float)(c13 * inv);
    m.E31 = (float)(c23 * inv);
    m.E12 = m.E21;
    m.E13 = m.E31;
    m.E22 = (float)((CC * AA - AC * AC) * inv);
    m.E32 = (float)((AC * AB - AA * BC) * inv);
    m.E23 = m.E32;
    m.E33 = (float)((AA * BB - AB * AB) * inv);
    return m;
}

}
