#include <cstdio>
struct CCombatRing { float RotationAngle, DeltaRotation; long Index; float RingThickness; void* p[3]; };
struct CDef { unsigned char _pad[0x48]; long DefenderWheelOuterRingStartIndex; };
struct CCombatWheel {
    CCombatRing* first; CCombatRing* last; CCombatRing* e; unsigned long size() const { return (unsigned long)(last - first); }
    bool IsCombatRingOuterRing(const CDef* def, unsigned long index) { if (index < size()) return first[index].Index >= def->DefenderWheelOuterRingStartIndex ? true : false; return false; }
};
int main()
{
    CCombatRing rings[3]; rings[0].Index = 0; rings[1].Index = 1; rings[2].Index = 2; CDef d; d.DefenderWheelOuterRingStartIndex = 1;
    CCombatWheel w; w.first = rings; w.last = rings + 3;
    if (w.IsCombatRingOuterRing(&d, 0) || !w.IsCombatRingOuterRing(&d, 1) || !w.IsCombatRingOuterRing(&d, 2) || w.IsCombatRingOuterRing(&d, 3) || w.IsCombatRingOuterRing(&d, (unsigned long)-1)) { std::printf("006e84a0_TEST FAIL\n"); return 1; }
    std::printf("006e84a0_TEST PASS\n");
    return 0;
}