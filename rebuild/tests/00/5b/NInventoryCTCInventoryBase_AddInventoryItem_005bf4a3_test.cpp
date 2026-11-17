// behaviour test for 005bf4a3: CTCInventoryBase::AddItem -- category lookup by the item's inventory category, add, notify
#include <cstdio>
struct CItem { int ItemType; int NumberOfItems; int PPerItemData[2]; int AverageBoughtPrice; };
static int GetInventoryCategory(int itemType) { return itemType >= 100 ? itemType / 100 : -1; }
struct CCategory {
    int GlobalDefIndex; int added; bool full;
    bool AddItem(const CItem&) { if (full) return false; ++added; return true; }
};
static CCategory* FindCategory(CCategory* first, CCategory* last, int defIndex)
{
    for (; first != last; ++first) if (first->GlobalDefIndex == defIndex) return first;
    return last;
}
struct CTCInventoryBase {
    CCategory* CategoriesFirst; CCategory* CategoriesLast;
    int notified; int lastType; bool a, b;
    void OnItemAdded(int itemType, bool x, bool y) { ++notified; lastType = itemType; a = x; b = y; }
    bool AddItem(const CItem& item)
    {
        int categoryDefIndex = GetInventoryCategory(item.ItemType);
        CCategory* last = CategoriesLast;
        CCategory* category = FindCategory(CategoriesFirst, last, categoryDefIndex);
        if (category != last && category->AddItem(item)) {
            OnItemAdded(item.ItemType, true, true);
            return true;
        }
        return false;
    }
};
int main()
{
    CCategory cats[3];
    for (int i = 0; i < 3; ++i) { cats[i].GlobalDefIndex = i + 1; cats[i].added = 0; cats[i].full = false; }
    cats[2].full = true;
    CTCInventoryBase inv; inv.CategoriesFirst = cats; inv.CategoriesLast = cats + 3; inv.notified = 0; inv.lastType = 0; inv.a = inv.b = false;
    CItem item; item.NumberOfItems = 1; item.PPerItemData[0] = item.PPerItemData[1] = 0; item.AverageBoughtPrice = 0;
    int bad = 0;
    item.ItemType = 205;   /* category 2 */
    if (!inv.AddItem(item) || cats[1].added != 1 || inv.notified != 1 || inv.lastType != 205 || !inv.a || !inv.b) ++bad;
    item.ItemType = 50;    /* no category (-1) */
    if (inv.AddItem(item) || inv.notified != 1) ++bad;
    item.ItemType = 399;   /* category 3 is full */
    if (inv.AddItem(item) || inv.notified != 1 || cats[2].added != 0) ++bad;
    item.ItemType = 900;   /* category 9 absent */
    if (inv.AddItem(item) || inv.notified != 1) ++bad;
    if (bad) { std::printf("005bf4a3_TEST FAIL\n"); return 1; }
    std::printf("005bf4a3_TEST PASS\n");
    return 0;
}