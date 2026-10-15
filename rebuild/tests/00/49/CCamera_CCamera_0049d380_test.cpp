#include <cstdio>
struct C3DVectorBits {
    unsigned long X, Y, Z;
    C3DVectorBits() : X(0), Y(0), Z(0) { }
    C3DVectorBits(const C3DVectorBits& o) : X(o.X), Y(o.Y), Z(o.Z) { }
};
struct CRightHandedSet {
    C3DVectorBits Up, Forward;
    CRightHandedSet() { }
    CRightHandedSet(const CRightHandedSet& o) : Up(o.Up), Forward(o.Forward) { }
};
struct CCamera {
    C3DVectorBits WorldPosition; CRightHandedSet RHSet; bool HeightLocked; bool ViewVecZLocked;
    unsigned long FOVFlags, HorizontalFOV, VerticalFOV, Zoom;
    CCamera() { }
    CCamera(const CCamera& o)
        : WorldPosition(o.WorldPosition), RHSet(o.RHSet), HeightLocked(o.HeightLocked),
          ViewVecZLocked(o.ViewVecZLocked), FOVFlags(o.FOVFlags), HorizontalFOV(o.HorizontalFOV),
          VerticalFOV(o.VerticalFOV), Zoom(o.Zoom) { }
};
int main()
{
    CCamera a;
    a.WorldPosition.X = 1; a.WorldPosition.Y = 2; a.WorldPosition.Z = 3;
    a.RHSet.Up.X = 4; a.RHSet.Up.Y = 5; a.RHSet.Up.Z = 6;
    a.RHSet.Forward.X = 7; a.RHSet.Forward.Y = 8; a.RHSet.Forward.Z = 9;
    a.HeightLocked = true; a.ViewVecZLocked = false; a.FOVFlags = 3; a.HorizontalFOV = 10;
    a.VerticalFOV = 11; a.Zoom = 12;
    CCamera b(a);
    if (b.WorldPosition.Z != 3 || b.RHSet.Up.Y != 5 || b.RHSet.Forward.Z != 9 || !b.HeightLocked ||
        b.ViewVecZLocked || b.FOVFlags != 3 || b.HorizontalFOV != 10 || b.VerticalFOV != 11 || b.Zoom != 12)
    { std::printf("0049d380_TEST FAIL\n"); return 1; }
    std::printf("0049d380_TEST PASS\n");
    return 0;
}