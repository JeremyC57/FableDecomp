#include <cstdio>
static int g_live = 0;
struct CPerItemData { long kills; float BestFishWeight; };
struct CItem { long ItemType; long NumberOfItems; CPerItemData* PPerItemData_Data; void* PPerItemData_Info; long AverageBoughtPrice; };
struct CItemVector {
    CItem* first; CItem* last; CItem* end_of_storage;
    CItemVector() : first(0), last(0), end_of_storage(0) { ++g_live; }
    ~CItemVector() { --g_live; }
};
static CItem g_stock[2]; static int g_nstock = 0;
struct Inv {
    void GetAllInventoryItemsOfType(long itemType, CItemVector& items)
    { for (int i = 0; i < g_nstock; ++i) if (g_stock[i].ItemType == itemType) { if (!items.first) items.first = items.last = &g_stock[i]; ++items.last; } }
    float GetBestFishWeight(long fishType)
    {
        CItemVector items;
        GetAllInventoryItemsOfType(fishType, items);
        float best;
        if (items.first != items.last && items.first->PPerItemData_Data != 0)
            best = items.first->PPerItemData_Data->BestFishWeight;
        else
            best = 0.0f;
        return best;
    }
};
int main()
{
    Inv inv; CPerItemData d; d.kills = 0; d.BestFishWeight = 3.5f;
    g_stock[0].ItemType = 4; g_stock[0].PPerItemData_Data = 0; g_stock[1].ItemType = 7; g_stock[1].PPerItemData_Data = &d; g_nstock = 2;
    if (inv.GetBestFishWeight(9) != 0.0f) { std::printf("005bfc90_TEST FAIL\n"); return 1; }
    if (inv.GetBestFishWeight(4) != 0.0f) { std::printf("005bfc90_TEST FAIL\n"); return 1; }
    if (inv.GetBestFishWeight(7) != 3.5f || g_live != 0) { std::printf("005bfc90_TEST FAIL\n"); return 1; }
    std::printf("005bfc90_TEST PASS\n");
    return 0;
}