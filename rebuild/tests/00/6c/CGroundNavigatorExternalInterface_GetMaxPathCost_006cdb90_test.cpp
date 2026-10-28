// Behaviour test for 0x006CDB90 CGroundNavigatorExternalInterface::GetMaxPathCost: the navigation def max path cost, def released (standalone copy of the logic)
#include <cstdio>
struct Def { long Refs; float MinPathCost, MaxPathCost; };
struct Nav { Def* d; void GetSubDef(Def** out) const { *out = d; if (d) ++d->Refs; }
    float GetMinPathCost() const { Def* p = 0; GetSubDef(&p); float v = p->MinPathCost; --p->Refs; return v; }
    float GetMaxPathCost() const { Def* p = 0; GetSubDef(&p); float v = p->MaxPathCost; --p->Refs; return v; } };

int main()
{
    Def d; d.Refs = 1; d.MinPathCost = 3.0f; d.MaxPathCost = 9.0f; Nav n; n.d = &d;
    if (n.GetMaxPathCost() != 9.0f || d.Refs != 1) { std::printf("006cdb90_TEST FAIL\n"); return 1; }
    std::printf("006cdb90_TEST PASS\n");
    return 0;
}