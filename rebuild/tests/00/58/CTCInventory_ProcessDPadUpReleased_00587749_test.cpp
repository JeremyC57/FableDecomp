#include <cstdio>
// Behaviour test for CTCInventory::ProcessDPadUpReleased (0x00587749): same logic over a copy of the model.
static int g_assignSlot = -99, g_assignFlag = -99, g_baseCalls = 0;
struct CLongVector {
    long* first; long* last; long* end_of_storage;
    long& operator[](unsigned long n) { return first[n]; }
};
class CTCInventory { public:
    CLongVector QuickAccessItems;
    bool EnableQuickAccessAssign;
    bool EnableQuickAccessRemove;
    void SetHighlightedItemAsQuickAccessItem(long slot) { g_assignSlot = (int)slot; }
    void SetQuickAccessAssign(bool enable) { g_assignFlag = enable ? 1 : 0; EnableQuickAccessAssign = enable; }
    void BaseProcessDPadUpReleased() { ++g_baseCalls; }
    void ProcessDPadUpReleased()
    {
        if (EnableQuickAccessAssign) {
            SetHighlightedItemAsQuickAccessItem(0);
            SetQuickAccessAssign(false);
        } else if (EnableQuickAccessRemove) {
            QuickAccessItems[0] = 0;
            EnableQuickAccessRemove = false;
        } else {
            BaseProcessDPadUpReleased();
        }
    }
};
int main()
{
    long slots[3] = { 7, 8, 9 };
    CTCInventory inv;
    inv.QuickAccessItems.first = slots; inv.QuickAccessItems.last = slots + 3; inv.QuickAccessItems.end_of_storage = slots + 3;
    inv.EnableQuickAccessAssign = true; inv.EnableQuickAccessRemove = true;
    inv.ProcessDPadUpReleased();
    if (g_assignSlot != 0 || g_assignFlag != 0 || inv.EnableQuickAccessAssign || slots[0] != 7 || g_baseCalls != 0) { std::printf("00587749_TEST FAIL\n"); return 1; }
    inv.ProcessDPadUpReleased();
    if (slots[0] != 0 || inv.EnableQuickAccessRemove || g_baseCalls != 0) { std::printf("00587749_TEST FAIL\n"); return 1; }
    inv.ProcessDPadUpReleased();
    if (g_baseCalls != 1) { std::printf("00587749_TEST FAIL\n"); return 1; }
    std::printf("00587749_TEST PASS\n");
    return 0;
}