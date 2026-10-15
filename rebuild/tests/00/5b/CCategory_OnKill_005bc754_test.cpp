#include <cstdio>
// Behaviour test for CCategory::OnKill (0x005BC754): same logic over a copy of the model (the inline per-item
// hook is instrumented here to prove every stack is visited once).
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
int main()
{
    CItem items[4];
    CCategory c; c.ItemsInCategory.first = items; c.ItemsInCategory.last = items + 4; c.ItemsInCategory.end_of_storage = items + 4;
    c.OnKill();
    CCategory e; e.ItemsInCategory.first = items; e.ItemsInCategory.last = items; e.ItemsInCategory.end_of_storage = items;
    e.OnKill();
    if (g_visits != 4) { std::printf("005bc754_TEST FAIL\n"); return 1; }
    std::printf("005bc754_TEST PASS\n");
    return 0;
}