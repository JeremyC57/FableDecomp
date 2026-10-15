#include <cstdio>
struct C3DVector { float X, Y, Z; };
static long g_frame = 0;
struct CWorld { static long GetFrameNumber() { return g_frame; } };
static CWorld g_world;
struct CTCPhysicsBase {
    C3DVector Position, OldPosition;
    long OldPositionLastFrameSet;
    bool Moving : 1;
    bool OldMoving : 1;
    CWorld* GetWorld() { return &g_world; }
    void UpdateOldPosition()
    {
        long frame = GetWorld()->GetFrameNumber();
        if (OldPositionLastFrameSet != frame) {
            OldPosition = Position;
            OldPositionLastFrameSet = frame;
            OldMoving = Moving;
        }
    }
};
int main()
{
    CTCPhysicsBase b;
    b.Position.X = 1.0f; b.Position.Y = 2.0f; b.Position.Z = 3.0f;
    b.OldPosition.X = b.OldPosition.Y = b.OldPosition.Z = 0.0f;
    b.OldPositionLastFrameSet = 4; g_frame = 4; b.Moving = true; b.OldMoving = false;
    b.UpdateOldPosition();
    if (b.OldPosition.X != 0.0f || b.OldMoving) { std::printf("006af740_TEST FAIL\n"); return 1; }
    g_frame = 5;
    b.UpdateOldPosition();
    if (b.OldPosition.X != 1.0f || b.OldPosition.Z != 3.0f || !b.OldMoving || !b.Moving || b.OldPositionLastFrameSet != 5) { std::printf("006af740_TEST FAIL\n"); return 1; }
    b.Moving = false; g_frame = 6;
    b.UpdateOldPosition();
    if (b.OldMoving || b.Moving) { std::printf("006af740_TEST FAIL\n"); return 1; }
    std::printf("006af740_TEST PASS\n");
    return 0;
}