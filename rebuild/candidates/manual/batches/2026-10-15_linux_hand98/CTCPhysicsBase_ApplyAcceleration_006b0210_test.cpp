#include <cstdio>
struct C3DVector {
    float X, Y, Z;
    C3DVector& operator+=(const C3DVector& v) { X += v.X; Y += v.Y; Z += v.Z; return *this; }
};
static int g_updated = 0;
struct CThing { void SetAsFrameUpdated() { ++g_updated; } };
struct CTCPhysicsBase {
    CThing* sub;
    C3DVector Velocity;
    bool Moving : 1;
    bool OldMoving : 1;
    void ApplyAcceleration(const C3DVector& acceleration)
    {
        Velocity += acceleration;
        sub->SetAsFrameUpdated();
        Moving = true;
    }
};
int main()
{
    CThing t; CTCPhysicsBase b; b.sub = &t; b.Velocity.X = 1.0f; b.Velocity.Y = 1.0f; b.Velocity.Z = 1.0f; b.Moving = false; b.OldMoving = true;
    C3DVector a; a.X = 0.5f; a.Y = -1.0f; a.Z = 2.0f;
    b.ApplyAcceleration(a);
    if (b.Velocity.X != 1.5f || b.Velocity.Y != 0.0f || b.Velocity.Z != 3.0f || !b.Moving || !b.OldMoving || g_updated != 1) { std::printf("006b0210_TEST FAIL\n"); return 1; }
    std::printf("006b0210_TEST PASS\n");
    return 0;
}
