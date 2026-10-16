#include <cstdio>
// Behaviour model of C4DVector::Interpolate (0x00A5B7D0)
struct C4DVector {
    float X, Y, Z, W;
    C4DVector() {}
    C4DVector(float x, float y, float z, float w) : X(x), Y(y), Z(z), W(w) {}
    const C4DVector operator-(const C4DVector& o) const { return C4DVector(X - o.X, Y - o.Y, Z - o.Z, W - o.W); }
    const C4DVector operator+(const C4DVector& o) const { return C4DVector(X + o.X, Y + o.Y, Z + o.Z, W + o.W); }
    const C4DVector operator*(float s) const { return C4DVector(X * s, Y * s, Z * s, W * s); }
    const C4DVector Interpolate(const C4DVector& to, float t) const { return (to - *this) * t + *this; }
};
int main()
{
    C4DVector a(0, 2, 4, 8), b(10, 4, 0, 8);
    C4DVector m = a.Interpolate(b, 0.5f);
    if (m.X != 5 || m.Y != 3 || m.Z != 2 || m.W != 8) { std::printf("00a5b7d0_TEST FAIL\n"); return 1; }
    C4DVector e = a.Interpolate(b, 1.0f);
    if (e.X != 10 || e.Z != 0) { std::printf("00a5b7d0_TEST FAIL\n"); return 1; }
    std::printf("00a5b7d0_TEST PASS\n");
    return 0;
}