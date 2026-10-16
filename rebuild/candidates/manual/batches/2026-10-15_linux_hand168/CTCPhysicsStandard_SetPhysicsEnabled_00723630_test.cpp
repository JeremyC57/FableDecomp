#include <cstdio>
// Behaviour model of CTCPhysicsStandard::SetPhysicsEnabled (0x00723630, slot 36)
struct C3DVector { float X, Y, Z; };
struct CTCPhysicsStandard {
    C3DVector Velocity; bool Moving, OldMoving, OnGround, OnMesh, PhysicsEnabled; int movingCalls; void* standingOn;
    void SetAsMoving(bool m) { Moving = m; ++movingCalls; }
    void SetThingStandingOn(void* p) { standingOn = p; }
    void SetPhysicsEnabled(bool enabled) {
        PhysicsEnabled = enabled;
        if (enabled) { Velocity.X = Velocity.Y = Velocity.Z = 0.0f; SetAsMoving(true); OnGround = false; OnMesh = false; SetThingStandingOn(0); return; }
        Moving = false; OldMoving = true;
    } };
int main()
{
    int dummy;
    CTCPhysicsStandard s = { { 1, 2, 3 }, false, false, true, true, false, 0, &dummy };
    s.SetPhysicsEnabled(true);
    if (!s.PhysicsEnabled || s.Velocity.X || s.Velocity.Y || s.Velocity.Z || !s.Moving || s.OnGround || s.OnMesh || s.standingOn || s.movingCalls != 1)
    { std::printf("00723630_TEST FAIL\n"); return 1; }
    s.SetPhysicsEnabled(false);
    if (s.PhysicsEnabled || s.Moving || !s.OldMoving || s.movingCalls != 1) { std::printf("00723630_TEST FAIL\n"); return 1; }
    std::printf("00723630_TEST PASS\n");
    return 0;
}
