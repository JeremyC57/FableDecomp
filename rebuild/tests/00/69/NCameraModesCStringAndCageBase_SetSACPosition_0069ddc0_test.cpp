#include <cstdio>
struct C3DVector { float X, Y, Z; };
struct CPhysicsState { C3DVector Position; C3DVector Velocity; };
struct CBase {
    virtual void v00() { }
    unsigned char _pad04[0x50];
    CPhysicsState GoTo;
};
struct CStringAndCageBase : CBase {
    virtual void SetSACPosition(const C3DVector& position) { GoTo.Position = position; }
};
int main()
{
    CStringAndCageBase s;
    s.GoTo.Velocity.X = 7.0f;
    C3DVector p; p.X = 1.5f; p.Y = -2.0f; p.Z = 3.25f;
    s.SetSACPosition(p);
    if (s.GoTo.Position.X != 1.5f || s.GoTo.Position.Y != -2.0f || s.GoTo.Position.Z != 3.25f || s.GoTo.Velocity.X != 7.0f)
    { std::printf("0069ddc0_TEST FAIL\n"); return 1; }
    std::printf("0069ddc0_TEST PASS\n");
    return 0;
}