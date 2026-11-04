// Behaviour test for NInventory::CTCInventoryBase::RemoveAllConfiscatedItems 0x005BF969: the confiscated item stacks are dropped
// (vector cleared, every element destroyed once, nothing given back) and the other inventory state is untouched.
#include <cstdio>
#include <vector>
static int g_destroyed = 0;
struct CItem {
    long ItemType, NumberOfItems;
    CItem(long t, long n) : ItemType(t), NumberOfItems(n) { }
    CItem(const CItem& o) : ItemType(o.ItemType), NumberOfItems(o.NumberOfItems) { }
    ~CItem() { ++g_destroyed; }
};
struct CTCInventoryBase {
    std::vector<CItem> ConfiscatedItems;
    std::vector<CItem> OwnedItems;
    void RemoveAllConfiscatedItems() { ConfiscatedItems.clear(); }
};
int main()
{
    CTCInventoryBase inv;
    inv.ConfiscatedItems.reserve(4);
    inv.ConfiscatedItems.push_back(CItem(1, 3));
    inv.ConfiscatedItems.push_back(CItem(2, 1));
    inv.ConfiscatedItems.push_back(CItem(7, 5));
    inv.OwnedItems.push_back(CItem(9, 9));
    g_destroyed = 0;
    inv.RemoveAllConfiscatedItems();
    if (!inv.ConfiscatedItems.empty() || g_destroyed != 3 || inv.OwnedItems.size() != 1 || inv.OwnedItems[0].ItemType != 9) {
        std::printf("005bf969_TEST FAIL\n"); return 1;
    }
    inv.RemoveAllConfiscatedItems();
    if (g_destroyed != 3) { std::printf("005bf969_TEST FAIL\n"); return 1; }
    std::printf("005bf969_TEST PASS\n");
    return 0;
}