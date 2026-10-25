// Behaviour test for 0x005BFB01 CTCInventoryBase::ConfiscateItems
#include <cstdio>
struct CItem { long ItemType; long NumberOfItems; unsigned int d, i; long AverageBoughtPrice; };
struct CItemVector {
    CItem buf[16]; unsigned long n, cap;
    CItemVector() : n(0), cap(0) {}
    unsigned long size() const { return n; }
    CItem& operator[](unsigned long k) { return buf[k]; }
    void reserve(unsigned long k) { if (k > cap) cap = k; }
    void push_back(const CItem& it) { buf[n++] = it; }
};
static bool IsConfiscatable(long t) { return (t & 1) != 0; }
static long g_removed = 0;
struct CInv {
    CItemVector ConfiscatedItems;
    void RemoveItemsOfType(long t, long c) { g_removed += t * 10 + c; }
    void ConfiscateItems(CItemVector& items)
    {
        ConfiscatedItems.reserve(ConfiscatedItems.size() + items.size());
        for (unsigned long i = 0; i < items.size(); ++i) {
            CItem& item = items[i];
            if (IsConfiscatable(item.ItemType)) {
                ConfiscatedItems.push_back(item);
                RemoveItemsOfType(item.ItemType, item.NumberOfItems);
            }
        }
    }
};
int main()
{
    CInv inv; CItemVector items;
    for (int k = 0; k < 4; ++k) { CItem it = { k + 1, 2, 0, 0, 0 }; items.push_back(it); }
    inv.ConfiscateItems(items);
    bool ok = inv.ConfiscatedItems.size() == 2 && inv.ConfiscatedItems.cap == 4
        && inv.ConfiscatedItems[1].ItemType == 3 && g_removed == 12 + 32;
    if (!ok) { std::printf("005bfb01_TEST FAIL\n"); return 1; }
    std::printf("005bfb01_TEST PASS\n");
    return 0;
}