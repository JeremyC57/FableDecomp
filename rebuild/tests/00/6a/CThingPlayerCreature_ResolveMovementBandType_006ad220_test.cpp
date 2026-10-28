// Behaviour test for 0x006AD220 CThingPlayerCreature::ResolveMovementBandType: the band of the relative movement acceleration magnitude (standalone copy of the logic)
#include <cstdio>
#include <cmath>
struct V { float X, Y, Z; float GetAccurateMagnitude() const { float x = X, y = Y, z = Z; return (float)std::sqrt(x * x + y * y + z * z); } };
long BandFromMagnitude(float m) { return m < 0.3f ? 0 : (m < 0.8f ? 1 : 2); }
struct Player { V RelativeMovementAcceleration; long MovementBandType; void Resolve() { MovementBandType = BandFromMagnitude(RelativeMovementAcceleration.GetAccurateMagnitude()); } };

int main()
{
    Player p; p.RelativeMovementAcceleration.X = 0.3f; p.RelativeMovementAcceleration.Y = 0.4f; p.RelativeMovementAcceleration.Z = 0.0f;
    if (std::fabs(p.RelativeMovementAcceleration.GetAccurateMagnitude() - 0.5f) > 1e-6f) { std::printf("006ad220_TEST FAIL\n"); return 1; }
    p.Resolve(); if (p.MovementBandType != 1) { std::printf("006ad220_TEST FAIL\n"); return 1; }
    p.RelativeMovementAcceleration.Z = 2.0f; p.Resolve(); if (p.MovementBandType != 2) { std::printf("006ad220_TEST FAIL\n"); return 1; }
    std::printf("006ad220_TEST PASS\n");
    return 0;
}