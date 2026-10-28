// CWorldMap::IsLineOfSight(const CThing& from, const CThing& to, bool checkThings) const -- line of sight between the eye points of two things (creature head, graphic helper point, else physics position): both in the same loaded map and the terrain line clear; optionally gathers the things in the bounding box
// Grade: DIFFER(607v599) -- real STLport list model; residue: ours stores the null list head then the allocated node at construction (retail keeps the node in esi across GetAllThingsInRoughArea and stores late, no null store), so the CWorld manager lands in esi instead of ebx and the filter temporaries swap slots -- lane gap: local STL container dead store kept / register caching
// Leaf: ?IsLineOfSight@CWorldMap@@QBE_NABVCThing@@0_N@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0063E6F0, 608 bytes, ret 0Ch.
// Callees: CThingCreatureBase::GetHeadPos(bool) 0x00661B40, NGraphicAppearance::CTCGraphicAppearance::GetFocalPos(float) 0x004C1060,
//   GetMapNumberAt(const C3DVector&) slot 16 (+0x40), ConvertWorldMapCoordsToLocalCoords(long, const C3DVector&) 0x004FC2A0,
//   IsLineOfSight(const C3DVector&, const C3DVector&) 0x0063AEE0, CWorld::PeekThingManager (slot 4, +0x10),
//   CThingManager::PeekThingSearchTools 0x0051E480, CThingSearchTools::GetAllThingsInRoughArea<const CThing,
//   std::list<const CThing*>, CFilter_Bind2<CThingFilter_IsNot, CThingFilter_IsNot> > 0x0063FAE0, std::_List_base dtor 0x0063F360.
// Retail quirk: for the SECOND thing the graphic-helper and physics-position branches store into the FIRST position
//   (copy-paste bug in the original source; both share the posA store tail).
// Layout: retail CThing PhysicsTC +0x60 (CTCPhysicsBase::Position +0x0C), PTCGraphicAppearance +0x64, ThingTypeFlags +0x6C
//   (Ego_r +0x7C; bit 3 = creature).
extern "C" void* __cdecl malloc(unsigned int size);                             // 0x00BFEA0E
class C3DVector
{
public:
    float X;
    float Y;
    float Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) {}
};

class C2DCoordI
{
public:
    long x;
    long y;
    C2DCoordI() {}
    C2DCoordI(long ax, long ay) : x(ax), y(ay) {}
};

class C2DBoxI
{
public:
    long TLX;
    long TLY;
    long BRX;
    long BRY;
    bool Overlapping(const C2DBoxI& other) const;                 // 0x004FAB40
};

class CMap
{
public:
    bool IsPassableAt(const C2DCoordI& pos) const;                // 0x0081F3D0
    bool IsCameraPassableAt(const C2DCoordI& pos) const;          // 0x0081F460
    // theme/height TU callees (0x0063Axxx rows)
    float GetGroundSizeZAt(const C3DVector& localPos) const;      // 0x0081EA50
    float GetInterpolatedSizeZAt(const C3DVector& localPos) const; // 0x008200B0
    float GetThemeMinCameraSizeZOffsetAt(const C2DCoordI& pos) const; // 0x0081FA90
    float GetThemeSizeZAt(const C2DCoordI& pos) const;            // 0x00820100
    float GetGroundSizeZAt(const C2DCoordI& pos) const;           // 0x00820080
    long GetSoundAt(const C2DCoordI& pos) const;                  // 0x0081F590
    bool IsPassableAtBlock(const C2DCoordI& pos) const;           // 0x0081F630
    C3DVector GetGroundFaceNormalAt(const C3DVector& localPos) const; // 0x0081F6C0
    long GetStrongestEngineThemeAt(const C2DCoordI& pos, long* strength) const; // 0x0081F910
};

// CMapInfo (PDB: 72 bytes): Area C2DBoxI +0x00, PMap +0x10, LevelName +0x18, LevelScriptName +0x1C,
// MapUID +0x20, Used +0x24, Placed +0x25, Activated +0x26, Loading +0x27, ... IsSea +0x40.
struct CMapInfo
{
    C2DBoxI        Area;                 // +0x00
    void*          PMap_Data;            // +0x10
    void*          PMap_Info;            // +0x14
    void*          LevelName;            // +0x18
    void*          LevelScriptName;      // +0x1C
    unsigned long  MapUID;               // +0x20
    bool           Used;                 // +0x24
    bool           Placed;               // +0x25
    bool           Activated;            // +0x26
    bool           Loading;              // +0x27
    bool           LoadedOnPlayerProximity; // +0x28
    unsigned char  _pad29[3];
    void*          SerialiseScript;      // +0x2C
    unsigned long  LevStamp[2];          // +0x30
    unsigned long  TngStamp[2];          // +0x38
    bool           IsSea;                // +0x40
    unsigned char  _pad41[7];
};

// STLport vector<CMapInfo> (CArray<CMapInfo>), 12 bytes in retail.
struct CMapInfoArray
{
    CMapInfo* _M_start;
    CMapInfo* _M_finish;
    CMapInfo* _M_end_of_storage;
    CMapInfo* begin() const { return _M_start; }
    unsigned int size() const { return _M_finish - _M_start; }
    const CMapInfo& operator[](unsigned int n) const { return *(begin() + n); }
    CMapInfo& operator[](unsigned int n) { return *(_M_start + n); }
};

namespace NGraphicAppearance {
class CTCGraphicAppearance {
public:
    C3DVector GetFocalPos(float interpolate) const;               // 0x004C1060
};
}
class CTCPhysicsBase {
public:
    unsigned char _pad00[0xc];
    C3DVector     Position;                                       // +0x0C
};
class CThing {
public:
    unsigned char         _pad00[0x60];
    CTCPhysicsBase*       PhysicsTC;                              // +0x60
    NGraphicAppearance::CTCGraphicAppearance* PTCGraphicAppearance;   // +0x64
    unsigned char         _pad68[4];
    unsigned long         ThingTypeFlags;                         // +0x6C
};
class CThingCreatureBase : public CThing {
public:
    C3DVector GetHeadPos(bool exact) const;                       // 0x00661B40
};
class CThingFilter_IsNot {
public:
    const CThing* Thing;
    CThingFilter_IsNot(const CThing* thing) : Thing(thing) {}
};
template <class A, class B> class CFilter_Bind2 {
public:
    const A* F1;
    const B* F2;
    CFilter_Bind2(const A& f1, const B& f2) : F1(&f1), F2(&f2) {}
};
namespace std {
template <class _Tp> class allocator {
public:
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
};
struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};
template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;
};
template <class _Tp, class _Alloc> class _List_base {
public:
    typedef _List_node<_Tp> _Node;
    _List_base(const _Alloc&) : _M_node(allocator<_Node>(), (_Node*)0)
    {
        _Node* __n = _M_node.allocate(1);
        __n->_M_next = __n;
        __n->_M_prev = __n;
        _M_node._M_data = __n;
    }
    ~_List_base();                                                // 0x0063F360
    _STLP_alloc_proxy<_Node*, _Node, allocator<_Node> > _M_node;
};
template <class _Tp, class _Alloc = allocator<_Tp> >
class list : public _List_base<_Tp, _Alloc> {
public:
    typedef _Alloc allocator_type;
    explicit list(const allocator_type& __a = allocator_type()) : _List_base<_Tp, _Alloc>(__a) { }
};
}
class CThingSearchTools {
public:
    template <class T, class C, class F>
    void GetAllThingsInRoughArea(const C2DBoxI& box, const F& filter, C& things) const;   // 0x0063FAE0 instance
};
class CThingManager {
public:
    const CThingSearchTools& PeekThingSearchTools() const;        // 0x0051E480
};
class CWorld {
public:
    virtual ~CWorld();                                            // slot 0
    virtual void Slot01(); virtual void Slot02(); virtual void Slot03();
    virtual const CThingManager& PeekThingManager() const;        // slot 4 (+0x10)
};
class CWorldMap
{
public:
    virtual ~CWorldMap();                                            // slot 0
    virtual void DeleteAllLevels();                                  // slot 1
    virtual void DeleteAllRegions();                                 // slot 2
    virtual void LoadFromFile();                                     // slot 3
    virtual void SaveToFile();                                       // slot 4
    virtual void RemoveLevel(long map);                              // slot 5
    virtual void LoadTopologyFromFile();                             // slot 6
    virtual void PostLoadTopology(long map);                         // slot 7
    virtual void ForceLoad(long map);                                // slot 8
    virtual void ForceUnload(long map);                              // slot 9
    virtual bool IsMapLoaded(long map) const;                        // slot 10 (+0x28) 0x004FC050
    virtual bool IsMapUsed(long map) const;                          // slot 11
    virtual long GetGranularity() const;                             // slot 12
    virtual const C2DBoxI& GetDimensions() const;                    // slot 13
    virtual CMap& GetMapAt(const C2DCoordI& pos);                    // slot 14
    virtual const CMap& PeekMapAt(const C2DCoordI& pos) const;       // slot 15
    // overloaded virtuals are laid out in reverse declaration order (C3DVector form is slot 16)
    virtual long GetMapNumberAt(const C2DCoordI& pos) const;         // slot 17 (+0x44) 0x004FC080
    virtual long GetMapNumberAt(const C3DVector& pos) const;         // slot 16 (+0x40) 0x004FC0C0
    virtual CMap& GetMap(long map);                                  // slot 18 (+0x48) 0x0051CE20
    virtual const CMap& PeekMap(long map) const;                     // slot 19 (+0x4C) 0x0051CE40

    long            f4;                    // +0x04
    CWorld*         World;                 // +0x08
    void*           Component;             // +0x0C
    void*           PThingMapManager;      // +0x10
    unsigned long   LoadedScriptedMapBrushes[3]; // +0x14 STLport map (12 bytes)
    CMapInfoArray   Maps;                  // +0x20 CArray<CMapInfo>
    unsigned long   Regions[3];            // +0x2C vector<CRegion>
    unsigned long   RegionTime[3];         // +0x38 vector<long>
    void*           PRegionGraph_Data;     // +0x44
    void*           PRegionGraph_Info;     // +0x48
    bool            SoundThemesDisabled;   // +0x4C
    long            Granularity;           // +0x50
    long            GranularityShift;      // +0x54
    long            MapPlacementPitch;     // +0x58
    C2DBoxI         Area;                  // +0x5C
    unsigned short* MapPlacement_start;    // +0x6C vector<unsigned short>
    unsigned short* MapPlacement_finish;   // +0x70
    unsigned short* MapPlacement_eos;      // +0x74

    // out of line in this TU (CWorldMap.cpp, 0x004FCxxx)
    bool IsPosInMap(const C2DCoordI& pos) const;                     // 0x004FC5B0
    bool IsPosInMap(const C3DVector& pos) const;                     // 0x004FC600
    C2DCoordI ConvertWorldMapCoordsToLocalCoords(long map, const C2DCoordI& pos) const; // 0x004FC3C0
    C3DVector ConvertWorldMapCoordsToLocalCoords(long map, const C3DVector& pos) const; // 0x004FC2A0
    bool IsLineOfSight(const C3DVector& from, const C3DVector& to) const;   // 0x0063AEE0
    bool IsLineOfSight(const CThing& from, const CThing& to, bool checkThings) const;
};

static inline float MinF(float a, float b) { return a < b ? a : b; }
static inline float MaxF(float a, float b) { return a > b ? a : b; }

bool CWorldMap::IsLineOfSight(const CThing& from, const CThing& to, bool checkThings) const
{
    const CTCPhysicsBase* physB = to.PhysicsTC;
    const CTCPhysicsBase* physA = from.PhysicsTC;
    C3DVector posA;
    C3DVector posB;
    if (from.ThingTypeFlags & 8)
        posA = ((const CThingCreatureBase&)from).GetHeadPos(true);
    else if (from.PTCGraphicAppearance)
        posA = from.PTCGraphicAppearance->GetFocalPos(1.0f);
    else
        posA = physA->Position;
    if (to.ThingTypeFlags & 8)
        posB = ((const CThingCreatureBase&)to).GetHeadPos(true);
    else if (to.PTCGraphicAppearance)
        posA = to.PTCGraphicAppearance->GetFocalPos(1.0f);
    else
        posA = physB->Position;

    long mapA = GetMapNumberAt(posA);
    long mapB = GetMapNumberAt(posB);
    if (mapA == 0 || mapA != mapB)
        return false;
    C3DVector localA = ConvertWorldMapCoordsToLocalCoords(mapA, posA);
    C3DVector localB = ConvertWorldMapCoordsToLocalCoords(mapA, posB);
    if (!IsLineOfSight(posA, posB))
        return false;
    if (checkThings)
    {
        const CThingManager& manager = World->PeekThingManager();
        C2DBoxI box;
        box.TLX = (long)MinF(posA.X, posB.X);
        box.TLY = (long)MinF(posA.Y, posB.Y);
        box.BRX = (long)MaxF(posA.X, posB.X);
        box.BRY = (long)MaxF(posA.Y, posB.Y);
        std::list<const CThing*> things;
        manager.PeekThingSearchTools().GetAllThingsInRoughArea<const CThing>(box,
            CFilter_Bind2<CThingFilter_IsNot, CThingFilter_IsNot>(CThingFilter_IsNot(&from), CThingFilter_IsNot(&to)), things);
        for (std::_List_node_base* it = things._M_node._M_data->_M_next; it != things._M_node._M_data; it = it->_M_next)
            ;
    }
    return true;
}
