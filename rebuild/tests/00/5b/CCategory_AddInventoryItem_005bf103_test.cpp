#include <cstdio>
struct CItem { long ItemType; long NumberOfItems; void* Data; void* Info; long AverageBoughtPrice; };
static int g_destroyed = 0;
struct CInventoryCategoryDef_Retail {
    long m_refCount; bool SelectEmptySlots; bool WrapHighlightCursor; long CategoryName;
    void Destroy() { ++g_destroyed; }
    void AddRef() { ++m_refCount; }
    void Release() { if (--m_refCount == 0) Destroy(); }
};
struct CDefRef {
    CInventoryCategoryDef_Retail* m_p;
    CDefRef(CInventoryCategoryDef_Retail* p) : m_p(p) { if (m_p) m_p->AddRef(); }
    ~CDefRef() { m_p->Release(); }
    CInventoryCategoryDef_Retail* operator->() const { return m_p; }
};
struct CDefPointer { CInventoryCategoryDef_Retail* m_pObject; CDefRef operator->() const { return CDefRef(m_pObject); } };
struct CItemVector {
    CItem* first; CItem* last; CItem* end_of_storage;
    unsigned long size() const { return (unsigned long)(last - first); }
    CItem& operator[](unsigned long n) { return first[n]; }
};
#define FAIL { std::printf("005bf103_TEST FAIL\n"); return 1; }
struct CItem_Methods : CItem {
    bool IsFull() { return NumberOfItems >= 3; }
    void Add(const CItem& item) { ItemType = item.ItemType; NumberOfItems += item.NumberOfItems; }
};
struct CCategory_Retail {
    CItem_Methods* first; CItem_Methods* last; long HighlightedItemIndex;
    CItem_Methods& At(unsigned long n) { return *(first + n); }
    long GetSlotWithItemsOfType(long t) { for (long i = 0; i < last - first; ++i) if (first[i].NumberOfItems && first[i].ItemType == t) return i; return -1; }
    long GetFirstEmptySlot() { for (long i = 0; i < last - first; ++i) if (!first[i].NumberOfItems) return i; return -1; }
    bool AddInventoryItem(const CItem& item)
    {
        long slot = GetSlotWithItemsOfType(item.ItemType);
        if (slot == -1) { slot = GetFirstEmptySlot(); if (slot == -1) return false; }
        if (At(slot).IsFull()) return false;
        At(slot).Add(item);
        if (At(HighlightedItemIndex).NumberOfItems == 0) HighlightedItemIndex = slot;
        return true;
    }
};
int main()
{
    CItem_Methods items[2];
    for (int i = 0; i < 2; ++i) { items[i].ItemType = 0; items[i].NumberOfItems = 0; }
    CCategory_Retail c; c.first = items; c.last = items + 2; c.HighlightedItemIndex = 1;
    CItem sword; sword.ItemType = 5; sword.NumberOfItems = 1;
    CItem axe; axe.ItemType = 6; axe.NumberOfItems = 1;
    if (!c.AddInventoryItem(sword) || items[0].ItemType != 5 || items[0].NumberOfItems != 1 || c.HighlightedItemIndex != 0) FAIL;
    if (!c.AddInventoryItem(sword) || items[0].NumberOfItems != 2) FAIL;
    if (!c.AddInventoryItem(axe) || items[1].ItemType != 6 || c.HighlightedItemIndex != 0) FAIL;
    CItem bow; bow.ItemType = 7; bow.NumberOfItems = 1;
    if (c.AddInventoryItem(bow)) FAIL;
    if (!c.AddInventoryItem(sword) || items[0].NumberOfItems != 3) FAIL;
    if (c.AddInventoryItem(sword) || items[0].NumberOfItems != 3) FAIL;
    std::printf("005bf103_TEST PASS\n");
    return 0;
}