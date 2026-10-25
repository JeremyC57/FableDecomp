// Behaviour test for 0x005BC649 NInventory::CTCInventoryBase::ProcessInventoryInputRight: the method comes from the source object
// (declared, not defined here); slot 46 records its argument.
#include <stdio.h>
namespace NInventory {

class CTCInventoryBase {
public:
    enum EInventorySelectionState { SELECTION_STATE_0 = 0, SELECTION_STATE_1 = 1 };
    CTCInventoryBase() : Calls(0), LastArg(-1) {}
    virtual ~CTCInventoryBase() {}                         // slot 0
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
    virtual void ProcessInventoryInputLeft() {}           // slot 40 (+0xa0)
    virtual void ProcessInventoryInputRight();          // slot 41 (+0xa4)
    virtual void _v42() {}
    virtual void _v43() {}
    virtual void _v44() {}
    virtual void _v45() {}
    virtual void SetInventorySelectionState(EInventorySelectionState state) { ++Calls; LastArg = state; }   // slot 46 (+0xb8)
    int Calls; int LastArg; unsigned char _pad_0x0c[0x10];
    EInventorySelectionState InventorySelectionState;    // +0x1c
};

} // namespace NInventory
using namespace NInventory;
int main()
{
    CTCInventoryBase a; int ok = 1;
    a.InventorySelectionState = CTCInventoryBase::SELECTION_STATE_0; a.ProcessInventoryInputRight();
    ok &= a.Calls == 1 && a.LastArg == 1;
    a.InventorySelectionState = CTCInventoryBase::SELECTION_STATE_1; a.ProcessInventoryInputRight();
    ok &= a.Calls == 1;
    a.InventorySelectionState = (CTCInventoryBase::EInventorySelectionState)2; a.ProcessInventoryInputRight();
    ok &= a.Calls == 1;
    printf(ok ? "005bc649_TEST PASS\n" : "005bc649_TEST FAIL\n");
    return ok ? 0 : 1;
}
