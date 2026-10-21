// behaviour test for 0x005D8E50 CTCInventoryItem::ReturnToInventory
#include <stdio.h>
class CThing { public: int Id; };
class CDefPointeeBase;
namespace NInventory {
class CTCInventoryBase {
public:
    CTCInventoryBase() : Returned(0) {}
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
    virtual void _v58() {}
    virtual void _v59() {}
    virtual void _v60() {}
    virtual void _v61() {}
    virtual void _v62() {}
    virtual void _v63() {}
    virtual void _v64() {}
    virtual void _v65() {}
    virtual void _v66() {}
    virtual void _v67() {}
    virtual void _v68() {}
    virtual void _v69() {}
    virtual void _v70() {}
    virtual void _v71() {}
    virtual void _v72() {}
    virtual void _v73() {}
    virtual void _v74() {}
    virtual void _v75() {}
    virtual void _v76() {}
    virtual void _v77() {}
    virtual void _v78() {}
    virtual void _v79() {}
    virtual void _v80() {}
    virtual void _v81() {}
    virtual void _v82() {}
    virtual void ReturnItemToInventory(CThing& thing) { Returned = &thing; }
    CThing* Returned;
};
}
class CTCBase {
public:
    virtual ~CTCBase() {}
    CThing* ParentThing; bool ToBeRemoved; bool IsInGlobalUpdateTCs;
};
class CTCInventoryItem : public CTCBase {
public:
    CDefPointeeBase* PDef_Object;
    NInventory::CTCInventoryBase* PInventory;
    void ReturnToInventory();
};
void CTCInventoryItem::ReturnToInventory()
{
    PInventory->ReturnItemToInventory(*ParentThing);
}
int main()
{
    CThing t; t.Id = 7; NInventory::CTCInventoryBase inv; CTCInventoryItem item;
    item.ParentThing = &t; item.PInventory = &inv; item.PDef_Object = 0;
    item.ReturnToInventory();
    int ok = inv.Returned == &t && inv.Returned->Id == 7;
    printf(ok ? "005d8e50_TEST PASS\n" : "005d8e50_TEST FAIL\n");
    return ok ? 0 : 1;
}