#include <cstdio>
#include <cstdlib>
#include <cstring>
// behaviour test: CEngineWorldMap::DeleteMap tears one map down and trims trailing empty slots
static int g_unloaded = 0, g_hidden = 0, g_unlinked = 0, g_rendererDeleted = 0, g_detailDeleted = 0, g_destroyed = 0;
class CEngineMap;
class CEngineLandscapeMap { public: void OnGameMapUnloaded() { ++g_unloaded; } void OnMapVisibilityChanged(bool v) { if (!v) ++g_hidden; } };
class CLocalDetailCacheMap { public: void OnMapVisibilityChanged(bool v) { if (!v) ++g_hidden; } };
struct _List_node { _List_node* _M_next; _List_node* _M_prev; CEngineMap* _M_data; };
class CEngineMap { public:
    CEngineLandscapeMap* LandscapeMap; CLocalDetailCacheMap* LocalDetailCacheMap; void* GameMap;
    _List_node* VisibleMapListIterator; bool IsInVisibleMapList;
    ~CEngineMap() { ++g_destroyed; }
    void ReleaseGameMap() { if (LandscapeMap) LandscapeMap->OnGameMapUnloaded(); GameMap = 0; }
};
static bool g_haveRenderers = true;
template <class T> class CArray { public:
    T* _M_start; T* _M_finish; T* _M_end_of_storage;
    unsigned long size() const { return (unsigned long)(_M_finish - _M_start); }
    bool empty() const { return size() == 0; }
    T& operator[](unsigned long i) { return _M_start[i]; }
    void resize(unsigned long n) { if (n < size()) _M_finish = _M_start + n; }
};
class CEngineWorldMap { public:
    short* Map;
    CArray<CEngineMap*> MapList;
    _List_node* VisibleMapList;
    void UnlinkMapFromWorld(CEngineMap*, long) { ++g_unlinked; }
    void DeleteMap(long index);
};
void CEngineWorldMap::DeleteMap(long index)
{
    if (MapList[index] == 0) return;
    MapList[index]->ReleaseGameMap();
    CEngineMap* map = MapList[index];
    if (map->IsInVisibleMapList) {
        _List_node* node = map->VisibleMapListIterator;
        _List_node* next = node->_M_next; _List_node* prev = node->_M_prev;
        prev->_M_next = next; next->_M_prev = prev;
        std::free(node);
        map->VisibleMapListIterator = VisibleMapList;
        map->IsInVisibleMapList = false;
    }
    if (map->LandscapeMap) map->LandscapeMap->OnMapVisibilityChanged(false);
    if (map->LocalDetailCacheMap) map->LocalDetailCacheMap->OnMapVisibilityChanged(false);
    if (Map) UnlinkMapFromWorld(MapList[index], index);
    if (g_haveRenderers) ++g_rendererDeleted;
    if (g_haveRenderers) ++g_detailDeleted;
    delete MapList[index];
    MapList[index] = 0;
    while (!MapList.empty() && MapList[MapList.size() - 1] == 0)
        MapList.resize(MapList.size() - 1);
}
int main()
{
    CEngineLandscapeMap land; CLocalDetailCacheMap detail;
    _List_node head; _List_node* node = (_List_node*)std::malloc(sizeof(_List_node));
    head._M_next = node; head._M_prev = node; node->_M_next = &head; node->_M_prev = &head;
    CEngineMap* a = new CEngineMap; a->LandscapeMap = &land; a->LocalDetailCacheMap = &detail; a->GameMap = &head;
    a->VisibleMapListIterator = node; a->IsInVisibleMapList = true;
    CEngineMap* list[4] = { 0, 0, 0, a };
    short grid[4];
    CEngineWorldMap w; w.Map = grid; w.MapList._M_start = list; w.MapList._M_finish = list + 4; w.MapList._M_end_of_storage = list + 4;
    w.VisibleMapList = &head;
    w.DeleteMap(3);
    bool ok = g_unloaded == 1 && g_hidden == 2 && g_unlinked == 1 && g_destroyed == 1 && head._M_next == &head
        && head._M_prev == &head && w.MapList.size() == 0 && g_rendererDeleted == 1;
    w.MapList._M_finish = list + 4;
    w.DeleteMap(2);     // empty slot: nothing happens
    ok = ok && w.MapList.size() == 4 && g_destroyed == 1;
    if (!ok) { std::printf("00b42b10_TEST FAIL\n"); return 1; }
    std::printf("00b42b10_TEST PASS\n");
    return 0;
}