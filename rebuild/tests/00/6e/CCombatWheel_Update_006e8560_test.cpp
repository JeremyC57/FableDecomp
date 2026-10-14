#include <cstdio>
struct CProng { bool Blocked; float x, y, z; long ProngBlockageTimer; void* occ[2]; };
struct CCombatRing { float RotationAngle, DeltaRotation; long Index; float RingThickness; CProng* first; CProng* last; CProng* e; };
struct CCombatWheel {
    CCombatRing* first; CCombatRing* last; CCombatRing* e;
    void Update()
    {
        for (CCombatRing* ring = first; ring != last; ++ring) {
            ring->RotationAngle += ring->DeltaRotation;
            for (CProng* p = ring->first; p != ring->last; ++p) if (p->ProngBlockageTimer > 0) { if (--p->ProngBlockageTimer == 0) p->Blocked = false; }
        }
    }
};
int main()
{
    CProng prongs[3]; prongs[0].Blocked = true; prongs[0].ProngBlockageTimer = 1; prongs[1].Blocked = true; prongs[1].ProngBlockageTimer = 2; prongs[2].Blocked = true; prongs[2].ProngBlockageTimer = 0;
    CCombatRing rings[2]; rings[0].RotationAngle = 1.0f; rings[0].DeltaRotation = 0.5f; rings[0].first = prongs; rings[0].last = prongs + 3; rings[1].RotationAngle = 2.0f; rings[1].DeltaRotation = -1.0f; rings[1].first = prongs; rings[1].last = prongs;
    CCombatWheel w; w.first = rings; w.last = rings + 2; w.Update();
    if (rings[0].RotationAngle != 1.5f || rings[1].RotationAngle != 1.0f || prongs[0].Blocked || prongs[0].ProngBlockageTimer != 0 || !prongs[1].Blocked || prongs[1].ProngBlockageTimer != 1 || !prongs[2].Blocked || prongs[2].ProngBlockageTimer != 0) { std::printf("006e8560_TEST FAIL\n"); return 1; }
    std::printf("006e8560_TEST PASS\n");
    return 0;
}