// Behaviour test for 0x00661CD0 CThingCreatureBase::GetCurrentSpeed (model copy)
#include <cstdio>
#include <cmath>
struct C3DVector { float X, Y, Z; };
struct Physics { C3DVector Position; C3DVector OldPosition; };
struct Creature { Physics* PhysicsTC; float GetCurrentSpeed() const {
    float dx = PhysicsTC->Position.X - PhysicsTC->OldPosition.X, dy = PhysicsTC->Position.Y - PhysicsTC->OldPosition.Y, dz = PhysicsTC->Position.Z - PhysicsTC->OldPosition.Z;
    return (float)std::sqrt(dx * dx + dy * dy + dz * dz); } };
int main()
{
    Physics p; p.Position.X = 3; p.Position.Y = 4; p.Position.Z = 12; p.OldPosition.X = 0; p.OldPosition.Y = 0; p.OldPosition.Z = 0;
    Creature c; c.PhysicsTC = &p;
    if (std::fabs(c.GetCurrentSpeed() - 13.0f) > 1e-5f) { std::printf("00661cd0_TEST FAIL\n"); return 1; }
    p.OldPosition = p.Position;
    if (c.GetCurrentSpeed() != 0.0f) { std::printf("00661cd0_TEST FAIL\n"); return 1; }
    std::printf("00661cd0_TEST PASS\n");
    return 0;
}