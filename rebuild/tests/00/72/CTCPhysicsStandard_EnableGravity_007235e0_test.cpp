#include <cstdio>
static const float g_minAirResistance = 0.0001f;
class CPhysicsDef { public: float AirResistance; };
struct CTCPhysicsStandard {
    CPhysicsDef* PhysicsDef;
    bool Moving : 1;
    bool OldMoving : 1;
    bool AlwaysFacingMovementDirection : 1;
    bool AngledWithGround : 1;
    bool FeelsGravity : 1;
    bool FeelsAirResistance : 1;
    void SetAsMoving(bool moving) { Moving = moving; }
    float GetAirResistance() { return PhysicsDef->AirResistance; }
    void vfunc_38(bool enable)
    {
        SetAsMoving(true);
        if (enable)
            FeelsGravity = GetAirResistance() > g_minAirResistance;
        else
            FeelsGravity = false;
    }
};
int main()
{
    CPhysicsDef d; d.AirResistance = 0.5f;
    CTCPhysicsStandard p; p.PhysicsDef = &d;
    p.Moving = false; p.OldMoving = true; p.AlwaysFacingMovementDirection = true; p.AngledWithGround = false;
    p.FeelsGravity = false; p.FeelsAirResistance = true;
    p.vfunc_38(true);
    if (!p.Moving || !p.FeelsGravity || !p.AlwaysFacingMovementDirection || p.AngledWithGround || !p.FeelsAirResistance) { std::printf("007235e0_TEST FAIL\n"); return 1; }
    d.AirResistance = 0.00001f; p.vfunc_38(true);
    if (p.FeelsGravity) { std::printf("007235e0_TEST FAIL\n"); return 1; }
    d.AirResistance = 1.0f; p.vfunc_38(true); p.vfunc_38(false);
    if (p.FeelsGravity || !p.FeelsAirResistance || !p.OldMoving) { std::printf("007235e0_TEST FAIL\n"); return 1; }
    std::printf("007235e0_TEST PASS\n");
    return 0;
}