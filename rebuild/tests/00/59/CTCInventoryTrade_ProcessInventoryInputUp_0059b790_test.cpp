// Behaviour test for 0x0059B790 CTCInventoryTrade::ProcessInventoryInputUp: the method comes from the source object.
#include <cstdio>
#include <cstring>
static char g_seq[16] = "";
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
    virtual void OnSelectedCategoryChanged() {}            // slot 28 (+0x70)
    virtual void OnHighlightedItemChanged() {}             // slot 29 (+0x74)
    virtual void _v30() {}
    virtual void _v31() {}
    virtual void _v32() {}
    virtual void _v33() {}
    virtual void _v34() {}
    virtual void _v35() {}
    virtual void _v36() {}
    virtual void _v37() {}
    virtual void ProcessInventoryInputUp() { std::strcat(g_seq, "U;"); } // slot 38 (+0x98)
    virtual void ProcessInventoryInputDown() { std::strcat(g_seq, "D;"); } // slot 39 (+0x9c)
};

} // namespace NInventory

class CTCInventoryTrade : public NInventory::CTCInventoryBase {   // RTTI .?AVCTCInventoryTrade@@, vtable 0x01253034
public:
    virtual void ProcessInventoryInputUp();           // slot 38 (0x0059B790)
    virtual void ProcessInventoryInputDown();         // slot 39 (0x0059B795)
};
void CTCInventoryTrade::ProcessInventoryInputDown() { }
int main()
{
    CTCInventoryTrade t;
    t.ProcessInventoryInputUp();
    if (std::strcmp(g_seq, "U;")) { std::printf("0059b790_TEST FAIL\n"); return 1; }
    std::printf("0059b790_TEST PASS\n");
    return 0;
}
