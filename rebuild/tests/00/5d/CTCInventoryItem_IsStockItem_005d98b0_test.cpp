// behaviour test for 0x005D98B0 CTCInventoryItem::IsStockItem (model: false when the def does not cache, else the sub-def lookup result)
#include <cstdio>
static bool g_cache = true, g_has = true;
static bool CacheDef(long) { return g_cache; }
static bool GetSubDef() { return g_has; }
static bool Is(long i) { if (!CacheDef(i)) return false; return GetSubDef(); }
int main()
{
    int ok = Is(1); g_has = false; ok = ok && !Is(1); g_cache = false; g_has = true; ok = ok && !Is(1);
    if (!ok) { std::printf("005d98b0_TEST FAIL\n"); return 1; }
    std::printf("005d98b0_TEST PASS\n");
    return 0;
}