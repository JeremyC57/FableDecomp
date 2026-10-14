#include <cstdio>
struct CCombatRing { float RotationAngle, DeltaRotation; long Index; float RingThickness; void* p[3]; void Reset() { Index = -1; } };
struct CCombatWheel { CCombatRing* first; CCombatRing* last; CCombatRing* e; void ResetRings() { for (CCombatRing* r = first; r != last; ++r) r->Reset(); } };
int main()
{
    CCombatRing rings[3]; rings[0].Index = rings[1].Index = rings[2].Index = 4; CCombatWheel w; w.first = rings; w.last = rings + 2; w.ResetRings();
    if (rings[0].Index != -1 || rings[1].Index != -1 || rings[2].Index != 4) { std::printf("006e8bd0_TEST FAIL\n"); return 1; }
    std::printf("006e8bd0_TEST PASS\n");
    return 0;
}
