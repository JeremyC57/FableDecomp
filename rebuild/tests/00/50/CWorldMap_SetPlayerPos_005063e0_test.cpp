// Behaviour test for CWorldMap::SetPlayerPos 0x005063E0: region/level loading decisions, position record, CacheMaps.
#include <cstdio>

class C3DVector { public: float X, Y, Z; };
struct CPlayerPositionInfo { bool Set; C3DVector Pos; };
template <class T> struct CVector {
    T* _M_start; T* _M_finish; T* _M_end_of_storage;
    T* begin() { return _M_start; }
    T& operator[](unsigned long n) { return *(begin() + n); }
};
enum ELoadType { LOAD_TYPE_DEFAULT = 0, LOAD_TYPE_X = 3 };
static bool g_bRegionLoadingEnabled = true;
static long g_unloadedRegion = -1, g_loadedRegion = -1, g_loadedLevel = -1; static int g_cache = 0;
static bool g_mapLoaded = false;

class CWorldMap { public:
    CVector<CPlayerPositionInfo> PlayerPositionInfo;
    virtual bool IsMapLoaded(long) const { return g_mapLoaded; }
    virtual long GetMapNumberAt(const C3DVector& pos) const { return (long)(pos.X / 100.0f); }   // map 0 = none
    virtual ~CWorldMap() {}
    long GetRegionNumberMapIsIn(long map) const { return map == 0 ? 0 : (map == 5 ? 0 : (map <= 2 ? 10 : 20)); }
    void UnloadRegion(long region, bool) { g_unloadedRegion = region; }
    void LoadRegion(long region, ELoadType, bool) { g_loadedRegion = region; }
    void LoadLevel(long map, ELoadType) { g_loadedLevel = map; }
    void CacheMaps(ELoadType) { ++g_cache; }
    void SetPlayerPos(long player, const C3DVector& pos, ELoadType loadType);
};

void CWorldMap::SetPlayerPos(long player, const C3DVector& pos, ELoadType loadType)
{
    if (g_bRegionLoadingEnabled)
    {
        long newMap = GetMapNumberAt(pos);
        if (!IsMapLoaded(newMap))
        {
            long newRegion = GetRegionNumberMapIsIn(newMap);
            if (PlayerPositionInfo[player].Set)
            {
                long oldRegion = GetRegionNumberMapIsIn(GetMapNumberAt(PlayerPositionInfo[player].Pos));
                if (oldRegion > 0 && oldRegion != newRegion)
                    UnloadRegion(oldRegion, true);
            }
            if (newRegion > 0)
                LoadRegion(newRegion, loadType, true);
            else if (newMap > 0)
                LoadLevel(newMap, loadType);
        }
    }
    PlayerPositionInfo[player].Set = true;
    PlayerPositionInfo[player].Pos = pos;
    CacheMaps(loadType);
}

int main()
{
    CPlayerPositionInfo infos[1] = { { false, { 150, 0, 0 } } };
    CWorldMap w; w.PlayerPositionInfo._M_start = infos; w.PlayerPositionInfo._M_finish = infos + 1; w.PlayerPositionInfo._M_end_of_storage = infos + 1;
    bool ok = true;
    C3DVector p = { 350, 1, 2 };                   // map 3 -> region 20
    w.SetPlayerPos(0, p, LOAD_TYPE_X);             // no previous position: just load region 20
    if (g_unloadedRegion != -1 || g_loadedRegion != 20 || !infos[0].Set || infos[0].Pos.X != 350 || g_cache != 1) ok = false;
    C3DVector q = { 150, 0, 0 };                   // map 1 -> region 10, previous region 20 unloaded
    w.SetPlayerPos(0, q, LOAD_TYPE_X);
    if (g_unloadedRegion != 20 || g_loadedRegion != 10 || infos[0].Pos.X != 150) ok = false;
    C3DVector r = { 550, 0, 0 };                   // map 5 has no region -> LoadLevel
    w.SetPlayerPos(0, r, LOAD_TYPE_X);
    if (g_loadedLevel != 5 || g_unloadedRegion != 10) ok = false;
    g_mapLoaded = true; g_loadedRegion = -1;
    w.SetPlayerPos(0, p, LOAD_TYPE_X);
    if (g_loadedRegion != -1 || infos[0].Pos.X != 350 || g_cache != 4) ok = false;
    if (!ok) { std::printf("005063e0_TEST FAIL\n"); return 1; }
    std::printf("005063e0_TEST PASS\n");
    return 0;
}