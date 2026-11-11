// Behaviour test for CTCGatherPointBase::OnInitialActivate (0x004E0ADC): it dispatches to the leaf class's virtual Init.
#include <stdio.h>
class CTCGatherPointBase { public:
    int baseInits;
    CTCGatherPointBase() : baseInits(0) { }
    virtual ~CTCGatherPointBase() { }
    virtual void OnInitialActivate();
    virtual void Init() { ++baseInits; }
};
void CTCGatherPointBase::OnInitialActivate()
{
    Init();
}
struct CTCGatherPointBuilding : CTCGatherPointBase {
    int buildingInits;
    CTCGatherPointBuilding() : buildingInits(0) { }
    virtual void Init() { ++buildingInits; }
};
int main(void)
{
    CTCGatherPointBase b; b.OnInitialActivate();
    CTCGatherPointBuilding g; g.OnInitialActivate(); g.OnInitialActivate();
    if (b.baseInits != 1 || g.buildingInits != 2 || g.baseInits != 0) { printf("004e0adc_TEST FAIL\n"); return 1; }
    printf("004e0adc_TEST PASS\n");
    return 0;
}