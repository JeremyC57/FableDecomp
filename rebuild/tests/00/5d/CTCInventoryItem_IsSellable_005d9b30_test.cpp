// behaviour test for 0x005D9B30 CTCInventoryItem::IsSellable(long)
// (model: a static accessor that caches the defs for the index, then reads the cached item def's field at +0x5C; false when the cache
// fails). Rewritten by agentNF59 on the new name's behaviour (the old test linked the extern "C" / free stand-ins); the catalog
// PassPattern is kept.
#include <stdio.h>
#include <stddef.h>
namespace model {
struct ItemDef { char _pad[0x5c]; bool Field; };
static ItemDef g_def;
static ItemDef* g_cached = 0;
static bool g_loads = false;
static long g_lastIndex = 0;
static bool CacheDef(long defIndex) { g_lastIndex = defIndex; g_cached = g_loads ? &g_def : 0; return g_loads; }
static bool IsSellable(long defIndex) { if (CacheDef(defIndex)) return g_cached->Field; return false; }
}
int main()
{
    model::g_def.Field = true;
    model::g_loads = true;
    bool a = model::IsSellable(7);
    model::g_loads = false;
    bool b = model::IsSellable(9);
    if (offsetof(model::ItemDef, Field) == 0x5c && a == true && b == false && model::g_lastIndex == 9)
        printf("IB_5d9b30_OK\n");
    else
        printf("FAIL\n");
    return 0;
}
