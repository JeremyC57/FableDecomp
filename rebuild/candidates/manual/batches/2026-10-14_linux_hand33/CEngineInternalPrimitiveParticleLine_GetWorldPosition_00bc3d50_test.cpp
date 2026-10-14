#include <cstdio>
struct C3DVector { float X, Y, Z; C3DVector() { } C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { } };
struct CLine { unsigned char _pad[0x48]; float sx, sy, sz, ex, ey, ez;
    bool GetWorldPosition(C3DVector& p) { C3DVector sum(ex + sx, ey + sy, ez + sz); C3DVector r(sum.X * 0.5f, sum.Y * 0.5f, sum.Z * 0.5f); p = r; return true; } };
int main()
{
    CLine l; l.sx = 1; l.sy = 2; l.sz = 3; l.ex = 5; l.ey = 10; l.ez = -3; C3DVector m;
    if (!l.GetWorldPosition(m) || m.X != 3.0f || m.Y != 6.0f || m.Z != 0.0f) { std::printf("00bc3d50_TEST FAIL\n"); return 1; }
    std::printf("00bc3d50_TEST PASS\n"); return 0;
}
