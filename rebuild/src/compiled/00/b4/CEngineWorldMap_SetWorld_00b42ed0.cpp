// CEngineWorldMap::SetWorld(const CIDrawWorld* world) -- attach the engine world map to a game world: rebuild the map list from the world's map infos, add every valid map, link the ones that already have a game map, then rebuild the grid
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B42ED0, length 276 bytes, `ret 4`.
// Signature from Ego_r.pdb: ?SetWorld@CEngineWorldMap@@QAEXPBVCIDrawWorld@@@Z.
// Callees: CIDrawWorld slot 2 (GetWorldMap), CIDrawWorldMap slots 0 (GetMapCount) and 1 (GetMapInfo by value, hidden
// result pointer); CEngineWorldMap::DeleteMap() @ 0x00B42C70 (the private all-maps overload), AddMap(long, long)
// @ 0x00B42D10, AutoBuildWorldMapGrid @ 0x00B40070; the map list's resize(n, const T&) @ 0x00B44170; ~CCharString
// @ 0x0099EAE0; CEngineLandscapeMap's empty OnGameMapLoaded @ 0x00BDBFE0 (a bare `ret`, called directly).
// Globals: the engine world map @ 0x01436E8C (read by the inlined CEngineMap::OnGameMapLoaded).
// Layout: retail CEngineWorldMap is a local model -- engine/CEngineWorldMap.h carries PDB offsets (GameWorld +0x218,
// GameWorldMap +0x21c) where retail has +0x198/+0x19c (the static-map bank file members are 0x80 smaller); MapList
// (12-byte CArray<CEngineMap*>) +0x20 agrees. CEngineMap (PDB): LandscapeMap +0, GameMap +8. CDrawMapInfo (PDB, 32
// bytes): Valid +0, PMap +4, StaticMeshName (CCharString) +8.
// Shape: the second loop calls CEngineWorldMap::OnGameMapLoaded(i) inline, which forwards to the inline
// CEngineMap::OnGameMapLoaded(i): the map pointer is read from the list before the global world map, the map info is a
// named local of that inline member, destroyed after the landscape-map call; the resize value 0 is a temporary homed in the dead `world` slot.
struct CCharString {
    void* PStringData;
    ~CCharString();                                     // 0x0099EAE0
};

struct CIDrawMap;
struct CDrawMapInfo {
    bool Valid;                                         // +0x00
    bool Loadable;                                      // +0x01
    bool GameLoadable;                                  // +0x02
    CIDrawMap* PMap;                                    // +0x04
    CCharString StaticMeshName;                         // +0x08
    long PosX, PosY;                                    // +0x0c C2DCoordI
    unsigned long UID;                                  // +0x14
    long ExtentsX, ExtentsY;                            // +0x18 C2DExtentsI
};

struct CIDrawWorldMap {
    virtual long GetMapCount() const = 0;                       // slot 0
    virtual CDrawMapInfo GetMapInfo(long index) const = 0;      // slot 1
};
struct CIDrawWorld {
    virtual void v00() const = 0;
    virtual void v01() const = 0;
    virtual const CIDrawWorldMap* GetWorldMap() const = 0;      // slot 2
};

struct CEngineLandscapeMap {
    void OnGameMapLoaded();                             // 0x00BDBFE0 (empty)
};

struct CEngineMap {
    CEngineLandscapeMap* LandscapeMap;                  // +0x00
    void* LocalDetailCacheMap;                          // +0x04
    const CIDrawMap* GameMap;                           // +0x08
    void OnGameMapLoaded(long mapIndex);
};

template <class T> struct CArray {
    T* First;
    T* Last;
    T* End;
    void resize(unsigned long count, const T& value);   // 0x00B44170
    T& operator[](long index) { return First[index]; }
};

struct CEngineWorldMap {
    void* _vptr;                                        // +0x000
    unsigned char _pad004[0x1c];                        // +0x004
    CArray<CEngineMap*> MapList;                        // +0x020
    unsigned char _pad02c[0x16c];                       // +0x02c
    const CIDrawWorld* GameWorld;                       // +0x198
    const CIDrawWorldMap* GameWorldMap;                 // +0x19c

    void SetWorld(const CIDrawWorld* world);
    void OnGameMapLoaded(long mapIndex) { MapList[mapIndex]->OnGameMapLoaded(mapIndex); }
    const CIDrawWorldMap* PeekGameWorldMap() const { return GameWorldMap; }
    void AddMap(long mapIndex, long openState);         // 0x00B42D10
private:
    void DeleteMap();                                   // 0x00B42C70
    void AutoBuildWorldMapGrid();                       // 0x00B40070
};
extern CEngineWorldMap* g_pEngineWorldMap;              // 0x01436E8C

inline void CEngineMap::OnGameMapLoaded(long mapIndex)
{
    CDrawMapInfo info = g_pEngineWorldMap->PeekGameWorldMap()->GetMapInfo(mapIndex);
    GameMap = info.PMap;
    LandscapeMap->OnGameMapLoaded();
}

void CEngineWorldMap::SetWorld(const CIDrawWorld* world)
{
    if (world == GameWorld)
        return;
    GameWorld = world;
    GameWorldMap = world->GetWorldMap();
    DeleteMap();
    long count = GameWorldMap->GetMapCount();
    MapList.resize(count, 0);
    long i;
    for (i = 1; i < count; ++i) {
        CDrawMapInfo info = GameWorldMap->GetMapInfo(i);
        if (info.Valid)
            AddMap(i, 0);
    }
    for (i = 1; i < count; ++i) {
        CDrawMapInfo info = GameWorldMap->GetMapInfo(i);
        if (info.Valid && info.PMap)
            OnGameMapLoaded(i);
    }
    AutoBuildWorldMapGrid();
}