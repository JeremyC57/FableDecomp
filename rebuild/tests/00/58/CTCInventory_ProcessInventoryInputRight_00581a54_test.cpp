#include <cstdio>
// Behaviour test for CTCInventory::ProcessInventoryInputRight (retail 0x00581A54): both quick-access modes end, then the
// base handler runs.
static int g_baseCalls = 0;
static int g_assignCalls = 0;
static bool g_assignArg = true;

struct CTCInventoryBase {
    void ProcessInventoryInputRight() { ++g_baseCalls; }
};

class CTCInventory : public CTCInventoryBase { public:
    bool EnableQuickAccessAssign;
    bool EnableQuickAccessRemove;
    void SetQuickAccessAssign(bool assign) { ++g_assignCalls; g_assignArg = assign; EnableQuickAccessAssign = assign; }
    void SetQuickAccessRemove(bool remove) { EnableQuickAccessRemove = remove; }
    void ProcessInventoryInputRight();
};

void CTCInventory::ProcessInventoryInputRight()
{
    SetQuickAccessAssign(false);
    SetQuickAccessRemove(false);
    CTCInventoryBase::ProcessInventoryInputRight();
}

int main()
{
    CTCInventory inv;
    inv.EnableQuickAccessAssign = true; inv.EnableQuickAccessRemove = true;
    inv.ProcessInventoryInputRight();
    if (inv.EnableQuickAccessAssign || inv.EnableQuickAccessRemove || g_baseCalls != 1 || g_assignCalls != 1 || g_assignArg) { std::printf("00581a54_TEST FAIL\n"); return 1; }
    std::printf("00581a54_TEST PASS\n");
    return 0;
}