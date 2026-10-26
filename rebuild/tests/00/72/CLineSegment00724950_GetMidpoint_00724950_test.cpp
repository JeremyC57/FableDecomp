#include <cstdio>
class C3DVector { public: float X, Y, Z; C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { } };
struct CLine { float sx, sy, sz, ex, ey, ez; C3DVector GetMidpoint() const { C3DVector sum(ex + sx, ey + sy, ez + sz); return C3DVector(sum.X * 0.5f, sum.Y * 0.5f, sum.Z * 0.5f); } };
int main()
{
    CLine l = { 1, 2, 3, 5, 10, -3 }; C3DVector m = l.GetMidpoint();
    if (m.X != 3.0f || m.Y != 6.0f || m.Z != 0.0f) { std::printf("00724950_TEST FAIL\n"); return 1; }
    std::printf("00724950_TEST PASS\n"); return 0;
}