#include <cstdio>
// Behaviour test for CTCInventory::GetContextSensitiveItemButtonIndex (retail 0x00581D4E).
struct CLongVector {
    long* first; long* last; long* end_of_storage;
    unsigned long size() const { return (unsigned long)(last - first); }
    long& operator[](unsigned long n) { return first[n]; }
};
class CTCInventory { public:
    CLongVector ContextSensitiveItems;
    long GetContextSensitiveItemButtonIndex(long item);
};
long CTCInventory::GetContextSensitiveItemButtonIndex(long item)
{
    for (unsigned long i = 0; i < ContextSensitiveItems.size(); ++i) {
        if (ContextSensitiveItems[i] == item)
            return i;
    }
    return -1;
}
int main()
{
    long items[4] = { 12, 0, 40, 12 };
    CTCInventory inv;
    inv.ContextSensitiveItems.first = items; inv.ContextSensitiveItems.last = items + 4; inv.ContextSensitiveItems.end_of_storage = items + 4;
    if (inv.GetContextSensitiveItemButtonIndex(12) != 0 || inv.GetContextSensitiveItemButtonIndex(40) != 2 ||
        inv.GetContextSensitiveItemButtonIndex(0) != 1 || inv.GetContextSensitiveItemButtonIndex(7) != -1) { std::printf("00581d4e_TEST FAIL\n"); return 1; }
    inv.ContextSensitiveItems.last = items;
    if (inv.GetContextSensitiveItemButtonIndex(12) != -1) { std::printf("00581d4e_TEST FAIL\n"); return 1; }
    std::printf("00581d4e_TEST PASS\n");
    return 0;
}