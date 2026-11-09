// Behaviour test for 0x008E8FF0 NCreatureMode::GetStrafingAnimationName(const CThing&, EMovementType) and the TU's
// (C3DVector, C3DVector, EMovementType) overload 0x008E8E50 it calls (C++03, self-contained model).
#include <cstdio>
#include <cstring>
#include <cmath>

struct V3 { float X, Y, Z; V3() { } V3(float x, float y, float z) : X(x), Y(y), Z(z) { } };
static const float DELTA_NOTIONAL_ZERO = 1e-4f;

// 0x008E8E50
static const char* StrafeName(const V3& movement, const V3& facing)
{
    float speed = (float)std::sqrt(movement.X * movement.X + movement.Z * movement.Z + movement.Y * movement.Y);
    V3 direction(movement.X, movement.Y, 0.0f);
    if (speed > DELTA_NOTIONAL_ZERO) { float r = 1.0f / speed; direction.X *= r; direction.Y *= r; direction.Z *= r; }
    V3 flat(facing.X, facing.Y, 0.0f);
    if (flat.X * flat.X + flat.Y * flat.Y > DELTA_NOTIONAL_ZERO * DELTA_NOTIONAL_ZERO) {
        float len = (float)std::sqrt(flat.X * flat.X + flat.Y * flat.Y); flat.X /= len; flat.Y /= len;   // AccurateNormalise
    }
    float side = direction.X * flat.Y - direction.Y * flat.X;
    float forward = direction.X * flat.X + direction.Y * flat.Y + direction.Z * flat.Z;
    const char* name = "STRAFE_IDLE";
    if (speed > 0.005f) {
        if (std::fabs(forward) > 1.0f / std::sqrt(2.0))
            name = forward < 0.0f ? "STRAFE_MOVEMENT_BACK" : "STRAFE_MOVEMENT_FORWARD";
        else
            name = side < 0.0f ? "STRAFE_MOVEMENT_LEFT" : "STRAFE_MOVEMENT_RIGHT";
    }
    return name;
}
struct Physics { V3 Position, OldPosition, Facing; V3 GetFacingVector() const { return Facing; } };
struct Thing { unsigned char ThingType; long PlayerMode; Physics* PhysicsTC; };
// 0x008E8FF0
static const char* StrafeNameForThing(const Thing& thing)
{
    if (thing.ThingType == 2 && thing.PlayerMode == 0x19)
        return "STRAFE_IDLE";
    const Physics* physics = thing.PhysicsTC;
    V3 movement;
    movement.X = physics->Position.X - physics->OldPosition.X;
    movement.Y = physics->Position.Y - physics->OldPosition.Y;
    movement.Z = physics->Position.Z - physics->OldPosition.Z;
    V3 facing = physics->GetFacingVector();
    return StrafeName(movement, facing);
}
static bool Is(const char* a, const char* b) { return std::strcmp(a, b) == 0; }
int main()
{
    Physics p; p.OldPosition = V3(10, 10, 0); p.Facing = V3(0, 2, 0);
    Thing t; t.ThingType = 1; t.PlayerMode = 0; t.PhysicsTC = &p;
    bool ok = true;
    p.Position = V3(10, 11, 0); ok = ok && Is(StrafeNameForThing(t), "STRAFE_MOVEMENT_FORWARD");
    p.Position = V3(10, 9, 0);  ok = ok && Is(StrafeNameForThing(t), "STRAFE_MOVEMENT_BACK");
    p.Position = V3(11, 10, 0); ok = ok && Is(StrafeNameForThing(t), "STRAFE_MOVEMENT_RIGHT");   // side = 1*1 - 0 > 0
    p.Position = V3(9, 10, 0);  ok = ok && Is(StrafeNameForThing(t), "STRAFE_MOVEMENT_LEFT");
    p.Position = V3(10.001f, 10, 0); ok = ok && Is(StrafeNameForThing(t), "STRAFE_IDLE");    // below 0.005 units
    p.Position = V3(10, 10, 3);  ok = ok && Is(StrafeNameForThing(t), "STRAFE_MOVEMENT_RIGHT");  // vertical only: flat 0 -> side 0 -> RIGHT
    t.ThingType = 2; t.PlayerMode = 0x19; p.Position = V3(10, 11, 0); ok = ok && Is(StrafeNameForThing(t), "STRAFE_IDLE");
    t.PlayerMode = 3; ok = ok && Is(StrafeNameForThing(t), "STRAFE_MOVEMENT_FORWARD");
    if (!ok) { std::printf("008e8ff0_TEST FAIL\n"); return 1; }
    std::printf("008e8ff0_TEST PASS\n");
    return 0;
}