#include <cstdio>
#include <cstdlib>
// behaviour test: CEngineWorldMap::AddMap rebuilds a map slot unless the UID is unchanged and pushes it to the visible list front
static int g_created = 0, g_rendererCreated = 0, g_detailCreated = 0, g_deleted = 0, g_shown = 0;
struct CIDrawMap {};
struct CDrawMapInfo { bool Valid; CIDrawMap* PMap; long PosX, PosY; unsigned long UID; long ExtentsX, ExtentsY; };
static CDrawMapInfo g_info;
struct CIDrawWorldMap { CDrawMapInfo GetMapInfo(long) const { return g_info; } };
struct CEngineMap;
struct CEngineLandscapeMap { void OnMapVisibilityChanged(bool v) { if (v) ++g_shown; } };
struct _List_node { _List_node* _M_next; _List_node* _M_prev; CEngineMap* _M_data; };
struct CEngineMap {
    CEngineLandscapeMap* LandscapeMap; void* LocalDetailCacheMap; const CIDrawMap* GameMap; long MapUID;
    _List_node* VisibleMapListIterator; bool IsInVisibleMapList;
    CEngineMap(const CIDrawMap* m, long, long, long, long, long uid, bool)
        : LandscapeMap(0), LocalDetailCacheMap(0), GameMap(m), MapUID(uid), VisibleMapListIterator(0), IsInVisibleMapList(false) { ++g_created; }
};
static CEngineLandscapeMap g_land;
struct CEngineMapList {
    _List_node* _M_node;
    _List_node* begin() { return _M_node->_M_next; }
    void push_front(CEngineMap* const& x)
    {
        _List_node* pos = begin();
        _List_node* tmp = (_List_node*)std::malloc(sizeof(_List_node));
        tmp->_M_data = x;
        _List_node* prev = pos->_M_prev;
        tmp->_M_next = pos; tmp->_M_prev = prev; prev->_M_next = tmp; pos->_M_prev = tmp;
    }
};
struct CEngineWorldMap {
    short* Map;
    CEngineMap* Slots[8]; unsigned long Count;
    CEngineMapList VisibleMapList;
    const CIDrawWorldMap* GameWorldMap;
    void DeleteMap(long i) { if (Slots[i]) { ++g_deleted; delete Slots[i]; Slots[i] = 0; } }
    void AddMap(long mapIndex, long flags);
};
void CEngineWorldMap::AddMap(long mapIndex, long flags)
{
    if (Map) { delete[] Map; Map = 0; }
    if ((unsigned long)mapIndex >= Count) { for (unsigned long i = Count; i < (unsigned long)mapIndex + 1; ++i) Slots[i] = 0; Count = mapIndex + 1; }
    CDrawMapInfo info = GameWorldMap->GetMapInfo(mapIndex);
    if (Slots[mapIndex] && (unsigned long)Slots[mapIndex]->MapUID == info.UID) return;
    DeleteMap(mapIndex);
    Slots[mapIndex] = new CEngineMap(info.PMap, info.PosX, info.PosY, info.ExtentsX, info.ExtentsY, info.UID, false);
    Slots[mapIndex]->LandscapeMap = &g_land;
    ++g_rendererCreated;
    if (!(flags & 1)) ++g_detailCreated;
    CEngineMap* map = Slots[mapIndex];
    if (!map->IsInVisibleMapList) {
        VisibleMapList.push_front(map);
        map->VisibleMapListIterator = VisibleMapList.begin();
        map->IsInVisibleMapList = true;
    }
    if (map->LandscapeMap) map->LandscapeMap->OnMapVisibilityChanged(true);
}
int main()
{
    _List_node head; head._M_next = &head; head._M_prev = &head;
    CIDrawWorldMap wm; CIDrawMap gm;
    g_info.Valid = true; g_info.PMap = &gm; g_info.UID = 42;
    CEngineWorldMap w; w.Map = new short[4]; w.Count = 1; w.Slots[0] = 0; w.VisibleMapList._M_node = &head; w.GameWorldMap = &wm;
    w.AddMap(3, 1);
    bool ok = w.Map == 0 && w.Count == 4 && g_created == 1 && w.Slots[3] && w.Slots[3]->MapUID == 42 && g_rendererCreated == 1
        && g_detailCreated == 0 && head._M_next->_M_data == w.Slots[3] && w.Slots[3]->VisibleMapListIterator == head._M_next
        && w.Slots[3]->IsInVisibleMapList && g_shown == 1;
    w.AddMap(3, 0);              // same UID: no rebuild
    ok = ok && g_created == 1 && g_deleted == 0;
    g_info.UID = 43;
    w.AddMap(3, 0);
    ok = ok && g_created == 2 && g_deleted == 1 && g_detailCreated == 1 && w.Slots[3]->MapUID == 43;
    if (!ok) { std::printf("00b42d10_TEST FAIL\n"); return 1; }
    std::printf("00b42d10_TEST PASS\n");
    return 0;
}