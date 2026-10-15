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
#define FAIL { std::printf("005bf24a_TEST FAIL\n"); return 1; }
struct CItem_Methods : CItem { void RemoveItem(long count) { NumberOfItems -= count; if (NumberOfItems < 0) NumberOfItems = 0; } };
struct CCategory_Retail {
    CItem_Methods* first; CItem_Methods* last; CDefPointer PDef; long SelectedItemIndex, HighlightedItemIndex;
    CItem_Methods& At(unsigned long n) { return *(first + n); }
    long GetNextOccupiedSlot(long index) { for (long i = index + 1; i < last - first; ++i) if (first[i].NumberOfItems) return i; return index; }
    void RemoveSelectedItem()
    {
        At(SelectedItemIndex).RemoveItem(1);
        if (At(SelectedItemIndex).NumberOfItems == 0) { SelectedItemIndex = -1; HighlightedItemIndex = 0; }
        if (!PDef->SelectEmptySlots && At(HighlightedItemIndex).NumberOfItems == 0)
            HighlightedItemIndex = GetNextOccupiedSlot(HighlightedItemIndex);
    }
};
int main()
{
    CItem_Methods items[3];
    for (int i = 0; i < 3; ++i) { items[i].ItemType = i; items[i].NumberOfItems = 0; }
    items[1].NumberOfItems = 2; items[2].NumberOfItems = 1;
    CInventoryCategoryDef_Retail def; def.m_refCount = 1; def.SelectEmptySlots = false;
    CCategory_Retail c; c.first = items; c.last = items + 3; c.PDef.m_pObject = &def;
    c.SelectedItemIndex = 1; c.HighlightedItemIndex = 1;
    c.RemoveSelectedItem();
    if (items[1].NumberOfItems != 1 || c.SelectedItemIndex != 1 || c.HighlightedItemIndex != 1) FAIL;
    c.RemoveSelectedItem();
    if (items[1].NumberOfItems != 0 || c.SelectedItemIndex != -1 || c.HighlightedItemIndex != 2) FAIL;
    def.SelectEmptySlots = true; c.SelectedItemIndex = 2; c.HighlightedItemIndex = 2; c.RemoveSelectedItem();
    if (c.SelectedItemIndex != -1 || c.HighlightedItemIndex != 0 || def.m_refCount != 1) FAIL;
    std::printf("005bf24a_TEST PASS\n");
    return 0;
}