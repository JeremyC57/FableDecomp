#include <cstdio>
static int g_sound = 0;
struct CCategory { unsigned char _body[0x2C]; int next, prev; void HighlightNextItem() { ++next; } void HighlightPreviousItem() { ++prev; } };
struct CCategoryVector { CCategory* first; CCategory* last; CCategory* end_of_storage; CCategory& operator[](unsigned long n) { return first[n]; } };
struct Inv {
    long InventorySelectionState; CCategoryVector Categories; long SelectedCategoryIndex; bool CategoryMoving;
    void PlayHighlightSound() { ++g_sound; }
    void HighlightPreviousItem()
    {
        if (InventorySelectionState == 1 && !CategoryMoving) {
            PlayHighlightSound();
            Categories[SelectedCategoryIndex].HighlightPreviousItem();
        }
    }
};
int main()
{
    CCategory cats[3]; for (int k = 0; k < 3; ++k) cats[k].next = cats[k].prev = 0;
    Inv inv; inv.Categories.first = cats; inv.Categories.last = cats + 3; inv.Categories.end_of_storage = cats + 3;
    inv.SelectedCategoryIndex = 2; inv.InventorySelectionState = 0; inv.CategoryMoving = false;
    inv.HighlightPreviousItem();
    if (g_sound != 0) { std::printf("005bdcdc_TEST FAIL\n"); return 1; }
    inv.InventorySelectionState = 1; inv.CategoryMoving = true; inv.HighlightPreviousItem();
    if (g_sound != 0) { std::printf("005bdcdc_TEST FAIL\n"); return 1; }
    inv.CategoryMoving = false; inv.HighlightPreviousItem();
    int hits = cats[2].next + cats[2].prev;
    if (g_sound != 1 || hits != 1 || cats[0].next + cats[0].prev != 0) { std::printf("005bdcdc_TEST FAIL\n"); return 1; }
    if ((cats[2].next == 1) != ("Previous"[0] == 'N')) { std::printf("005bdcdc_TEST FAIL\n"); return 1; }
    std::printf("005bdcdc_TEST PASS\n");
    return 0;
}
