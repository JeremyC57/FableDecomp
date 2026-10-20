// CWorldMap::ActivateNavMap(long) -- make a map's navigation map active, building and adding it to the navigator first when it is not loaded
// Leaf: ActivateNavMap
// Retail: Fable.exe (TLC, VC7.1) @ 0x0050AF10, 260 bytes, `ret 4`. PDB ?ActivateNavMap@CWorldMap@@QAEXJ@Z.
// Callees: CNavigatorManager::IsNavigationMapLoaded 0x00A14E80, AddNavigationMap 0x00A156D0, ActivateNavigationMap
//   0x00A16050 (through World +0x08 -> PNavigationManager +0x48); CCharString(const char*, long) 0x0099EBF0 ("NULL"
//   0x012393E4), ~CCharString 0x0099EAE0, CWorldMap::GetMapNavigationAreaInit 0x0050A650, free 0x00BFEA14 (x2),
//   ~CNavigationAreaUpdate 0x0051D530 (landed as CVectorQuad_0051d530: frees the three POD vectors and destroys the
//   layer vector; label "CEngineScreenEffectOutlineGlow::FinishWithDynamicTextures" is wrong).
// Layout: CNavigationAreaInit = CNavigationAreaUpdate (four retail 12-byte vectors, 0x30) + WorldArea +0x30, MapIndex
//   +0x40, DetailedAreas +0x44, NavRegionSeeds +0x50 (Ego_r.pdb with 16-byte vectors: 0x74 bytes; retail 0x5C);
//   CWorldMap Maps +0x20 (0x48-byte CMapInfo, Area +0).
// Shape: the init local's inline constructor zeroes all 23 dwords (VC schedules them after the literal's pushes); the
//   "NULL" literal is an implicit temporary homed in the dead `map` argument slot; the derived destructor frees its
//   two POD vectors inline (zero held in esi) and calls the out-of-line base destructor.
extern "C" void __cdecl free(void* block);        // 0x00BFEA14
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
struct C2DBoxI {
    long TLX, TLY, BRX, BRY;
    C2DBoxI() : TLX(0), TLY(0), BRX(0), BRY(0) { }
};
template <class T> struct vector {                     // STLport vector of PODs (12 bytes in retail)
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    vector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    ~vector()
    {
        if (_M_start)
            free(_M_start);
    }
};
struct CNavigationLayerUpdate;
struct CNavigationTransitionPoint;
struct CNavigationPosition { float X, Y, Z; };
struct C2DBoxF { float TLX, TLY, BRX, BRY; };
struct CVectorOfLayerUpdates {                         // vector<CNavigationLayerUpdate> (its dtor lives in the base dtor)
    CNavigationLayerUpdate* _M_start;
    CNavigationLayerUpdate* _M_finish;
    CNavigationLayerUpdate* _M_end_of_storage;
    CVectorOfLayerUpdates() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
};
template <class T> struct CPodVector {                 // base members: constructed inline, destroyed by ~CNavigationAreaUpdate
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    CPodVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
};
struct CNavigationAreaUpdate {                         // 0x30 bytes in retail (PDB 0x40: four 16-byte vectors)
    CVectorOfLayerUpdates                  NavigationLayers;   // +0x00
    CPodVector<CNavigationTransitionPoint> TransitionPoints;   // +0x0C
    CPodVector<CNavigationPosition>        ActionPoints;       // +0x18
    CPodVector<unsigned char>              TopologyWeights;    // +0x24
    ~CNavigationAreaUpdate();                          // 0x0051D530
};
struct CNavigationAreaInit : CNavigationAreaUpdate {   // 0x5C bytes in retail (PDB 0x74)
    C2DBoxI                     WorldArea;             // +0x30
    long                        MapIndex;              // +0x40
    vector<C2DBoxF>             DetailedAreas;         // +0x44
    vector<CNavigationPosition> NavRegionSeeds;        // +0x50
    CNavigationAreaInit() : MapIndex(0) { }
};
struct CNavigatorManager {
    bool IsNavigationMapLoaded(long map) const;                    // 0x00A14E80
    void AddNavigationMap(long map, const CNavigationAreaInit& init);   // 0x00A156D0
    void ActivateNavigationMap(long map);                          // 0x00A16050
};
struct CWorld {
    unsigned char      _pad00[0x48];
    CNavigatorManager* PNavigationManager;             // +0x48
};
struct CMapInfo {                                      // 0x48 bytes
    C2DBoxI       Area;                                // +0x00
    unsigned char _body[0x38];
};
class CWorldMap {
public:
    unsigned char _pad00[8];
    CWorld*       World;                               // +0x08
    unsigned char _pad0c[0x14];
    CMapInfo*     Maps;                                // +0x20 (CArray<CMapInfo> start)
    void GetMapNavigationAreaInit(long map, const C2DBoxI& area, const CCharString& name, CNavigationAreaInit& init);   // 0x0050A650
    void ActivateNavMap(long map);
};
void CWorldMap::ActivateNavMap(long map)
{
    if (!World->PNavigationManager->IsNavigationMapLoaded(map)) {
        CNavigationAreaInit init;
        GetMapNavigationAreaInit(map, Maps[map].Area, "NULL", init);
        World->PNavigationManager->AddNavigationMap(map, init);
    }
    World->PNavigationManager->ActivateNavigationMap(map);
}
