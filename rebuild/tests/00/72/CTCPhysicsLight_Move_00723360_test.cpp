#include <cstdio>
class C3DVector { public:
    float X, Y, Z;
    C3DVector& operator+=(const C3DVector& v) { X += v.X; Y += v.Y; Z += v.Z; return *this; }
};
static float g_fGravityStep = 0.5f;
struct CTCPhysicsLight {
    C3DVector Position;
    C3DVector Velocity;
    bool gravity;
    C3DVector lastSet;
    int setCalls;
    virtual ~CTCPhysicsLight() { }
    virtual bool IsAffectedByGravity() { return gravity; }
    virtual void SetPosition(const C3DVector& pos) { ++setCalls; lastSet = pos; }
    void Move()
    {
        Position += Velocity;
        if (IsAffectedByGravity())
            Velocity.Z -= g_fGravityStep;
        SetPosition(Position);
    }
};
int main()
{
    CTCPhysicsLight l; l.setCalls = 0; l.gravity = true;
    l.Position.X = 0.0f; l.Position.Y = 0.0f; l.Position.Z = 10.0f;
    l.Velocity.X = 1.0f; l.Velocity.Y = 2.0f; l.Velocity.Z = 0.0f;
    l.Move();
    if (l.Position.X != 1.0f || l.Position.Y != 2.0f || l.Position.Z != 10.0f || l.Velocity.Z != -0.5f || l.setCalls != 1 || l.lastSet.X != 1.0f) { std::printf("00723360_TEST FAIL\n"); return 1; }
    l.gravity = false; l.Move();
    if (l.Position.Z != 9.5f || l.Velocity.Z != -0.5f || l.setCalls != 2) { std::printf("00723360_TEST FAIL\n"); return 1; }
    std::printf("00723360_TEST PASS\n");
    return 0;
}