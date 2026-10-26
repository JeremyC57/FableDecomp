#include <cstdio>
class C3DVector { public: float X, Y, Z; };
static long g_frame = 0;
static int g_worldCalls = 0;
class CWorld { public: static long GetFrame() { return g_frame; } };
static CWorld g_world;
class CTCPhysicsBase { public:
    C3DVector Position;
    C3DVector OldPosition;
    long OldPositionLastFrameSet;
    bool Moving : 1;
    bool OldMoving : 1;
    CWorld* GetWorld() { ++g_worldCalls; return &g_world; }
    void ResetInterpolation()
    {
        OldPosition = Position;
        OldPositionLastFrameSet = GetWorld()->GetFrame();
        OldMoving = Moving;
    }
};
int main()
{
    CTCPhysicsBase b;
    b.Position.X = 4.0f; b.Position.Y = 5.0f; b.Position.Z = 6.0f;
    b.OldPosition.X = b.OldPosition.Y = b.OldPosition.Z = 0.0f;
    b.Moving = true; b.OldMoving = false; g_frame = 1234;
    b.ResetInterpolation();
    if (b.OldPosition.X != 4.0f || b.OldPosition.Z != 6.0f || b.OldPositionLastFrameSet != 1234 || !b.OldMoving || !b.Moving || g_worldCalls != 1) { std::printf("006af790_TEST FAIL\n"); return 1; }
    b.Moving = false; b.ResetInterpolation();
    if (b.OldMoving) { std::printf("006af790_TEST FAIL\n"); return 1; }
    std::printf("006af790_TEST PASS\n");
    return 0;
}