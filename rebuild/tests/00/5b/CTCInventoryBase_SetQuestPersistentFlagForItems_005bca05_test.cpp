#include <cstdio>
// Behaviour test for CTCInventoryBase::SetQuestPersistentFlagForItems (005bca05): same logic over a copy of the model.
static int g_calls = 0, g_trueCalls = 0;
struct CCategory { int id; void SetQuestPersistentFlagForItems(bool flag) { ++g_calls; if (flag) ++g_trueCalls; } };
struct CCategoryVector {
    CCategory* first; CCategory* last; CCategory* end_of_storage;
    unsigned long size() const { return (unsigned long)(last - first); }
    CCategory& operator[](unsigned long n) { return first[n]; }
};
struct CTCInventoryBase_Retail {
    CCategoryVector Categories;
    void SetQuestPersistentFlagForItems(bool flag) { for (unsigned long i = 0; i < Categories.size(); ++i) Categories[i].SetQuestPersistentFlagForItems(flag); }
};
int main()
{
    CCategory cats[3];
    CTCInventoryBase_Retail inv; inv.Categories.first = cats; inv.Categories.last = cats + 3; inv.Categories.end_of_storage = cats + 3;
    inv.SetQuestPersistentFlagForItems(true);
    inv.SetQuestPersistentFlagForItems(false);
    if (g_calls != 6 || g_trueCalls != 3) { std::printf("005bca05_TEST FAIL\n"); return 1; }
    std::printf("005bca05_TEST PASS\n");
    return 0;
}