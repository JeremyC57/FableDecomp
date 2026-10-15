#include <cstdio>
// Behaviour test for CTCInventoryBase::OnKill (0x005BC9C8): same logic over a copy of the model.
static int g_visits = 0;
struct CItem { unsigned char _body[0x14]; void OnKill() { ++g_visits; } };
struct CItemVector {
    CItem* first; CItem* last; CItem* end_of_storage;
    unsigned long size() const { return (unsigned long)(last - first); }
    CItem& operator[](unsigned long n) { return first[n]; }
};
struct CCategory {
    unsigned char PAssociatedThing[0x8]; CItemVector ItemsInCategory;
    void OnKill() { for (unsigned long i = 0; i < ItemsInCategory.size(); ++i) ItemsInCategory[i].OnKill(); }
};
struct CCategoryVector {
    CCategory* first; CCategory* last; CCategory* end_of_storage;
    unsigned long size() const { return (unsigned long)(last - first); }
    CCategory& operator[](unsigned long n) { return first[n]; }
};
struct CTCInventoryBase_Retail {
    CCategoryVector Categories; bool Dying;
    void OnKill() { Dying = true; for (unsigned long i = 0; i < Categories.size(); ++i) Categories[i].OnKill(); }
};
int main()
{
    CItem items[5];
    CCategory cats[2];
    cats[0].ItemsInCategory.first = items; cats[0].ItemsInCategory.last = items + 2; cats[0].ItemsInCategory.end_of_storage = items + 2;
    cats[1].ItemsInCategory.first = items + 2; cats[1].ItemsInCategory.last = items + 5; cats[1].ItemsInCategory.end_of_storage = items + 5;
    CTCInventoryBase_Retail inv; inv.Dying = false;
    inv.Categories.first = cats; inv.Categories.last = cats + 2; inv.Categories.end_of_storage = cats + 2;
    inv.OnKill();
    if (!inv.Dying || g_visits != 5) { std::printf("005bc9c8_TEST FAIL\n"); return 1; }
    std::printf("005bc9c8_TEST PASS\n");
    return 0;
}