#ifndef FABLE_ENGINE_CWORLDMAP_H
#define FABLE_ENGINE_CWORLDMAP_H
#ifdef FABLE_RETAIL_CWORLDMAP
// Retail opt-in (Codex pass 11 consolidation 2, merged with retry11a row 0x00521620): the polymorphic retail class -- RTTI
// CWorldMap : CBaseClassNonCopyable (vptr +0x00), CIDrawWorldMap (vptr +0x04); the members of the default branch below from
// +0x08 (STLport containers as opaque 4-aligned storage), sizeof 0xD8. Virtual order: Ego_r.pdb CWorldMap (TPI 0x27B6E).
#include "engine/CBaseClassNonCopyable.h"
#include "engine/CIDrawWorldMap.h"
#include "engine/CCountedPointer.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CArray.h"
#include "engine/C2DBoxI.h"
#include "boost/scoped_ptr.h"
#include "stlport/fable_containers.h"
#include "stlport/fable_map.h"
class CMapInfo; class CThing; class CWideString;
class CNavigationAreaInit; class CNavigationAreaUpdate;
class CWorld; class CMainGameComponent; class CRegionConnectionGraph; class CWADBank; class CLevelLoader; class CAFile; class CRegion;
class CASuspendableProcess; class CMap; class C2DCoordI; class C2DBoxI; class C3DVector; class CDrawMapInfo; class CCharString;
namespace NThingMap { class CThingMapManager; }
namespace std { template<class T> struct less; template<class K,class C,class A> class set; }
class CWorldMap : public CBaseClassNonCopyable, public CIDrawWorldMap {
public:
    virtual ~CWorldMap();
    virtual void DeleteAllLevels();
    virtual void DeleteAllRegions();
    virtual void LoadFromFile(CAFile&, long);
    virtual void SaveToFile(CAFile&);
    virtual void RemoveLevel(long);
    virtual void LoadTopologyFromFile(long, CCountedPointer<CAFile>, CASuspendableProcess&);
    virtual void PostLoadTopology(long);
    virtual void ForceLoad(long);
    virtual void ForceUnload(long);
    virtual bool IsMapLoaded(long) const;
    virtual bool IsMapUsed(long) const;
    virtual long GetGranularity() const;
    virtual const C2DBoxI& GetDimensions() const;
    virtual CMap& GetMapAt(const C2DCoordI&);
    virtual const CMap& PeekMapAt(const C2DCoordI&) const;
    // Header wave 2026-11-03: retail vtable 0x01244AEC holds GetMapNumberAt(const C3DVector&) 0x004FC0C0 in slot 16 and
    // GetMapNumberAt(const C2DCoordI&) 0x004FC080 in slot 17 (0x00821010 calls [vtbl+0x44] for the C2DCoordI form); VC7.1 lays an
    // overload pair out in reverse declaration order, so the C2DCoordI form is declared first.
    virtual long GetMapNumberAt(const C2DCoordI&) const;
    virtual long GetMapNumberAt(const C3DVector&) const;
    virtual CMap& GetMap(long);
    virtual const CMap& PeekMap(long) const;
    virtual void GetAllMapsInArea(const C2DBoxI&, std::vector<unsigned long, std::allocator<unsigned long> >&) const;
    virtual void GetAllLoadedMapsInArea(const C2DBoxI&, std::vector<unsigned long, std::allocator<unsigned long> >&) const;
    virtual void PostRegionLoad(long);
    virtual C2DCoordI GetMapPos(long) const;
    virtual bool IsMapChangeable(long) const;
    virtual bool IsPosChangeable(const C2DCoordI&) const;
    virtual std::set<CCharString, std::less<CCharString>, std::allocator<CCharString> > GetLoadableSections() const;
    virtual void Update();
    virtual void HeightAreaChanged(const C2DBoxI*);
    virtual long DrawGetFrame() const;
    virtual long DrawGetNoMaps() const;
    virtual CDrawMapInfo DrawGetMapInfo(long) const;
    virtual C2DBoxI DrawGetDimensions() const;
    virtual long DrawGetGranularity() const;
    virtual bool DrawIsPosInMap(const C3DVector&) const;
    virtual float DrawGetSizeZAt(const C3DVector&, float) const;
    virtual C3DVector DrawConvertWorldMapCoordsToLocalCoords(long, const C3DVector&) const;
    virtual long DrawGetMyPlayerMapNumber() const;
    virtual bool DrawFindLastUnblockedGroundPositionAndReportCollision(const C3DVector&, const C3DVector&, C3DVector&) const;
    virtual bool DrawFindLastUnblockedThemePositionAndReportCollision(const C3DVector&, const C3DVector&, C3DVector&) const;
protected:
    virtual void LoadScriptBrushesFromFile(long, CCountedPointer<CAFile>);
    virtual long AllocateNewMap();
public:
    // Ego_r.pdb / FableWin.pdb CWorldMap::ELoadType (nested; LF_ENUMERATE WAIT_FOR_LOAD = 0, DONT_WAIT_FOR_LOAD = 1, Codex pass 13
    // TPI evidence; 0x00501150 runs the level loader to completion for WAIT_FOR_LOAD), the protected
    // ?LoadLevelFromFiles@CWorldMap@@IAEXJV?$CCountedPointer@VCAFile@@@@00W4ELoadType@1@@Z (Ego_r 0x004D55C0, chunk 719), and the public
    // ?LoadRegion@CWorldMap@@QAEXJW4ELoadType@1@_N@Z, ?GetRegionNumberMapIsIn@CWorldMap@@QBEJJ@Z,
    // ?GenerateRegionConnectivityGraph@CWorldMap@@QAEXXZ (both PDBs; Codex pass 13 rows 0x00501450 / 0x004FD040).
    enum ELoadType { WAIT_FOR_LOAD=0,DONT_WAIT_FOR_LOAD=1 };
protected:
    void LoadLevelFromFiles(long levelID, CCountedPointer<CAFile> topologyFile, CCountedPointer<CAFile> navigationFile,
                            CCountedPointer<CAFile> thingFile, ELoadType loadType);
public:
    void LoadRegion(long,ELoadType,bool);
    long GetRegionNumberMapIsIn(long)const;
    void GenerateRegionConnectivityGraph();
    unsigned long GetMapUID(long map) const;                 // 0x004FCE00 ?GetMapUID@CWorldMap@@QBEKJ@Z
    long GetRegionNumberFromName(const CCharString&) const;      // these five: callees of 0x00606300 / 0x0074A5D0
    CRegion& GetRegion(long);
    // Chunk 734 (row 0x004A5A40 CWorld::UpdateFrame, on PeekWorldMap()): the const overload ?GetRegion@CWorldMap@@QBEABVCRegion@@J@Z
    // (both PDBs; retail 0x004FC180) and ?GetLoadedRegion@CWorldMap@@QBEJXZ (both PDBs; retail 0x004FB150).
    const CRegion& GetRegion(long) const;                    // 0x004FC180
    long GetLoadedRegion() const;                            // 0x004FB150
    bool IsPosInMap(const C3DVector&) const;
    bool IsPosPassable(const C2DCoordI&) const;
    float GetGroundSizeZAt(const C3DVector&) const;
protected:
    class CPlayerPositionInfo;
    void UnloadRegion(long,bool);
    void SaveMapsToString(CCharString&);                      // 0x004FB990 ?SaveMapsToString@CWorldMap@@IAEXAAVCCharString@@@Z
    void SaveRegionsToString(CCharString&);                   // 0x004FD040 ?SaveRegionsToString@CWorldMap@@IAEXAAVCCharString@@@Z
    void GetMapNavigationAreaInit(long,const C2DBoxI*,const CCharString&,CNavigationAreaInit&) const;
public:
    void GetMapNavigationAreaUpdateInfo(long,const C2DBoxI&,const CCharString&,CNavigationAreaUpdate&) const;
#ifdef FABLE_RETAIL_CWORLDMAP_POPULATE
    // Retail opt-in (chunk 879, row 0x00502E90): Ego_r.pdb / FableWin.pdb ?PopulateRegionWithVillagers@CWorldMap@@QAEXAAVCThing@@@Z
    // (public, called by PostRegionLoad 0x005064C0). Declaration only: no layout change.
    void PopulateRegionWithVillagers(CThing& village);       // 0x00502E90
#endif
    long GetNavigationLayerAt(const C3DVector&,const CThing*) const;
#ifdef FABLE_RETAIL_CWORLDMAP_STRONGESTTHEME
    // Retail opt-in (chunk 802, row 0x0066CF00 CRegionMinimap::PopulateRegionMinimap): Ego_r.pdb / FableWin.pdb
    // ?GetStrongestEngineThemeAt@CWorldMap@@QBEJABVC3DVector@@AAJ@Z (Ego_r 0x006E84E0), out of line in retail at 0x0063ADF0
    // (the caller passes the position and the strength out-word by address, `this` the const world map). Declaration only.
    long GetStrongestEngineThemeAt(const C3DVector& pos, long& strength) const;   // 0x0063ADF0
#endif
#ifdef FABLE_RETAIL_CWORLDMAP_HEIGHTZ
    // Chunk 764 (row 0x007BC004 CTCShotDefinitionNymphFun::InitMovement): Ego_r.pdb ?GetHeightZBelowPos@CWorldMap@@QBEMABVC3DVector@@@Z
    // (retail 0x0063E480). Declaration only.
    float GetHeightZBelowPos(const C3DVector& pos) const;
#endif
#ifdef FABLE_RETAIL_CWORLDMAP_THINGMAPMANAGER
    // Chunk 761 (row 0x0071A790 ~CTCMapwho, via the inline CTCMapwho::RemoveFromMapwho): FableWin.pdb
    // ?GetThingMapManager@CWorldMap@@QAEAAVCThingMapManager@NThingMap@@XZ (and the const QBE twin), inline in retail: the
    // scoped_ptr word +0x10 is read in place (`call 0x00686D30 ; mov ecx,[eax+10h]`). Needs FABLE_RETAIL_SCOPED_PTR (operator*).
    NThingMap::CThingMapManager& GetThingMapManager() { return *PThingMapManager; }
    const NThingMap::CThingMapManager& GetThingMapManager() const { return *PThingMapManager; }
#endif
protected:
    void GetLevelFilesFromWorldFile(const CWideString&,std::vector<CWideString>&);
    CWorld&                 World;                           // +0x08
    const CMainGameComponent& Component;                       // +0x0c
    boost::scoped_ptr<NThingMap::CThingMapManager> PThingMapManager;           // +0x10 boost::scoped_ptr<NThingMap::CThingMapManager>
    std::map<CCharString,long> LoadedScriptedMapBrushes;     // +0x14 map<CCharString,long>
    CArray<CMapInfo> Maps;                         // +0x20 CArray<CMapInfo>
    std::vector<CRegion> Regions;                      // +0x2c vector<CRegion>
    std::vector<long> RegionTime;                   // +0x38 vector<long>
    CCountedPointer<CRegionConnectionGraph> PRegionGraph;    // +0x44
    bool                    SoundThemesDisabled;             // +0x4c
    const long Granularity;                     // +0x50
    const long GranularityShift;                // +0x54
    long                    MapPlacementPitch;               // +0x58
    C2DBoxI Area;                         // +0x5c C2DBoxI (TLX, TLY, BRX, BRY)
    std::vector<unsigned short> MapPlacement;                 // +0x6c vector<unsigned short>
    std::vector<CPlayerPositionInfo> PlayerPositionInfo;           // +0x78 vector<CWorldMap::CPlayerPositionInfo>
    std::vector<C2DBoxI> HeightChangedAreas;           // +0x84 vector<C2DBoxI>
    CArray<long> LoadedMaps;                   // +0x90 CArray<long>
    long                    LoadedRegion;                    // +0x9c
    CCountedPointer<CWADBank> LevelWADBank;                  // +0xa0
    bool                    UseLevelWADBank;                 // +0xa8
    unsigned long           CurrentMapUIDCount;              // +0xac
    std::vector<C2DCoordI> DestroyedThemePositions;      // +0xb0 vector<C2DCoordI>
    CCountedPointer<CLevelLoader> PLevelLoader;              // +0xbc
    CIntelligentPointer<CThing> PCameraRegionParticle;        // +0xc4 CIntelligentPointer<CThing>
    CIntelligentPointer<CThing> PHeroBreathParticle;          // +0xcc CIntelligentPointer<CThing>
    bool                    RegionUnloading;                 // +0xd4
    bool                    PassabilityBeingShown;           // +0xd5
};
FABLE_STATIC_ASSERT(sizeof(CWorldMap) == 0xd8);
#else
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

#endif // FABLE_RETAIL_CWORLDMAP
#endif // FABLE_ENGINE_CWORLDMAP_H
