// behaviour test for 005d9c60: CTCInventoryItem::IsClothing(long) -- true only for a cached def whose inventory type is 0x12
#include <cstdio>
struct CInventoryItemDef { int InventoryType; };
static CInventoryItemDef g_defs[3];
static const CInventoryItemDef* s_CachedItemDef = 0;
static bool CacheDef(long defIndex)
{
    if (defIndex < 0 || defIndex >= 3) return false;
    s_CachedItemDef = &g_defs[defIndex];
    return true;
}
static int GetInventoryType(long defIndex)
{
    if (CacheDef(defIndex))
        return s_CachedItemDef->InventoryType;
    return 0;
}
static bool IsClothing(long defIndex) { return GetInventoryType(defIndex) == 0x12; }
int main()
{
    int bad = 0;
    g_defs[0].InventoryType = 0x12; g_defs[1].InventoryType = 0x11; g_defs[2].InventoryType = 0;
    if (!IsClothing(0) || IsClothing(1) || IsClothing(2) || IsClothing(3) || IsClothing(-1)) ++bad;
    if (bad) { std::printf("005d9c60_TEST FAIL\n"); return 1; }
    std::printf("005d9c60_TEST PASS\n");
    return 0;
}