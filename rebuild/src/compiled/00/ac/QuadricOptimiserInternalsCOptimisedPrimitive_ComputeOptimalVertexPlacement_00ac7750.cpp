// QuadricOptimiserInternals::COptimisedPrimitive::ComputeOptimalVertexPlacement(const CQuadric&, const C3DVector&, const C3DVector&) -- choose the contracted edge position that minimises the quadric error.
// Leaf: ?ComputeOptimalVertexPlacement@COptimisedPrimitive@QuadricOptimiserInternals@@AAE?AVC3DVector@@ABVCQuadric@2@ABV3@1@Z
// Retail: 0x00AC7750, 495 bytes, thiscall.
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_MAGNITUDE
#define FABLE_RETAIL_CQUADRIC_GETINVERSEMATRIXPART
#define FABLE_RETAIL_CQUADRIC_OPTIMALPLACEMENT
#define FABLE_RETAIL_QUADRIC_COPTIMISEDPRIMITIVE
#define FABLE_RETAIL_QUADRIC_COPTIMISEDPRIMITIVE_OPTIMALPLACEMENT
#include <math.h>
#include "engine/QuadricOptimiserTypes.h"
#include "engine/COptimisedPrimitive.h"

#if defined(_MSC_VER)
#define FABLE_QUADRIC_BOUNDARY __declspec(noinline)
#else
#define FABLE_QUADRIC_BOUNDARY
#endif

extern const float DELTA_NOTIONAL_ZERO;

namespace QuadricOptimiserInternals {

// The retail function calls these helpers in the same module. Keeping their
// bodies visible gives the compiler that call context.
FABLE_QUADRIC_BOUNDARY double CQuadric::EvaluateError(const C3DVector& v) const
{
    float x = v.X, y = v.Y, z = v.Z;
    return (z * z) * CC + (y * z * 2.0f) * BC + (y * y) * BB + (x * z * 2.0f) * AC + (x * y * 2.0f) * AB
         + (z * 2.0f) * CD + (y * 2.0f) * BD + (x * 2.0f) * AD + (x * x) * AA + DD;
}
FABLE_QUADRIC_BOUNDARY CMatrix3x4 CQuadric::GetInverseMatrixPart() const
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
C3DVector COptimisedPrimitive::ComputeOptimalVertexPlacement(
    const CQuadric& quadric, const C3DVector& position1, const C3DVector& position2)
{
    C3DVector midpoint = (position1 + position2) * 0.5f;
    float distance = (position2 - position1).GetMagnitude();
    if (quadric.GetDeterminant() > DELTA_NOTIONAL_ZERO) {
        CMatrix3x4 inverse = quadric.GetInverseMatrixPart();
        C3DVector vect_part = -quadric.GetVectorPart();
        C3DVector optimal;
        optimal.X = vect_part.X * inverse.E11 + (vect_part.Y * inverse.E21 + vect_part.Z * inverse.E31);
        optimal.Y = vect_part.X * inverse.E12 + (vect_part.Y * inverse.E22 + vect_part.Z * inverse.E32);
        optimal.Z = vect_part.X * inverse.E13 + (vect_part.Y * inverse.E23 + vect_part.Z * inverse.E33);
        float error_optimal = (float)quadric.EvaluateError(optimal);
        float error_midpoint = (float)quadric.EvaluateError(midpoint);
        if (error_optimal < error_midpoint) {
            if ((optimal - midpoint).GetMagnitude() < distance * 2.0f)
                return optimal;
        }
    }
    return midpoint;
}
}