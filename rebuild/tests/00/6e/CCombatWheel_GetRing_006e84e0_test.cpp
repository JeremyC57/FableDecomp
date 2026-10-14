#include <cstdio>
struct CCombatRing { float RotationAngle, DeltaRotation; long Index; float RingThickness; void* p[3]; };
struct CCombatWheel {
    CCombatRing* first; CCombatRing* last; CCombatRing* e; unsigned long size() const { return (unsigned long)(last - first); }
    CCombatRing* GetRing(long index) { if (index >= 0 && (unsigned long)index < size()) return &first[index]; return 0; }
};
int main()
{
    CCombatRing rings[3]; CCombatWheel w; w.first = rings; w.last = rings + 3;
    if (w.GetRing(0) != &rings[0] || w.GetRing(2) != &rings[2] || w.GetRing(3) != 0 || w.GetRing(-1) != 0) { std::printf("006e84e0_TEST FAIL\n"); return 1; }
    std::printf("006e84e0_TEST PASS\n");
    return 0;
}