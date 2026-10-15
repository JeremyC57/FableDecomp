#include <cstdio>
// Behaviour test for CTCInventoryBase::DeselectAll (0x005BCCF1): same logic over a copy of the model.
struct CCategory {
    long SelectedItemIndex; long HighlightedItemIndex; bool Selected;
    void Deselect() { SelectedItemIndex = -1; HighlightedItemIndex = 0; }
};
struct CCategoryVector {
    CCategory* first; CCategory* last; CCategory* end_of_storage;
    unsigned long size() const { return (unsigned long)(last - first); }
    CCategory& operator[](unsigned long n) { return first[n]; }
};
struct CTCInventoryBase_Retail {
    CCategoryVector Categories;
    void DeselectAll() { for (unsigned long i = 0; i < Categories.size(); ++i) Categories[i].Deselect(); }
};
int main()
{
    CCategory cats[3] = { { 2, 5, true }, { 0, 1, false }, { -1, 0, false } };
    CTCInventoryBase_Retail inv; inv.Categories.first = cats; inv.Categories.last = cats + 3; inv.Categories.end_of_storage = cats + 3;
    inv.DeselectAll();
    for (int i = 0; i < 3; ++i)
        if (cats[i].SelectedItemIndex != -1 || cats[i].HighlightedItemIndex != 0) { std::printf("005bccf1_TEST FAIL\n"); return 1; }
    if (!cats[0].Selected) { std::printf("005bccf1_TEST FAIL\n"); return 1; }
    std::printf("005bccf1_TEST PASS\n");
    return 0;
}