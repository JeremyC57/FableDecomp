#include <cstdio>
struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    C3DVector operator+(const C3DVector& o) const { return C3DVector(X + o.X, Y + o.Y, Z + o.Z); }
    C3DVector operator*(float f) const { return C3DVector(X * f, Y * f, Z * f); }
    C3DVector Interpolate(const C3DVector& to, float t) const { return (to - *this) * t + *this; }
};

int main()
{
    C3DVector a(1.0f, -3.0f, 4.0f), b(3.0f, 1.0f, 6.0f);
    C3DVector r = a.Interpolate(b, 0.5f);
    if (r.X != 2.0f || r.Y != -1.0f || r.Z != 5.0f) { std::printf("004c22c0_TEST FAIL\n"); return 1; }
    std::printf("004c22c0_TEST PASS\n");
    return 0;
}