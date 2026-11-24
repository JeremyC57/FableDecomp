#include <stdio.h>
#include <math.h>
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

static bool check(double a, double b, double c, double d, double e, double f) {
    QuadricOptimiserInternals::CQuadric quad;
    quad.AA=a; quad.AB=b; quad.AC=c; quad.BB=d; quad.BC=e; quad.CC=f;
    quad.AD=quad.BD=quad.CD=quad.DD=quad.Weight=0;
    CMatrix3x4 r=quad.GetInverseMatrixPart();
    const double source[3][3]={{a,b,c},{b,d,e},{c,e,f}};
    const double inverse[3][3]={{r.E11,r.E12,r.E13},{r.E21,r.E22,r.E23},{r.E31,r.E32,r.E33}};
    for (int i=0; i<3; ++i)
        for (int j=0; j<3; ++j) {
            double value=0;
            for (int k=0; k<3; ++k) value+=source[i][k]*inverse[k][j];
            if (!(fabs(value-(i==j?1.0:0.0))<0.00001)) return false;
        }
    return true;
}

int main() {
    if (!check(1,0,0,1,0,1) || !check(-2,0,0,4,0,8) ||
        !check(4,1,2,3,0.5,5) || !check(1,2,0,0,1,-1)) return 1;
    puts("00ac70f0_TEST PASS");
}