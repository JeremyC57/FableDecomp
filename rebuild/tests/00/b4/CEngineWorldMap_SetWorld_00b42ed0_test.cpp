#include <cstdio>
#include <cstring>

static int g_stringDtors;
struct CCharString { void* PStringData; ~CCharString() { ++g_stringDtors; } };
struct CIDrawMap { int id; };
struct CDrawMapInfo {
    bool Valid, Loadable, GameLoadable; CIDrawMap* PMap; CCharString StaticMeshName;
    long PosX, PosY; unsigned long UID; long ExtentsX, ExtentsY;
};
struct CIDrawWorldMap {
    virtual long GetMapCount() const = 0;
    virtual CDrawMapInfo GetMapInfo(long index) const = 0;
};
struct CIDrawWorld {
    virtual void v00() const {}
    virtual void v01() const {}
    virtual const CIDrawWorldMap* GetWorldMap() const = 0;
};
static int g_landscapeCalls;
struct CEngineLandscapeMap { void OnGameMapLoaded() { ++g_landscapeCalls; } };
struct CEngineMap {
    CEngineLandscapeMap* LandscapeMap; void* LocalDetailCacheMap; const CIDrawMap* GameMap;
    void OnGameMapLoaded(long mapIndex);
};
static CEngineMap* g_storage[8];
template <class T> struct CArray {
    T* First; T* Last; T* End;
    void resize(unsigned long count, const T& value) { First = (T*)g_storage; Last = First + count; End = Last; for (unsigned long i = 0; i < count; ++i) First[i] = value; }
    T& operator[](long index) { return First[index]; }
};
static char g_log[64];
static void Log(char c) { size_t n = std::strlen(g_log); g_log[n] = c; g_log[n + 1] = 0; }
static CEngineMap g_maps[8];
static CEngineLandscapeMap g_landscape;
struct CEngineWorldMap {
    CArray<CEngineMap*> MapList;
    const CIDrawWorld* GameWorld;
    const CIDrawWorldMap* GameWorldMap;
    void SetWorld(const CIDrawWorld* world);
    void OnGameMapLoaded(long mapIndex) { MapList[mapIndex]->OnGameMapLoaded(mapIndex); }
    const CIDrawWorldMap* PeekGameWorldMap() const { return GameWorldMap; }
    void AddMap(long mapIndex, long openState) { Log((char)('0' + mapIndex)); if (openState) Log('!'); MapList[mapIndex] = &g_maps[mapIndex]; g_maps[mapIndex].LandscapeMap = &g_landscape; g_maps[mapIndex].GameMap = 0; }
    void DeleteMap() { Log('X'); }
    void AutoBuildWorldMapGrid() { Log('G'); }
};
static CEngineWorldMap* g_pEngineWorldMap;
inline void CEngineMap::OnGameMapLoaded(long mapIndex)
{
    CDrawMapInfo info = g_pEngineWorldMap->PeekGameWorldMap()->GetMapInfo(mapIndex);
    GameMap = info.PMap;
    LandscapeMap->OnGameMapLoaded();
}
void CEngineWorldMap::SetWorld(const CIDrawWorld* world)
{
    if (world == GameWorld) return;
    GameWorld = world;
    GameWorldMap = world->GetWorldMap();
    DeleteMap();
    long count = GameWorldMap->GetMapCount();
    MapList.resize(count, 0);
    long i;
    for (i = 1; i < count; ++i) {
        CDrawMapInfo info = GameWorldMap->GetMapInfo(i);
        if (info.Valid) AddMap(i, 0);
    }
    for (i = 1; i < count; ++i) {
        CDrawMapInfo info = GameWorldMap->GetMapInfo(i);
        if (info.Valid && info.PMap) OnGameMapLoaded(i);
    }
    AutoBuildWorldMapGrid();
}

static CIDrawMap g_gameMaps[8];
struct CTestWorldMap : CIDrawWorldMap {
    long GetMapCount() const { return 5; }
    CDrawMapInfo GetMapInfo(long index) const
    {
        CDrawMapInfo info; std::memset(&info, 0, sizeof(info));
        info.Valid = (index != 2);                       // map 2 invalid
        info.PMap = (index == 3 || index == 2) ? 0 : &g_gameMaps[index];   // map 3 valid but no game map
        return info;
    }
};
static CTestWorldMap g_worldMap;
struct CTestWorld : CIDrawWorld { const CIDrawWorldMap* GetWorldMap() const { return &g_worldMap; } };

int main()
{
    CTestWorld world;
    CEngineWorldMap ewm; std::memset(&ewm, 0, sizeof(ewm));
    g_pEngineWorldMap = &ewm;
    ewm.SetWorld(&world);
    if (std::strcmp(g_log, "X134G") != 0 || ewm.GameWorld != &world || ewm.GameWorldMap != &g_worldMap ||
        ewm.MapList.Last - ewm.MapList.First != 5 || ewm.MapList.First[0] != 0 || ewm.MapList.First[2] != 0 ||
        g_maps[1].GameMap != &g_gameMaps[1] || g_maps[4].GameMap != &g_gameMaps[4] || g_maps[3].GameMap != 0 ||
        g_landscapeCalls != 2 || g_stringDtors < 10) {   // >= 10: VC7.1 has no NRVO, GetMapInfo copies out
        std::printf("00b42ed0_TEST FAIL\n"); return 1;
    }
    g_log[0] = 0;
    ewm.SetWorld(&world);                                // same world: nothing happens
    if (g_log[0] != 0) { std::printf("00b42ed0_TEST FAIL\n"); return 1; }
    std::printf("00b42ed0_TEST PASS\n");
    return 0;
}