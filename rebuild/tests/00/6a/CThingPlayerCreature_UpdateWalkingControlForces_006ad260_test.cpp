// Test for CThingPlayerCreature::UpdateWalkingControlForces (0x006AD260): each component's summed vector is divided by its
// count and accumulated, the total becomes RelativeMovementAcceleration, the component map is emptied, and the movement band
// is resolved from the total's magnitude before the remaining resolve steps run in order.
#include <cstdio>
#include <cmath>
struct Vec { float X, Y, Z; };
struct Component { Vec Sum; long Count; };
struct Player {
    Component Components[4]; int ComponentCount; Vec RelativeMovementAcceleration; int MovementBandType; int Steps[3]; int StepCount;
    int BandFromMagnitude(float m) const { return m < 0.5f ? 0 : (m < 1.5f ? 1 : 2); }
    void UpdateWalkingControlForces()
    {
        Vec a = {0.0f, 0.0f, 0.0f};
        for (int i = 0; i < ComponentCount; ++i) {
            float s = 1.0f / (float)Components[i].Count;
            a.X += Components[i].Sum.X * s; a.Y += Components[i].Sum.Y * s; a.Z += Components[i].Sum.Z * s;
        }
        RelativeMovementAcceleration = a;
        ComponentCount = 0;
        float x = a.X, y = a.Y, z = a.Z;
        MovementBandType = BandFromMagnitude((float)std::sqrt(x * x + y * y + z * z));
        Steps[StepCount++] = 1; Steps[StepCount++] = 2; Steps[StepCount++] = 3;
    }
};
int main()
{
    Player p; p.StepCount = 0; p.ComponentCount = 2;
    p.Components[0].Sum.X = 2.0f; p.Components[0].Sum.Y = 0.0f; p.Components[0].Sum.Z = 0.0f; p.Components[0].Count = 2;
    p.Components[1].Sum.X = 0.0f; p.Components[1].Sum.Y = 3.0f; p.Components[1].Sum.Z = 0.0f; p.Components[1].Count = 3;
    p.UpdateWalkingControlForces();
    if (p.RelativeMovementAcceleration.X != 1.0f || p.RelativeMovementAcceleration.Y != 1.0f || p.RelativeMovementAcceleration.Z != 0.0f ||
        p.ComponentCount != 0 || p.MovementBandType != 1 || p.StepCount != 3 || p.Steps[2] != 3) { std::printf("006ad260_TEST FAIL\n"); return 1; }
    p.StepCount = 0; p.UpdateWalkingControlForces();
    if (p.RelativeMovementAcceleration.X != 0.0f || p.MovementBandType != 0) { std::printf("006ad260_TEST FAIL\n"); return 1; }
    std::printf("006ad260_TEST PASS\n");
    return 0;
}