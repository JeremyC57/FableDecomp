#include <cstdio>
// Behaviour test for CTCInventory::GetQuickAccessNextFreeSlot (retail 0x00582027).
struct CLongVector {
    long* first; long* last; long* end_of_storage;
    unsigned long size() const { return (unsigned long)(last - first); }
    long& operator[](unsigned long n) { return first[n]; }
};
struct CTCInventory {
    CLongVector QuickAccessItems;
    long GetQuickAccessNextFreeSlot(long item);
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
static int Fail() { std::printf("00582027_TEST FAIL\n"); return 1; }
int main()
{
    long slots[4] = { 5, -2, 0, 9 };
    CTCInventory inv;
    inv.QuickAccessItems.first = slots; inv.QuickAccessItems.last = slots + 4; inv.QuickAccessItems.end_of_storage = slots + 4;
    if (inv.GetQuickAccessNextFreeSlot(7) != 2) return Fail();     // first empty slot
    if (inv.GetQuickAccessNextFreeSlot(5) != -1) return Fail();    // already assigned before the free slot
    if (inv.GetQuickAccessNextFreeSlot(-2) != 2) return Fail();    // negative entries never count as a match
    if (inv.GetQuickAccessNextFreeSlot(9) != 2) return Fail();     // match after the free slot is not reached
    slots[2] = 4;
    if (inv.GetQuickAccessNextFreeSlot(1) != -1) return Fail();    // no free slot
    inv.QuickAccessItems.last = slots;
    if (inv.GetQuickAccessNextFreeSlot(1) != -1) return Fail();
    std::printf("00582027_TEST PASS\n");
    return 0;
}
