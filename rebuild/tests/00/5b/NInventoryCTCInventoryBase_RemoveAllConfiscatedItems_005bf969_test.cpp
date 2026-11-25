// Behaviour test for NInventory::CTCInventoryBase::RemoveAllConfiscatedItems 0x005BF969: the confiscated item stacks are dropped
// (vector cleared, every element destroyed once, nothing given back) and the other inventory state is untouched. CItem's
// destructor is the implicit one (FABLE_RETAIL_NINVENTORY_CITEM_TRANSFERIN): it runs only the PPerItemData member's destructor,
// modelled here by a counting member.
#include <cstdio>
#include <vector>
static int g_destroyed = 0;
struct PerItemRef {                                   // stands for CCountedPointer<NInventory::CPerItemData> PPerItemData
    long Tag;
    explicit PerItemRef(long t) : Tag(t) { }
    PerItemRef(const PerItemRef& o) : Tag(o.Tag) { }
    ~PerItemRef() { ++g_destroyed; }
};
struct CItem {                                        // destructor implicit
    long ItemType, NumberOfItems;
    PerItemRef PPerItemData;
    long AverageBoughtPrice;
    CItem(long t, long n) : ItemType(t), NumberOfItems(n), PPerItemData(t * 100), AverageBoughtPrice(0) { }
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
    inv.OwnedItems.reserve(2);
    inv.ConfiscatedItems.push_back(CItem(1, 3));
    inv.ConfiscatedItems.push_back(CItem(2, 1));
    inv.ConfiscatedItems.push_back(CItem(7, 5));
    inv.OwnedItems.push_back(CItem(9, 9));
    g_destroyed = 0;
    inv.RemoveAllConfiscatedItems();
    if (!inv.ConfiscatedItems.empty() || g_destroyed != 3 || inv.OwnedItems.size() != 1 || inv.OwnedItems[0].ItemType != 9
        || inv.OwnedItems[0].PPerItemData.Tag != 900) {
        std::printf("005bf969_TEST FAIL\n"); return 1;
    }
    inv.RemoveAllConfiscatedItems();
    if (g_destroyed != 3) { std::printf("005bf969_TEST FAIL\n"); return 1; }
    std::printf("005bf969_TEST PASS\n");
    return 0;
}
