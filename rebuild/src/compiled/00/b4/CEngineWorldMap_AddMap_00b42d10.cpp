// CEngineWorldMap::AddMap(long mapIndex, long flags) -- (re)create the engine map for one game map: drop the grid, grow MapList, fetch the game map's info and, unless the slot already holds the same map UID, delete the old map, build a new CEngineMap, register it with the landscape renderer (and the local-detail generator unless bit 0 is set), put it at the front of the visible-map list and tell its landscape/local-detail maps it is visible
// Leaf: AddMap
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B42D10, length 352 bytes (with int3 padding; body 0x15d), `ret 8`.
// PDB: ?AddMap@CEngineWorldMap@@QAEXJJ@Z; CEngineMap ctor ??0CEngineMap@@QAE@PBVCIDrawMap@@JJJJJVCCharString@@_N@Z.
// Callees: operator delete[] @ 0x00BFEB1C, the MapList resize(n, const T&) @ 0x00B44170, CIDrawWorldMap slot 1
//   (GetMapInfo by value), CEngineWorldMap::DeleteMap(long) @ 0x00B42B10, operator new @ 0x00BFEA1A, the CCharString copy
//   constructor @ 0x0099EC30, CEngineMap::CEngineMap @ 0x00B429E0, CEngineLandscapeRenderer::CreateMap(CEngineMap*, long)
//   @ 0x00B67140 (global @ 0x01436EA8), CEngineLocalDetailGenerator::CreateMap(CEngineMap*) @ 0x00B59800 (global
//   @ 0x01436E88), malloc @ 0x00BFEA0E (STLport list node), CEngineLandscapeMap / CLocalDetailCacheMap::
//   OnMapVisibilityChanged(bool) @ 0x00BDC3C0 / 0x00BDE160, ~CCharString @ 0x0099EAE0.
// Header: engine/CEngineWorldMap.h flattens MapList/VisibleMapList/StaticMapFile to byte arrays and carries the PDB offsets
//   (StaticMapFile +0x40, GameWorld +0x218) where retail has +0x34/+0x198, so the class is modelled locally.
// Layout: as the landed SetWorld 0x00B42ED0 (MapList +0x20, GameWorldMap +0x19c, CDrawMapInfo 32 bytes) and DeleteMap
//   0x00B42B10 (Map +0x0c, VisibleMapList node pointer +0x2c; CEngineMap MapUID +0x18, VisibleMapListIterator +0x58,
//   IsInVisibleMapList +0x64; retail CEngineMap is 0x68 bytes).
// Shape: the resize value 0 is a temporary in the dead mapIndex slot; the early exit for an unchanged map still destroys
//   the info's name; the visible-list insert is the inlined STLport `insert(begin(), map)` (malloc'd node, placement
//   construction behind a null test) and the iterator is re-read as begin().
#include <new>
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E
void __cdecl operator delete[](void* block);          // 0x00BFEB1C

class CCharString { public:
    void* PStringData;
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
};

class CIDrawMap;
class CDrawMapInfo { public:
    bool Valid;                                         // +0x00
    bool Loadable;                                      // +0x01
    bool GameLoadable;                                  // +0x02
    CIDrawMap* PMap;                                    // +0x04
    CCharString StaticMeshName;                         // +0x08
    long PosX, PosY;                                    // +0x0c C2DCoordI
    unsigned long UID;                                  // +0x14
    long ExtentsX, ExtentsY;                            // +0x18 C2DExtentsI
};

class CIDrawWorldMap { public:
    virtual long GetMapCount() const = 0;                       // slot 0
    virtual CDrawMapInfo GetMapInfo(long index) const = 0;      // slot 1
};

class CEngineMap;
class CEngineLandscapeMap { public:
    void OnMapVisibilityChanged(bool visible);      // 0x00BDC3C0
};
class CLocalDetailCacheMap { public:
    void OnMapVisibilityChanged(bool visible);      // 0x00BDE160
};
class CEngineLandscapeRenderer { public:
    void CreateMap(CEngineMap* map, long flags);    // 0x00B67140
};
extern CEngineLandscapeRenderer* g_pLandscapeRenderer;   // 0x01436EA8
class CEngineLocalDetailGenerator { public:
    void CreateMap(CEngineMap* map);                // 0x00B59800
};
extern CEngineLocalDetailGenerator* g_pLocalDetailGenerator;   // 0x01436E88

struct _List_node {
    _List_node* _M_next;
    _List_node* _M_prev;
    CEngineMap* _M_data;
};

class CEngineMap { public:                                 // retail 0x68 bytes
    CEngineLandscapeMap*  LandscapeMap;             // +0x00
    CLocalDetailCacheMap* LocalDetailCacheMap;      // +0x04
    const CIDrawMap*      GameMap;                  // +0x08
    unsigned char         _pad0c[0x18 - 0x0c];      // MapWorldPosition
    long                  MapUID;                   // +0x18
    unsigned char         _pad1c[0x58 - 0x1c];
    _List_node*           VisibleMapListIterator;   // +0x58
    unsigned char         _pad5c[0x64 - 0x5c];
    bool                  IsInVisibleMapList;       // +0x64
    unsigned char         _pad65[0x68 - 0x65];
    CEngineMap(const CIDrawMap* gameMap, long posX, long posY, long width, long height, long uid, CCharString staticMapName, bool rebuild);   // 0x00B429E0
};

template <class T> class CArray { public:                  // retail CArray (STLport vector): begin, end, capacity
    T* First;
    T* Last;
    T* End;
    unsigned long size() const { return Last - First; }
    void resize(unsigned long count, const T& value);   // 0x00B44170
    T& operator[](long index) { return First[index]; }
};

struct CEngineMapList {                             // STLport list<CEngineMap*> (the node pointer)
    _List_node* _M_node;
    _List_node* begin() { return _M_node->_M_next; }
    _List_node* insert(_List_node* pos, CEngineMap* const& x)
    {
        _List_node* tmp = (_List_node*)malloc(sizeof(_List_node));
        new (&tmp->_M_data) CEngineMap*(x);
        _List_node* prev = pos->_M_prev;
        tmp->_M_next = pos;
        tmp->_M_prev = prev;
        prev->_M_next = tmp;
        pos->_M_prev = tmp;
        return tmp;
    }
    void push_front(CEngineMap* const& x) { insert(begin(), x); }
};

class CEngineWorldMap { public:
    void*                 _vptr;                    // +0x000
    unsigned char         _pad004[0x8];
    short*                Map;                      // +0x00c
    unsigned char         _pad010[0x10];
    CArray<CEngineMap*>   MapList;                  // +0x020
    CEngineMapList        VisibleMapList;           // +0x02c
    unsigned char         _pad030[0x168];
    const void*           GameWorld;                // +0x198
    const CIDrawWorldMap* GameWorldMap;             // +0x19c
    void DeleteMap(long mapIndex);                  // 0x00B42B10
    void AddMap(long mapIndex, long flags);
};

void CEngineWorldMap::AddMap(long mapIndex, long flags)
{
    if (Map) {
        delete[] Map;
        Map = 0;
    }
    if ((unsigned long)mapIndex >= MapList.size())
        MapList.resize(mapIndex + 1, 0);
    CDrawMapInfo info = GameWorldMap->GetMapInfo(mapIndex);
    if (MapList[mapIndex] && MapList[mapIndex]->MapUID == info.UID)
        return;
    DeleteMap(mapIndex);
    MapList[mapIndex] = new CEngineMap(info.PMap, info.PosX, info.PosY, info.ExtentsX, info.ExtentsY, info.UID, info.StaticMeshName, false);
    g_pLandscapeRenderer->CreateMap(MapList[mapIndex], flags);
    if (!(flags & 1))
        g_pLocalDetailGenerator->CreateMap(MapList[mapIndex]);
    CEngineMap* map = MapList[mapIndex];
    if (!map->IsInVisibleMapList) {
        VisibleMapList.push_front(map);
        map->VisibleMapListIterator = VisibleMapList.begin();
        map->IsInVisibleMapList = true;
    }
    if (map->LandscapeMap)
        map->LandscapeMap->OnMapVisibilityChanged(true);
    if (map->LocalDetailCacheMap)
        map->LocalDetailCacheMap->OnMapVisibilityChanged(true);
}