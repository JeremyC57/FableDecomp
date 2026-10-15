#include <cstdio>
struct CTCMapwho;
static int g_destroyed, g_baseDtors;
struct CThingMapManager { void DestroyMapwhoNode(CTCMapwho*) { ++g_destroyed; } };
struct CWorldMap { CThingMapManager* m_pThingMapManager; };
static CWorldMap* g_map;
struct CTCBase {
    CTCBase() { }
    virtual ~CTCBase() { ++g_baseDtors; }
    CWorldMap* GetWorldMap() { return g_map; }
};
struct CTCMapwho : CTCBase {
    unsigned char InMapwho : 1;
    CTCMapwho(bool in) { InMapwho = in ? 1 : 0; }
    void RemoveFromMapwho() { if (InMapwho) { CWorldMap* map = GetWorldMap(); map->m_pThingMapManager->DestroyMapwhoNode(this); } }
    virtual ~CTCMapwho() { RemoveFromMapwho(); }
};
int main()
{
    CThingMapManager mgr; CWorldMap map; map.m_pThingMapManager = &mgr; g_map = &map;
    CTCBase* a = new CTCMapwho(false); delete a;
    if (g_destroyed != 0 || g_baseDtors != 1) { std::printf("0071a810_TEST FAIL\n"); return 1; }
    CTCBase* b = new CTCMapwho(true); delete b;
    if (g_destroyed != 1 || g_baseDtors != 2) { std::printf("0071a810_TEST FAIL\n"); return 1; }
    std::printf("0071a810_TEST PASS\n");
    return 0;
}
