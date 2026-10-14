#include <cstdio>
struct CItem { long ItemType; long NumberOfItems; void* PPerItemData_Data; void* PPerItemData_Info; long AverageBoughtPrice; };
static int g_calls = 0; static long g_lastArg = -99; static long g_lastArg2 = -99;
struct CItem_Methods : CItem {
    bool IsSameItemTypeAsDefIndex(long d) { return NumberOfItems != 0 && d == ItemType; }
    bool IsQuestPersistent() { return AverageBoughtPrice < 0; }
    bool IsSlotFull() { return NumberOfItems >= 10; }
    void SetQuestPersistentFlag(bool f) { ++g_calls; AverageBoughtPrice = f ? -1 : 1; }
    void UpdateNonQuestPersistentRefCount(bool f) { ++g_calls; AverageBoughtPrice += f ? 1 : -1; }
    void RemoveItem(long n) { ++g_calls; NumberOfItems -= n; }
};
struct CItemVector { CItem_Methods* first; CItem_Methods* last; CItem_Methods* end_of_storage;
    unsigned long size() const { return (unsigned long)(last - first); } bool empty() const { return first == last; }
    CItem_Methods& operator[](unsigned long n) { return first[n]; } };
struct CDefRetail { long m_refCount; long NumberOfSlots; bool AllowItemsToFillMoreThanOneSlot; long CategoryIdentifier; int m_destroyed;
    void AddRef() { ++m_refCount; } void Destroy() { ++m_destroyed; } void Release() { if (--m_refCount == 0) Destroy(); } };
struct CDefRef { CDefRetail* m_p; CDefRef(CDefRetail* p) : m_p(p) { if (m_p) m_p->AddRef(); } ~CDefRef() { m_p->Release(); } CDefRetail* operator->() const { return m_p; } };
struct CDefPointer { CDefRetail* m_pObject; CDefRef operator->() const { return CDefRef(m_pObject); } };
struct CInv { virtual void RemoveItems(long t, long n) { ++g_calls; g_lastArg = t; g_lastArg2 = n; } virtual void OnItemSelected(long t) { ++g_calls; g_lastArg = t; } virtual void OnItemOfTypeSelected(long t, int) { ++g_calls; g_lastArg = t; } };
struct CCategory_Retail { unsigned char PAssociatedThing[8]; CItemVector ItemsInCategory; CDefPointer PDef; CInv* PInventory; long GlobalDefIndex; long SelectedItemIndex; long HighlightedItemIndex; bool Selected; bool DisplayItems;
    long GetSlotWithItemsOfType(long t) { for (CItem_Methods* p = ItemsInCategory.first; p != ItemsInCategory.last; ++p) if (p->ItemType == t && p->NumberOfItems != 0) return (long)(p - ItemsInCategory.first); return -1; }
    long GetFirstEmptySlot() { for (CItem_Methods* p = ItemsInCategory.first; p != ItemsInCategory.last; ++p) if (p->NumberOfItems == 0) return (long)(p - ItemsInCategory.first); return -1; }
    bool CanAddItem(long t) { long slot = GetSlotWithItemsOfType(t); if (slot != -1) { CItem_Methods* item = &ItemsInCategory[slot]; return !item->IsSlotFull(); } return GetFirstEmptySlot() != -1; }
};
static CItem_Methods g_items[4];
static CInv g_inv; static CDefRetail g_def = { 1, 6, true, 42, 0 };
static CCategory_Retail MakeCategory(unsigned n) {
    CItem items[4] = { { 7, 3, 0, 0, 100 }, { 8, 0, 0, 0, 200 }, { 9, 10, 0, 0, -300 }, { 7, 1, 0, 0, 400 } };
    for (unsigned i = 0; i < 4; ++i) static_cast<CItem&>(g_items[i]) = items[i];
    CCategory_Retail c; c.ItemsInCategory.first = g_items; c.ItemsInCategory.last = g_items + n; c.ItemsInCategory.end_of_storage = g_items + 4;
    c.PDef.m_pObject = &g_def; c.PInventory = &g_inv; c.SelectedItemIndex = 2; c.Selected = false; return c;
}

int main()
{
    CCategory_Retail c = MakeCategory(4);
    if (!c.CanAddItem(7) || c.CanAddItem(9) || !c.CanAddItem(5)) { std::printf("005bf19c_TEST FAIL\n"); return 1; }
    g_items[1].NumberOfItems = 2; if (c.CanAddItem(5)) { std::printf("005bf19c_TEST FAIL\n"); return 1; }
    std::printf("005bf19c_TEST PASS\n");
    return 0;
}
