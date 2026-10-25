// 005022f0 behaviour test: CWorldMap::DeleteAllLevels (match.cpp body; the loaded-map swap-with-empty swaps each
// member through an std::swap template), host stand-ins. C++03, g++ -w; also VC7.1 /Od.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { printf("FAIL line %d: %s\n", __LINE__, #c); g_fail = 1; } } while (0)
static std::string g_log;
static void logs(const std::string& s) { g_log += s; g_log += ";"; }
static std::string num(long v) { char b[16]; sprintf(b, "%ld", v); return b; }

#include <algorithm> // std::swap (the host library already defines it in namespace std)
struct CThingManager { void DeleteAllThings() { logs("deleteall"); } };
struct CNavigatorManager { void RemoveNavigationMap(long map) { logs("nav-" + num(map)); } };
struct CEngineManager {
    static CEngineManager& Get() { static CEngineManager m; return m; }
    void RemoveMap(long map) { logs("eng-" + num(map)); }
};
struct CWorld {
    CThingManager Things;
    CNavigatorManager Nav;
    CNavigatorManager* PNavigationManager;
    CThingManager* GetThingManager() { return &Things; }
};
struct __false_type { };
struct CMapInfo {
    long Id;
    bool Cleared;
    void Clear() { Cleared = true; logs("clear-" + num(Id)); }
};
static int g_destroyed = 0;
static void DestroyRange(CMapInfo* first, CMapInfo* last, const __false_type&) { g_destroyed += (int)(last - first); }
struct CMapInfoVector {
    CMapInfo* _M_start;
    CMapInfo* _M_finish;
    CMapInfo* _M_end_of_storage;
    long size() const { return _M_finish - _M_start; }
    CMapInfo& operator[](long n) { return _M_start[n]; }
    CMapInfo* begin() { return _M_start; }
    CMapInfo* end() { return _M_finish; }
    static CMapInfo* copy(CMapInfo* first, CMapInfo* last, CMapInfo* result)
    {
        for (long n = last - first; n > 0; --n) {
            *result = *first;
            ++first;
            ++result;
        }
        return result;
    }
    CMapInfo* erase(CMapInfo* first, CMapInfo* last)
    {
        CMapInfo* i = copy(last, _M_finish, first);
        __false_type tag;
        DestroyRange(i, _M_finish, tag);
        _M_finish = i;
        return first;
    }
    void clear() { erase(begin(), end()); }
    void resize(unsigned int n)
    {
        for (CMapInfo* p = _M_finish; p < _M_start + n; ++p) { p->Id = -1; p->Cleared = false; }
        _M_finish = _M_start + n;
    }
};
struct CShortVector {
    unsigned short* _M_start;
    unsigned short* _M_finish;
    unsigned short* _M_end_of_storage;
    unsigned int size() const { return _M_finish - _M_start; }
    unsigned short& operator[](unsigned int n) { return _M_start[n]; }
};
static int g_frees = 0;
struct CLongArray {
    long* _M_start;
    long* _M_finish;
    long* _M_end_of_storage;
    unsigned int capacity() const { return _M_end_of_storage - _M_start; }
    static long* CopyDown(long* src, long* srcEnd, long* dest)
    {
        if (src == srcEnd)
            return dest;
        unsigned int bytes = (char*)srcEnd - (char*)src;
        return (long*)((char*)memmove(dest, src, bytes) + bytes);
    }
    void Erase(long* eraseFirst, long* eraseLast)
    {
        _M_finish = CopyDown(eraseLast, _M_finish, eraseFirst);
    }
    CLongArray() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    ~CLongArray() { if (_M_start) { ++g_frees; free(_M_start); } }
    void swap(CLongArray& x)
    {
        std::swap(_M_start, x._M_start);
        std::swap(_M_finish, x._M_finish);
        std::swap(_M_end_of_storage, x._M_end_of_storage);
    }
    void Reset()
    {
        if (capacity() > 0) {
            Erase(_M_start, _M_finish);
            CLongArray empty;
            swap(empty);
        }
    }
};
struct CWorldMap {
    CWorld*        World;
    CMapInfoVector Maps;
    CShortVector   MapPlacement;
    CLongArray     LoadedMaps;
    void DeleteAllLevels();
};
void CWorldMap::DeleteAllLevels()
{
    World->GetThingManager()->DeleteAllThings();
    for (long i = 1; i < Maps.size(); ++i) {
        World->PNavigationManager->RemoveNavigationMap(i);
        CEngineManager::Get().RemoveMap(i);
        Maps[i].Clear();
    }
    Maps.clear();
    Maps.resize(1);
    for (unsigned int j = 0; j < MapPlacement.size(); ++j)
        MapPlacement[j] = 0;
    LoadedMaps.Reset();
}

int main()
{
    CWorld world; world.PNavigationManager = &world.Nav;
    CMapInfo maps[4];
    for (int k = 0; k < 4; ++k) { maps[k].Id = k; maps[k].Cleared = false; }
    unsigned short grid[5] = { 1, 2, 3, 4, 5 };
    CWorldMap wm; wm.World = &world;
    wm.Maps._M_start = maps; wm.Maps._M_finish = maps + 3; wm.Maps._M_end_of_storage = maps + 4;
    wm.MapPlacement._M_start = grid; wm.MapPlacement._M_finish = grid + 5; wm.MapPlacement._M_end_of_storage = grid + 5;
    long* loaded = (long*)malloc(4 * sizeof(long)); loaded[0] = 7; loaded[1] = 8;
    wm.LoadedMaps._M_start = loaded; wm.LoadedMaps._M_finish = loaded + 2; wm.LoadedMaps._M_end_of_storage = loaded + 4;

    // maps 1 and 2 are removed from navigation and the engine and cleared (map 0 is kept), in order
    wm.DeleteAllLevels();
    CHECK(g_log == "deleteall;nav-1;eng-1;clear-1;nav-2;eng-2;clear-2;");
    CHECK(!maps[0].Cleared && maps[1].Cleared && maps[2].Cleared);
    // clear() destroys all 3, resize(1) leaves one fresh entry
    CHECK(g_destroyed == 3 && wm.Maps.size() == 1 && maps[0].Id == -1);
    // the placement grid is zeroed
    for (int k = 0; k < 5; ++k) CHECK(grid[k] == 0);
    // the loaded-map list is emptied and its storage released through the swapped-out temporary
    CHECK(wm.LoadedMaps._M_start == 0 && wm.LoadedMaps._M_finish == 0 && wm.LoadedMaps._M_end_of_storage == 0);
    CHECK(g_frees == 1);

    // a second call: only map 0 remains (no per-map work); the empty loaded list has no capacity, nothing freed
    g_log = ""; g_destroyed = 0;
    wm.DeleteAllLevels();
    CHECK(g_log == "deleteall;" && g_destroyed == 1 && wm.Maps.size() == 1 && g_frees == 1);

    // a loaded list with capacity but no entries is still released
    long* spare = (long*)malloc(2 * sizeof(long));
    wm.LoadedMaps._M_start = spare; wm.LoadedMaps._M_finish = spare; wm.LoadedMaps._M_end_of_storage = spare + 2;
    wm.DeleteAllLevels();
    CHECK(wm.LoadedMaps._M_start == 0 && g_frees == 2);

    if (!g_fail) printf("005022f0_TEST PASS\n");
    return g_fail;
}