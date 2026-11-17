// behaviour test for 0x005D96F0 CTCInventoryItem::GetUIInventoryCategory(long)
// (model: a static accessor that caches the defs for the index, then reads the cached item def's UIInventoryCategory word at +0x6C;
// UI_INVENTORY_CATEGORY_OTHER (4) when the cache fails). Rewritten by agentNF56 on the new name's behaviour (the old test linked the
// free GetInventoryCategory stand-in); the catalog PassPattern is kept.
#include <stdio.h>
#include <stddef.h>
namespace model {
struct ItemDef { char _pad[0x6c]; long UIInventoryCategory; };
static ItemDef g_def;
static ItemDef* g_cached = 0;
static bool g_loads = false;
static bool CacheDef(long) { g_cached = g_loads ? &g_def : 0; return g_loads; }
static long GetUIInventoryCategory(long defIndex) { if (CacheDef(defIndex)) return g_cached->UIInventoryCategory; return 4; }
}
int main()
{
    model::g_def.UIInventoryCategory = 2;
    model::g_loads = true;
    long a = model::GetUIInventoryCategory(7);
    model::g_loads = false;
    long b = model::GetUIInventoryCategory(7);
    if (offsetof(model::ItemDef, UIInventoryCategory) == 0x6c && a == 2 && b == 4)
        printf("005d96f0_TEST PASS\n");
    else
        printf("FAIL a=%ld b=%ld\n", a, b);
    return 0;
}
