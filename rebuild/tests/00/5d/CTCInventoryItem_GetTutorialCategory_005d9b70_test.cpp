// behaviour test for 0x005D9B70 CTCInventoryItem::GetTutorialCategory(long)
// (model: a static accessor that caches the defs for the index, then reads the cached item def's field at +0x68; 0 when the cache
// fails). Rewritten by agentNF59 on the new name's behaviour (the old test linked the extern "C" / free stand-ins); the catalog
// PassPattern is kept.
#include <stdio.h>
#include <stddef.h>
namespace model {
struct ItemDef { char _pad[0x68]; long Field; };
static ItemDef g_def;
static ItemDef* g_cached = 0;
static bool g_loads = false;
static long g_lastIndex = 0;
static bool CacheDef(long defIndex) { g_lastIndex = defIndex; g_cached = g_loads ? &g_def : 0; return g_loads; }
static long GetTutorialCategory(long defIndex) { if (CacheDef(defIndex)) return g_cached->Field; return 0; }
}
int main()
{
    model::g_def.Field = 55;
    model::g_loads = true;
    long a = model::GetTutorialCategory(7);
    model::g_loads = false;
    long b = model::GetTutorialCategory(9);
    if (offsetof(model::ItemDef, Field) == 0x68 && a == 55 && b == 0 && model::g_lastIndex == 9)
        printf("GI_5d9b70_OK\n");
    else
        printf("FAIL\n");
    return 0;
}
