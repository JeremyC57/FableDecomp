#include <cstdio>
// Behaviour test for CTCInventory::ProcessButtonAReleased (retail 0x00581A6D).
static int g_selects = 0, g_closes = 0, g_base = 0;
static bool g_selectResult = false;

struct CTCInventoryBase {
    long InventorySelectionState;
    virtual void AddCloseInventoryEvent() { ++g_closes; }
    void SelectHighlightedItem(bool& closeInventory) { ++g_selects; if (g_selectResult) closeInventory = true; }
    void ProcessButtonAReleased() { ++g_base; }
    virtual ~CTCInventoryBase() { }
};

class CTCInventory : public CTCInventoryBase { public:
    bool EnableQuickAccessAssign;
    bool EnableQuickAccessRemove;
    void ProcessButtonAReleased();
};

void CTCInventory::ProcessButtonAReleased()
{
    if (!EnableQuickAccessAssign && !EnableQuickAccessRemove && InventorySelectionState == 1) {
        bool closeInventory = false;
        SelectHighlightedItem(closeInventory);
        if (closeInventory)
            AddCloseInventoryEvent();
    }
    CTCInventoryBase::ProcessButtonAReleased();
}

static int Fail() { std::printf("00581a6d_TEST FAIL\n"); return 1; }

int main()
{
    CTCInventory inv;
    inv.InventorySelectionState = 1; inv.EnableQuickAccessAssign = false; inv.EnableQuickAccessRemove = false;
    g_selectResult = false; inv.ProcessButtonAReleased();
    if (g_selects != 1 || g_closes != 0 || g_base != 1) return Fail();
    g_selectResult = true; inv.ProcessButtonAReleased();
    if (g_selects != 2 || g_closes != 1 || g_base != 2) return Fail();
    inv.EnableQuickAccessAssign = true; inv.ProcessButtonAReleased();
    if (g_selects != 2 || g_closes != 1 || g_base != 3) return Fail();
    inv.EnableQuickAccessAssign = false; inv.EnableQuickAccessRemove = true; inv.ProcessButtonAReleased();
    if (g_selects != 2 || g_base != 4) return Fail();
    inv.EnableQuickAccessRemove = false; inv.InventorySelectionState = 0; inv.ProcessButtonAReleased();
    if (g_selects != 2 || g_base != 5) return Fail();
    std::printf("00581a6d_TEST PASS\n");
    return 0;
}