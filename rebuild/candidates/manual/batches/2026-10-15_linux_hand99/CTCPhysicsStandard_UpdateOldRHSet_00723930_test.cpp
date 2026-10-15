#include <cstdio>
struct C3DVector { float X, Y, Z; };
struct CRightHandedSet { C3DVector Up; C3DVector Forward; };
static long g_frame = 0;
static int g_getWorldCalls = 0;
struct CWorld { static long GetFrameNumber() { return g_frame; } };
static CWorld g_world;
struct CTCPhysicsStandard {
    CRightHandedSet RHSet, OldRHSet;
    long RHSetLastFrameSet;
    CWorld* GetWorld() { ++g_getWorldCalls; return &g_world; }
    void UpdateOldRHSet()
    {
        long frame = GetWorld()->GetFrameNumber();
        if (RHSetLastFrameSet != frame) {
            OldRHSet = RHSet;
            RHSetLastFrameSet = frame;
        }
    }
};
int main()
{
    CTCPhysicsStandard p;
    p.RHSetLastFrameSet = 7; g_frame = 7;
    p.RHSet.Up.X = 1.0f; p.RHSet.Forward.Z = 2.0f; p.OldRHSet.Up.X = 9.0f; p.OldRHSet.Forward.Z = 9.0f;
    p.UpdateOldRHSet();
    if (p.OldRHSet.Up.X != 9.0f || g_getWorldCalls != 1) { std::printf("00723930_TEST FAIL\n"); return 1; }
    g_frame = 8;
    p.UpdateOldRHSet();
    if (p.OldRHSet.Up.X != 1.0f || p.OldRHSet.Forward.Z != 2.0f || p.RHSetLastFrameSet != 8) { std::printf("00723930_TEST FAIL\n"); return 1; }
    std::printf("00723930_TEST PASS\n");
    return 0;
}
