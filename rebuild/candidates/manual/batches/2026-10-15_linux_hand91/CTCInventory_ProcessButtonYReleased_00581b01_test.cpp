#include <cstdio>
// Behaviour test for CTCInventory::ProcessButtonYReleased (retail 0x00581B01).
static int g_base = 0, g_assignCalls = 0;
static bool g_assignArg = false;

struct CTCInventoryBase {
    long InventorySelectionState;
    void ProcessButtonYReleased() { ++g_base; }
};

struct CTCInventory : CTCInventoryBase {
    bool EnableQuickAccessAssign;
    bool EnableQuickAccessRemove;
    void SetQuickAccessAssign(bool assign) { ++g_assignCalls; g_assignArg = assign; }
    void ProcessButtonYReleased();
};

void CTCInventory::ProcessButtonYReleased()
{
    if (InventorySelectionState == 1) {
        if (EnableQuickAccessAssign)
            SetQuickAccessAssign(false);
        else if (!EnableQuickAccessRemove)
            SetQuickAccessAssign(true);
    }
    CTCInventoryBase::ProcessButtonYReleased();
}

static int Fail() { std::printf("00581b01_TEST FAIL\n"); return 1; }

int main()
{
    CTCInventory inv;
    inv.InventorySelectionState = 1; inv.EnableQuickAccessAssign = true; inv.EnableQuickAccessRemove = false;
    inv.ProcessButtonYReleased();
    if (g_assignCalls != 1 || g_assignArg != false || g_base != 1) return Fail();
    inv.EnableQuickAccessAssign = false;
    inv.ProcessButtonYReleased();
    if (g_assignCalls != 2 || g_assignArg != true || g_base != 2) return Fail();
    inv.EnableQuickAccessRemove = true;
    inv.ProcessButtonYReleased();
    if (g_assignCalls != 2 || g_base != 3) return Fail();
    inv.InventorySelectionState = 2; inv.EnableQuickAccessAssign = true;
    inv.ProcessButtonYReleased();
    if (g_assignCalls != 2 || g_base != 4) return Fail();
    std::printf("00581b01_TEST PASS\n");
    return 0;
}
