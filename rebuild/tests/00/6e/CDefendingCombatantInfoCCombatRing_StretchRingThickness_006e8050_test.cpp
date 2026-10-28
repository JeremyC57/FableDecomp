// Behaviour test for 0x006E8050 CCombatRing::StretchRingThickness: inner ring grows to the combat type limit, outer rings to 1.5 attacker diameters (standalone copy of the logic)
#include <cstdio>
struct Ring { long Index; float RingThickness; void Stretch(float radius, float limit) {
    if (Index != 0) { float diameter = radius + radius; if (diameter > RingThickness) RingThickness = diameter * 1.5f; }
    else { if (limit > RingThickness) RingThickness = limit; } } };

int main()
{
    Ring r; r.Index = 0; r.RingThickness = 1.0f; r.Stretch(5.0f, 2.0f); if (r.RingThickness != 2.0f) { std::printf("006e8050_TEST FAIL\n"); return 1; }
    r.Stretch(5.0f, 1.5f); if (r.RingThickness != 2.0f) { std::printf("006e8050_TEST FAIL\n"); return 1; }
    r.Index = 1; r.RingThickness = 1.0f; r.Stretch(1.0f, 9.0f); if (r.RingThickness != 3.0f) { std::printf("006e8050_TEST FAIL\n"); return 1; }
    r.Stretch(1.0f, 9.0f); if (r.RingThickness != 3.0f) { std::printf("006e8050_TEST FAIL\n"); return 1; }
    std::printf("006e8050_TEST PASS\n");
    return 0;
}