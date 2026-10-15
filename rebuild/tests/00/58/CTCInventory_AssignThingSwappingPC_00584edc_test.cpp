#include <cstdio>
// Behaviour test for CTCInventory::AssignThingSwappingPC (retail 0x00584EDC).
struct CComponent { long id; };
struct CLongVector {
    long* first; long* last; long* end_of_storage;
    long& operator[](unsigned long n) { return first[n]; }
};
static int g_assigns = 0;
static CComponent* g_lastComponent = 0;
struct CTCInventory {
    CLongVector QuickAccessItems;
    void AssignThingPC(long item, unsigned long slot, CComponent* component)
    {
        ++g_assigns; QuickAccessItems[slot] = item; g_lastComponent = component;
    }
    void AssignThingSwappingPC(long item, unsigned long slot, long previousSlot, CComponent* component, CComponent* previousComponent);
};
void CTCInventory::AssignThingSwappingPC(long item, unsigned long slot, long previousSlot, CComponent* component, CComponent* previousComponent)
{
    long previousItem = QuickAccessItems[slot];
    AssignThingPC(item, slot, component);
    if (previousItem)
        AssignThingPC(previousItem, previousSlot, previousComponent);
}
static int Fail() { std::printf("00584edc_TEST FAIL\n"); return 1; }
int main()
{
    long slots[3] = { 11, 0, 33 };
    CComponent a = { 1 }, b = { 2 };
    CTCInventory inv;
    inv.QuickAccessItems.first = slots; inv.QuickAccessItems.last = slots + 3; inv.QuickAccessItems.end_of_storage = slots + 3;
    inv.AssignThingSwappingPC(50, 0, 1, &a, &b);              // swap: 11 moves to slot 1
    if (g_assigns != 2 || slots[0] != 50 || slots[1] != 11 || g_lastComponent != &b) return Fail();
    slots[2] = 0;
    inv.AssignThingSwappingPC(60, 2, 0, &a, &b);              // empty target: one assignment
    if (g_assigns != 3 || slots[2] != 60 || slots[0] != 50 || g_lastComponent != &a) return Fail();
    std::printf("00584edc_TEST PASS\n");
    return 0;
}