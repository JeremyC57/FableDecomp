#include <cstdio>
struct C3DVector { float X, Y, Z; };
static int g_updated = 0;
struct CThing { void SetAsFrameUpdated() { ++g_updated; } };
struct CTCPhysicsBase {
    CThing* sub;
    C3DVector Position;
    C3DVector OldPosition;
    bool Moving : 1;
    bool OldMoving : 1;
    int setCalls;
    virtual ~CTCPhysicsBase() { }
    virtual void SetPosition(const C3DVector& pos) { Position = pos; ++setCalls; }
    void ResetPosition(const C3DVector& pos)
    {
        SetPosition(pos);
        OldPosition = pos;
        sub->SetAsFrameUpdated();
        Moving = true;
    }
};
int main()
{
    CThing t; CTCPhysicsBase b; b.sub = &t; b.setCalls = 0; b.Moving = false; b.OldMoving = false;
    b.OldPosition.X = b.OldPosition.Y = b.OldPosition.Z = 0.0f;
    C3DVector p; p.X = 7.0f; p.Y = 8.0f; p.Z = 9.0f;
    b.ResetPosition(p);
    if (b.setCalls != 1 || b.Position.Y != 8.0f || b.OldPosition.X != 7.0f || b.OldPosition.Z != 9.0f || !b.Moving || b.OldMoving || g_updated != 1) { std::printf("006b01d0_TEST FAIL\n"); return 1; }
    std::printf("006b01d0_TEST PASS\n");
    return 0;
}