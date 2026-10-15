#include <cstdio>
long GetInventoryCategory(long itemType) { return itemType >= 100 ? itemType / 100 : -1; }
struct CCategory { long GlobalDefIndex; long price; long GetBoughtPrice(long itemType) { return price + itemType; } };
CCategory* FindCategoryWithDefIndex(CCategory* first, CCategory* last, long idx) { while (first != last && first->GlobalDefIndex != idx) ++first; return first; }
struct CCategoryVector { CCategory* first; CCategory* last; CCategory* end_of_storage; };
struct Inv {
    CCategoryVector Categories;
    long GetBoughtPrice(long itemType)
    {
        CCategory* category = FindCategoryWithDefIndex(Categories.first, Categories.last, GetInventoryCategory(itemType));
        return category->GetBoughtPrice(itemType);
    }
};
int main()
{
    CCategory cats[3] = { { 1, 1000 }, { 2, 2000 }, { 3, 3000 } };
    Inv inv; inv.Categories.first = cats; inv.Categories.last = cats + 3; inv.Categories.end_of_storage = cats + 3;
    if (inv.GetBoughtPrice(205) != 2205 || inv.GetBoughtPrice(301) != 3301 || inv.GetBoughtPrice(100) != 1100) { std::printf("005be8d3_TEST FAIL\n"); return 1; }
    std::printf("005be8d3_TEST PASS\n");
    return 0;
}
