#include <cstdio>
struct C3DVector {
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
};
struct CRightHandedSet {
    C3DVector Up, Forward;
    CRightHandedSet(const C3DVector& up, const C3DVector& forward) : Up(up), Forward(forward) { }
};
struct CCamera {
    C3DVector WorldPosition; CRightHandedSet RHSet; bool HeightLocked; bool ViewVecZLocked;
    unsigned long FOVFlags; float HorizontalFOV, VerticalFOV, Zoom;
    CCamera(const C3DVector& position, const C3DVector& forward, const C3DVector& up, float h, float v)
        : WorldPosition(position), RHSet(up, forward), HeightLocked(false), ViewVecZLocked(false),
          FOVFlags(1), HorizontalFOV(h), VerticalFOV(v)
    {
        if (v > 0.0f)
            FOVFlags = 3;
    }
};
int main()
{
    C3DVector p(1, 2, 3), f(4, 5, 6), u(7, 8, 9);
    CCamera a(p, f, u, 1.5f, 0.75f);
    if (a.WorldPosition.Y != 2 || a.RHSet.Up.X != 7 || a.RHSet.Forward.Z != 6 || a.HeightLocked ||
        a.ViewVecZLocked || a.FOVFlags != 3 || a.HorizontalFOV != 1.5f || a.VerticalFOV != 0.75f)
    { std::printf("00a0c130_TEST FAIL\n"); return 1; }
    CCamera b(p, f, u, 1.5f, 0.0f);
    if (b.FOVFlags != 1) { std::printf("00a0c130_TEST FAIL\n"); return 1; }
    CCamera c(p, f, u, 1.5f, -2.0f);
    if (c.FOVFlags != 1) { std::printf("00a0c130_TEST FAIL\n"); return 1; }
    std::printf("00a0c130_TEST PASS\n");
    return 0;
}