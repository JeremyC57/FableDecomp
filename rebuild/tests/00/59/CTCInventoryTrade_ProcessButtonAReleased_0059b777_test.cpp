// behaviour test for 0x0059B777
#include <stdio.h>
namespace NInventory {
class CTCInventoryBase {
public:
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
    virtual void _v46() {}
    virtual void _v47() {}
    virtual void _v48() {}
    virtual void _v49() {}
    virtual void _v50() {}
    virtual void _v51() {}
    virtual void _v52() {}
    virtual void _v53() {}
    virtual void _v54() {}
    virtual void _v55() {}
    virtual void _v56() {}
    virtual void _v57() {}
    int baseCalls, tradeCalls;
    virtual void ProcessButtonAReleased() { ++baseCalls; }
    virtual void _v59() {}
    virtual void _v60() {}
    virtual void _v61() {}
    virtual void _v62() {}
    virtual void _v63() {}
    virtual void _v64() {}
    virtual void _v65() {}
    virtual void _v66() {}
    virtual void _v67() {}
    virtual void ProcessTradeAction() { ++tradeCalls; }
    unsigned char _pad04[0x1c - 4];
    long          InventorySelectionState;               // +0x1c
};
}
class CTCInventoryTrade : public NInventory::CTCInventoryBase {
public:
    virtual void ProcessButtonAReleased();
};
void CTCInventoryTrade::ProcessButtonAReleased()
{
    if (InventorySelectionState == 1)
        ProcessTradeAction();
    NInventory::CTCInventoryBase::ProcessButtonAReleased();
}
int main()
{
    CTCInventoryTrade t; t.baseCalls = t.tradeCalls = 0; int ok = 1;
    t.InventorySelectionState = 0; t.ProcessButtonAReleased(); ok &= t.baseCalls == 1 && t.tradeCalls == 0;
    t.InventorySelectionState = 1; t.ProcessButtonAReleased(); ok &= t.baseCalls == 2 && t.tradeCalls == 1;
    printf(ok ? "0059b777_TEST PASS\n" : "0059b777_TEST FAIL\n");
    return ok ? 0 : 1;
}