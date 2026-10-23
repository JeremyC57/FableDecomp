// Grade: DIFFER(599v599) -- same length; family gap: whole-program register caching (retail keeps the new list head in esi across the FindThingsInRoughArea call and stores it to the list late; ours stores at construction), so the manager lands in esi instead of ebx; the two IsNot filter temporaries home in each other's dead argument slot
// CWorldMap::IsLineOfSight(const CThing* from, const CThing* to, bool checkThings) const -- line of sight between the eye points of two things (creature head, graphic helper point, else physics position): both in the same loaded map and the terrain line clear; optionally gathers the things in the bounding box
// Leaf: ?IsLineOfSight@CWorldMap@@QBE_NPBVCThing@@0_N@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0063E6F0, 608 bytes, ret 0Ch.
// Callees: CThingCreatureBase::GetHeadPos(bool) 0x00661B40, CTCGraphicAppearance::GetHelperPointOrCentre(float) 0x004C1060,
//   GetMapNumberAt(const C3DVector&) slot 16 (+0x40), ConvertWorldMapCoordsToLocalCoords(long, const C3DVector&) 0x004FC2A0,
//   IsLineOfSight(const C3DVector&, const C3DVector&) 0x0063AEE0, World slot 4 (+0x10) thing manager, 0x0051E480
//   (manager->+0x24->+0x20 search tools), FindThingsInRoughArea instantiation 0x0063FAE0, list dtor 0x0063F360.
// Retail quirk: for the SECOND thing the graphic-helper and physics-position branches store into the FIRST position
//   (copy-paste bug in the original source; both share the posA store tail).
// Layout: retail CThing PhysicsTC +0x60 (position +0x0C), graphic appearance +0x64, flags byte +0x6C (bit 3 = creature).
#include <math.h>
struct C3DVector
{
    float X;
    float Y;
    float Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) {}
};

struct C2DCoordI
{
    long x;
    long y;
    C2DCoordI() {}
    C2DCoordI(long ax, long ay) : x(ax), y(ay) {}
};

struct C2DBoxI
{
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

struct CTCGraphicAppearance {
    C3DVector GetHelperPointOrCentre(float t) const;              // 0x004C1060
};
struct CTCPhysicsBase {
    unsigned char _pad00[0xc];
    C3DVector     Position;                                       // +0x0C
};
class CThing {
public:
    unsigned char         _pad00[0x60];
    CTCPhysicsBase*       PhysicsTC;                              // +0x60
    CTCGraphicAppearance* GraphicAppearance;                      // +0x64
    unsigned char         _pad68[4];
    unsigned char         Flags;                                  // +0x6C
};
class CThingCreatureBase : public CThing {
public:
    C3DVector GetHeadPos(bool exact) const;                       // 0x00661B40
};
struct CThingFilter_IsNot {
    const CThing* Thing;
    CThingFilter_IsNot(const CThing* thing) : Thing(thing) {}
};
template<class A, class B> struct CFilter_Bind2 {
    const A& First;
    const B& Second;
    CFilter_Bind2(const A& a, const B& b) : First(a), Second(b) {}
};
typedef CFilter_Bind2<CThingFilter_IsNot, CThingFilter_IsNot> CThingFilter_IsNotEither;
template<class T> struct list_node { list_node* _M_next; list_node* _M_prev; T _M_data; };
struct CThingList {
    list_node<const CThing*>* _M_node;
    CThingList() { list_node<const CThing*>* node = (list_node<const CThing*>*)::operator new(sizeof(list_node<const CThing*>)); node->_M_next = node; node->_M_prev = node; _M_node = node; }
    ~CThingList();                                                // 0x0063F360 (out of line)
};
struct CThingSearchTools {
    bool FindThingsInRoughArea(const C2DBoxI& box, const CThingFilter_IsNotEither& filter, CThingList& out); // 0x0063FAE0
};
struct CThingSearchHolder { unsigned char _pad[0x20]; CThingSearchTools* Tools; };
struct CThingManager {
    unsigned char _pad[0x24]; CThingSearchHolder* Holder;
    CThingSearchTools* GetSearchTools() const;                    // 0x0051E480
};
struct CWorld {
    virtual void v0(); virtual void v1(); virtual void v2(); virtual void v3();
    virtual CThingManager* GetThingManager();                     // slot 4 (+0x10)
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
    bool IsLineOfSight(const CThing* from, const CThing* to, bool checkThings) const;
};

static inline float MinF(float a, float b) { return a < b ? a : b; }
static inline float MaxF(float a, float b) { return a > b ? a : b; }

bool CWorldMap::IsLineOfSight(const CThing* from, const CThing* to, bool checkThings) const
{
    const CTCPhysicsBase* physB = to->PhysicsTC;
    const CTCPhysicsBase* physA = from->PhysicsTC;
    C3DVector posA;
    C3DVector posB;
    if (from->Flags & 8)
        posA = ((const CThingCreatureBase*)from)->GetHeadPos(true);
    else if (from->GraphicAppearance)
        posA = from->GraphicAppearance->GetHelperPointOrCentre(1.0f);
    else
        posA = physA->Position;
    if (to->Flags & 8)
        posB = ((const CThingCreatureBase*)to)->GetHeadPos(true);
    else if (to->GraphicAppearance)
        posA = to->GraphicAppearance->GetHelperPointOrCentre(1.0f);
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
        CThingManager* manager = World->GetThingManager();
        C2DBoxI box;
        box.TLX = (long)MinF(posA.X, posB.X);
        box.TLY = (long)MinF(posA.Y, posB.Y);
        box.BRX = (long)MaxF(posA.X, posB.X);
        box.BRY = (long)MaxF(posA.Y, posB.Y);
        CThingList things;
        manager->GetSearchTools()->FindThingsInRoughArea(box, CThingFilter_IsNotEither(CThingFilter_IsNot(from), CThingFilter_IsNot(to)), things);
        for (list_node<const CThing*>* it = things._M_node->_M_next; it != things._M_node; it = it->_M_next)
            ;
    }
    return true;
}
