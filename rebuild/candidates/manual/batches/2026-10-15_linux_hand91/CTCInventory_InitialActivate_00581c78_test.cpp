#include <cstdio>
// Behaviour test for CTCInventory::InitialActivate (retail 0x00581C78).
static int g_baseCalls = 0;
static long g_held[8];      // count held per item type

struct CLongVector {
    long* first; long* last; long* end_of_storage;
    unsigned long size() const { return (unsigned long)(last - first); }
    long& operator[](unsigned long n) { return first[n]; }
};

struct CTCInventoryBase {
    void InitialActivate() { ++g_baseCalls; }
    long GetNumberOfItemsOfTypeInInventory(long type) { return (type >= 0 && type < 8) ? g_held[type] : 0; }
};

struct CTCInventory : CTCInventoryBase {
    CLongVector QuickAccessItems;
    void InitialActivate();
};

void CTCInventory::InitialActivate()
{
    CTCInventoryBase::InitialActivate();
    for (unsigned long i = 0; i < QuickAccessItems.size(); ++i) {
        if (GetNumberOfItemsOfTypeInInventory(QuickAccessItems[i]) == 0)
            QuickAccessItems[i] = 0;
    }
}

int main()
{
    long slots[4] = { 3, 5, 0, 7 };
    for (int k = 0; k < 8; ++k) g_held[k] = 0;
    g_held[3] = 2; g_held[7] = 1;
    CTCInventory inv;
    inv.QuickAccessItems.first = slots; inv.QuickAccessItems.last = slots + 4; inv.QuickAccessItems.end_of_storage = slots + 4;
    inv.InitialActivate();
    if (g_baseCalls != 1 || slots[0] != 3 || slots[1] != 0 || slots[2] != 0 || slots[3] != 7) { std::printf("00581c78_TEST FAIL\n"); return 1; }
    inv.QuickAccessItems.last = slots;   // empty vector: only the base runs
    inv.InitialActivate();
    if (g_baseCalls != 2) { std::printf("00581c78_TEST FAIL\n"); return 1; }
    std::printf("00581c78_TEST PASS\n");
    return 0;
}
