#include <cstdio>
// Behaviour model of CTCInventory::AddContextSensitivePreferredItem (0x0058502D): a valid def the hero carries is
// appended {def, button, priority} unless already registered; returns whether it was added.
struct Item { long def, button, priority; };
static Item g_items[8]; static int g_count;
static bool Add(long defIndex, long carried, long button, long priority)
{
    if (defIndex > 0 && carried > 0) {
        for (int i = 0; i < g_count; ++i)
            if (g_items[i].def > 0 && g_items[i].def == defIndex) return false;
        Item it; it.def = defIndex; it.button = button; it.priority = priority;
        g_items[g_count++] = it;
        return true;
    }
    return false;
}
int main()
{
    if (!Add(7, 1, 2, 3) || g_count != 1 || g_items[0].button != 2 || g_items[0].priority != 3) { std::printf("0058502d_TEST FAIL\n"); return 1; }
    if (Add(7, 1, 4, 5) || Add(8, 0, 1, 1) || Add(0, 3, 1, 1) || g_count != 1) { std::printf("0058502d_TEST FAIL\n"); return 1; }
    if (!Add(9, 2, 0, 0) || g_count != 2) { std::printf("0058502d_TEST FAIL\n"); return 1; }
    std::printf("0058502d_TEST PASS\n");
    return 0;
}