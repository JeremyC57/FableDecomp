#include <cstdio>
#include <cmath>
// Behaviour test: anonymous GetRelativePosition -- the index-th of count points on a spiral: azimuth fraction*2pi in XY, elevation fraction*pi/2 lifting toward the levitation offset.
#define CHECK(c) if (!(c)) { std::printf("007f8220_TEST FAIL\n"); return 1; }
class C3DVector { public:
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector operator*(float s) const { return C3DVector(X * s, Y * s, Z * s); }
    C3DVector operator+(const C3DVector& o) const { return C3DVector(X + o.X, Y + o.Y, Z + o.Z); }
};
static C3DVector g_LevitationOffset(0.0f, 0.0f, 2.5f);
static C3DVector GetRelativePosition(long index, long count)
{
    float fraction = (float)index / count;
    float elevation = fraction * 0.25f;
    C3DVector dir = C3DVector(1.0f, 0.0f, 0.0f) * (float)std::cos(fraction * 6.2831854820251465)
                  + C3DVector(0.0f, 1.0f, 0.0f) * (float)std::sin(fraction * 6.2831854820251465);
    return g_LevitationOffset * (float)std::cos(elevation * 6.2831854820251465) + dir * (float)std::sin(elevation * 6.2831854820251465);
}
static bool Near(float a, float b) { return a - b < 1e-4f && b - a < 1e-4f; }
int main()
{
    C3DVector p0 = GetRelativePosition(0, 4);
    CHECK(Near(p0.X, 0.0f) && Near(p0.Y, 0.0f) && Near(p0.Z, 2.5f))
    C3DVector p1 = GetRelativePosition(1, 4);
    float s = (float)std::sin(0.0625 * 6.2831854820251465), c = (float)std::cos(0.0625 * 6.2831854820251465);
    CHECK(Near(p1.X, 0.0f) && Near(p1.Y, s) && Near(p1.Z, 2.5f * c))
    C3DVector p2 = GetRelativePosition(2, 4);
    CHECK(p2.X < 0.0f && Near(p2.Y, 0.0f))
    std::printf("007f8220_TEST PASS\n");
    return 0;
}