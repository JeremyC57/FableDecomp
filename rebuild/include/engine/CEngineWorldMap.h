#ifndef FABLE_ENGINE_CENGINEWORLDMAP_H
#define FABLE_ENGINE_CENGINEWORLDMAP_H
#ifdef FABLE_RETAIL_CENGINEWORLDMAP
// Retail opt-in (chunk 723): CEngineWorldMap : CEngineComponent (RTTI); members from +0x0C as the default model below, MapList typed
// (CArray<CEngineMap*>, retail +0x20) with the inline count / index accessors (Ego_r.pdb / FableWin.pdb
// ?GetMapCount@CEngineWorldMap@@QBEJXZ, ?GetMap@CEngineWorldMap@@QBEPAVCEngineMap@@J@Z): ~CEngineLandscapeRenderer 0x00B67920 loops
// `i < GetMapCount()` as a signed pointer difference (`sub ; sar 2 ; cmp ; jl`) and reads `MapList[i]`. Other members opaque.
#ifndef FABLE_RETAIL_CENGINECOMPONENT
#define FABLE_RETAIL_CENGINECOMPONENT
#endif
#include <stddef.h>
#include "rebuild_abi.h"
#include "engine/CArray.h"
#include "engine/CEngineComponent.h"
class CEngineMap;
class CIDrawWorld;
class CIDrawWorldMap;
class CWideStringData;
class CEngineInit;
class CEngineWorldMap : public CEngineComponent {
public:
    CEngineWorldMap(const CEngineInit& init);                            // 0x00B423F0 ??0CEngineWorldMap@@QAE@ABVCEngineInit@@@Z (chunk 731)
    virtual ~CEngineWorldMap();                                          // ??1CEngineWorldMap@@UAE@XZ
    virtual unsigned long PeekSceneFilterFlags() const;                  // ?PeekSceneFilterFlags@CEngineWorldMap@@UBEKXZ (chunk 731)
    long GetMapCount() const { return MapList.size(); }
    CEngineMap* GetMap(long index) const { return MapList[index]; }
#ifdef FABLE_RETAIL_CENGINEWORLDMAP_ONGAMEMAPLOADED
    // NF56 opt-in (row 0x00B401E0): Ego_r.pdb 0x009E0560 / FableWin.pdb 0x02D615D0 ?OnGameMapLoaded@CEngineWorldMap@@QAEXJ@Z (public), out of
    // line at 0x00B401E0 (the CEngine::SetMapAsLoaded 0x00B23E30 tail-jump target). Declaration only.
    void OnGameMapLoaded(long index);                                    // 0x00B401E0
#endif
#ifdef FABLE_RETAIL_CENGINEWORLDMAP_ONGAMEMAPUNLOADED
    // NF160 opt-in (row 0x00B40230): Ego_r.pdb 0x009E05B0 / FableWin.pdb 0x02D61810 ?OnGameMapUnloaded@CEngineWorldMap@@QAEXJ@Z (public), out of
    // line at 0x00B40230 (the CEngine::SetMapAsUnloaded 0x00B23E60 tail-jump target). Declaration only.
    void OnGameMapUnloaded(long index);                                  // 0x00B40230
#endif
#ifdef FABLE_RETAIL_CENGINEWORLDMAP_SETMAPVISIBILITY
    // NF160 opt-in (row 0x00B42390): Ego_r.pdb 0x009E2500 / FableWin.pdb 0x02D625D0 ?SetMapVisibility@CEngineWorldMap@@QAEXJ_N@Z (public), out of line
    // at 0x00B42390 (the CEngine::SetMapVisibility 0x00B23E40 tail-jump target). Declaration only.
    void SetMapVisibility(long index, bool visible);                     // 0x00B42390
#endif
#ifdef FABLE_RETAIL_CENGINEWORLDMAP_SETMAPVISIBILITYALLMAPS
    // NF160 opt-in (row 0x00B423B0): Ego_r.pdb 0x009E2520 / FableWin.pdb 0x02D62820 ?SetMapVisibilityAllMaps@CEngineWorldMap@@QAEX_N@Z
    // (public), out of line at 0x00B423B0 (the CEngine::SetMapVisibilityAllMaps 0x00B23E50 tail-jump target). Declaration only.
    void SetMapVisibilityAllMaps(bool visible);                          // 0x00B423B0
#endif
#ifdef FABLE_RETAIL_CENGINEWORLDMAP_GETMAPXY
    // Retail opt-in (chunk 1013b): FableWin.pdb ?GetMap@CEngineWorldMap@@QBEPAVCEngineMap@@JJ@Z, inline in retail: the map that
    // owns world cell (x, y) from the 32x32-cell index grid Map (0 or less = none), NULL off the grid. Expanded in
    // CEngineMap::PeekThemeId 0x00B43720 (`test edx,edx ; jl` .. `shl ebp,5 ; cmp` .. `movsx eax,word [edx+eax*2] ; test ; jle`
    // .. `mov eax,[edx+eax*4]`) and its two siblings.
#ifdef FABLE_RETAIL_CENGINEWORLDMAP_GETMAP_ORIGINAL_PASS26
    // Pass26 00b43720/00b43870/00b439c0. Original donor02cc98a0
    // subtracts offsets in the callee, then positive nested tests and direct MapList[].
    CEngineMap* GetMap(long world_x,long world_y) const
    {
        world_x-=MapOffsetX;
        world_y-=MapOffsetY;
        if (world_x>=0 && world_y>=0) {
            if (world_x<MapArrayWidth*32 && world_y<MapArrayHeight*32) {
                long index=Map[(world_x>>5)+(world_y>>5)*MapArrayWidth];
                if (index>0) return MapList[index];
                else return 0;
            }
        }
        return 0;
    }
#else
    CEngineMap* GetMap(long x, long y) const
    {
        if (x < 0 || y < 0 || x >= MapArrayWidth * 32 || y >= MapArrayHeight * 32)
            return 0;
        long index = Map[(y >> 5) * MapArrayWidth + (x >> 5)];
        if (index <= 0)
            return 0;
        return GetMap(index);
    }
#endif
#endif
#ifdef FABLE_RETAIL_CENGINEWORLDMAP_PEEKLANDSCAPE
    // Retail opt-in (chunk 2037, row 0x00B52DD0 CEngineWeatherRenderer::ClipBoxToLandscape): FableWin.pdb
    // ?MapHasDataAtPoint@CEngineWorldMap@@QBE_NJJ@Z (also Ego_r.pdb 0x009FC700) and ?PeekLandscapeHeight@CEngineWorldMap@@QBEMJJ@Z,
    // inline in retail (FableWin keeps their COMDATs right after ClipBoxToLandscape, 0x02D3B1F0 / 0x02D3B260). Bodies in
    // engine/CEngineMap.h (FABLE_RETAIL_CENGINEMAP_PEEK_BODIES): they need CEngineMap complete.
    bool MapHasDataAtPoint(long x, long y) const;
    float PeekLandscapeHeight(long x, long y) const;
#endif
    short*         Map;                                    // +0x0c
    long             MapArrayWidth;                          // +0x10
    long             MapArrayHeight;                         // +0x14
    long             MapOffsetX;                             // +0x18
    long             MapOffsetY;                             // +0x1c
    CArray<CEngineMap*> MapList;                             // +0x20
#ifdef FABLE_RETAIL_CENGINEWORLDMAP_VISIBLEMAPLIST
    // Opt-in (NF46 round 7, rows 0x00B599C0 / 0x00B59A30 / 0x00B684F0): Ego_r.pdb VisibleMapList +0x2c, std::list<CEngineMap*> (the 4-byte
    // retail STLport list; the rows walk `_M_node->_M_next .. _M_node` from GPEngineWorldMap + 0x2C). Same size and later offsets.
    std::list<CEngineMap*> VisibleMapList;                   // +0x2c
#endif
#ifdef FABLE_RETAIL_CENGINEWORLDMAP_GAMEWORLD
    // Retail opt-in (chunk 2031, row 0x00B65A20 CEngineSkyRenderer::RenderOuterSky): Ego_r.pdb GameWorld (CIDrawWorld*) at retail +0x198
    // with FableWin.pdb ?PeekGameWorld@CEngineWorldMap@@QBEPBVCIDrawWorld@@XZ, inline in retail (0x00B65A48..0x00B65A55:
    // `mov ecx,[map+198h] ; mov edx,[ecx] ; call [edx+4]`, the CIDrawWorld slot-1 DrawGetEnvironment). The members around it stay opaque.
    const CIDrawWorld* PeekGameWorld() const { return GameWorld; }
#ifdef FABLE_RETAIL_CENGINEWORLDMAP_GAMEWORLDMAP
    // Opt-in (NF46 round 7, row 0x00B401E0): Ego_r.pdb GameWorldMap (CIDrawWorldMap*) at retail +0x19C, right after GameWorld, with FableWin.pdb
    // ?PeekGameWorldMap@CEngineWorldMap@@QBEPBVCIDrawWorldMap@@XZ, inline in retail (0x00B401E0: `mov ecx,[map+19Ch] ; call [edx+4]`, the
    // CIDrawWorldMap slot-1 DrawGetMapInfo). Same size and later offsets.
    const CIDrawWorldMap* PeekGameWorldMap() const { return GameWorldMap; }
#endif
#ifdef FABLE_RETAIL_CENGINEWORLDMAP_STATICMAPFILE
    // NF65 overlay opt-in (row 0x00B3FF70): Ego_r.pdb / FableWin.pdb ?CreateNewBankMap@CEngineWorldMap@@AAEKABVCCharString@@@Z (private,
    // out of line at 0x00B3FF70) and the StaticMapFile member (Ego_r.pdb CStaticMapBankFile; retail +0x34: 0x00B3FFC3 `lea edi,[ebx+34h]`
    // before CBankFile::AddEntry / FindIndexBySymbol), after StaticMapBankName (CWideString, +0x30, opaque here). Same size and later
    // offsets. Needs the FABLE_RETAIL_CSTATICMAPBANKFILE_CLASS class first.
private:
    unsigned long CreateNewBankMap(const CCharString& name);             // 0x00B3FF70 (private in both PDBs: AAE)
#ifdef FABLE_RETAIL_CENGINEWORLDMAP_OPENSTATICMAPS
    // NF65 overlay opt-in (row 0x00B42750): Ego_r.pdb ?OpenStaticMaps@CEngineWorldMap@@AAEXW4STATIC_MAP_OPEN_STATE@CEngineMap@@@Z (0x009E7B10)
    // and the private members it calls, out of line: ?AutoBuildWorldMapGrid@CEngineWorldMap@@AAEXXZ (0x00B40070, the landed binding) and
    // ?OpenStaticMapsUsingCommonRetailHeader@CEngineWorldMap@@AAEXJ@Z (Ego_r.pdb 0x009E75F0; retail 0x00B420F0 is the call OpenStaticMaps makes
    // with the index of CommonStaticMapHeaderEntryName). Also the StaticMapBankName / StaticMapOpenState members (+0x30 / +0x1A8, typed only
    // under this macro). Needs fable_string.h and engine/CEngineMap.h first. Declarations only.
    void OpenStaticMaps(CEngineMap::STATIC_MAP_OPEN_STATE state);           // 0x00B42750
    void AutoBuildWorldMapGrid();                                          // 0x00B40070
    void OpenStaticMapsUsingCommonRetailHeader(long entry);                // 0x00B420F0
#endif
#ifdef FABLE_RETAIL_CENGINEWORLDMAP_LOADWATERDATA
    // NF76 opt-in (row 0x00B41FA0): Ego_r.pdb 0x009E2760 / FableWin.pdb 0x02D5FE00 ?LoadWaterData@CEngineWorldMap@@AAEXXZ (private).
    // Declaration only.
    void LoadWaterData();                                                  // 0x00B41FA0
#endif
#ifndef FABLE_RETAIL_CENGINEWORLDMAP_VISIBLEMAPLIST
    unsigned char       _pad_0x2c[0x4];                      // +0x2c VisibleMapList
#endif
#ifdef FABLE_RETAIL_CENGINEWORLDMAP_OPENSTATICMAPS
    CWideString         StaticMapBankName;                   // +0x30
#else
    unsigned char       StaticMapBankName[0x4];              // +0x30 CWideString
#endif
    CStaticMapBankFile  StaticMapFile;                       // +0x34
#else
private:
#ifdef FABLE_RETAIL_CENGINEWORLDMAP_VISIBLEMAPLIST
    unsigned char       _pad_0x30[0x168];                    // +0x30 (after the _VISIBLEMAPLIST member)
#else
    unsigned char       _pad_0x2c[0x16c];                    // +0x2c
#endif
#endif
    CIDrawWorld*        GameWorld;                           // +0x198
#ifdef FABLE_RETAIL_CENGINEWORLDMAP_GAMEWORLDMAP
    CIDrawWorldMap*     GameWorldMap;                        // +0x19c
    unsigned char       _pad_0x1a0[0x20];                    // +0x1a0 StaticMapQuality .. (not modelled)
#else
#ifdef FABLE_RETAIL_CENGINEWORLDMAP_OPENSTATICMAPS
    unsigned char       _pad_0x19c[0xc];                     // +0x19c GameWorldMap .. (not modelled)
    CEngineMap::STATIC_MAP_OPEN_STATE StaticMapOpenState;    // +0x1a8
    unsigned char       _pad_0x1ac[0x14];                    // +0x1ac
#else
    unsigned char       _pad_0x19c[0x24];                    // +0x19c GameWorldMap .. (not modelled)
#endif
#endif
#else
private:
    // Chunk 731: retail CEngineWorldMap is 0x1C0 bytes (CEngine::Initialise 0x00B26360 news 0x1C0 before ??0CEngineWorldMap@@QAE@
    // ABVCEngineInit@@@Z 0x00B423F0), not the 0x360 the generated container-size model gives: the two CStaticMapBankFile members
    // are smaller in retail. The members after MapList (VisibleMapList .. CommonHeaderPosLookupTable, see the default model
    // below) are opaque until their retail offsets are proven; no landed row reads them (0x00B67920 uses MapList only).
#ifdef FABLE_RETAIL_CENGINEWORLDMAP_VISIBLEMAPLIST
    unsigned char       _pad_0x30[0x190];                    // +0x30 (after the _VISIBLEMAPLIST member)
#else
    unsigned char       _pad_0x2c[0x194];                    // +0x2c
#endif
#endif
};
FABLE_STATIC_ASSERT_AT(74, sizeof(CEngineWorldMap) == 0x1c0);
FABLE_STATIC_ASSERT_AT(75, offsetof(CEngineWorldMap, MapList) == 0x20);
#else // flattened Ego_r layout (the landed default, unchanged)
// GENERATED by tools/decomp_pipeline/gen_class_headers.py -- do not edit by hand.
// Donor: Ego_r.pdb (ghidra_out/struct_layouts_egor.tsv) sizeof CEngineWorldMap = 0x464.
// Container layout: retail STLport 4.6 (list 4, vector/map/set/string/CArray 0xc, deque 0x28, CVectorMap 0x10, bases from Fable.exe RTTI): sizeof CEngineWorldMap = 0x360.
// Retail evidence: 0 trusted landed function(s); reconciliation clean.
// Members are original Lionhead names; `_pad_*` are unnamed bytes; opaque
// `unsigned char X[N]` members carry their real (composite/template) type in a comment.
#include <stddef.h>
#include "rebuild_abi.h"

class CIDrawWorld;
class CIDrawWorldMap;
class CWideStringData;

#pragma pack(push, 1)
class CEngineWorldMap {
public:
    void*            __vftable;                              // +0x00 vptr, or first dword of a flattened base subobject
    unsigned char    _pad_0x04[0x4];                         // +0x04
    unsigned char    _pad_0x08[0x4];                         // +0x08
    void*            Map;                                    // +0x0c short *
    long             MapArrayWidth;                          // +0x10
    long             MapArrayHeight;                         // +0x14
    long             MapOffsetX;                             // +0x18
    long             MapOffsetY;                             // +0x1c
    unsigned char    MapList[0xc];                           // +0x20 CArray<CEngineMap_*>
    unsigned char    VisibleMapList[0x4];                    // +0x2c list<CEngineMap_*,std::allocator<CEngineMap_*>_>
    CWideStringData* StaticMapBankName_PStringData;          // +0x30 CWideString::PStringData
    unsigned char    StaticMapFile[0x164];                   // +0x34 CStaticMapBankFile
    CIDrawWorld*     GameWorld;                              // +0x198
    CIDrawWorldMap*  GameWorldMap;                           // +0x19c
    long             StaticMapQuality;                       // +0x1a0
    bool             EnableMinimumStaticMapRebuild;          // +0x1a4
    bool             FirstWorldUpdate;                       // +0x1a5
    unsigned char    _pad_0x1a6[0x2];                        // +0x1a6
    unsigned char    StaticMapOpenState[0x4];                // +0x1a8 STATIC_MAP_OPEN_STATE
    bool             DynamicUpdateEnabled;                   // +0x1ac
    bool             BuildRetailStaticMaps;                  // +0x1ad
    unsigned char    _pad_0x1ae[0x2];                        // +0x1ae
    unsigned char    StaticMapBuildStatus[0x4];              // +0x1b0 STATIC_MAP_BUILD_STATUS
    unsigned char    ChangedAreas[0xc];                      // +0x1b4 CArray<C2DBoxI>
    unsigned char    IntermidiateStaticMapFile[0x164];       // +0x1c0 CStaticMapBankFile
    bool             GlobalMapDataLoaded;                    // +0x324
    unsigned char    _pad_0x325[0x3];                        // +0x325
    float            StaticMapGenerationEstimatedMaxCameraHeightAboveLandscape; // +0x328
    bool             StaticMapGlobalOptimizationsEnabled;    // +0x32c
    unsigned char    _pad_0x32d[0x1];                        // +0x32d
    unsigned char    _pad_0x32e[0x2];                        // +0x32e
    unsigned char    ForcedMapUpdateList[0xc];               // +0x330 CArray<CCharString>
    unsigned char    CommonHeaderStream[0x18];               // +0x33c CMemoryDataOutputStream
    unsigned char    CommonHeaderPosLookupTable[0xc];        // +0x354 map<CCharString,long,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,long>_>_>
};
#pragma pack(pop)

FABLE_STATIC_ASSERT_AT(130, sizeof(CEngineWorldMap) == 0x360);
FABLE_STATIC_ASSERT_AT(131, offsetof(CEngineWorldMap, __vftable) == 0x0);
FABLE_STATIC_ASSERT_AT(132, offsetof(CEngineWorldMap, Map) == 0xc);
FABLE_STATIC_ASSERT_AT(133, offsetof(CEngineWorldMap, MapArrayWidth) == 0x10);
FABLE_STATIC_ASSERT_AT(134, offsetof(CEngineWorldMap, MapArrayHeight) == 0x14);
FABLE_STATIC_ASSERT_AT(135, offsetof(CEngineWorldMap, MapOffsetX) == 0x18);
FABLE_STATIC_ASSERT_AT(136, offsetof(CEngineWorldMap, MapOffsetY) == 0x1c);
FABLE_STATIC_ASSERT_AT(137, offsetof(CEngineWorldMap, MapList) == 0x20);
FABLE_STATIC_ASSERT_AT(138, offsetof(CEngineWorldMap, VisibleMapList) == 0x2c);
FABLE_STATIC_ASSERT_AT(139, offsetof(CEngineWorldMap, StaticMapBankName_PStringData) == 0x30);
FABLE_STATIC_ASSERT_AT(140, offsetof(CEngineWorldMap, StaticMapFile) == 0x34);
FABLE_STATIC_ASSERT_AT(141, offsetof(CEngineWorldMap, GameWorld) == 0x198);
FABLE_STATIC_ASSERT_AT(142, offsetof(CEngineWorldMap, GameWorldMap) == 0x19c);
FABLE_STATIC_ASSERT_AT(143, offsetof(CEngineWorldMap, StaticMapQuality) == 0x1a0);
FABLE_STATIC_ASSERT_AT(144, offsetof(CEngineWorldMap, EnableMinimumStaticMapRebuild) == 0x1a4);
FABLE_STATIC_ASSERT_AT(145, offsetof(CEngineWorldMap, FirstWorldUpdate) == 0x1a5);
FABLE_STATIC_ASSERT_AT(146, offsetof(CEngineWorldMap, StaticMapOpenState) == 0x1a8);
FABLE_STATIC_ASSERT_AT(147, offsetof(CEngineWorldMap, DynamicUpdateEnabled) == 0x1ac);
FABLE_STATIC_ASSERT_AT(148, offsetof(CEngineWorldMap, BuildRetailStaticMaps) == 0x1ad);
FABLE_STATIC_ASSERT_AT(149, offsetof(CEngineWorldMap, StaticMapBuildStatus) == 0x1b0);
FABLE_STATIC_ASSERT_AT(150, offsetof(CEngineWorldMap, ChangedAreas) == 0x1b4);
FABLE_STATIC_ASSERT_AT(151, offsetof(CEngineWorldMap, IntermidiateStaticMapFile) == 0x1c0);
FABLE_STATIC_ASSERT_AT(152, offsetof(CEngineWorldMap, GlobalMapDataLoaded) == 0x324);
FABLE_STATIC_ASSERT_AT(153, offsetof(CEngineWorldMap, StaticMapGenerationEstimatedMaxCameraHeightAboveLandscape) == 0x328);
FABLE_STATIC_ASSERT_AT(154, offsetof(CEngineWorldMap, StaticMapGlobalOptimizationsEnabled) == 0x32c);
FABLE_STATIC_ASSERT_AT(155, offsetof(CEngineWorldMap, ForcedMapUpdateList) == 0x330);
FABLE_STATIC_ASSERT_AT(156, offsetof(CEngineWorldMap, CommonHeaderStream) == 0x33c);
FABLE_STATIC_ASSERT_AT(157, offsetof(CEngineWorldMap, CommonHeaderPosLookupTable) == 0x354);
#endif // FABLE_RETAIL_CENGINEWORLDMAP

#endif // FABLE_ENGINE_CENGINEWORLDMAP_H
