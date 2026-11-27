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
#ifdef FABLE_RETAIL_CWORLDMAP_GETMAPBOX
#include "engine/CMapInfo.h"
#endif
class CMapInfo; class CThing; class CWideString;
class CNavigationAreaInit; class CNavigationAreaUpdate;
class CWorld; class CMainGameComponent; class CRegionConnectionGraph; class CWADBank; class CLevelLoader; class CAFile; class CRegion;
class CASuspendableProcess; class CMap; class C2DCoordI; class C2DBoxI; class C3DVector; class CDrawMapInfo; class CCharString;
namespace NThingMap { class CThingMapManager; }
namespace std { template<class T> struct less; template<class K,class C,class A> class set; }
#ifdef FABLE_RETAIL_CWORLDMAP_ISTHEMEDESTROYABLE
class CGameDefinitionManager;
#endif
#ifdef FABLE_RETAIL_CWORLDMAP_IMPASSABILITYAROUND
#include "engine/C2DCoordI.h"
class C3DVector;
// Retail opt-in (egod2a, row 0x007243C0): ?g_BlockNeighbourOffsets@@3QBVC2DCoordI@@B, the nine block neighbour offsets read by
// CWorldMap::GetImpassabilityFactorAround (reloc_bindings.tsv 0x01385978, bound by 0x0063E3C0) and the row's loop (0x0072445E).
extern const C2DCoordI g_BlockNeighbourOffsets[9];                    // 0x01385978
#endif
class CWorldMap : public CBaseClassNonCopyable, public CIDrawWorldMap {
public:
#ifdef FABLE_RETAIL_CWORLDMAP_SETREGIONSASCONNECTED
    // Opt-in (NF433, row 0x0088E3D0 CGameScriptInterface::MiniMapAllowRouteBetweenRegions): Ego_r.pdb / FableWin.pdb
    // ?SetRegionsAsConnectedInGraph@CWorldMap@@QAEXABVCCharString@@0_N@Z, out of line in retail at 0x004FC970 (the row's call on
    // CWorld::GetWorldMap's result, CWorld vtable slot 12). Declaration only, no layout change.
    void SetRegionsAsConnectedInGraph(const CCharString& region_1, const CCharString& region_2, bool connected);   // 0x004FC970
#endif
#ifdef FABLE_RETAIL_CWORLDMAP_LOCAL_TO_WORLD_VECTOR_PASS26
    // Ego_r 004CFC10 and FableWin 01C83290, called by LoadRoute on PeekWorldMap().
    C2DVector ConvertLocalCoordsToWorldMapCoords(long region, const C2DVector& position) const;
#endif
#ifdef FABLE_RETAIL_CWORLDMAP_IMPASSABILITYAROUND
    // Ego_r.pdb ?GetImpassabilityFactorAround@CWorldMap@@QBEXABVC3DVector@@AAV?$vector@MV?$allocator@M@std@@@std@@_N@Z (out of line,
    // retail 0x0063E3C0; called at 0x0072441E). Declaration only.
    void GetImpassabilityFactorAround(const C3DVector& pos, std::vector<float>& factors, bool navigation) const;   // 0x0063E3C0
#endif
#ifdef FABLE_RETAIL_CWORLDMAP_HEIGHT_BELOW
 class CThingFilterBase: public std::unary_function<CThing,bool> {public: virtual bool operator()(const CThing&)const{return true;}};
 float GetHeightZBelowAt(const C3DVector&,float,bool,const CThingFilterBase&)const;
#endif
#ifdef FABLE_RETAIL_CWORLDMAP_GETMAPBOX
    const C2DBoxI& GetMapBox(long n) const { return Maps[n].Area; }
#endif
#ifdef FABLE_RETAIL_CWORLDMAP_CTOR
    // Opt-in (chunk 2051, row 0x004A6E30 CWorld::Init): Ego_r.pdb ??0CWorldMap@@QAE@ABVCMainGameComponent@@AAVCWorld@@ABVC2DBoxI@@K@Z, retail 0x005066E0. Declaration only.
    CWorldMap(const CMainGameComponent& component, CWorld& world, const C2DBoxI& area, unsigned long levels);   // 0x005066E0
#endif
#ifdef FABLE_RETAIL_CWORLDMAP_WORLDLOAD
    // Both original complete class records and actual original caller chain.
    CCountedPointer<CLevelLoader> GetLevelLoader()const{return PLevelLoader;}
    void CreateStaticMaps(const CWideString&,class CIEngine&);
    void CreateWorldLevelWad(const CWideString&,const CWideString&);
    void SetAsUsingWAD(const CWideString&);
#endif
#ifdef FABLE_RETAIL_LEVEL_LOADER_OBJECT_LOAD_PASS29
    void ActivateNavMap(long);
    void PostLoadInitialise(long);
    void ActivateTopology(long);
    void SetMapAsLoading(long, bool);
    void SetRegionAsLoaded(long, const CCharString&);
#endif
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
#ifdef FABLE_RETAIL_CWORLDMAP_GETNOMAPS
    // Opt-in (row 0x00523CF0): Ego_r.pdb / FableWin.pdb ?GetNoMaps@CWorldMap@@QBEJXZ, inline in retail (`add eax,20h` = &Maps, then size()).
    long GetNoMaps() const { return Maps.size(); }
#endif
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
#ifdef FABLE_RETAIL_CWORLDMAP_UNLOADLEVEL_DECL
    // Opt-in (NF321, 2026-11-23, row 0x004FF8A0 ForceUnload): Ego_r.pdb / FableWin.pdb ?UnloadLevel@CWorldMap@@IAEXJ@Z (protected,
    // non-virtual; retail 0x004FEDE0, which the donor FableWin ForceUnload calls). Declaration only.
    void UnloadLevel(long);                                                                              // 0x004FEDE0
#endif
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
#ifdef FABLE_RETAIL_CWORLDMAP_SETPLAYERPOS
    // Nested opt-in (chunk Q25, row 0x00489D40 CPlayer::CreateCharacter): Ego_r.pdb / FableWin.pdb
    // ?SetPlayerPos@CWorldMap@@QAEXJABVC3DVector@@W4ELoadType@1@@Z, out of line in retail at 0x005063E0 (`ret 0Ch`). Declaration only.
    void SetPlayerPos(long playerNumber, const C3DVector& pos, ELoadType loadType);   // 0x005063E0
#endif
#ifdef FABLE_RETAIL_CWORLDMAP_PASS22_SOUNDTHEME
    // Retail opt-in (Codex pass 22): Ego_r/PDB member and native0x0063A6E0, called from CMusicManager::Update.
    long GetBlockValue(const C2DCoordI& block);
#endif
    long GetRegionNumberMapIsIn(long)const;
    void GenerateRegionConnectivityGraph();
    unsigned long GetMapUID(long map) const;                 // 0x004FCE00 ?GetMapUID@CWorldMap@@QBEKJ@Z
    long GetRegionNumberFromName(const CCharString&) const;      // these five: callees of 0x00606300 / 0x0074A5D0
    CRegion& GetRegion(long);
    // Chunk 734 (row 0x004A5A40 CWorld::UpdateFrame, on PeekWorldMap()): the const overload ?GetRegion@CWorldMap@@QBEABVCRegion@@J@Z
    // (both PDBs; retail 0x004FC180) and ?GetLoadedRegion@CWorldMap@@QBEJXZ (both PDBs; retail 0x004FB150).
    const CRegion& GetRegion(long) const;                    // 0x004FC180
    long GetLoadedRegion() const;                            // 0x004FB150
#ifdef FABLE_RETAIL_CWORLDMAP_REGIONCOUNT
    // Retail opt-in (agent4008 scout, row 0x0060EEB0 CTCInventoryMap::ConstructPCMapList): Ego_r.pdb + FableWin.pdb
    // ?GetRegionCount@CWorldMap@@QBEJXZ, out of line at 0x004FB470 (`(end - begin) / 0x58` over +0x2C/+0x30, the vector GetRegion
    // 0x004FC180 indexes with stride 0x58 = sizeof(CRegion)). The landed label / binding "DrawGetNoMaps" / ?GetNoMaps@ is wrong:
    // DrawGetNoMaps is the virtual CIDrawWorldMap slot and GetNoMaps (FableWin only) counts maps, not regions. Declaration only.
    long GetRegionCount() const;                             // 0x004FB470
#endif
#ifdef FABLE_RETAIL_CWORLDMAP_LEVELHEADER
    // Opt-in (agentNF54, rows 0x00506BB0 / 0x00506C40): Ego_r.pdb + FableWin.pdb ?GetLevelStaticMapChecksum@CWorldMap@@QBEKABVCCharString@@@Z
    // (public const, retail 0x00506C40) and the protected const overload ?GetLevelDimensions@CWorldMap@@IBE?AVC2DBoxI@@AAVCAFile@@@Z
    // (retail 0x00506BB0; Ego_r 0x004D4730, FableWin 0x01C8C5F0). Declarations only.
    unsigned long GetLevelStaticMapChecksum(const CCharString& levelName) const;   // 0x00506C40
protected:
    C2DBoxI GetLevelDimensions(CAFile& file) const;                               // 0x00506BB0
public:
#endif
#ifdef FABLE_RETAIL_CWORLDMAP_SETSOUNDTHEMESDISABLED
    // Opt-in (wfE11, row 0x004A3740 CWorld::UpdateRegionLoad): FableWin.pdb ?SetSoundThemesDisabled@CWorldMap@@QAEX_N@Z, inline in retail
    // (`mov byte [eax+4Ch],1` = SoundThemesDisabled on GetWorldMap(), 0x004A40CF). Body only: no layout change.
    void SetSoundThemesDisabled(bool disabled) { SoundThemesDisabled = disabled; }
#endif // FABLE_RETAIL_CWORLDMAP_SETSOUNDTHEMESDISABLED
#ifdef FABLE_RETAIL_CWORLDMAP_MOVEPLAYERPOS
    // Opt-in (wfE11, row 0x004A3740 CWorld::UpdateRegionLoad): Ego_r.pdb ?StartMovePlayerPos@CWorldMap@@QAEXJABVC3DVector@@W4ELoadType@1@@Z (0x00502500),
    // ?EndMovePlayerPos@CWorldMap@@QAEXXZ (0x005064B0) and ?IsLoadingAnyMap@CWorldMap@@QBE_NXZ (0x004FB4A0), all called. Needs the nested ELoadType
    // (declared above under FABLE_RETAIL_CWORLDMAP). Declarations only.
    void StartMovePlayerPos(long player, const C3DVector& pos, ELoadType load_type);   // 0x00502500
    void EndMovePlayerPos();                                             // 0x005064B0
    bool IsLoadingAnyMap() const;                                        // 0x004FB4A0
#ifdef FABLE_RETAIL_CWORLDMAP_CACHEMAPS
    // Nested opt-in (agentNF36 2026-11-16, row 0x005064B0 EndMovePlayerPos = `push 0 ; call 0x00502850 ; ret`): Ego_r.pdb + FableWin.pdb
    // ?CacheMaps@CWorldMap@@IAEXW4ELoadType@1@@Z (protected, nested ELoadType), retail 0x00502850. Declaration only.
protected:
    void CacheMaps(ELoadType load_type);                                 // 0x00502850
public:
#endif
#endif // FABLE_RETAIL_CWORLDMAP_MOVEPLAYERPOS
#ifdef FABLE_RETAIL_CWORLDMAP_PREPAREFORLOAD
    // NF161 opt-in (q4647, row 0x004FF560): Ego_r.pdb / FableWin.pdb ?PrepareForLoad@CWorldMap@@QAEXXZ (public; landed callers 0x004A0220 /
    // 0x004A2E90 bind it), retail `UnloadRegion(LoadedRegion, false)` (the protected 0x004FEEC0 below, LoadedRegion +0x9c). Declaration only.
    void PrepareForLoad();                                               // 0x004FF560
#endif
#ifdef FABLE_RETAIL_CWORLDMAP_SAVEGAMESTATE
    // Opt-in (chunk 2052, row 0x0049F4C0): Ego_r.pdb ?SaveGameState@CWorldMap@@QAEXAAVCPersistContext@@@Z, out of line at 0x004FDE80.
    void SaveGameState(CPersistContext& context);                       // 0x004FDE80
#endif
#ifdef FABLE_RETAIL_CWORLDMAP_LOADGAMESTATE
    // Opt-in (chunk 2052, row 0x004A21F0): Ego_r.pdb ?LoadGameState@CWorldMap@@QAEXAAVCPersistContext@@@Z, out of line at 0x005079C0.
    void LoadGameState(CPersistContext& context);                       // 0x005079C0
#endif
    bool IsPosInMap(const C3DVector&) const;
    bool IsPosPassable(const C2DCoordI&) const;
#ifdef FABLE_RETAIL_EXPLOSIVE_TRAIL_UPDATE_PASS28
    // Native 004fcc40 and both original PDB overload sets: the call uses create_pos directly.
    bool IsPosPassable(const C3DVector&) const;
#endif
    float GetGroundSizeZAt(const C3DVector&) const;
#ifdef FABLE_RETAIL_CWORLDMAP_PASS22_WATERHEIGHT
    // Both original PDBs and native0063A420.
    float GetWaterSizeZAt(const C3DVector&)const;
#endif
protected:
#ifdef FABLE_RETAIL_CWORLDMAP_PLAYERPOSITION_PASS27
    // Ego_r nested record CPlayerPositionInfo. Set is at +0 and Pos at +1.
#pragma pack(push, 1)
    class CPlayerPositionInfo {
    public:
        bool Set;
        C3DVector Pos;
    };
#pragma pack(pop)
#else
    class CPlayerPositionInfo;
#endif
    void UnloadRegion(long,bool);
    void SaveMapsToString(CCharString&);                      // 0x004FB990 ?SaveMapsToString@CWorldMap@@IAEXAAVCCharString@@@Z
    void SaveRegionsToString(CCharString&);                   // 0x004FD040 ?SaveRegionsToString@CWorldMap@@IAEXAAVCCharString@@@Z
    void GetMapNavigationAreaInit(long,const C2DBoxI*,const CCharString&,CNavigationAreaInit&) const;
public:
    void GetMapNavigationAreaUpdateInfo(long,const C2DBoxI&,const CCharString&,CNavigationAreaUpdate&) const;
    long GetNavigationLayerAt(const C3DVector&,const CThing*) const;
#ifdef FABLE_RETAIL_CWORLDMAP_STRONGESTTHEME
    // Retail opt-in (chunk 802, row 0x0066CF00 CRegionMinimap::PopulateRegionMinimap): Ego_r.pdb / FableWin.pdb
    // ?GetStrongestEngineThemeAt@CWorldMap@@QBEJABVC3DVector@@AAJ@Z (Ego_r 0x006E84E0), out of line in retail at 0x0063ADF0
    // (the caller passes the position and the strength out-word by address, `this` the const world map). Declaration only.
    long GetStrongestEngineThemeAt(const C3DVector& pos, long& strength) const;   // 0x0063ADF0
#endif
#ifdef FABLE_RETAIL_CWORLDMAP_ISTHEMEDESTROYABLE
    // NF41 opt-in (2026-11-16, row 0x0063DD70): Ego_r.pdb 0x006EAC20 ?IsThemeDestroyableAt@CWorldMap@@QBE_NABVC3DVector@@@Z (public
    // const) and 0x004CE990 ?PeekDefinitionManager@CWorldMap@@IBEABVCGameDefinitionManager@@XZ (protected const, retail 0x004FB310).
    // Declarations only.
    bool IsThemeDestroyableAt(const C3DVector& pos) const;                       // 0x0063DD70
protected:
    const CGameDefinitionManager& PeekDefinitionManager() const;                 // 0x004FB310
public:
#endif
#ifdef FABLE_RETAIL_CWORLDMAP_MAPNUMBERFROMSCRIPTNAME
    // Chunk 1010 (row 0x008930F0 CGameScriptInterface::GetThingWithScriptName): Ego_r.pdb / FableWin.pdb
    // ?GetMapNumberFromScriptName@CWorldMap@@QBEJABVCCharString@@@Z, out of line in retail at 0x004FB880 (called on PeekWorldMap()).
    // Declaration only.
    long GetMapNumberFromScriptName(const CCharString& script_name) const;   // 0x004FB880
#endif
#ifdef FABLE_RETAIL_CWORLDMAP_MAPNUMBERFROMMAPUID
    // Opt-in (chunk 2010, row 0x005248B0 NThingManager::CEntityRuntimePersistence::LoadFromGameStateScript): Ego_r.pdb / FableWin.pdb
    // ?GetMapNumberFromMapUID@CWorldMap@@QBEJK@Z, out of line at 0x004FB920 (landed); FableWin.pdb ?GetNoMaps@CWorldMap@@QBEJXZ, inline in
    // retail (the caller divides Maps' finish - start by 0x48 in place, 0x0052495E..0x00524977). Declarations / inline body only.
    long GetMapNumberFromMapUID(unsigned long uid) const;   // 0x004FB920
    long GetNoMaps() const { return Maps.size(); }
#endif
#ifdef FABLE_RETAIL_CWORLDMAP_UPDATEDESTROYEDTHEMEAREAS
    // Nested opt-in (chunk 1013, row 0x004FEC80): Ego_r.pdb / FableWin.pdb ?UpdateDestroyedThemeAreas@CWorldMap@@IAEXXZ (protected).
protected:
    void UpdateDestroyedThemeAreas();                                                     // 0x004FEC80
public:
#endif
#if defined(FABLE_RETAIL_CWORLDMAP_AREACOVERSONELEVEL_BODY) && defined(_MSC_VER)
    // Nested opt-in (chunk 1013, row 0x004FF150 CWorldMap::GetAllMapsInArea): the body of the landed 0x004FB160 (same CWorldMap.cpp TU in
    // retail), visible and out of line, so the caller knows the box it passes is only read: retail keeps the clamped box.TLX / TLY in
    // ebx / edi across the call (0x004FF211 `sar edi,cl ; sar ebx,cl`). Needs C2DCoordI's (x, y) constructor.
    __declspec(noinline) bool AreaCoversOneLevel(const C2DBoxI& area) const              // 0x004FB160
    {
        if (GetMapNumberAt(C2DCoordI(area.TLX, area.TLY)) == GetMapNumberAt(C2DCoordI(area.BRX, area.TLY))
            && GetMapNumberAt(C2DCoordI(area.BRX, area.TLY)) == GetMapNumberAt(C2DCoordI(area.TLX, area.BRY))
            && GetMapNumberAt(C2DCoordI(area.TLX, area.BRY)) == GetMapNumberAt(C2DCoordI(area.BRX, area.BRY))
            && GetMapNumberAt(C2DCoordI(area.BRX, area.BRY)) != 0)
            return true;
        return false;
    }
#elif defined(FABLE_RETAIL_CWORLDMAP_AREACOVERSONELEVEL) || defined(FABLE_RETAIL_CWORLDMAP_AREACOVERSONELEVEL_BODY)
    // Nested opt-in (chunk 1013, row 0x004FF150 CWorldMap::GetAllMapsInArea): Ego_r.pdb / FableWin.pdb
    // ?AreaCoversOneLevel@CWorldMap@@QBE_NABVC2DBoxI@@@Z, out of line in retail at 0x004FB160. Declaration only.
    bool AreaCoversOneLevel(const C2DBoxI& area) const;                                   // 0x004FB160
#endif
#ifdef FABLE_RETAIL_CWORLDMAP_LOADTHINGSWITHSECTIONNAMES
    // Nested opt-in (chunk 1013, row 0x00500EB0): Ego_r.pdb / FableWin.pdb
    // ?LoadThingsWithSectionNames@CWorldMap@@QAEXABV?$set@VCCharString@@U?$less@VCCharString@@@std@@V?$allocator@VCCharString@@@3@@std@@W4ELoadType@1@@Z
    // and the public static ?GetThingFilenameFromLevelFilename@CWorldMap@@SI?AVCWideString@@ABV2@@Z (retail 0x004FAFF0, __fastcall: the
    // hidden result in ecx, the level filename in edx). Declarations only.
    void LoadThingsWithSectionNames(const std::set<CCharString, std::less<CCharString>, std::allocator<CCharString> >& sections, ELoadType loadType);
    static CWideString __fastcall GetThingFilenameFromLevelFilename(const CWideString& levelFilename);   // 0x004FAFF0
#endif
#ifdef FABLE_RETAIL_CWORLDMAP_UPDATENAVMAPS
    // Nested opt-in (chunk 1013, row 0x00501990): Ego_r.pdb / FableWin.pdb
    // ?UpdateNavMaps@CWorldMap@@QAEXABV?$set@VCCharString@@U?$less@VCCharString@@@std@@V?$allocator@VCCharString@@@3@@std@@W4ELoadType@1@@Z.
    void UpdateNavMaps(const std::set<CCharString, std::less<CCharString>, std::allocator<CCharString> >& sections, ELoadType loadType);
#endif
#ifdef FABLE_RETAIL_CWORLDMAP_LOADLEVEL
    // Nested opt-in (chunk 1013, row 0x00502620): Ego_r.pdb / FableWin.pdb ?LoadLevel@CWorldMap@@IAEXJW4ELoadType@1@@Z (protected) and the
    // public static ?GetThingFilenameFromLevelFilename@CWorldMap@@SI?AVCWideString@@ABV2@@Z (retail 0x004FAFF0, __fastcall).
protected:
    void LoadLevel(long map, ELoadType loadType);                                         // 0x00502620
public:
#ifndef FABLE_RETAIL_CWORLDMAP_LOADTHINGSWITHSECTIONNAMES
    static CWideString __fastcall GetThingFilenameFromLevelFilename(const CWideString& levelFilename);   // 0x004FAFF0
#endif
#endif
#ifdef FABLE_RETAIL_CWORLDMAP_LOADWORLDFROMBINARY
    // Retail opt-in (chunk 1013b, row 0x00507650): Ego_r.pdb / FableWin.pdb ?LoadWorldFromBinaryFile@CWorldMap@@QAEXAAVCWideString@@@Z
    // and the protected ?SetMapPlacement@CWorldMap@@IAEXABVC2DBoxI@@J@Z (retail 0x004FC9C0).
    void LoadWorldFromBinaryFile(CWideString& filename);
protected:
    void SetMapPlacement(const C2DBoxI& area, long map);
public:
#endif
#ifdef FABLE_RETAIL_CWORLDMAP_POPULATE
    // Retail opt-in (chunk 879, row 0x00502E90; re-based by agent wfE10): Ego_r.pdb / FableWin.pdb
    // ?PopulateRegionWithVillagers@CWorldMap@@QAEXAAVCThing@@@Z (public, called by PostRegionLoad 0x005064C0). Declaration only.
    void PopulateRegionWithVillagers(CThing& village);       // 0x00502E90
#endif
#ifdef FABLE_RETAIL_CWORLDMAP_LOADFROMFILE
    // Retail opt-in (chunk H40g, row 0x00507C30 CWorldMap::LoadFromFile; re-based by agent wfE10): Ego_r.pdb names bound by argument
    // shape at the call sites of 0x00507C30: ?AllocateMapUID@CWorldMap@@IAEKJ@Z 0x004FCE20, ?IsMapPlacementPositionFree@CWorldMap@@IAE_NABVC2DBoxI@@@Z
    // 0x004FCA50, ?GetLevelDimensions@CWorldMap@@QBE?AVC2DBoxI@@ABVCCharString@@@Z 0x00506E30, ?SaveWorldToBinaryFile@CWorldMap@@QAEXAAVCWideString@@@Z
    // 0x004FBDE0, ?CompileLoadableGlobalThingsToSingleFile@CWorldMap@@IAEXABVCWideString@@@Z 0x004FE030,
    // ?LoadAllLoadableGlobalThingsFromSingleFile@CWorldMap@@IAEXABVCWideString@@@Z 0x004FE2A0, ?LoadLoadableGlobalThings@CWorldMap@@IAEXXZ 0x004FDBC0,
    // ?LoadRegionGraph@CWorldMap@@QAEXABVCWideString@@@Z 0x00506D40. Declarations only.
#ifdef FABLE_RETAIL_CWORLDMAP_ALLOCATEMAPUID_PROTECTED
    // Add-only nested opt-in (NF293, 2026-11-23; row 0x00507C30): Ego_r.pdb + FableWin.pdb ?AllocateMapUID@CWorldMap@@IAEKJ@Z (protected,
    // 0x004FCE20). Declaration only.
protected:
#endif
    unsigned long AllocateMapUID(long map);
#ifdef FABLE_RETAIL_CWORLDMAP_ALLOCATEMAPUID_PROTECTED
public:
#endif
    bool IsMapPlacementPositionFree(const C2DBoxI& area);
    C2DBoxI GetLevelDimensions(const CCharString& levelName) const;
    void SaveWorldToBinaryFile(CWideString& filename);
    void CompileLoadableGlobalThingsToSingleFile(const CWideString& filename);
    void LoadAllLoadableGlobalThingsFromSingleFile(const CWideString& filename);
    void LoadLoadableGlobalThings();
    void LoadRegionGraph(const CWideString& filename);
#endif
#ifdef FABLE_RETAIL_CWORLDMAP_GROUNDFACENORMAL
    // Nested opt-in (chunk Q14, row 0x0066C900 CRegionMinimap::IlluminateRegionMinimapTheme): Ego_r.pdb (0x006E7420) / FableWin.pdb
    // (0x024A4450) ?GetGroundFaceNormalAt@CWorldMap@@QBE?AVC3DVector@@ABV2@@Z, out of line in retail at 0x0063AB10 (the row calls it
    // with ecx = the world map, the position and the hidden result pushed; GetThemeFaceNormalAt 0x0063AB90 follows it). Declaration only.
    C3DVector GetGroundFaceNormalAt(const C3DVector& pos) const;                          // 0x0063AB10
#endif
#ifdef FABLE_RETAIL_CWORLDMAP_THEMEFACENORMAL
    // Nested opt-in (chunk 1013, row 0x0063AB90): Ego_r.pdb / FableWin.pdb ?GetThemeFaceNormalAt@CWorldMap@@QBE?AVC3DVector@@ABV2@@Z and
    // ?GetThemeSizeZAtBlock@CWorldMap@@QBEMABVC2DCoordI@@@Z (retail 0x0063A7E0). Declarations only.
    C3DVector GetThemeFaceNormalAt(const C3DVector& pos) const;                           // 0x0063AB90
    float GetThemeSizeZAtBlock(const C2DCoordI& pos) const;                               // 0x0063A7E0
#endif
#ifdef FABLE_RETAIL_CWORLDMAP_HEIGHTAREA
    // Nested opt-in (chunk 1013, row 0x004FEB70 CWorldMap::HeightAreaChanged): Ego_r.pdb / FableWin.pdb
    // ?ClipAndConvertWorldMapAreaToLocalArea@CWorldMap@@QBE?AVC2DBoxI@@JABV2@@Z (retail 0x004FC4D0, C2DBoxI by hidden pointer) and the
    // protected ?UpdateAllEdges@CWorldMap@@IAEXXZ (retail 0x0063DFD0). Declarations only.
    C2DBoxI ClipAndConvertWorldMapAreaToLocalArea(long map, const C2DBoxI& area) const;   // 0x004FC4D0
protected:
    void UpdateAllEdges();                                                                // 0x0063DFD0
public:
#endif
#ifdef FABLE_RETAIL_CWORLDMAP_LINEOFSIGHT
    // Nested opt-in (chunk 1013, row 0x0063E6F0): Ego_r.pdb ?ConvertWorldMapCoordsToLocalCoords@CWorldMap@@QBE?AVC3DVector@@JABV2@@Z
    // (Ego_r 0x004CFA40; retail 0x004FC2A0, C3DVector by hidden pointer), ?IsLineOfSight@CWorldMap@@QBE_NABVC3DVector@@0@Z (Ego_r
    // 0x006E8890; retail 0x0063AEE0) and ?IsLineOfSight@CWorldMap@@QBE_NABVCThing@@0_N@Z (Ego_r 0x006EB120; retail 0x0063E6F0).
    // Declarations only: no layout change.
    C3DVector ConvertWorldMapCoordsToLocalCoords(long map, const C3DVector& pos) const;   // 0x004FC2A0
    bool IsLineOfSight(const C3DVector& from, const C3DVector& to) const;                 // 0x0063AEE0
    bool IsLineOfSight(const CThing& from, const CThing& to, bool checkThings) const;     // 0x0063E6F0
#endif
#ifdef FABLE_RETAIL_CWORLDMAP_FINDLASTUNBLOCKEDGROUND
    // Retail opt-in (chunk 2035, row 0x006FDD60 CGameCameraBase::Update): Ego_r.pdb / FableWin.pdb
    // ?FindLastUnblockedGroundPositionAndReportCollision@CWorldMap@@QBE_NABVC3DVector@@0AAV2@@Z, out of line in retail at 0x0063D960
    // (the landed bound spelling; called on GetWorldMap() with the from / to vectors and the result out-reference, 0x006FE382).
    // Declaration only: no layout change.
    bool FindLastUnblockedGroundPositionAndReportCollision(const C3DVector& from, const C3DVector& to, C3DVector& result) const;   // 0x0063D960
#endif
#ifdef FABLE_RETAIL_CWORLDMAP_FINDMAXZINCELL
    // Opt-in (NF52, rows 0x0063B750 / 0x0063B7F0): Ego_r.pdb ?IsPosInMap@CWorldMap@@QBE_NABVC2DCoordI@@@Z (Ego_r 0x004CFD60; retail
    // 0x004FC5B0), ?GetGroundSizeZAtBlock@CWorldMap@@QBEMABVC2DCoordI@@@Z (Ego_r 0x006E6F30; retail 0x0063A660) and the protected
    // ?FindMaxGroundZInCell@CWorldMap@@IBEMPBVC3DVector@@@Z / ?FindMaxThemeZInCell@CWorldMap@@IBEMPBVC3DVector@@@Z (Ego_r 0x006E78E0 /
    // 0x006E7A00; retail 0x0063B750 / 0x0063B7F0). GetThemeSizeZAtBlock is FABLE_RETAIL_CWORLDMAP_THEMEFACENORMAL's. Declarations only.
    bool IsPosInMap(const C2DCoordI& pos) const;                                          // 0x004FC5B0
    float GetGroundSizeZAtBlock(const C2DCoordI& pos) const;                              // 0x0063A660
protected:
    float FindMaxGroundZInCell(const C3DVector* cell) const;                              // 0x0063B750
    float FindMaxThemeZInCell(const C3DVector* cell) const;                               // 0x0063B7F0
public:
#endif
#ifdef FABLE_RETAIL_CWORLDMAP_COLLISION_CORNERS_PASS23
    // Original Ego_r/FableWin typed symbols: protected static const C2DCoordI[4].
    // Donor024a8ab2 indexes LineCollisionAround4Square; native0063d198 uses0137b730.
protected:
    static const C2DCoordI LineCollisionAround4Square[4];
public:
#endif
#ifdef FABLE_RETAIL_CWORLDMAP_LINESEGMENT
    // Nested opt-in (chunk 1013, rows 0x0063B3F0 / 0x0063D0A0 / 0x0063D4A0): Ego_r.pdb protected const
    // ?CreateLineSegment@CWorldMap@@IBE_NPBVC3DVector@@0PAV2@1@Z (Ego_r 0x006E7530; retail 0x0063B3F0),
    // ?FindGroundCollisionPointWithinCell@CWorldMap@@IBE_NPBVC3DVector@@00PAV2@1@Z (Ego_r 0x006E9B50; retail 0x0063D0A0) and
    // ?FindThemeCollisionPointWithinCell@CWorldMap@@IBE_NPBVC3DVector@@00PAV2@1@Z (Ego_r 0x006E9F10; retail 0x0063D4A0).
    // Declarations only: no layout change.
protected:
    bool CreateLineSegment(const C3DVector* from, const C3DVector* to, C3DVector* segStart, C3DVector* segEnd) const;
    bool FindGroundCollisionPointWithinCell(const C3DVector* start, const C3DVector* end, const C3DVector* cellPos, C3DVector* direction, C3DVector* hit) const;
    bool FindThemeCollisionPointWithinCell(const C3DVector* start, const C3DVector* end, const C3DVector* cellPos, C3DVector* direction, C3DVector* hit) const;
    // Their callees (Ego_r.pdb protected const, retail out of line): ?PointIsWithinLineSegment@CWorldMap@@IBE_NPBVC3DVector@@00@Z
    // 0x0063BAB0 and ?PointIsWithinCellTR/BL/TL/BR@CWorldMap@@IBE_NPBVC3DVector@@0@Z 0x0063B890 / 0x0063B920 / 0x0063B9B0 / 0x0063BA30
    // (Ego_r 0x006E7B10 .. 0x006E7D30 in the same order); public ?GetThemeSizeZAt@CWorldMap@@QBEMABVC3DVector@@@Z 0x0063A390.
    bool PointIsWithinLineSegment(const C3DVector* point, const C3DVector* start, const C3DVector* end) const;   // 0x0063BAB0
    bool PointIsWithinCellTR(const C3DVector* point, const C3DVector* cell) const;                             // 0x0063B890
    bool PointIsWithinCellBL(const C3DVector* point, const C3DVector* cell) const;                             // 0x0063B920
    bool PointIsWithinCellTL(const C3DVector* point, const C3DVector* cell) const;                             // 0x0063B9B0
    bool PointIsWithinCellBR(const C3DVector* point, const C3DVector* cell) const;                             // 0x0063BA30
public:
    float GetThemeSizeZAt(const C3DVector& pos) const;                                                         // 0x0063A390
public:
#endif
#ifdef FABLE_RETAIL_CWORLDMAP_THEMEMINCAMERASIZEZOFFSET
    // Opt-in (NF62, row 0x0063A540): Ego_r.pdb ?GetThemeMinCameraSizeZOffsetAt@CWorldMap@@QBEMABVC3DVector@@@Z (retail 0x0063A540).
    // Declaration only.
    float GetThemeMinCameraSizeZOffsetAt(const C3DVector& pos) const;   // 0x0063A540
#endif
#ifdef FABLE_RETAIL_CWORLDMAP_CONVERTCOORDSTOLOCALBLOCK
    // NF70 opt-in (round 7, rows 0x0063A7E0 / 0x0063AE70): Ego_r.pdb 0x004CFB60 / FableWin.pdb 0x01C83150
    // ?ConvertWorldMapCoordsToLocalCoords@CWorldMap@@QBE?AVC2DCoordI@@JABV2@@Z (retail 0x004FC3C0, out of line; landed 0x0063A910 binds it).
    // Declaration only: no layout change.
    C2DCoordI ConvertWorldMapCoordsToLocalCoords(long map, const C2DCoordI& pos) const;   // 0x004FC3C0
#endif
#ifdef FABLE_RETAIL_CWORLDMAP_STRONGESTTHEMEATBLOCK
    // NF70 opt-in (round 7, row 0x0063AE70): Ego_r.pdb 0x006E74B0 / FableWin.pdb 0x024A4C40
    // ?GetStrongestEngineThemeAtBlock@CWorldMap@@QBEJABVC2DCoordI@@AAJ@Z. Declaration only: no layout change.
    long GetStrongestEngineThemeAtBlock(const C2DCoordI& pos, long& strength) const;      // 0x0063AE70
#endif
#ifdef FABLE_RETAIL_CWORLDMAP_THEMEATBLOCK
    // NF81 opt-in (round 7, q10976 / q10977..q10980 binders, rows 0x0063A970 / 0x0063A9E0 / 0x0063AA50 / 0x0063AAB0): Ego_r.pdb 0x006E7260 /
    // 0x006E72D0 / 0x006E7340 / 0x006E73B0 and FableWin.pdb 0x024A41D0 / 0x024A4280 / 0x024A4330 / 0x024A43C0
    // ?SetThemeAtBlock@CWorldMap@@QAEXABVC2DCoordI@@KKW4EThemePaintType@@@Z, ?SetThemeBlendAtBlock@CWorldMap@@QAEXABVC2DCoordI@@EKW4EThemePaintType@@@Z,
    // ?GetThemeAtBlock@CWorldMap@@QBEKABVC2DCoordI@@KW4EThemePaintType@@@Z, ?GetThemeBlendAtBlock@CWorldMap@@QBEKABVC2DCoordI@@KW4EThemePaintType@@@Z
    // (public; Ego_d.pdb parameter names). Needs engine/CMap.h with FABLE_RETAIL_CMAP_ENGINETHEMEAT included first (EThemePaintType).
    // Declarations only: no layout change.
    void SetThemeAtBlock(const C2DCoordI& pos, unsigned long theme, unsigned long theme_index, EThemePaintType PaintType);        // 0x0063A970
    void SetThemeBlendAtBlock(const C2DCoordI& pos, unsigned char blend, unsigned long theme_index, EThemePaintType PaintType);   // 0x0063A9E0
    unsigned long GetThemeAtBlock(const C2DCoordI& pos, unsigned long theme_index, EThemePaintType PaintType) const;             // 0x0063AA50
    unsigned long GetThemeBlendAtBlock(const C2DCoordI& pos, unsigned long theme_index, EThemePaintType PaintType) const;        // 0x0063AAB0
#endif
#ifdef FABLE_RETAIL_CWORLDMAP_NAVIGATIONSECTION
    // Opt-in (NF62, row 0x004FE630): Ego_r.pdb protected const ?GetNavigationSectionOffsetInFile@CWorldMap@@IBE?AU?$pair@VCCharString@@J@std@@
    // AAVCAFile@@ABV?$set@VCCharString@@U?$less@VCCharString@@@std@@V?$allocator@VCCharString@@@3@@3@@Z (Ego_r 0x004D24E0; retail
    // 0x004FE630). Needs stlport/fable_map.h (set) and fable_string.h ahead. Declaration only.
protected:
    std::pair<CCharString,long> GetNavigationSectionOffsetInFile(CAFile& file, const std::set<CCharString, std::less<CCharString>, std::allocator<CCharString> >& sections) const;   // 0x004FE630
public:
#endif
#ifdef FABLE_RETAIL_CWORLDMAP_CREATENAVMAP
    // Opt-in (NF62, row 0x00500230): Ego_r.pdb ?CreateNavMap@CWorldMap@@QAEXJABV?$set@VCCharString@@U?$less@VCCharString@@@std@@V?$allocator@
    // VCCharString@@@3@@std@@AAVCAFile@@AAVCASuspendableProcess@@@Z (public; retail 0x00500230, `ret 10h`). Declaration only.
    void CreateNavMap(long map, const std::set<CCharString, std::less<CCharString>, std::allocator<CCharString> >& sections, CAFile& file,
                      CASuspendableProcess& process);   // 0x00500230
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
FABLE_STATIC_ASSERT_AT(309, sizeof(CWorldMap) == 0xd8);
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
#ifdef FABLE_RETAIL_CWORLDMAP_HOVERQUEEN_QUERY_PASS23
 // Retail opt-in (Codex pass23): exact original const-reference query, no data-layout change.
 float GetGroundSizeZAt(const class C3DVector& pos) const;
#endif
#ifdef FABLE_RETAIL_CWORLDMAP_CYLINDER_QUERY
 // Retail opt-in (Codex pass23): original const-reference query, no data-layout change.
 float GetHeightZBelowPos(const C3DVector& pos) const;
#endif
#ifdef FABLE_RETAIL_CWORLDMAP_FLAT_ISLOADINGANYMAP
 // Add-only opt-in (naming agent NF292, 2026-11-23, binder 0x006E11F0 CTCDParticleEmitter::Create): Ego_r.pdb / FableWin.pdb
 // ?IsLoadingAnyMap@CWorldMap@@QBE_NXZ (retail 0x004FB4A0), as the retail branch declares it. Declaration only: no layout change.
 bool IsLoadingAnyMap() const;                                      // 0x004FB4A0
#endif
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

FABLE_STATIC_ASSERT_AT(372, sizeof(CWorldMap) == 0xd8);
FABLE_STATIC_ASSERT_AT(373, offsetof(CWorldMap, __vftable) == 0x0);
FABLE_STATIC_ASSERT_AT(374, offsetof(CWorldMap, f4) == 0x4);
FABLE_STATIC_ASSERT_AT(375, offsetof(CWorldMap, World) == 0x8);
FABLE_STATIC_ASSERT_AT(376, offsetof(CWorldMap, Component) == 0xc);
FABLE_STATIC_ASSERT_AT(377, offsetof(CWorldMap, PThingMapManager_ptr) == 0x10);
FABLE_STATIC_ASSERT_AT(378, offsetof(CWorldMap, LoadedScriptedMapBrushes) == 0x14);
FABLE_STATIC_ASSERT_AT(379, offsetof(CWorldMap, Maps) == 0x20);
FABLE_STATIC_ASSERT_AT(380, offsetof(CWorldMap, Regions) == 0x2c);
FABLE_STATIC_ASSERT_AT(381, offsetof(CWorldMap, RegionTime) == 0x38);
FABLE_STATIC_ASSERT_AT(382, offsetof(CWorldMap, PRegionGraph_Data) == 0x44);
FABLE_STATIC_ASSERT_AT(383, offsetof(CWorldMap, PRegionGraph_Info) == 0x48);
FABLE_STATIC_ASSERT_AT(384, offsetof(CWorldMap, SoundThemesDisabled) == 0x4c);
FABLE_STATIC_ASSERT_AT(385, offsetof(CWorldMap, Granularity) == 0x50);
FABLE_STATIC_ASSERT_AT(386, offsetof(CWorldMap, GranularityShift) == 0x54);
FABLE_STATIC_ASSERT_AT(387, offsetof(CWorldMap, MapPlacementPitch) == 0x58);
FABLE_STATIC_ASSERT_AT(388, offsetof(CWorldMap, Area_TLX) == 0x5c);
FABLE_STATIC_ASSERT_AT(389, offsetof(CWorldMap, Area_TLY) == 0x60);
FABLE_STATIC_ASSERT_AT(390, offsetof(CWorldMap, Area_BRX) == 0x64);
FABLE_STATIC_ASSERT_AT(391, offsetof(CWorldMap, Area_BRY) == 0x68);
FABLE_STATIC_ASSERT_AT(392, offsetof(CWorldMap, MapPlacement) == 0x6c);
FABLE_STATIC_ASSERT_AT(393, offsetof(CWorldMap, PlayerPositionInfo) == 0x78);
FABLE_STATIC_ASSERT_AT(394, offsetof(CWorldMap, HeightChangedAreas) == 0x84);
FABLE_STATIC_ASSERT_AT(395, offsetof(CWorldMap, LoadedMaps) == 0x90);
FABLE_STATIC_ASSERT_AT(396, offsetof(CWorldMap, LoadedRegion) == 0x9c);
FABLE_STATIC_ASSERT_AT(397, offsetof(CWorldMap, LevelWADBank_Data) == 0xa0);
FABLE_STATIC_ASSERT_AT(398, offsetof(CWorldMap, LevelWADBank_Info) == 0xa4);
FABLE_STATIC_ASSERT_AT(399, offsetof(CWorldMap, UseLevelWADBank) == 0xa8);
FABLE_STATIC_ASSERT_AT(400, offsetof(CWorldMap, CurrentMapUIDCount) == 0xac);
FABLE_STATIC_ASSERT_AT(401, offsetof(CWorldMap, DestroyedThemePositions) == 0xb0);
FABLE_STATIC_ASSERT_AT(402, offsetof(CWorldMap, PLevelLoader_Data) == 0xbc);
FABLE_STATIC_ASSERT_AT(403, offsetof(CWorldMap, PLevelLoader_Info) == 0xc0);
FABLE_STATIC_ASSERT_AT(404, offsetof(CWorldMap, PCameraRegionParticle) == 0xc4);
FABLE_STATIC_ASSERT_AT(405, offsetof(CWorldMap, PHeroBreathParticle) == 0xcc);
FABLE_STATIC_ASSERT_AT(406, offsetof(CWorldMap, RegionUnloading) == 0xd4);
FABLE_STATIC_ASSERT_AT(407, offsetof(CWorldMap, PassabilityBeingShown) == 0xd5);

#endif // FABLE_RETAIL_CWORLDMAP
#endif // FABLE_ENGINE_CWORLDMAP_H
