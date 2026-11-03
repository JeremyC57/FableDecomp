#if defined(FABLE_RETAIL_NAVIGATION_UPDATE) || defined(FABLE_RETAIL_INVENTORY_MAP_SERIALISE) || defined(FABLE_RETAIL_VALIDPOSITIONS) || defined(FABLE_RETAIL_ENTITY_PERSIST) || defined(FABLE_RETAIL_REGION_GRAPH) || defined(FABLE_RETAIL_HIT_EFFECT) || defined(FABLE_RETAIL_MAP_BRUSHES)
#ifndef FABLE_ENGINE_CWORLDMAP_H
#define FABLE_ENGINE_CWORLDMAP_H
#include "engine/CBaseClassNonCopyable.h"
#include "engine/CIDrawWorldMap.h"
#include "engine/CCountedPointer.h"
#ifdef FABLE_RETAIL_NAVIGATION_UPDATE
#include "engine/CVectorMap.h"
#include "engine/CMapInfo.h"
#endif
#include "stlport/fable_containers.h"
class CWorld;class CNavigationAreaUpdate;class CThing;
class CRegion;class CAFile;class CASuspendableProcess;class CMap;class CCharString;class C2DCoordI;
namespace std { template<class T>struct less;template<class K,class Compare,class A>class set; }
// Ego_r TPI0x27b6e, original two bases at0/4; native container reductions give complete d8.
class CWorldMap : public CBaseClassNonCopyable, public CIDrawWorldMap {
public: virtual ~CWorldMap();
public: virtual void DeleteAllLevels();
public: virtual void DeleteAllRegions();
public: virtual void LoadFromFile(CAFile &,long);
public: virtual void SaveToFile(CAFile &);
public: virtual void RemoveLevel(long);
public: virtual void LoadTopologyFromFile(long,CCountedPointer<CAFile>,CASuspendableProcess &);
public: virtual void PostLoadTopology(long);
public: virtual void ForceLoad(long);
public: virtual void ForceUnload(long);
public: virtual bool IsMapLoaded(long) const;
public: virtual bool IsMapUsed(long) const;
public: virtual long GetGranularity() const;
public: virtual const C2DBoxI & GetDimensions() const;
public: virtual CMap & GetMapAt(const C2DCoordI &);
public: virtual const CMap & PeekMapAt(const C2DCoordI &) const;
#ifdef FABLE_RETAIL_MAP_BRUSHES
// VC7.1 reverses declarations within an overload group: original C2DCoordI slot17.
public: virtual long GetMapNumberAt(const C2DCoordI &) const;
public: virtual long GetMapNumberAt(const C3DVector &) const;
#else
public: virtual long GetMapNumberAt(const C3DVector &) const;
public: virtual long GetMapNumberAt(const C2DCoordI &) const;
#endif
public: virtual CMap & GetMap(long);
public: virtual const CMap & PeekMap(long) const;
public: virtual void GetAllMapsInArea(const C2DBoxI &,std::vector<unsigned long,std::allocator<unsigned long> > &) const;
public: virtual void GetAllLoadedMapsInArea(const C2DBoxI &,std::vector<unsigned long,std::allocator<unsigned long> > &) const;
public: virtual void PostRegionLoad(long);
public: virtual C2DCoordI GetMapPos(long) const;
public: virtual bool IsMapChangeable(long) const;
public: virtual bool IsPosChangeable(const C2DCoordI &) const;
public: virtual std::set<CCharString,std::less<CCharString>,std::allocator<CCharString> > GetLoadableSections() const;
public: virtual void Update();
public: virtual void HeightAreaChanged(const C2DBoxI *);
public: virtual long DrawGetFrame() const;
public: virtual long DrawGetNoMaps() const;
public: virtual CDrawMapInfo DrawGetMapInfo(long) const;
public: virtual C2DBoxI DrawGetDimensions() const;
public: virtual long DrawGetGranularity() const;
public: virtual bool DrawIsPosInMap(const C3DVector &) const;
public: virtual float DrawGetSizeZAt(const C3DVector &,float) const;
public: virtual C3DVector DrawConvertWorldMapCoordsToLocalCoords(long,const C3DVector &) const;
public: virtual long DrawGetMyPlayerMapNumber() const;
public: virtual bool DrawFindLastUnblockedGroundPositionAndReportCollision(const C3DVector &,const C3DVector &,C3DVector &) const;
public: virtual bool DrawFindLastUnblockedThemePositionAndReportCollision(const C3DVector &,const C3DVector &,C3DVector &) const;
protected: virtual void LoadScriptBrushesFromFile(long,CCountedPointer<CAFile>);
protected: virtual long AllocateNewMap();

public:
#ifdef FABLE_RETAIL_ENTITY_PERSIST
    unsigned long GetMapUID(long) const;
#endif
#if defined(FABLE_RETAIL_REGION_GRAPH) || defined(FABLE_RETAIL_INVENTORY_MAP_SERIALISE)
    long GetRegionNumberFromName(const CCharString&) const;
#endif
#ifdef FABLE_RETAIL_HIT_EFFECT
    long GetLoadedRegion() const;
    const CRegion& GetRegion(long) const;
#endif
#ifdef FABLE_RETAIL_INVENTORY_MAP_SERIALISE
    CRegion& GetRegion(long);
#endif
#ifdef FABLE_RETAIL_NAVIGATION_UPDATE
    void GetMapNavigationAreaUpdateInfo(long,const C2DBoxI&,const CCharString&,CNavigationAreaUpdate&) const;
    long GetNavigationLayerAt(const C3DVector&,const CThing* =0) const;
    C2DBoxI ClipAndConvertWorldMapAreaToLocalArea(long,const C2DBoxI&) const;
#endif
    bool IsPosInMap(const C3DVector&) const;
    bool IsPosPassable(const C2DCoordI&) const;
    float GetGroundSizeZAt(const C3DVector&) const;
protected:
#ifdef FABLE_RETAIL_NAVIGATION_UPDATE
    CWorld* World;
    unsigned char Component[4],PThingMapManager[4];
    unsigned char LoadedScriptedMapBrushes[12];
    CArray<CMapInfo> Maps;
    unsigned char Regions[12],RegionTime[12],PRegionGraph[8];
    bool SoundThemesDisabled;
    long Granularity,GranularityShift,MapPlacementPitch;
    C2DBoxI Area;
    unsigned char MapPlacement[12],PlayerPositionInfo[12],HeightChangedAreas[12],LoadedMaps[12];
    long LoadedRegion;
    unsigned char LevelWADBank[8];
    bool UseLevelWADBank;
    unsigned long CurrentMapUIDCount;
    unsigned char DestroyedThemePositions[12],PLevelLoader[8],PCameraRegionParticle[8],PHeroBreathParticle[8];
    bool RegionUnloading,PassabilityBeingShown;
#else
    unsigned char _pad_0x08[0xd0]; // Original unused world/map/region/container storage, complete native extent.
#endif
};
FABLE_STATIC_ASSERT(sizeof(CWorldMap)==0xd8);
#endif
#else
#ifndef FABLE_ENGINE_CWORLDMAP_H
#define FABLE_ENGINE_CWORLDMAP_H
// GENERATED by tools/decomp_pipeline/gen_class_headers.py -- do not edit by hand.
// Donor: Ego_r.pdb (ghidra_out/struct_layouts_egor.tsv) sizeof CWorldMap = 0xfc.
// Container layout: retail STLport 4.6 (list 4, vector/map/set/string/CArray 0xc, deque 0x28, CVectorMap 0x10, bases from Fable.exe RTTI): sizeof CWorldMap = 0xd8.
// Retail evidence: 11 trusted landed function(s); reconciliation 1 conflict(s) in 1 file(s) vs 10 agreeing.
// Members are original Lionhead names; `_pad_*` are unnamed bytes; opaque
// `unsigned char X[N]` members carry their real (composite/template) type in a comment.
#include <stddef.h>
#include "rebuild_abi.h"

class CCPPointerInfo;
class CLevelLoader;
class CMainGameComponent;
class CRegionConnectionGraph;
struct CThingMapManager;
class CWADBank;
class CWorld;

#pragma pack(push, 1)
class CWorldMap {
public:
    void*                   __vftable;                       // +0x00 vptr, or first dword of a flattened base subobject
    long                    f4;                              // +0x04 retail-only long (previous header)
    CWorld*                 World;                           // +0x08
    CMainGameComponent*     Component;                       // +0x0c
    CThingMapManager*       PThingMapManager_ptr;            // +0x10 scoped_ptr<NThingMap::CThingMapManager>::ptr
    unsigned char           LoadedScriptedMapBrushes[0xc];   // +0x14 map<CCharString,long,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,long>_>_>
    unsigned char           Maps[0xc];                       // +0x20 CArray<CMapInfo>
    unsigned char           Regions[0xc];                    // +0x2c vector<CRegion,std::allocator<CRegion>_>
    unsigned char           RegionTime[0xc];                 // +0x38 vector<long,std::allocator<long>_>
    CRegionConnectionGraph* PRegionGraph_Data;               // +0x44 CCountedPointer<CRegionConnectionGraph>::Data
    CCPPointerInfo*         PRegionGraph_Info;               // +0x48 CCountedPointer<CRegionConnectionGraph>::Info
    bool                    SoundThemesDisabled;             // +0x4c
    unsigned char           _pad_0x4d[0x3];                  // +0x4d
    long                    Granularity;                     // +0x50
    long                    GranularityShift;                // +0x54
    long                    MapPlacementPitch;               // +0x58
    long                    Area_TLX;                        // +0x5c C2DBoxI::TLX
    long                    Area_TLY;                        // +0x60 C2DBoxI::TLY
    long                    Area_BRX;                        // +0x64 C2DBoxI::BRX
    long                    Area_BRY;                        // +0x68 C2DBoxI::BRY
    unsigned char           MapPlacement[0xc];               // +0x6c vector<unsigned_short,std::allocator<unsigned_short>_>
    unsigned char           PlayerPositionInfo[0xc];         // +0x78 vector<CWorldMap::CPlayerPositionInfo,std::allocator<CWorldMap::CPlayerPositionInfo>_>
    unsigned char           HeightChangedAreas[0xc];         // +0x84 vector<C2DBoxI,std::allocator<C2DBoxI>_>
    unsigned char           LoadedMaps[0xc];                 // +0x90 CArray<long>
    long                    LoadedRegion;                    // +0x9c
    CWADBank*               LevelWADBank_Data;               // +0xa0 CCountedPointer<CWADBank>::Data
    CCPPointerInfo*         LevelWADBank_Info;               // +0xa4 CCountedPointer<CWADBank>::Info
    bool                    UseLevelWADBank;                 // +0xa8
    unsigned char           _pad_0xa9[0x3];                  // +0xa9
    unsigned long           CurrentMapUIDCount;              // +0xac
    unsigned char           DestroyedThemePositions[0xc];    // +0xb0 vector<C2DCoordI,std::allocator<C2DCoordI>_>
    CLevelLoader*           PLevelLoader_Data;               // +0xbc CCountedPointer<CLevelLoader>::Data
    CCPPointerInfo*         PLevelLoader_Info;               // +0xc0 CCountedPointer<CLevelLoader>::Info
    unsigned char           PCameraRegionParticle[0x8];      // +0xc4 CIntelligentPointer<CThing>
    unsigned char           PHeroBreathParticle[0x8];        // +0xcc CIntelligentPointer<CThing>
    bool                    RegionUnloading;                 // +0xd4
    bool                    PassabilityBeingShown;           // +0xd5
    unsigned char           _pad_0xd6[0x2];                  // +0xd6
};
#pragma pack(pop)

FABLE_STATIC_ASSERT(sizeof(CWorldMap) == 0xd8);
FABLE_STATIC_ASSERT(offsetof(CWorldMap, __vftable) == 0x0);
FABLE_STATIC_ASSERT(offsetof(CWorldMap, f4) == 0x4);
FABLE_STATIC_ASSERT(offsetof(CWorldMap, World) == 0x8);
FABLE_STATIC_ASSERT(offsetof(CWorldMap, Component) == 0xc);
FABLE_STATIC_ASSERT(offsetof(CWorldMap, PThingMapManager_ptr) == 0x10);
FABLE_STATIC_ASSERT(offsetof(CWorldMap, LoadedScriptedMapBrushes) == 0x14);
FABLE_STATIC_ASSERT(offsetof(CWorldMap, Maps) == 0x20);
FABLE_STATIC_ASSERT(offsetof(CWorldMap, Regions) == 0x2c);
FABLE_STATIC_ASSERT(offsetof(CWorldMap, RegionTime) == 0x38);
FABLE_STATIC_ASSERT(offsetof(CWorldMap, PRegionGraph_Data) == 0x44);
FABLE_STATIC_ASSERT(offsetof(CWorldMap, PRegionGraph_Info) == 0x48);
FABLE_STATIC_ASSERT(offsetof(CWorldMap, SoundThemesDisabled) == 0x4c);
FABLE_STATIC_ASSERT(offsetof(CWorldMap, Granularity) == 0x50);
FABLE_STATIC_ASSERT(offsetof(CWorldMap, GranularityShift) == 0x54);
FABLE_STATIC_ASSERT(offsetof(CWorldMap, MapPlacementPitch) == 0x58);
FABLE_STATIC_ASSERT(offsetof(CWorldMap, Area_TLX) == 0x5c);
FABLE_STATIC_ASSERT(offsetof(CWorldMap, Area_TLY) == 0x60);
FABLE_STATIC_ASSERT(offsetof(CWorldMap, Area_BRX) == 0x64);
FABLE_STATIC_ASSERT(offsetof(CWorldMap, Area_BRY) == 0x68);
FABLE_STATIC_ASSERT(offsetof(CWorldMap, MapPlacement) == 0x6c);
FABLE_STATIC_ASSERT(offsetof(CWorldMap, PlayerPositionInfo) == 0x78);
FABLE_STATIC_ASSERT(offsetof(CWorldMap, HeightChangedAreas) == 0x84);
FABLE_STATIC_ASSERT(offsetof(CWorldMap, LoadedMaps) == 0x90);
FABLE_STATIC_ASSERT(offsetof(CWorldMap, LoadedRegion) == 0x9c);
FABLE_STATIC_ASSERT(offsetof(CWorldMap, LevelWADBank_Data) == 0xa0);
FABLE_STATIC_ASSERT(offsetof(CWorldMap, LevelWADBank_Info) == 0xa4);
FABLE_STATIC_ASSERT(offsetof(CWorldMap, UseLevelWADBank) == 0xa8);
FABLE_STATIC_ASSERT(offsetof(CWorldMap, CurrentMapUIDCount) == 0xac);
FABLE_STATIC_ASSERT(offsetof(CWorldMap, DestroyedThemePositions) == 0xb0);
FABLE_STATIC_ASSERT(offsetof(CWorldMap, PLevelLoader_Data) == 0xbc);
FABLE_STATIC_ASSERT(offsetof(CWorldMap, PLevelLoader_Info) == 0xc0);
FABLE_STATIC_ASSERT(offsetof(CWorldMap, PCameraRegionParticle) == 0xc4);
FABLE_STATIC_ASSERT(offsetof(CWorldMap, PHeroBreathParticle) == 0xcc);
FABLE_STATIC_ASSERT(offsetof(CWorldMap, RegionUnloading) == 0xd4);
FABLE_STATIC_ASSERT(offsetof(CWorldMap, PassabilityBeingShown) == 0xd5);

#endif // FABLE_ENGINE_CWORLDMAP_H

#endif
