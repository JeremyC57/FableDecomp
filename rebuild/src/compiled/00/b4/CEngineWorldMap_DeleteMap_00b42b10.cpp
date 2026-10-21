// CEngineWorldMap::DeleteMap(long index) -- tear down one engine map: detach its game map, drop it from the visible-map list, tell its landscape and local-detail maps it is hidden, unlink it from the world grid, the landscape renderer and the local-detail generator, delete it, clear its slot and trim trailing empty slots from MapList
// Leaf: ?DeleteMap@CEngineWorldMap
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B42B10, length 352 bytes (with int3 padding; body 0x156), `ret 4`.
// PDB: ?DeleteMap@CEngineWorldMap@@QAEXJ@Z.
// Callees: CEngineLandscapeMap @ 0x00BDBFF0 (no-argument member; manifest "$E2", named OnGameMapUnloaded after the PDB
//   method list, unconfirmed), CEngineLandscapeMap::OnMapVisibilityChanged(bool) @ 0x00BDC3C0, CLocalDetailCacheMap::
//   OnMapVisibilityChanged(bool) @ 0x00BDE160, CEngineWorldMap::UnlinkMapFromWorld(CEngineMap*, long) @ 0x00B3EE80,
//   CEngineLandscapeRenderer::DeleteMap(CEngineMap*) @ 0x00B67180 (global @ 0x01436EA8), the local-detail generator's
//   map removal @ 0x00B59840 (global @ 0x01436E88; manifest label "WorldFrameUpdate" is propagated), ~CEngineMap @ 0x00B42A90
//   (manifest "~CBankFileEntryUpdateData", propagated), operator delete @ 0x00BFE9BC, free @ 0x00BFEA14 (list node),
//   memmove @ 0x00BFEAE6, the MapList fill-insert @ 0x00B43F80 (vector<CEngineMap*>::_M_fill_insert).
// Header: engine/CEngineWorldMap.h flattens MapList/VisibleMapList/StaticMapFile to byte arrays and carries the PDB offsets
//   (StaticMapFile +0x40, GameWorld +0x218) where retail has +0x34/+0x198, so the class is modelled locally.
// Layout: CEngineWorldMap as the landed CloseStaticMap 0x00B40000 (Map +0x0c, MapList +0x20, VisibleMapList's node
//   pointer +0x2c: retail 12-byte CArray, STLport list); CEngineMap = Ego_r.pdb less 4 after +0x3c (LandscapeMap +0,
//   LocalDetailCacheMap +4, GameMap +8, VisibleMapListIterator +0x58, IsInVisibleMapList +0x64).
// Shape: every MapList[index] access re-reads the CArray; the visible-list erase is the inlined STLport list erase
//   (unlink, free the node) and the iterator is reset to end(); the trailing trim is `while (size && back() == 0)
//   resize(size() - 1)` with the STLport resize/erase/__copy_trivial inlines and a zero temporary in the dead index slot.
extern "C" void* __cdecl memmove(void* dest, const void* src, unsigned int count);   // 0x00BFEAE6
extern "C" void __cdecl free(void* block);                                         // 0x00BFEA14
void __cdecl operator delete(void* block);                                          // 0x00BFE9BC

struct CEngineMap;

struct CEngineLandscapeMap {
    void OnGameMapUnloaded();                       // 0x00BDBFF0
    void OnMapVisibilityChanged(bool visible);      // 0x00BDC3C0
};
struct CLocalDetailCacheMap {
    void OnMapVisibilityChanged(bool visible);      // 0x00BDE160
};
struct CEngineLandscapeRenderer {
    void DeleteMap(CEngineMap* map);                // 0x00B67180
};
extern CEngineLandscapeRenderer* g_pLandscapeRenderer;   // 0x01436EA8
struct CEngineLocalDetailGenerator {
    void DeleteMap(CEngineMap* map);                // 0x00B59840
};
extern CEngineLocalDetailGenerator* g_pLocalDetailGenerator;   // 0x01436E88

struct _List_node {
    _List_node* _M_next;
    _List_node* _M_prev;
    CEngineMap* _M_data;
};

struct CIDrawMap;
struct CEngineMap {
    CEngineLandscapeMap*  LandscapeMap;             // +0x00
    CLocalDetailCacheMap* LocalDetailCacheMap;      // +0x04
    CIDrawMap*            GameMap;                  // +0x08
    unsigned char         _pad0c[0x58 - 0x0c];
    _List_node*           VisibleMapListIterator;   // +0x58
    unsigned char         _pad5c[0x64 - 0x5c];
    bool                  IsInVisibleMapList;       // +0x64
    ~CEngineMap();                                  // 0x00B42A90
    void ReleaseGameMap()
    {
        if (LandscapeMap)
            LandscapeMap->OnGameMapUnloaded();
        GameMap = 0;
    }
};

template <class T> struct CArray {                  // retail CArray (STLport vector): begin, end, capacity
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    unsigned long size() const { return _M_finish - _M_start; }
    bool empty() const { return size() == 0; }
    T& operator[](unsigned long i) { return _M_start[i]; }
    void _M_fill_insert(T* pos, unsigned long n, const T& x);   // 0x00B43F80
    static T* __copy_trivial(const T* first, const T* last, T* result)
    {
        if (first == last)
            return result;
        unsigned long bytes = (unsigned long)((const char*)last - (const char*)first);
        return (T*)((char*)memmove(result, first, bytes) + bytes);
    }
    void erase(T* first, T* last)
    {
        _M_finish = __copy_trivial(last, _M_finish, first);
    }
    void resize(unsigned long n, T x = T())
    {
        if (n < size())
            erase(_M_start + n, _M_finish);
        else
            _M_fill_insert(_M_finish, n - size(), x);
    }
};

struct CEngineWorldMap {
    void*               _vptr;                      // +0x00
    unsigned char       _pad04[0x8];
    short*              Map;                        // +0x0c
    long                MapArrayWidth;              // +0x10
    long                MapArrayHeight;             // +0x14
    long                MapOffsetX;                 // +0x18
    long                MapOffsetY;                 // +0x1c
    CArray<CEngineMap*> MapList;                    // +0x20
    _List_node*         VisibleMapList;             // +0x2c (STLport list node pointer)
    void UnlinkMapFromWorld(CEngineMap* map, long index);   // 0x00B3EE80
    void DeleteMap(long index);
};

void CEngineWorldMap::DeleteMap(long index)
{
    if (MapList[index] == 0)
        return;
    MapList[index]->ReleaseGameMap();
    CEngineMap* map = MapList[index];
    if (map->IsInVisibleMapList) {
        _List_node* node = map->VisibleMapListIterator;
        _List_node* next = node->_M_next;
        _List_node* prev = node->_M_prev;
        prev->_M_next = next;
        next->_M_prev = prev;
        free(node);
        map->VisibleMapListIterator = VisibleMapList;
        map->IsInVisibleMapList = false;
    }
    if (map->LandscapeMap)
        map->LandscapeMap->OnMapVisibilityChanged(false);
    if (map->LocalDetailCacheMap)
        map->LocalDetailCacheMap->OnMapVisibilityChanged(false);
    if (Map)
        UnlinkMapFromWorld(MapList[index], index);
    if (g_pLandscapeRenderer)
        g_pLandscapeRenderer->DeleteMap(MapList[index]);
    if (g_pLocalDetailGenerator)
        g_pLocalDetailGenerator->DeleteMap(MapList[index]);
    delete MapList[index];
    MapList[index] = 0;
    while (!MapList.empty() && MapList[MapList.size() - 1] == 0)
        MapList.resize(MapList.size() - 1);
}