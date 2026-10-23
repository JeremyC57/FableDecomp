#include <cstdio>
long GetInventoryCategory(long itemType) { return itemType >= 100 ? itemType / 100 : -1; }
struct CCategory { long GlobalDefIndex; long removedType, removedCount; void RemoveItem(long t, long n) { removedType = t; removedCount = n; } };
CCategory* FindCategoryWithDefIndex_005c08d3(CCategory* first, CCategory* last, long idx) { while (first != last && first->GlobalDefIndex != idx) ++first; return first; }
struct CCategoryVector { CCategory* first; CCategory* last; CCategory* end_of_storage; };
struct Inv {
    CCategoryVector Categories;
    void RemoveItemFromInventory(long itemType, long numberOfItems)
    {
        long categoryDefIndex = GetInventoryCategory(itemType);
        FindCategoryWithDefIndex_005c08d3(Categories.first, Categories.last, categoryDefIndex)->RemoveItem(itemType, numberOfItems);
    }
};
int main()
{
    CCategory cats[3] = { { 1, 0, 0 }, { 2, 0, 0 }, { 3, 0, 0 } };
    Inv inv; inv.Categories.first = cats; inv.Categories.last = cats + 3; inv.Categories.end_of_storage = cats + 3;
    inv.RemoveItemFromInventory(207, 4);
    if (cats[1].removedType != 207 || cats[1].removedCount != 4 || cats[0].removedCount || cats[2].removedCount) { std::printf("005bf3c6_TEST FAIL\n"); return 1; }
    std::printf("005bf3c6_TEST PASS\n");
    return 0;
}