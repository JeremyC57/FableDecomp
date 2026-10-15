// Behaviour test for CLocalDetailCacheMap::CLocalDetailCacheMap(CEngineMap*) (0x00BDEFC0): unlocked, no file-block
// manager, the map stored, the root quad-tree element built over (0, 0, width, height) for this cache, empty containers.
#include <cstdio>
struct CEngineMap { unsigned short MapWidth; unsigned short MapHeight; };
struct CLocalDetailCacheMap;
static CLocalDetailCacheMap* g_qtMap = 0; static long g_qt[4];
struct CVec { void* a; void* b; void* c; CVec() : a(0), b(0), c(0) { } };
struct CLocalDetailCacheMap {
    struct CQuadTreeElement { int built; CQuadTreeElement(CLocalDetailCacheMap* m, long x, long y, long w, long h) : built(1) { g_qtMap = m; g_qt[0] = x; g_qt[1] = y; g_qt[2] = w; g_qt[3] = h; } };
    bool LockedFromMemoryRelocation; void* FileBlockManager; CEngineMap* Map; CQuadTreeElement QuadTree; CVec AreaUpdateList; CVec CollectionTypePalette;
    CLocalDetailCacheMap(CEngineMap* map)
        : LockedFromMemoryRelocation(false), FileBlockManager(0), QuadTree(this, 0, 0, map->MapWidth, map->MapHeight)
    {
        Map = map;
    }
};
int main()
{
    CEngineMap em; em.MapWidth = 40000; em.MapHeight = 512;
    CLocalDetailCacheMap c(&em);
    bool ok = !c.LockedFromMemoryRelocation && c.FileBlockManager == 0 && c.Map == &em && c.QuadTree.built == 1 && g_qtMap == &c
           && g_qt[0] == 0 && g_qt[1] == 0 && g_qt[2] == 40000 && g_qt[3] == 512
           && c.AreaUpdateList.a == 0 && c.AreaUpdateList.c == 0 && c.CollectionTypePalette.a == 0 && c.CollectionTypePalette.c == 0;
    if (!ok) { std::printf("00bdefc0_TEST FAIL\n"); return 1; }
    std::printf("00bdefc0_TEST PASS\n");
    return 0;
}