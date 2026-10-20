// Behaviour test for CWorldMap::UnloadLevel 0x004FEDE0: loaded-list removal, map pointer release and the manager calls.
#include <cstdio>
#include <cstring>

struct CCPPointerInfo { long Count; void (*Destroy)(void* data); void* Data; };
static int g_destroyed = 0;
static void DestroyMap(void*) { ++g_destroyed; }
template<class T> struct CCountedPointer {
    T* Data; CCPPointerInfo* Info;
    void Clear()
    {
        if (Info) { --Info->Count; if (Info->Count == 0) { Info->Destroy(Info->Data); delete Info; } }
        Info = 0; Data = 0;
    }
};
struct CIteratorTag { CIteratorTag() { } };
static long* Find(long* first, long* last, const long& value, const CIteratorTag&)
{
    for (; first != last; ++first) if (*first == value) return first;
    return last;
}
struct CLongArray {
    long* Start; long* Finish; long* EndOfStorage;
    long* Erase(long* position)
    {
        long* next = position + 1;
        if (Finish != next) std::memmove(position, next, (char*)Finish - (char*)next);
        --Finish;
        return position;
    }
};
struct CMap { int dummy; };
struct CMapInfo { CCountedPointer<CMap> PMap; bool Activated; };
static long g_engineRemoved = -1, g_thingUnload = -1, g_thingMapRemoved = -1, g_navRemoved = -1; static bool g_unloadFlag = false;
static bool g_navLoaded = false;
struct CEngineManager { static CEngineManager& Get() { static CEngineManager m; return m; } void RemoveMap(long m) { g_engineRemoved = m; } };
struct CThingManager { void OnLevelUnload(long m, bool b) { g_thingUnload = m; g_unloadFlag = b; } };
struct CThingMapManager { void RemoveGameMap(long m) { g_thingMapRemoved = m; } };
struct CNavigatorManager {
    bool IsNavigationMapLoaded(long) const { return g_navLoaded; }
    void RemoveNavigationMap(long m) { g_navRemoved = m; }
};
static CThingManager g_tm;
struct CWorld { CNavigatorManager* NavigatorManager; virtual CThingManager* GetThingManager() { return &g_tm; } virtual ~CWorld() {} };
struct CWorldMap {
    CWorld* World; CThingMapManager* PThingMapManager; CMapInfo* Maps; CLongArray LoadedMaps;
    void UnloadLevel(long map);
};

void CWorldMap::UnloadLevel(long map)
{
    Maps[map].Activated = false;
    LoadedMaps.Erase(Find(LoadedMaps.Start, LoadedMaps.Finish, map, CIteratorTag()));
    CEngineManager::Get().RemoveMap(map);
    World->GetThingManager()->OnLevelUnload(map, true);
    Maps[map].PMap.Clear();
    PThingMapManager->RemoveGameMap(map);
    if (World->NavigatorManager->IsNavigationMapLoaded(map))
        World->NavigatorManager->RemoveNavigationMap(map);
}

int main()
{
    CMap mapObj; CMapInfo maps[3];
    for (int i = 0; i < 3; ++i) { maps[i].Activated = true; maps[i].PMap.Data = 0; maps[i].PMap.Info = 0; }
    CCPPointerInfo* info = new CCPPointerInfo; info->Count = 1; info->Destroy = DestroyMap; info->Data = &mapObj;
    maps[2].PMap.Data = &mapObj; maps[2].PMap.Info = info;
    long loaded[4] = { 1, 2, 0, 0 };
    CNavigatorManager nav; CWorld world; world.NavigatorManager = &nav; CThingMapManager tmm;
    CWorldMap w; w.World = &world; w.PThingMapManager = &tmm; w.Maps = maps;
    w.LoadedMaps.Start = loaded; w.LoadedMaps.Finish = loaded + 2; w.LoadedMaps.EndOfStorage = loaded + 4;
    bool ok = true;
    g_navLoaded = true;
    w.UnloadLevel(1);
    if (maps[1].Activated || w.LoadedMaps.Finish != loaded + 1 || loaded[0] != 2) ok = false;
    if (g_engineRemoved != 1 || g_thingUnload != 1 || !g_unloadFlag || g_thingMapRemoved != 1 || g_navRemoved != 1) ok = false;
    g_navLoaded = false; g_navRemoved = -1;
    w.UnloadLevel(2);
    if (w.LoadedMaps.Finish != loaded || g_destroyed != 1 || maps[2].PMap.Data || maps[2].PMap.Info || g_navRemoved != -1) ok = false;
    if (!ok) { std::printf("004fede0_TEST FAIL\n"); return 1; }
    std::printf("004fede0_TEST PASS\n");
    return 0;
}