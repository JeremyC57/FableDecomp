// Behaviour test (0x004ff8b0): CWorldMap::RemoveLevel unloads a loaded map, clears its placement cells, tells the engine and marks the slot unused
#include <cstdio>
class C2DBoxI { public: long TLX, TLY, BRX, BRY; };
class CMapInfo { public: C2DBoxI Area; bool Used; bool Loaded; };
template <class T> class CArray {
public:
    T* Start; T* Finish; T* EndOfStorage;
    T* begin() { return Start; }
    T& operator[](unsigned long i) { return *(begin() + i); }
};
static int g_unloads = 0, g_placement = 0, g_from = -1, g_to = -1, g_removed = -1; static bool g_usedAtRemove = false;
class CWorldMap;
static CWorldMap* g_wm = 0;
class CEngineManager { public: void RemoveMap(long map); };
static CEngineManager g_engine;
template <class T> class CTBaseSingleton { public: static T* Get() { return &g_engine; } };
class CWorldMap {
public:
    CArray<CMapInfo> Maps;
    virtual bool IsMapLoaded(long map) const { return const_cast<CWorldMap*>(this)->Maps[map].Loaded; }
    virtual ~CWorldMap() { }
    void UnloadLevel(long map) { ++g_unloads; Maps[map].Loaded = false; }
    void ChangeMapPlacementValuesTo(long from, long to) { ++g_placement; g_from = from; g_to = to; }
    void RemoveLevel(long map)
    {
        CMapInfo& info = Maps[map];
        if (IsMapLoaded(map))
            UnloadLevel(map);
        ChangeMapPlacementValuesTo(map, 0);
        CTBaseSingleton<CEngineManager>::Get()->RemoveMap(map);
        info.Used = false;
    }
};
void CEngineManager::RemoveMap(long map) { g_removed = map; g_usedAtRemove = g_wm->Maps[map].Used; }
int main()
{
    CMapInfo infos[3];
    for (int i = 0; i < 3; ++i) { infos[i].Used = true; infos[i].Loaded = (i == 2); }
    CWorldMap wm; g_wm = &wm; wm.Maps.Start = infos; wm.Maps.Finish = infos + 3; wm.Maps.EndOfStorage = infos + 3;
    wm.RemoveLevel(1);                          // not loaded: no unload
    if (g_unloads != 0 || g_placement != 1 || g_from != 1 || g_to != 0 || g_removed != 1 || !g_usedAtRemove || infos[1].Used)
        { std::printf("004ff8b0_TEST FAIL\n"); return 1; }
    wm.RemoveLevel(2);
    if (g_unloads != 1 || infos[2].Loaded || g_placement != 2 || g_removed != 2 || infos[2].Used || !infos[0].Used)
        { std::printf("004ff8b0_TEST FAIL\n"); return 1; }
    std::printf("004ff8b0_TEST PASS\n");
    return 0;
}