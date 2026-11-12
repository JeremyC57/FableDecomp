// QuadricOptimiserInternals::CQuadric::GetInverseMatrixPart() const -- the inverse of the quadric's symmetric 3x3 part (AA AB AC / AB BB BC / AC BC CC) by cofactors, as the rotation rows of a CMatrix3x4 (translation row left unset)
// Grade: DIFFER(ours v retail) -- 229v237 on engine/QuadricOptimiserTypes.h (opt-in FABLE_RETAIL_CQUADRIC_GETINVERSEMATRIXPART) + engine/CMatrix3x4.h: x87 commutative operand order (retail fld AB/fmul BC in c23 and fld BB/fmul AA in E33; both source orders compile identically), retail spills E31 to a float temp in c33's dead slot and copies it to E13 with an integer move where ours keeps it on the stack (fst/fstp), and retail schedules the rep-movsd result copy setup (lea esi, mov edi) early.
// Leaf: ?GetInverseMatrixPart@CQuadric@QuadricOptimiserInternals@@QBE?AVCMatrix3x4@@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AC70F0, length 237 bytes, __thiscall, `ret 4` (hidden result pointer).
#define FABLE_RETAIL_CQUADRIC_GETINVERSEMATRIXPART
#include "engine/QuadricOptimiserTypes.h"
namespace QuadricOptimiserInternals {

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
