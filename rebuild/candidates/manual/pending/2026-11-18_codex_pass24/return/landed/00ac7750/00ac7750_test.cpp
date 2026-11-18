// Standalone C++03 fixture for the recovered optimal-position method.
#include <math.h>
#include <cstdio>

struct C3DVector {
    float X, Y, Z;

    C3DVector() : X(0), Y(0), Z(0) {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}

    C3DVector operator+(const C3DVector& v) const { return C3DVector(X + v.X, Y + v.Y, Z + v.Z); }
    C3DVector operator-(const C3DVector& v) const { return C3DVector(X - v.X, Y - v.Y, Z - v.Z); }
    C3DVector operator-() const { return C3DVector(-X, -Y, -Z); }
    C3DVector operator*(float scale) const { return C3DVector(X * scale, Y * scale, Z * scale); }
    float GetMagnitude() const { return (float)sqrt(X * X + Y * Y + Z * Z); }
};

struct CMatrix3x4 {
    float E11, E12, E13, E21, E22, E23, E31, E32, E33;
};

namespace QuadricOptimiserInternals {

const float DELTA_NOTIONAL_ZERO = 0.0001f;

struct CQuadric {
    double determinant;
    C3DVector optimum;
    CMatrix3x4 inverse;

    CQuadric(double d, const C3DVector& p) : determinant(d), optimum(p) {
        CMatrix3x4 identity = { 1, 0, 0, 0, 1, 0, 0, 0, 1 };
        inverse = identity;
    }

    double GetDeterminant() const { return determinant; }
    CMatrix3x4 GetInverseMatrixPart() const { return inverse; }
    C3DVector GetVectorPart() const { return -optimum; }
    double EvaluateError(const C3DVector& point) const {
        C3DVector delta = point - optimum;
        return delta.X * delta.X + delta.Y * delta.Y + delta.Z * delta.Z;
    }
};

struct COptimisedPrimitive {
    C3DVector ComputeOptimalVertexPlacement(const CQuadric&, const C3DVector&, const C3DVector&);
};

// Recovered method body, with game dependencies supplied by the fixture.
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

static bool Equals(const C3DVector& point, float x, float y, float z)
{
    return point.X == x && point.Y == y && point.Z == z;
}

int main()
{
    using namespace QuadricOptimiserInternals;
    const C3DVector a(0, 0, 0), b(4, 0, 0);
    const CQuadric good(1, C3DVector(3, 1, 2));
    const CQuadric singular(DELTA_NOTIONAL_ZERO, C3DVector(3, 1, 2));
    const CQuadric tied(1, C3DVector(2, 0, 0));
    const CQuadric out_of_range(1, C3DVector(10, 0, 0));
    CQuadric scaled(1, C3DVector(3, 1, 2));
    scaled.inverse.E11 = 0.5f;
    COptimisedPrimitive owner;
    const bool ok = Equals(owner.ComputeOptimalVertexPlacement(good, a, b), 3, 1, 2)
        && Equals(owner.ComputeOptimalVertexPlacement(singular, a, b), 2, 0, 0)
        && Equals(owner.ComputeOptimalVertexPlacement(tied, a, b), 2, 0, 0)
        && Equals(owner.ComputeOptimalVertexPlacement(out_of_range, a, b), 2, 0, 0)
        && Equals(owner.ComputeOptimalVertexPlacement(scaled, a, b), 1.5f, 1, 2);
    std::puts(ok ? "00ac7750_TEST PASS" : "00ac7750_TEST FAIL");
    return ok ? 0 : 1;
}
