// behaviour test for 0x00AF3CF0 NParticleEngine::GFInterpolate(const C3DVector& a, const C3DVector& b, float t): a * (1 - t) + b * t
#include <cstdio>
static bool Near(float a, float b) { float d = a - b; return d < 0.0001f && d > -0.0001f; }
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector operator*(float s) const { return C3DVector(X * s, Y * s, Z * s); }
    C3DVector operator+(const C3DVector& v) const { return C3DVector(X + v.X, Y + v.Y, Z + v.Z); }
};
static C3DVector GFInterpolate(const C3DVector& a, const C3DVector& b, float t) { return a * (1.0f - t) + b * t; }
int main()
{

    C3DVector a(1.0f, 2.0f, -4.0f), b(3.0f, -2.0f, 8.0f);
    C3DVector r0 = GFInterpolate(a, b, 0.0f), r1 = GFInterpolate(a, b, 1.0f), rh = GFInterpolate(a, b, 0.25f);
    if (!Near(r0.X, 1.0f) || !Near(r0.Y, 2.0f) || !Near(r0.Z, -4.0f)) { std::printf("00af3cf0_TEST FAIL\n"); return 1; }
    if (!Near(r1.X, 3.0f) || !Near(r1.Y, -2.0f) || !Near(r1.Z, 8.0f)) { std::printf("00af3cf0_TEST FAIL\n"); return 1; }
    if (!Near(rh.X, 1.5f) || !Near(rh.Y, 1.0f) || !Near(rh.Z, -1.0f)) { std::printf("00af3cf0_TEST FAIL\n"); return 1; }
    if (sizeof(C3DVector) != 12) { std::printf("00af3cf0_TEST FAIL\n"); return 1; }
    std::printf("00af3cf0_TEST PASS\n");
    return 0;
}