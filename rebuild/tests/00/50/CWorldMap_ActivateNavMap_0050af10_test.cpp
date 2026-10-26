// Behaviour test for CWorldMap::ActivateNavMap (0x0050AF10): an unloaded map is built from its area with the "NULL"
// name and added before activation; a loaded map is only activated.
#include <cstdio>
#include <cstring>
class CCharString { public: const char* Text; CCharString(const char* t) : Text(t) {} };
class C2DBoxI { public: long TLX, TLY, BRX, BRY; };
class CNavigationAreaInit { public: C2DBoxI WorldArea; long MapIndex; CNavigationAreaInit() : MapIndex(0) { WorldArea.TLX = WorldArea.TLY = WorldArea.BRX = WorldArea.BRY = 0; } };
static int g_log[8]; static int g_n = 0; static long g_addedArea = -1; static int g_nameOk = 0;
class CNavigatorManager { public:
    bool Loaded;
    bool IsNavigationMapLoaded(long) const { g_log[g_n++] = 1; return Loaded; }
    void AddNavigationMap(long map, const CNavigationAreaInit& init) { g_log[g_n++] = 3; g_addedArea = init.WorldArea.BRX + map; }
    void ActivateNavigationMap(long map) { g_log[g_n++] = 4 + (int)map * 10; }
};
class CWorld { public: CNavigatorManager* PNavigationManager; };
class CMapInfo { public: C2DBoxI Area; };
class CWorldMap { public:
    CWorld* World; CMapInfo* Maps;
    void GetMapNavigationAreaInit(long map, const C2DBoxI& area, const CCharString& name, CNavigationAreaInit& init)
    { g_log[g_n++] = 2; init.WorldArea = area; init.MapIndex = map; g_nameOk = std::strcmp(name.Text, "NULL") == 0; }
    void ActivateNavMap(long map)
    {
        if (!World->PNavigationManager->IsNavigationMapLoaded(map)) {
            CNavigationAreaInit init;
            GetMapNavigationAreaInit(map, Maps[map].Area, "NULL", init);
            World->PNavigationManager->AddNavigationMap(map, init);
        }
        World->PNavigationManager->ActivateNavigationMap(map);
    }
};
int main()
{
    CMapInfo maps[3] = { { { 0, 0, 10, 10 } }, { { 0, 0, 20, 20 } }, { { 0, 0, 30, 30 } } };
    CNavigatorManager nav = { false }; CWorld w = { &nav }; CWorldMap m = { &w, maps };
    m.ActivateNavMap(2);
    bool ok = g_n == 4 && g_log[0] == 1 && g_log[1] == 2 && g_log[2] == 3 && g_log[3] == 24 && g_addedArea == 32 && g_nameOk;
    nav.Loaded = true; g_n = 0;
    m.ActivateNavMap(1);
    ok = ok && g_n == 2 && g_log[0] == 1 && g_log[1] == 14;
    if (!ok) { std::printf("0050af10_TEST FAIL\n"); return 1; }
    std::printf("0050af10_TEST PASS\n");
    return 0;
}