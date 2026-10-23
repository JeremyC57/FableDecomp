// Grade: DIFFER(605v601) -- family gap: whole-program register caching (retail keeps 1<<shift in ebp across the MapPlacement resize call and divides by it again; ours reloads Granularity from the member), which also shifts the ebp push and the area-pointer reuse
// CWorldMap::CWorldMap(CMainGameComponent* component, CWorld* world, const C2DBoxI& area, long granularityShift) -- build an empty world map over the area: one default map info and region, four empty player-position slots, the map-placement grid sized from the granularity, and a fresh thing map manager; sound themes start disabled
// Leaf: ??0CWorldMap@@QAE@PAVCMainGameComponent@@PAVCWorld@@ABVC2DBoxI@@J@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x005066E0, 601 bytes (608 with int3 padding), ret 10h.
// Callees: CBase ctor 0x0099A2F0, malloc 0x00BFEA0E (STLport node / vector storage), CArray<CMapInfo>(long) 0x00515AD0,
//   CRegion() 0x006BC410, CRegion(const CRegion&) 0x0051A900, ~CRegion 0x0051A110, CBaseIntelligentPointer() 0x00A01B10,
//   vector<unsigned short>::resize 0x00517900, operator new 0x00BFEA1A, NThingMap::CThingMapManager() 0x005191E0.
// Layout (retail; Ego_r.pdb CWorldMap with 12-byte STLport containers, everything after +0x14 sits lower than the PDB):
//   vptrs +0x00 / +0x04, World +0x08, Component +0x0C, PThingMapManager +0x10, LoadedScriptedMapBrushes +0x14, Maps +0x20,
//   Regions +0x2C, RegionTime +0x38, PRegionGraph +0x44, SoundThemesDisabled +0x4C, Granularity +0x50, GranularityShift
//   +0x54, MapPlacementPitch +0x58, Area +0x5C, MapPlacement +0x6C, PlayerPositionInfo +0x78 (13-byte packed entries),
//   HeightChangedAreas +0x84, LoadedMaps +0x90, LoadedRegion +0x9C, LevelWADBank +0xA0, UseLevelWADBank +0xA8,
//   CurrentMapUIDCount +0xAC, DestroyedThemePositions +0xB0, PLevelLoader +0xBC, PCameraRegionParticle +0xC4,
//   PHeroBreathParticle +0xCC, RegionUnloading +0xD4, PassabilityBeingShown +0xD5.
#include <new>
#include <stdlib.h>

struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) {}
};
struct C2DBoxI { long TLX, TLY, BRX, BRY; };
struct CBase {
    CBase();                                           // 0x0099A2F0
    virtual ~CBase();                                  // 0x0099A300
};
struct CWorldMapBase2 {
    CWorldMapBase2() {}
    virtual void v00();
    virtual ~CWorldMapBase2() {}
};
struct CCPPointerInfo;
template <class T> struct CCountedPointer {
    T*              Data;
    CCPPointerInfo* Info;
    CCountedPointer() : Data(0), Info(0) {}
};
template <class T> struct CRetailVector {
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    CRetailVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
};
struct CUShortVector : CRetailVector<unsigned short> {
    void resize(unsigned long n, const unsigned short& value);   // 0x00517900
};
struct CBaseIntelligentPointer {
    CBaseIntelligentPointer();                         // 0x00A01B10
    virtual void v00();
    void* m_pItem;
};
struct CThing;
template <class T> struct CIntelligentPointer : CBaseIntelligentPointer {
    CIntelligentPointer() {}
    virtual void v00();
};
struct CRegion {
    CRegion();                                         // 0x006BC410
    CRegion(const CRegion& other);                     // 0x0051A900
    ~CRegion();                                        // 0x0051A110
    unsigned char _body[0x58];
};
template <class T> struct CVectorBase {
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    __forceinline CVectorBase(unsigned long n) : _M_start(0), _M_finish(0), _M_end_of_storage(0)
    {
        _M_start = (T*)malloc(n * sizeof(T));
        _M_finish = _M_start;
        _M_end_of_storage = _M_start + n;
    }
};
template <class T> __forceinline T* UninitializedFillN(T* first, unsigned long n, const T& value)
{
    T* cur = first;
    for (; n > 0; --n, ++cur)
        new (cur) T(value);
    return cur;
}
struct CRegionVector : CVectorBase<CRegion> {
    __forceinline explicit CRegionVector(unsigned long n) : CVectorBase<CRegion>(n) { _M_finish = UninitializedFillN(_M_start, n, CRegion()); }
};
struct CMapInfo { unsigned char _body[0x48]; };
struct CMapInfoArray {
    CMapInfo* _M_start;
    CMapInfo* _M_finish;
    CMapInfo* _M_end_of_storage;
    CMapInfoArray(long count);                         // 0x00515AD0
};
#pragma pack(push, 1)
struct CPlayerPositionInfo {
    bool      Valid;
    C3DVector Pos;
    CPlayerPositionInfo() : Valid(false) {}
    CPlayerPositionInfo(const CPlayerPositionInfo& other) : Valid(other.Valid), Pos(other.Pos) {}
};
#pragma pack(pop)
struct CPlayerPositionVector : CVectorBase<CPlayerPositionInfo> {
    __forceinline explicit CPlayerPositionVector(unsigned long n) : CVectorBase<CPlayerPositionInfo>(n) { _M_finish = UninitializedFillN(_M_start, n, CPlayerPositionInfo()); }
};
struct SRbNode { char _M_color; SRbNode* _M_parent; SRbNode* _M_left; SRbNode* _M_right; void* key; long value; };
struct CScriptedBrushMap {
    SRbNode* _M_header;
    long     _M_node_count;
    long     _M_compare;
    __forceinline CScriptedBrushMap()
    {
        _M_header = 0;
        _M_header = (SRbNode*)malloc(sizeof(SRbNode));
        _M_node_count = 0;
        _M_header->_M_color = 0;
        _M_header->_M_parent = 0;
        _M_header->_M_left = _M_header;
        _M_header->_M_right = _M_header;
    }
};
namespace NThingMap {
struct CThingMapManager {
    CThingMapManager();                                // 0x005191E0
    virtual ~CThingMapManager();
    unsigned char _body[0x34];
};
}
template <class T> struct scoped_ptr {
    T* ptr;
    scoped_ptr() : ptr(0) {}
    void reset(T* p) { T* old = ptr; ptr = p; if (old) delete old; }
};
struct CMainGameComponent;
struct CWorld;
struct CWADBank;
struct CRegionConnectionGraph;
struct CLevelLoader;

class CWorldMap : public CBase, public CWorldMapBase2 {
public:
    CWorldMap(CMainGameComponent* component, CWorld* world, const C2DBoxI& area, long granularityShift);
    CWorld*                                World;                    // +0x08
    CMainGameComponent*                    Component;                // +0x0C
    scoped_ptr<NThingMap::CThingMapManager> PThingMapManager;        // +0x10
    CScriptedBrushMap                      LoadedScriptedMapBrushes; // +0x14
    CMapInfoArray                          Maps;                     // +0x20
    CRegionVector                          Regions;                  // +0x2C
    CRetailVector<long>                    RegionTime;               // +0x38
    CCountedPointer<CRegionConnectionGraph> PRegionGraph;            // +0x44
    bool                                   SoundThemesDisabled;      // +0x4C
    long                                   Granularity;              // +0x50
    long                                   GranularityShift;         // +0x54
    long                                   MapPlacementPitch;        // +0x58
    C2DBoxI                                Area;                     // +0x5C
    CUShortVector                          MapPlacement;             // +0x6C
    CPlayerPositionVector                  PlayerPositionInfo;       // +0x78
    CRetailVector<C2DBoxI>                 HeightChangedAreas;       // +0x84
    CRetailVector<long>                    LoadedMaps;               // +0x90
    long                                   LoadedRegion;             // +0x9C
    CCountedPointer<CWADBank>              LevelWADBank;             // +0xA0
    bool                                   UseLevelWADBank;          // +0xA8
    unsigned long                          CurrentMapUIDCount;       // +0xAC
    CRetailVector<long>                    DestroyedThemePositions;  // +0xB0
    CCountedPointer<CLevelLoader>          PLevelLoader;             // +0xBC
    CIntelligentPointer<CThing>            PCameraRegionParticle;    // +0xC4
    CIntelligentPointer<CThing>            PHeroBreathParticle;      // +0xCC
    bool                                   RegionUnloading;          // +0xD4
    bool                                   PassabilityBeingShown;    // +0xD5
};

CWorldMap::CWorldMap(CMainGameComponent* component, CWorld* world, const C2DBoxI& area, long granularityShift)
    : World(world), Component(component), Maps(1), Regions(1),
      Granularity(1 << granularityShift), GranularityShift(granularityShift), PlayerPositionInfo(4),
      LoadedRegion(0), UseLevelWADBank(false), CurrentMapUIDCount(1), RegionUnloading(false), PassabilityBeingShown(false)
{
    Area = area;
    MapPlacement.resize(((Area.BRY - Area.TLY) / Granularity) * ((Area.BRX - Area.TLX) / Granularity), 0);
    MapPlacementPitch = (Area.BRX - Area.TLX) / Granularity;
    PThingMapManager.reset(new NThingMap::CThingMapManager());
    SoundThemesDisabled = true;
}
