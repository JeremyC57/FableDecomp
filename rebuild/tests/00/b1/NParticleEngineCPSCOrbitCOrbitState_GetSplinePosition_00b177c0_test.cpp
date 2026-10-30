// Behaviour test for CPSCOrbit::COrbitState::GetSplinePosition (0x00B177C0): cubic Hermite position.
#include <cstdio>
#include <cmath>
class C3DVector { public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }
    const C3DVector operator*(float f) const { return C3DVector(X * f, Y * f, Z * f); }
    const C3DVector operator+(const C3DVector& o) const { return C3DVector(X + o.X, Y + o.Y, Z + o.Z); }
};
class COrbitState {
public:
    float NextAngle, RotateSpeedRandomValue;
    C3DVector FromPosition, ToPosition, NextPosition, FromTangent, ToTangent;
    C3DVector GetSplinePosition(float t) const
    {
        float t2 = t * t;
        float t3 = t * t2;
        float h1 = 2.0f * t3 - 3.0f * t2 + 1.0f;
        float h2 = 3.0f * t2 - 2.0f * t3;
        float h3 = t3 - 2.0f * t2 + t;
        float h4 = t3 - t2;
        C3DVector result = FromPosition * h1 + ToPosition * h2 + FromTangent * h3 + ToTangent * h4;
        return result;
    }
};
static bool near(float a, float b) { return std::fabs(a - b) < 1e-4f; }
int main()
{
    COrbitState s;
    s.FromPosition = C3DVector(1, 2, 3); s.ToPosition = C3DVector(5, -2, 7);
    s.NextPosition = C3DVector(99, 99, 99);
    s.FromTangent = C3DVector(4, 0, 0); s.ToTangent = C3DVector(0, 4, 0);
    C3DVector a = s.GetSplinePosition(0.0f), b = s.GetSplinePosition(1.0f), m = s.GetSplinePosition(0.5f);
    // ends hit the positions; the midpoint is (P0+P1)/2 + (T0-T1)/8
    bool ok = near(a.X, 1) && near(a.Y, 2) && near(a.Z, 3) && near(b.X, 5) && near(b.Y, -2) && near(b.Z, 7) &&
              near(m.X, 3 + 0.5f) && near(m.Y, 0 - 0.5f) && near(m.Z, 5);
    // derivative at 0 approximates FromTangent
    C3DVector e = s.GetSplinePosition(0.001f);
    ok = ok && std::fabs((e.X - a.X) / 0.001f - 4.0f) < 0.05f && std::fabs((e.Y - a.Y) / 0.001f) < 0.05f;
    if (!ok) { std::printf("00b177c0_TEST FAIL\n"); return 1; }
    std::printf("00b177c0_TEST PASS\n");
    return 0;
}