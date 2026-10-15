#include <cstdio>
// Behaviour test for CTCInventory::GetQuickAccessItem (retail 0x00581E33).
struct CLongVector {
    long* first; long* last; long* end_of_storage;
    unsigned long size() const { return (unsigned long)(last - first); }
    long& operator[](unsigned long n) { return first[n]; }
};
struct CTCInventory {
    CLongVector QuickAccessItems;
    CLongVector ContextSensitiveItems;
    long GetQuickAccessItem(unsigned long index);
};
long CTCInventory::GetQuickAccessItem(unsigned long index)
{
    if (index < ContextSensitiveItems.size())
        return ContextSensitiveItems[index];
    return QuickAccessItems[index - ContextSensitiveItems.size()];
}
int main()
{
    long cs[2] = { 101, 102 };
    long qa[3] = { 201, 202, 203 };
    CTCInventory inv;
    inv.ContextSensitiveItems.first = cs; inv.ContextSensitiveItems.last = cs + 2; inv.ContextSensitiveItems.end_of_storage = cs + 2;
    inv.QuickAccessItems.first = qa; inv.QuickAccessItems.last = qa + 3; inv.QuickAccessItems.end_of_storage = qa + 3;
    if (inv.GetQuickAccessItem(0) != 101 || inv.GetQuickAccessItem(1) != 102 || inv.GetQuickAccessItem(2) != 201 ||
        inv.GetQuickAccessItem(4) != 203) { std::printf("00581e33_TEST FAIL\n"); return 1; }
    inv.ContextSensitiveItems.last = cs;
    if (inv.GetQuickAccessItem(0) != 201) { std::printf("00581e33_TEST FAIL\n"); return 1; }
    std::printf("00581e33_TEST PASS\n");
    return 0;
}