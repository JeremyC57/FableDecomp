#include <cstdio>
struct C3DVector { float X, Y, Z; C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { } C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { } };
struct C3DHelperPoint { C3DVector Position; long BoneIndex; C3DHelperPoint(const C3DVector& p, long b) : Position(p), BoneIndex(b) { } };
int main()
{
    C3DVector v(1.5f, -2.5f, 3.5f); C3DHelperPoint h(v, 11);
    if (sizeof(h) != 16 || h.Position.X != 1.5f || h.Position.Y != -2.5f || h.Position.Z != 3.5f || h.BoneIndex != 11) { std::printf("00a2e470_TEST FAIL\n"); return 1; }
    std::printf("00a2e470_TEST PASS\n"); return 0;
}
