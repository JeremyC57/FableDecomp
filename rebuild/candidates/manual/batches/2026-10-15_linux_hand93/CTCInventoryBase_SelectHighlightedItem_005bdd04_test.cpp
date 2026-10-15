#include <cstdio>
struct CCategory { unsigned char _body[0x2C]; int hits; void SelectHighlightedItem(bool& selected) { ++hits; selected = true; } };
struct CCategoryVector {
    CCategory* first; CCategory* last; CCategory* end_of_storage;
    unsigned long size() const { return (unsigned long)(last - first); }
    CCategory& operator[](unsigned long n) { return first[n]; }
};
struct Inv {
    CCategoryVector Categories; long SelectedCategoryIndex;
    void SelectHighlightedItem(bool& selected)
    {
        selected = false;
        if (SelectedCategoryIndex >= 0 && (unsigned long)SelectedCategoryIndex < Categories.size())
            Categories[SelectedCategoryIndex].SelectHighlightedItem(selected);
    }
};
int main()
{
    CCategory cats[2]; cats[0].hits = cats[1].hits = 0;
    Inv inv; inv.Categories.first = cats; inv.Categories.last = cats + 2; inv.Categories.end_of_storage = cats + 2;
    bool sel = true;
    inv.SelectedCategoryIndex = -1; inv.SelectHighlightedItem(sel);
    if (sel || cats[0].hits || cats[1].hits) { std::printf("005bdd04_TEST FAIL\n"); return 1; }
    sel = true; inv.SelectedCategoryIndex = 2; inv.SelectHighlightedItem(sel);
    if (sel || cats[0].hits || cats[1].hits) { std::printf("005bdd04_TEST FAIL\n"); return 1; }
    inv.SelectedCategoryIndex = 1; inv.SelectHighlightedItem(sel);
    if (!sel || cats[1].hits != 1 || cats[0].hits) { std::printf("005bdd04_TEST FAIL\n"); return 1; }
    std::printf("005bdd04_TEST PASS\n");
    return 0;
}
