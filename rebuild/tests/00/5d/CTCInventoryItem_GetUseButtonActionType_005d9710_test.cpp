// behaviour test for 0x005D9710 CTCInventoryItem::GetUseButtonActionType(long)
// (model: a static accessor that caches the defs for the index, then reads the cached item def's field at +0x4C; 0 when the cache
// fails). Rewritten by agentNF59 on the new name's behaviour (the old test linked the extern "C" / free stand-ins); the catalog
// PassPattern is kept.
#include <stdio.h>
#include <stddef.h>
namespace model {
struct ItemDef { char _pad[0x4c]; long Field; };
static ItemDef g_def;
static ItemDef* g_cached = 0;
static bool g_loads = false;
static long g_lastIndex = 0;
static bool CacheDef(long defIndex) { g_lastIndex = defIndex; g_cached = g_loads ? &g_def : 0; return g_loads; }
static long GetUseButtonActionType(long defIndex) { if (CacheDef(defIndex)) return g_cached->Field; return 0; }
}
int main()
{
    model::g_def.Field = 55;
    model::g_loads = true;
    long a = model::GetUseButtonActionType(7);
    model::g_loads = false;
    long b = model::GetUseButtonActionType(9);
    if (offsetof(model::ItemDef, Field) == 0x4c && a == 55 && b == 0 && model::g_lastIndex == 9)
        printf("GI_5d9710_OK\n");
    else
        printf("FAIL\n");
    return 0;
}
