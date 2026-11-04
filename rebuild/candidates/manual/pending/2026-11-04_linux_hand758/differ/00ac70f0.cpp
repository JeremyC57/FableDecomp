// QuadricOptimiserInternals::CQuadric::GetInverseMatrixPart() const -- the inverse of the quadric's symmetric 3x3 part (AA AB AC / AB BB BC / AC BC CC) by cofactors, as the rotation rows of a CMatrix3x4 (translation row left unset)
// Grade: DIFFER(229v237) -- x87 commutative operand order (retail fld AB/fmul BC in c23, fld BB/fmul AA in E33, fld c23/fmul inv) invariant to source order; retail spills E31 to a float temp copied to E13 through eax and schedules the result pointer/rep-movsd setup early; 10 grades (leaf rename to GetInverseMatrixPart, chained/float-local stores, operand orders, negated cofactor)
// Leaf: ?GetInverseMatrixPart@CQuadric@QuadricOptimiserInternals@@QBE?AVCMatrix3x4@@XZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AC70F0, length 237 bytes, `ret 4` (hidden result pointer).
// Constant: 1.0 (double) 0x01237F00. Layout: the eleven doubles of the landed Initialise (0x00AC7050) model; CMatrix3x4
// is E11..E43 floats (PDB). Used by the mesh simplifier to solve for the optimal collapse position.
// Shape: three cofactors kept as double locals (the frame is 8-aligned: `and esp,-8`), the determinant expanded along the
// first column and inverted with `1.0 / det`; the symmetric entries are chained `a = b = x` stores; the remaining
// cofactors are written inline in their assignments; the result is a named local returned by the implicit copy
// (`rep movsd` of 12 dwords, so E41..E43 carry whatever the frame held).
class CMatrix3x4 { public:
    float E11, E12, E13;
    float E21, E22, E23;
    float E31, E32, E33;
    float E41, E42, E43;
};
namespace QuadricOptimiserInternals {

class CQuadric { public:
    double AA, AB, AC, AD;
    double BB, BC, BD;
    double CC, CD;
    double DD;
    double Weight;
    CMatrix3x4 GetInverseMatrixPart() const;
};

CMatrix3x4 CQuadric::GetInverseMatrixPart() const
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
    float e21 = (float)(c13 * inv);
    m.E21 = e21;
    float e31 = (float)(c23 * inv);
    m.E31 = e31;
    m.E13 = e31;
    m.E12 = e21;
    m.E22 = (float)((CC * AA - AC * AC) * inv);
    m.E32 = (float)((AC * AB - AA * BC) * inv);
    m.E23 = m.E32;
    m.E33 = (float)((AA * BB - AB * AB) * inv);
    return m;
}

}
