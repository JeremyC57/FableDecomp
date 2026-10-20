// Behaviour test for CWorldMap::StartMovePlayerPos 0x00502500: region swap vs unload/force-load, then record the position.
#include <cstdio>

struct C3DVector { float X, Y, Z; };
struct CPlayerPositionInfo { bool Set; C3DVector Pos; };
template <class T> struct CVector {
    T* _M_start; T* _M_finish; T* _M_end_of_storage;
    T* begin() { return _M_start; }
    T& operator[](unsigned long n) { return *(begin() + n); }
};
enum ELoadType { LOAD_TYPE_DEFAULT = 0, LOAD_TYPE_X = 2 };
static bool g_bRegionLoadingEnabled = true;
static long g_unloadedRegion = -1, g_loadedRegion = -1, g_forced = -1; static ELoadType g_loadType = LOAD_TYPE_DEFAULT;
static int g_unloadAll = 0; static bool g_mapLoaded = false;

struct CWorldMap {
    CVector<CPlayerPositionInfo> PlayerPositionInfo;
    virtual void ForceLoad(long map) { g_forced = map; }
    virtual bool IsMapLoaded(long) const { return g_mapLoaded; }
    virtual long GetMapNumberAt(const C3DVector& pos) const { return (long)(pos.X / 100.0f) + 1; }   // 100-unit maps
    virtual ~CWorldMap() {}
    long GetRegionNumberMapIsIn(long map) const { return map <= 2 ? 10 : 20; }
    void UnloadRegion(long region, bool) { g_unloadedRegion = region; }
    void LoadRegion(long region, ELoadType loadType, bool) { g_loadedRegion = region; g_loadType = loadType; }
    void UnloadAllMapsExceptPlayerPos() { ++g_unloadAll; }
    void StartMovePlayerPos(long player, const C3DVector& pos, ELoadType loadType);
};

void CWorldMap::StartMovePlayerPos(long player, const C3DVector& pos, ELoadType loadType)
{
    long newMap = GetMapNumberAt(pos);
    if (g_bRegionLoadingEnabled)
    {
        long oldRegion = GetRegionNumberMapIsIn(GetMapNumberAt(PlayerPositionInfo[player].Pos));
        long newRegion = GetRegionNumberMapIsIn(newMap);
        if (oldRegion != newRegion)
        {
            UnloadRegion(oldRegion, true);
            LoadRegion(newRegion, loadType, true);
        }
    }
    else
    {
        UnloadAllMapsExceptPlayerPos();
        if (!IsMapLoaded(newMap))
            ForceLoad(newMap);
    }
    PlayerPositionInfo[player].Set = true;
    PlayerPositionInfo[player].Pos = pos;
}

int main()
{
    CPlayerPositionInfo infos[2] = { { false, { 50, 0, 0 } }, { false, { 150, 0, 0 } } };
    CWorldMap w; w.PlayerPositionInfo._M_start = infos; w.PlayerPositionInfo._M_finish = infos + 2; w.PlayerPositionInfo._M_end_of_storage = infos + 2;
    bool ok = true;
    C3DVector p1 = { 120, 5, 6 };                         // map 2, same region 10 as map 1
    w.StartMovePlayerPos(0, p1, LOAD_TYPE_X);
    if (g_unloadedRegion != -1 || !infos[0].Set || infos[0].Pos.X != 120 || infos[0].Pos.Z != 6) ok = false;
    C3DVector p2 = { 350, 0, 0 };                         // map 4, region 20
    w.StartMovePlayerPos(0, p2, LOAD_TYPE_X);
    if (g_unloadedRegion != 10 || g_loadedRegion != 20 || g_loadType != LOAD_TYPE_X || infos[0].Pos.X != 350) ok = false;
    g_bRegionLoadingEnabled = false; g_mapLoaded = false;
    w.StartMovePlayerPos(1, p1, LOAD_TYPE_DEFAULT);
    if (g_unloadAll != 1 || g_forced != 2 || !infos[1].Set || infos[1].Pos.X != 120) ok = false;
    g_mapLoaded = true; g_forced = -1;
    w.StartMovePlayerPos(1, p2, LOAD_TYPE_DEFAULT);
    if (g_unloadAll != 2 || g_forced != -1) ok = false;
    if (!ok) { std::printf("00502500_TEST FAIL\n"); return 1; }
    std::printf("00502500_TEST PASS\n");
    return 0;
}