#include <cstdio>
struct C3DVector { float X, Y, Z; };
struct CTCPhysicsLight {
    C3DVector Position;
    C3DVector OldPosition;
    int setCalls;
    virtual ~CTCPhysicsLight() { }
    virtual void SetPosition(const C3DVector& pos) { ++setCalls; Position.X = pos.X + 100.0f; Position.Y = pos.Y; Position.Z = pos.Z; }
    void ResetPosition(const C3DVector& pos)
    {
        SetPosition(pos);
        OldPosition = Position;
    }
};
int main()
{
    CTCPhysicsLight l; l.setCalls = 0;
    C3DVector p; p.X = 1.0f; p.Y = 2.0f; p.Z = 3.0f;
    l.ResetPosition(p);
    if (l.setCalls != 1 || l.OldPosition.X != 101.0f || l.OldPosition.Y != 2.0f || l.OldPosition.Z != 3.0f) { std::printf("00723320_TEST FAIL\n"); return 1; }
    std::printf("00723320_TEST PASS\n");
    return 0;
}