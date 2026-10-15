#include <cstdio>
// Behaviour test for CTCInventoryBase::UpdateNonQuestPersistentRefCountForItems (005bca46): same logic over a copy of the model.
static int g_calls = 0, g_trueCalls = 0;
struct CCategory { int id; void UpdateNonQuestPersistentRefCountForItems(bool increment) { ++g_calls; if (increment) ++g_trueCalls; } };
struct CCategoryVector {
    CCategory* first; CCategory* last; CCategory* end_of_storage;
    unsigned long size() const { return (unsigned long)(last - first); }
    CCategory& operator[](unsigned long n) { return first[n]; }
};
struct CTCInventoryBase_Retail {
    CCategoryVector Categories;
    void UpdateNonQuestPersistentRefCountForItems(bool increment) { for (unsigned long i = 0; i < Categories.size(); ++i) Categories[i].UpdateNonQuestPersistentRefCountForItems(increment); }
};
int main()
{
    CCategory cats[3];
    CTCInventoryBase_Retail inv; inv.Categories.first = cats; inv.Categories.last = cats + 3; inv.Categories.end_of_storage = cats + 3;
    inv.UpdateNonQuestPersistentRefCountForItems(true);
    inv.UpdateNonQuestPersistentRefCountForItems(false);
    if (g_calls != 6 || g_trueCalls != 3) { std::printf("005bca46_TEST FAIL\n"); return 1; }
    std::printf("005bca46_TEST PASS\n");
    return 0;
}