// 00ac70f0 QuadricOptimiserInternals::CQuadric::GetInverseMatrixPart
// Leaf: ?GetInverseMatrixPart@CQuadric@QuadricOptimiserInternals@@QBE?AVCMatrix3x4@@XZ
// Flags:
#define FABLE_RETAIL_CQUADRIC_GETINVERSEMATRIXPART
#define FABLE_RETAIL_CQUADRIC_DETERMINANT_PASS27
#include "engine/QuadricOptimiserTypes.h"
#include "fable_assert.h"

namespace QuadricOptimiserInternals {
CMatrix3x4 CQuadric::GetInverseMatrixPart() const
{
    CMatrix3x4 matrix;
    double det = GetDeterminant();
    FABLE_RETAIL_ASSERT(det != 0.0);
    double rec = 1.0 / det;
    matrix.E11 = static_cast<float>((BB * CC - BC * BC) * rec);
    matrix.E21 = static_cast<float>((AC * BC - AB * CC) * rec);
    matrix.E31 = static_cast<float>((AB * BC - AC * BB) * rec);
    matrix.E12 = static_cast<float>((BC * AC - AB * CC) * rec);
    matrix.E22 = static_cast<float>((AA * CC - AC * AC) * rec);
    matrix.E32 = static_cast<float>((AC * AB - AA * BC) * rec);
    matrix.E13 = static_cast<float>((AB * BC - BB * AC) * rec);
    matrix.E23 = static_cast<float>((AB * AC - AA * BC) * rec);
    matrix.E33 = static_cast<float>((AA * BB - AB * AB) * rec);
    return matrix;
}
}