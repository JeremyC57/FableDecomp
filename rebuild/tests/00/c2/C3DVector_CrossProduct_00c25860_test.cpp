// 00c25860 behaviour test: C3DVector::CrossProduct (match.cpp body). C++03, g++ -w; also VC7.1 /Od.
#include <stdio.h>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { printf("FAIL line %d: %s\n", __LINE__, #c); g_fail = 1; } } while (0)
struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector CrossProduct(const C3DVector& other) const;
};
C3DVector C3DVector::CrossProduct(const C3DVector& other) const
{
    const C3DVector& o = other;
    return C3DVector(Y * o.Z - Z * other.Y,
                     Z * other.X - o.Z * X,
                     X * other.Y - Y * other.X);
}
int main()
{
    C3DVector x(1.0f, 0.0f, 0.0f), y(0.0f, 1.0f, 0.0f);
    C3DVector z = x.CrossProduct(y);
    CHECK(z.X == 0.0f && z.Y == 0.0f && z.Z == 1.0f);
    C3DVector w = y.CrossProduct(x);
    CHECK(w.X == 0.0f && w.Y == 0.0f && w.Z == -1.0f);
    C3DVector a(2.0f, 3.0f, 4.0f), b(5.0f, 6.0f, 7.0f);
    C3DVector c = a.CrossProduct(b);          // (3*7-4*6, 4*5-2*7, 2*6-3*5) = (-3, 6, -3)
    CHECK(c.X == -3.0f && c.Y == 6.0f && c.Z == -3.0f);
    C3DVector s = a.CrossProduct(a);
    CHECK(s.X == 0.0f && s.Y == 0.0f && s.Z == 0.0f);
    C3DVector h(0.5f, -1.5f, 2.0f), k(4.0f, 0.25f, -1.0f);
    C3DVector r = h.CrossProduct(k);          // (1.5-0.5, 8+0.5, 0.125+6) = (1, 8.5, 6.125)
    CHECK(r.X == 1.0f && r.Y == 8.5f && r.Z == 6.125f);
    if (!g_fail) printf("00c25860_TEST PASS\n");
    return g_fail;
}