// Behaviour test for 0x0121AD80: the dynamic initializer of BACKWARDS_FORTY_FIVE_DEGREES_ORIENTATION.
#include <cstdio>
#include <cmath>
class C3DVector { public: float X, Y, Z; C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { } };
class CRightHandedSet { public: C3DVector Up, Forward; CRightHandedSet(const C3DVector& u, const C3DVector& f) : Up(u), Forward(f) { } };
static const CRightHandedSet BACKWARDS_FORTY_FIVE_DEGREES_ORIENTATION(C3DVector((float)std::sqrt(0.5), -(float)std::sqrt(0.5), 0.0f),
                                                                      C3DVector((float)std::sqrt(0.5), (float)std::sqrt(0.5), 0.0f));
static bool near(float a, float b) { return std::fabs(a - b) < 1e-6f; }
int main()
{
    const CRightHandedSet& s = BACKWARDS_FORTY_FIVE_DEGREES_ORIENTATION;
    const float r = 0.70710678f;
    if (!near(s.Up.X, r) || !near(s.Up.Y, -r) || s.Up.Z != 0.0f || !near(s.Forward.X, r) || !near(s.Forward.Y, r) || s.Forward.Z != 0.0f
        || !near(s.Up.X * s.Forward.X + s.Up.Y * s.Forward.Y, 0.0f))
    { std::printf("0121ad80_TEST FAIL\n"); return 1; }
    std::printf("0121ad80_TEST PASS\n");
    return 0;
}