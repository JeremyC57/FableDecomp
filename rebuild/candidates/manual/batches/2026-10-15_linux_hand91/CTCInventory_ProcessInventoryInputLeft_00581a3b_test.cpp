#include <cstdio>
// Behaviour test for CTCInventory::ProcessInventoryInputLeft (retail 0x00581A3B): both quick-access modes end, then the
// base handler runs.
static int g_baseCalls = 0;
static int g_assignCalls = 0;
static bool g_assignArg = true;

struct CTCInventoryBase {
    void ProcessInventoryInputLeft() { ++g_baseCalls; }
};

struct CTCInventory : CTCInventoryBase {
    bool EnableQuickAccessAssign;
    bool EnableQuickAccessRemove;
    void SetQuickAccessAssign(bool assign) { ++g_assignCalls; g_assignArg = assign; EnableQuickAccessAssign = assign; }
    void SetQuickAccessRemove(bool remove) { EnableQuickAccessRemove = remove; }
    void ProcessInventoryInputLeft();
};

void CTCInventory::ProcessInventoryInputLeft()
{
    SetQuickAccessAssign(false);
    SetQuickAccessRemove(false);
    CTCInventoryBase::ProcessInventoryInputLeft();
}

int main()
{
    CTCInventory inv;
    inv.EnableQuickAccessAssign = true; inv.EnableQuickAccessRemove = true;
    inv.ProcessInventoryInputLeft();
    if (inv.EnableQuickAccessAssign || inv.EnableQuickAccessRemove || g_baseCalls != 1 || g_assignCalls != 1 || g_assignArg) { std::printf("00581a3b_TEST FAIL\n"); return 1; }
    std::printf("00581a3b_TEST PASS\n");
    return 0;
}
