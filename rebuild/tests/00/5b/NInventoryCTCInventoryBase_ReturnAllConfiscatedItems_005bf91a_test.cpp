// Behaviour test for NInventory::CTCInventoryBase::ReturnAllConfiscatedItems 0x005BF91A: every confiscated stack is given back once
// through AddInventoryItem, in order, then ConfiscatedItems is emptied (erase(begin(), end())), destroying each element once.
// CItem's destructor is the implicit one (FABLE_RETAIL_NINVENTORY_CITEM_TRANSFERIN): it runs only the PPerItemData member's
// destructor (+8), modelled here by a counting member.
#include <cstdio>
static int g_released = 0;
struct PerItemRef {                                   // stands for CCountedPointer<NInventory::CPerItemData> PPerItemData
    long Tag;
    ~PerItemRef() { ++g_released; Tag = -1; }
};
struct CItem {                                        // 0x14 bytes in retail; destructor implicit
    long ItemType, NumberOfItems;
    PerItemRef PPerItemData;
    long AverageBoughtPrice;
};
struct CItemVector {
    CItem* first; CItem* last; CItem* end_of_storage;
    unsigned long size() const { return (unsigned long)(last - first); }
    CItem& operator[](unsigned long n) { return first[n]; }
    CItem* erase(CItem* from, CItem* to)
    {
        CItem* d = from;
        for (CItem* s = to; s != last; ++s, ++d) { d->ItemType = s->ItemType; d->NumberOfItems = s->NumberOfItems; d->PPerItemData.Tag = s->PPerItemData.Tag; d->AverageBoughtPrice = s->AverageBoughtPrice; }
        for (CItem* p = d; p != last; ++p) p->~CItem();
        last = d;
        return from;
    }
    void clear() { erase(first, last); }
};
struct Inv {
    CItemVector ConfiscatedItems; long returned; long order; int adds;
    void AddInventoryItem(const CItem& item) { returned += item.NumberOfItems; order = order * 10 + item.ItemType; ++adds; }
    void ReturnAllConfiscatedItems()
    {
        for (unsigned long i = 0; i < ConfiscatedItems.size(); ++i)
            AddInventoryItem(ConfiscatedItems[i]);
        ConfiscatedItems.clear();
    }
};
static unsigned char g_store[3 * sizeof(CItem)];
int main()
{
    CItem* it = (CItem*)g_store;
    for (int k = 0; k < 3; ++k) { it[k].ItemType = k + 1; it[k].NumberOfItems = 7 + 5 * k; it[k].PPerItemData.Tag = 100 + k; it[k].AverageBoughtPrice = 9; }
    Inv inv; inv.returned = 0; inv.order = 0; inv.adds = 0;
    inv.ConfiscatedItems.first = it; inv.ConfiscatedItems.last = it + 3; inv.ConfiscatedItems.end_of_storage = it + 3;
    inv.ReturnAllConfiscatedItems();
    if (inv.adds != 3 || inv.returned != 7 + 12 + 17 || inv.order != 123 || inv.ConfiscatedItems.size() != 0 || g_released != 3
        || it[0].PPerItemData.Tag != -1 || it[2].PPerItemData.Tag != -1) { std::printf("005bf91a_TEST FAIL\n"); return 1; }
    inv.ReturnAllConfiscatedItems();                  // empty: nothing given back, nothing destroyed
    if (inv.adds != 3 || g_released != 3) { std::printf("005bf91a_TEST FAIL\n"); return 1; }
    std::printf("005bf91a_TEST PASS\n");
    return 0;
}
