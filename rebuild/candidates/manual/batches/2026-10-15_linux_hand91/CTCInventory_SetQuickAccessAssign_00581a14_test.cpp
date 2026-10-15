#include <cstdio>
// Behaviour test for CTCInventory::SetQuickAccessAssign(bool) (retail 0x00581A14).
struct CTCInventoryBase {
    bool CanItemBeQuickAssigned;
    bool GetCanItemBeQuickAssigned() { return CanItemBeQuickAssigned; }
};

struct CTCInventory : CTCInventoryBase {
    bool EnableQuickAccessAssign;
    bool EnableQuickAccessRemove;
    void SetQuickAccessAssign(bool assign);
};

void CTCInventory::SetQuickAccessAssign(bool assign)
{
    EnableQuickAccessAssign = GetCanItemBeQuickAssigned() ? assign : false;
}

int main()
{
    CTCInventory inv;
    inv.EnableQuickAccessRemove = true;
    bool table[4][3] = { { false, false, false }, { false, true, false }, { true, false, false }, { true, true, true } };
    for (int i = 0; i < 4; ++i) {
        inv.CanItemBeQuickAssigned = table[i][0];
        inv.EnableQuickAccessAssign = !table[i][2];
        inv.SetQuickAccessAssign(table[i][1]);
        if (inv.EnableQuickAccessAssign != table[i][2] || !inv.EnableQuickAccessRemove) { std::printf("00581a14_TEST FAIL\n"); return 1; }
    }
    std::printf("00581a14_TEST PASS\n");
    return 0;
}
