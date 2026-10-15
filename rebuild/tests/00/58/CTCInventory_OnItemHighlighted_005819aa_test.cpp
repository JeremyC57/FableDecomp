#include <cstdio>
// Behaviour test for CTCInventory::OnItemHighlighted(long) (retail 0x005819AA): five base calls in
// order, each with the item.
static char g_log[16];
static int g_n = 0;
static long g_items[8];

struct CTCInventoryBase {
    void Note(char c, long item) { if (g_n < 8) { g_log[g_n] = c; g_items[g_n] = item; } ++g_n; }
    void ResetInventoryAnim(long item) { Note('R', item); }
    void OnItemHighlighted(long item) { Note('H', item); }
    void SetCanItemBeQuickAssigned(long item) { Note('Q', item); }
    void SetCanItemBeUsed(long item) { Note('U', item); }
    void SetNumberOfSelectedItemOwned(long item) { Note('N', item); }
};

struct CTCInventory : CTCInventoryBase {
    void OnItemHighlighted(long item);
};

void CTCInventory::OnItemHighlighted(long item)
{
    ResetInventoryAnim(item);
    CTCInventoryBase::OnItemHighlighted(item);
    SetCanItemBeQuickAssigned(item);
    SetCanItemBeUsed(item);
    SetNumberOfSelectedItemOwned(item);
}

int main()
{
    CTCInventory inv;
    inv.OnItemHighlighted(42);
    const char* want = "RHQUN";
    if (g_n != 5) { std::printf("005819aa_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 5; ++i)
        if (g_log[i] != want[i] || g_items[i] != 42) { std::printf("005819aa_TEST FAIL\n"); return 1; }
    std::printf("005819aa_TEST PASS\n");
    return 0;
}