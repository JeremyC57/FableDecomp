// Behaviour test for 0x007236E0 CTCPhysicsStandard::ResetInterpolation (slot 80): OldRHSet (+0x68) = RHSet (+0x50), RHSetLastFrameSet
// (+0x80) = the world frame, then the base CTCPhysicsBase::ResetInterpolation runs (OldPosition = Position).
#include <cstdio>
#include <cstddef>
struct C3DVector { float X, Y, Z; };
struct CRightHandedSet { C3DVector Up, Forward; };
class CWorld { public: static long GetFrame(); };
static long g_frame = 0x4321;
static int g_getWorldCalls = 0;
static int g_baseCalls = 0;
long CWorld::GetFrame() { return g_frame; }
static CWorld g_world;
class CTCPhysicsBase {
public:
    char      base[0xc];                                   // CTCBase
    C3DVector Position;                                    // +0x0c
    C3DVector OldPosition;                                 // +0x18
    char      rest[0x40 - 0x24];
    CWorld& GetWorld() const { ++g_getWorldCalls; return g_world; }
    void ResetInterpolation() { ++g_baseCalls; OldPosition = Position; }
};
class CTCPhysicsStandard : public CTCPhysicsBase {
public:
    int             PPhysicsMeshInfo;                      // +0x40
    C3DVector       OffsetVector3D;                        // +0x44
    CRightHandedSet RHSet;                                 // +0x50
    CRightHandedSet OldRHSet;                              // +0x68
    int             RHSetLastFrameSet;                     // +0x80
    void ResetInterpolation()
    {
        OldRHSet = RHSet;
        RHSetLastFrameSet = GetWorld().GetFrame();
        CTCPhysicsBase::ResetInterpolation();
    }
};
int main()
{
    CTCPhysicsStandard p;
    C3DVector up = { 1.0f, 2.0f, 3.0f }, fwd = { 4.0f, 5.0f, 6.0f }, pos = { 7.0f, 8.0f, 9.0f }, zero = { 0.0f, 0.0f, 0.0f };
    p.RHSet.Up = up; p.RHSet.Forward = fwd; p.OldRHSet.Up = zero; p.OldRHSet.Forward = zero;
    p.Position = pos; p.OldPosition = zero; p.RHSetLastFrameSet = 0;
    p.ResetInterpolation();
    bool ok = p.OldRHSet.Up.Y == 2.0f && p.OldRHSet.Forward.Z == 6.0f && p.RHSetLastFrameSet == 0x4321 && p.OldPosition.X == 7.0f
              && g_getWorldCalls == 1 && g_baseCalls == 1
              && offsetof(CTCPhysicsStandard, RHSet) == 0x50 && offsetof(CTCPhysicsStandard, OldRHSet) == 0x68
              && offsetof(CTCPhysicsStandard, RHSetLastFrameSet) == 0x80;
    if (ok) { std::printf("CTCPhysicsStandard_007236e0_TEST PASS\n"); return 0; }
    std::printf("CTCPhysicsStandard_007236e0_TEST FAIL\n");
    return 1;
}
