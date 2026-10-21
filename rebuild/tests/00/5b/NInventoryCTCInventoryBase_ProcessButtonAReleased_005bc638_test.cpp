// behaviour test for 0x005BC638 NInventory::CTCInventoryBase::ProcessButtonAReleased
#include <stdio.h>
namespace NInventory {
class CTCInventoryBase {
public:
    enum EInventorySelectionState { SELECTION_STATE_0 = 0, SELECTION_STATE_1 = 1 };
    CTCInventoryBase() : Calls(0), LastArg(-1) {}
    virtual ~CTCInventoryBase() {}
    virtual void _v01() {}
    virtual void _v02() {}
    virtual void _v03() {}
    virtual void _v04() {}
    virtual void _v05() {}
    virtual void _v06() {}
    virtual void _v07() {}
    virtual void _v08() {}
    virtual void _v09() {}
    virtual void _v10() {}
    virtual void _v11() {}
    virtual void _v12() {}
    virtual void _v13() {}
    virtual void _v14() {}
    virtual void _v15() {}
    virtual void _v16() {}
    virtual void _v17() {}
    virtual void _v18() {}
    virtual void _v19() {}
    virtual void _v20() {}
    virtual void _v21() {}
    virtual void _v22() {}
    virtual void _v23() {}
    virtual void _v24() {}
    virtual void _v25() {}
    virtual void _v26() {}
    virtual void _v27() {}
    virtual void _v28() {}
    virtual void _v29() {}
    virtual void _v30() {}
    virtual void _v31() {}
    virtual void _v32() {}
    virtual void _v33() {}
    virtual void _v34() {}
    virtual void _v35() {}
    virtual void _v36() {}
    virtual void _v37() {}
    virtual void _v38() {}
    virtual void _v39() {}
    virtual void _v40() {}
    virtual void _v41() {}
    virtual void _v42() {}
    virtual void _v43() {}
    virtual void _v44() {}
    virtual void _v45() {}
    virtual void SetInventorySelectionState(EInventorySelectionState state) { ++Calls; LastArg = state; }
    virtual void ProcessButtonAReleased();
    int Calls; int LastArg;
    EInventorySelectionState InventorySelectionState;
};
void CTCInventoryBase::ProcessButtonAReleased()
{
    switch (InventorySelectionState) {
    case SELECTION_STATE_1:
        SetInventorySelectionState(SELECTION_STATE_0);
        break;
    }
}
}
using namespace NInventory;
int main()
{
    CTCInventoryBase a; int ok = 1;
    a.InventorySelectionState = CTCInventoryBase::SELECTION_STATE_1; a.ProcessButtonAReleased();
    ok &= a.Calls == 1 && a.LastArg == 0;
    a.InventorySelectionState = CTCInventoryBase::SELECTION_STATE_0; a.ProcessButtonAReleased();
    ok &= a.Calls == 1;
    a.InventorySelectionState = (CTCInventoryBase::EInventorySelectionState)2; a.ProcessButtonAReleased();
    ok &= a.Calls == 1;
    printf(ok ? "005bc638_TEST PASS\n" : "005bc638_TEST FAIL\n");
    return ok ? 0 : 1;
}