// Behaviour test for 0x00885680 CEnvironmentSpellEffectInterface::InterpolateRadialBlurPositionParams(float, const&, const&)
#include <cstdio>
#include <cmath>

class C3DVector { public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector operator*(float f) const { return C3DVector(X * f, Y * f, Z * f); }
    C3DVector operator+(const C3DVector& v) const { return C3DVector(X + v.X, Y + v.Y, Z + v.Z); }
};
struct CRadialBlurPositionParams {
    C3DVector AreaCenter, BlurCenter;
    int AreaTrackPos, BlurTrackPos;
};
static CRadialBlurPositionParams Interpolate(float t, const CRadialBlurPositionParams& from, const CRadialBlurPositionParams& to)
{
    CRadialBlurPositionParams result;
    if (from.BlurTrackPos == to.BlurTrackPos) result.BlurCenter = from.BlurCenter * (1.0f - t) + to.BlurCenter * t;
    else result.BlurCenter = to.BlurCenter;
    if (from.AreaTrackPos == to.AreaTrackPos) result.AreaCenter = from.AreaCenter * (1.0f - t) + to.AreaCenter * t;
    else result.AreaCenter = to.AreaCenter;
    result.AreaTrackPos = to.AreaTrackPos;
    result.BlurTrackPos = to.BlurTrackPos;
    return result;
}
static bool near(float a, float b) { return std::fabs(a - b) < 1e-5f; }
int main()
{
    int fail = 0;
    CRadialBlurPositionParams a, b;
    a.AreaCenter = C3DVector(0.0f, 0.0f, 0.0f); a.BlurCenter = C3DVector(4.0f, 8.0f, -4.0f); a.AreaTrackPos = 1; a.BlurTrackPos = 2;
    b.AreaCenter = C3DVector(10.0f, 20.0f, 30.0f); b.BlurCenter = C3DVector(0.0f, 0.0f, 4.0f); b.AreaTrackPos = 1; b.BlurTrackPos = 2;
    CRadialBlurPositionParams r = Interpolate(0.5f, a, b);
    if (!near(r.AreaCenter.X, 5.0f) || !near(r.AreaCenter.Z, 15.0f) || !near(r.BlurCenter.Y, 4.0f) || !near(r.BlurCenter.Z, 0.0f)) fail |= 1;
    if (r.AreaTrackPos != 1 || r.BlurTrackPos != 2) fail |= 2;
    b.BlurTrackPos = 3;                                         // different kinds: take `to`'s blur centre
    r = Interpolate(0.5f, a, b);
    if (!near(r.BlurCenter.X, 0.0f) || !near(r.BlurCenter.Z, 4.0f) || r.BlurTrackPos != 3 || !near(r.AreaCenter.Y, 10.0f)) fail |= 4;
    b.AreaTrackPos = 0;
    r = Interpolate(0.25f, a, b);
    if (!near(r.AreaCenter.Y, 20.0f) || r.AreaTrackPos != 0) fail |= 8;
    if (fail) { std::printf("00885680_TEST FAIL\n"); return 1; }
    std::printf("00885680_TEST PASS\n");
    return 0;
}