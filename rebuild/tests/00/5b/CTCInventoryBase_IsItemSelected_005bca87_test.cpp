#include <cstdio>
// Behaviour test for CTCInventoryBase::IsItemSelected (0x005BCA87): same logic over a copy of the model.
struct CItem { long ItemType; unsigned char _rest[0x10]; };
struct CCategory {
    CItem* ItemsFirst; long SelectedItemIndex;
    long GetSelectedItem() { long index = SelectedItemIndex; if (index >= 0) return ItemsFirst[index].ItemType; return 0; }
};
struct CCategoryVector {
    CCategory* first; CCategory* last; CCategory* end_of_storage;
    unsigned long size() const { return (unsigned long)(last - first); }
    CCategory& operator[](unsigned long n) { return first[n]; }
};
struct CTCInventoryBase_Retail {
    CCategoryVector Categories;
    bool IsItemSelected(long itemType)
    {
        for (unsigned long i = 0; i < Categories.size(); ++i) {
            if (Categories[i].GetSelectedItem() == itemType)
                return true;
        }
        return false;
    }
};
int main()
{
    CItem a[2], b[2]; a[0].ItemType = 11; a[1].ItemType = 12; b[0].ItemType = 21; b[1].ItemType = 22;
    CCategory cats[2]; cats[0].ItemsFirst = a; cats[0].SelectedItemIndex = 1; cats[1].ItemsFirst = b; cats[1].SelectedItemIndex = -1;
    CTCInventoryBase_Retail inv; inv.Categories.first = cats; inv.Categories.last = cats + 2; inv.Categories.end_of_storage = cats + 2;
    if (!inv.IsItemSelected(12) || inv.IsItemSelected(11) || inv.IsItemSelected(21) || !inv.IsItemSelected(0)) { std::printf("005bca87_TEST FAIL\n"); return 1; }
    std::printf("005bca87_TEST PASS\n");
    return 0;
}