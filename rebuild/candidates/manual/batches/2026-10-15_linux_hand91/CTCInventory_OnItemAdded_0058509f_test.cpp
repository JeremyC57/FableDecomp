#include <cstdio>
// Behaviour test for CTCInventory::OnItemAdded (retail 0x0058509F).
struct CLongVector {
    long* first; long* last; long* end_of_storage;
    unsigned long size() const { return (unsigned long)(last - first); }
    long& operator[](unsigned long n) { return first[n]; }
};
static int g_baseCalls = 0, g_sets = 0;
static long g_setSlot = -2;
struct CTCInventoryBase {
    CLongVector QuickAccessItems;
    void OnItemAdded(long item, bool assignQuickAccess, bool silent) { (void)item; (void)assignQuickAccess; (void)silent; ++g_baseCalls; }
};
struct CTCInventory : CTCInventoryBase {
    bool ContextSensitiveUpdate;
    long GetQuickAccessNextFreeSlot(long item);
    void SetThingAsQuickAccessItem(long item, long slot, bool silent) { (void)silent; ++g_sets; g_setSlot = slot; QuickAccessItems[slot] = item; }
    void OnItemAdded(long item, bool assignQuickAccess, bool silent);
};
long CTCInventory::GetQuickAccessNextFreeSlot(long item)
{
    long result = -1;
    for (unsigned long i = 0; i < QuickAccessItems.size(); ++i) {
        if (QuickAccessItems[i] == 0) {
            result = i;
            break;
        }
        if (QuickAccessItems[i] > 0 && QuickAccessItems[i] == item)
            break;
    }
    return result;
}
void CTCInventory::OnItemAdded(long item, bool assignQuickAccess, bool silent)
{
    CTCInventoryBase::OnItemAdded(item, assignQuickAccess, silent);
    if (assignQuickAccess) {
        long slot = GetQuickAccessNextFreeSlot(item);
        if (slot != -1)
            SetThingAsQuickAccessItem(item, slot, false);
    }
    ContextSensitiveUpdate = true;
}
static int Fail() { std::printf("0058509f_TEST FAIL\n"); return 1; }
int main()
{
    long slots[3] = { 4, 0, 0 };
    CTCInventory inv;
    inv.QuickAccessItems.first = slots; inv.QuickAccessItems.last = slots + 3; inv.QuickAccessItems.end_of_storage = slots + 3;
    inv.ContextSensitiveUpdate = false;
    inv.OnItemAdded(9, false, false);                       // no quick-access assignment
    if (g_baseCalls != 1 || g_sets != 0 || !inv.ContextSensitiveUpdate) return Fail();
    inv.ContextSensitiveUpdate = false;
    inv.OnItemAdded(9, true, false);                        // goes to slot 1
    if (g_baseCalls != 2 || g_sets != 1 || g_setSlot != 1 || slots[1] != 9 || !inv.ContextSensitiveUpdate) return Fail();
    inv.OnItemAdded(4, true, false);                        // already assigned: no set
    if (g_sets != 1) return Fail();
    slots[2] = 7;
    inv.OnItemAdded(8, true, false);                        // no free slot
    if (g_sets != 1 || g_baseCalls != 4) return Fail();
    std::printf("0058509f_TEST PASS\n");
    return 0;
}
