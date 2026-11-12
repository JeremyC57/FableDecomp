// Behaviour test for 0x0121ACA0: the dynamic initializer of LEFT_FORTY_FIVE_DEGREES_ORIENTATION.
#include <cstdio>
#include <cmath>
class C3DVector { public: float X, Y, Z; C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { } };
class CRightHandedSet { public: C3DVector Up, Forward; CRightHandedSet(const C3DVector& u, const C3DVector& f) : Up(u), Forward(f) { } };
static const float s = (float)std::sqrt(0.5);
static const CRightHandedSet LEFT_FORTY_FIVE_DEGREES_ORIENTATION(C3DVector(-s, -s, 0.0f), C3DVector(0.0f, 0.0f, -1.0f));
static bool near(float a, float b) { return std::fabs(a - b) < 1e-6f; }
int main()
{
    const CRightHandedSet& r = LEFT_FORTY_FIVE_DEGREES_ORIENTATION;
    const float h = 0.70710678f;
    if (!near(r.Up.X, -h) || r.Up.Z != 0.0f || r.Forward.X != 0.0f || r.Forward.Y != 0.0f
        || !near(r.Up.X * r.Up.X + r.Up.Y * r.Up.Y, 1.0f) || !near(r.Up.X * r.Forward.X + r.Up.Y * r.Forward.Y + r.Up.Z * r.Forward.Z, 0.0f))
    { std::printf("0121aca0_TEST FAIL\n"); return 1; }
    std::printf("0121aca0_TEST PASS\n");
    return 0;
}