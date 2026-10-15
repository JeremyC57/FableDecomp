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
#define FAIL { std::printf("005bd9b2_TEST FAIL\n"); return 1; }
struct CTCInventoryBase {
    bool m_toggle; long m_lastType, m_lastContext; int m_selected, m_deselected;
    virtual ~CTCInventoryBase() { }
    virtual void OnItemSelected(long t, long c) { ++m_selected; m_lastType = t; m_lastContext = c; }
    virtual void OnItemDeselected(long t, long c) { ++m_deselected; m_lastType = t; m_lastContext = c; }
    bool CanToggleSelection() { return m_toggle; }
};
struct CCategory_Retail {
    CItemVector ItemsInCategory; CTCInventoryBase* PInventory; long GlobalDefIndex, SelectedItemIndex, HighlightedItemIndex;
    void SetSelectedItemIndex(long index) { GlobalDefIndex = index; SelectedItemIndex = index; if (index == -1) SelectedItemIndex = 0; }
    void SelectHighlightedItem(long context)
    {
        if (ItemsInCategory[HighlightedItemIndex].NumberOfItems == 0) return;
        bool deselected = false;
        if (PInventory->CanToggleSelection()) {
            if (SelectedItemIndex == HighlightedItemIndex) { SelectedItemIndex = -1; HighlightedItemIndex = 0; deselected = true; }
            else SetSelectedItemIndex(HighlightedItemIndex);
        }
        long itemType = ItemsInCategory[HighlightedItemIndex].ItemType;
        CTCInventoryBase* inventory = PInventory;
        if (deselected) inventory->OnItemDeselected(itemType, context);
        else inventory->OnItemSelected(itemType, context);
    }
};
int main()
{
    CItem items[3];
    for (int i = 0; i < 3; ++i) { items[i].ItemType = 10 + i; items[i].NumberOfItems = i; }
    CTCInventoryBase inv; inv.m_toggle = true; inv.m_selected = inv.m_deselected = 0;
    CCategory_Retail c; c.ItemsInCategory.first = items; c.ItemsInCategory.last = items + 3; c.PInventory = &inv;
    c.SelectedItemIndex = -1; c.HighlightedItemIndex = 0; c.GlobalDefIndex = 0;
    c.SelectHighlightedItem(5);
    if (inv.m_selected != 0 || inv.m_deselected != 0) FAIL;
    c.HighlightedItemIndex = 2; c.SelectHighlightedItem(5);
    if (inv.m_selected != 1 || inv.m_lastType != 12 || inv.m_lastContext != 5 || c.SelectedItemIndex != 2) FAIL;
    c.SelectHighlightedItem(6);
    if (inv.m_deselected != 1 || c.SelectedItemIndex != -1 || c.HighlightedItemIndex != 0 || inv.m_lastType != 10 || inv.m_lastContext != 6) FAIL;
    inv.m_toggle = false; c.HighlightedItemIndex = 1; c.SelectHighlightedItem(7);
    if (inv.m_selected != 2 || c.SelectedItemIndex != -1 || inv.m_lastType != 11) FAIL;
    std::printf("005bd9b2_TEST PASS\n");
    return 0;
}