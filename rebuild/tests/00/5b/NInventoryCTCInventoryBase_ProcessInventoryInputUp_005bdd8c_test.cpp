// Behaviour test for 0x005BDD8C NInventory::CTCInventoryBase::ProcessInventoryInputUp: the method comes from the source object.
#include <cstdio>
#include <cstring>
#include <stddef.h>
static char g_seq[32] = "";
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
    virtual void OnSelectedCategoryChanged() { std::strcat(g_seq, "c"); } // slot 28 (+0x70)
    virtual void OnHighlightedItemChanged() { std::strcat(g_seq, "h"); } // slot 29 (+0x74)
    virtual void _v30() {}
    virtual void _v31() {}
    virtual void _v32() {}
    virtual void _v33() {}
    virtual void _v34() {}
    virtual void _v35() {}
    virtual void _v36() {}
    virtual void _v37() {}
    virtual void ProcessInventoryInputUp();                // slot 38 (+0x98)
    virtual void ProcessInventoryInputDown() {}            // slot 39 (+0x9c)
    unsigned char _pad_0x04[0x18];
    long          InventorySelectionState;       // +0x1C
    void SelectPreviousCategory();
    void HighlightPreviousItem();
};

void CTCInventoryBase::SelectPreviousCategory() { std::strcat(g_seq, "S"); }
void CTCInventoryBase::HighlightPreviousItem() { std::strcat(g_seq, "H"); }

} // namespace NInventory
int main()
{
    NInventory::CTCInventoryBase inv;
    inv.InventorySelectionState = 0; inv.ProcessInventoryInputUp();
    inv.InventorySelectionState = 1; inv.ProcessInventoryInputUp();
    inv.InventorySelectionState = 2; inv.ProcessInventoryInputUp();
    if (std::strcmp(g_seq, "ScHh") || offsetof(NInventory::CTCInventoryBase, InventorySelectionState) != 0x1c) { std::printf("005bdd8c_TEST FAIL\n"); return 1; }
    std::printf("005bdd8c_TEST PASS\n");
    return 0;
}
