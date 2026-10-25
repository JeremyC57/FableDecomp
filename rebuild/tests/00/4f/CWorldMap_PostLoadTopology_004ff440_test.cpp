// Behaviour test (0x004ff440): CWorldMap::PostLoadTopology marks the map's heights changed, inits passability for its area, updates edges and tells the engine
#include <cstdio>
class C2DBoxI { public: long TLX, TLY, BRX, BRY; };
static int g_step = 0, g_heights = 0, g_pass = 0, g_time = 0, g_edges = 0, g_loaded = 0, g_loadedMap = -1;
static const C2DBoxI* g_heightArg = (const C2DBoxI*)1; static long g_passTLX = 0;
class CMap { public: void HeightAreaChanged(const C2DBoxI* area) { g_heights = ++g_step; g_heightArg = area; } };
class CMapInfo { public: C2DBoxI Area; CMap* PMap; void* PMapInfo; };
template <class T> class CArray {
public:
    T* Start; T* Finish; T* EndOfStorage;
    T* begin() { return Start; }
    T& operator[](unsigned long i) { return *(begin() + i); }
};
class CEngineManager { public: void SetMapAsLoaded(long map) { g_loaded = ++g_step; g_loadedMap = map; } };
static CEngineManager g_engine;
template <class T> class CTBaseSingleton { public: static T* Get() { return &g_engine; } };
static double GFGetTime() { g_time = ++g_step; return 1.0; }
class CWorldMap {
public:
    CArray<CMapInfo> Maps;
    void InitPassabilityFromThemesIfNecessary(const C2DBoxI& area) { g_pass = ++g_step; g_passTLX = area.TLX; }
    void UpdateAllEdges() { g_edges = ++g_step; }
    void PostLoadTopology(long map)
    {
        Maps[map].PMap->HeightAreaChanged(0);
        InitPassabilityFromThemesIfNecessary(Maps[map].Area);
        GFGetTime();
        UpdateAllEdges();
        CTBaseSingleton<CEngineManager>::Get()->SetMapAsLoaded(map);
    }
};
int main()
{
    CMap maps[3];
    CMapInfo infos[3];
    for (int i = 0; i < 3; ++i) { infos[i].PMap = &maps[i]; infos[i].Area.TLX = 10 * i; infos[i].Area.TLY = 0; infos[i].Area.BRX = 0; infos[i].Area.BRY = 0; }
    CWorldMap wm; wm.Maps.Start = infos; wm.Maps.Finish = infos + 3; wm.Maps.EndOfStorage = infos + 3;
    wm.PostLoadTopology(2);
    if (g_heights != 1 || g_heightArg != 0 || g_pass != 2 || g_passTLX != 20 || g_time != 3 || g_edges != 4 || g_loaded != 5 || g_loadedMap != 2)
        { std::printf("004ff440_TEST FAIL\n"); return 1; }
    std::printf("004ff440_TEST PASS\n");
    return 0;
}