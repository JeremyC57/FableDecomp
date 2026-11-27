#ifndef FABLE_ENGINE_CENGINEMAP_H
#define FABLE_ENGINE_CENGINEMAP_H
#include "engine/C3DBoundingBox.h"
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CEngineStaticMapFileBlockManager.h"
#include "engine/CArray.h"
#include "engine/C2DCoordI.h"
#ifdef FABLE_RETAIL_CENGINEMAP_PEEK
#include "engine/GFFloatToLongNear.h"
#endif
class CEngineLandscapeMap;
class CLocalDetailCacheMap;
class CIDrawMap;
class CEngineStaticMapEdgeHeights;
class CDataInputStream;
// Layout: complete original member set from Ego_r TPI 0x5a932. Retail
// uses 12-byte vectors/maps rather than the donor 16-byte containers.
// Native OpenRetailStaticMap confirms +0x1c name,+0x20 state,+0x24 bounds,
// +0x3c counted entry and +0x44 block manager. This row is static.
class CEngineMap {
public:
    enum STATIC_MAP_OPEN_STATE {
        STATIC_MAP_OPEN_STATE_CLOSED=0,
        STATIC_MAP_OPEN_STATE_OPEN_FOR_USE=1,
        STATIC_MAP_OPEN_STATE_OPEN_FOR_UPDATE=2
    };
    CEngineMap(const CIDrawMap*,long,long,long,long,long,CCharString,bool);
    ~CEngineMap();
#ifdef FABLE_RETAIL_CENGINEMAP_OPENRETAILSTATICMAP_PASS26
    // Original Ego_d/FableWin method, actual caller donor02d62360; declaration only.
    void OpenRetailStaticMap(class CStaticMapBankFile&, class CDataInputStream&,
        std::map<CCharString,long>&);
#endif
#ifdef FABLE_RETAIL_CENGINEMAP_OPENDEVELOPMENTSTATICMAP
    // NF65 overlay opt-in (row 0x00B42750 CEngineWorldMap::OpenStaticMaps): Ego_r.pdb 0x009E31E0
    // ?OpenDevelopmentStaticMap@CEngineMap@@QAEXAAVCStaticMapBankFile@@W4STATIC_MAP_OPEN_STATE@1@@Z (public), out of line at 0x00B42530 (the
    // landed self row). Declaration only.
    void OpenDevelopmentStaticMap(class CStaticMapBankFile& file, STATIC_MAP_OPEN_STATE state);   // 0x00B42530
#endif
#ifdef FABLE_RETAIL_CENGINEMAP_CLOSESTATICMAP
    // NF76 opt-in (row 0x00B42530 CEngineMap::OpenDevelopmentStaticMap): Ego_r.pdb 0x009E3120 ?CloseStaticMap@CEngineMap@@QAEXXZ (public,
    // out of line at 0x00B3EF40: `mov ecx,esi ; call` at 0x00B42549). Declaration only.
    void CloseStaticMap();                                               // 0x00B3EF40
#endif
#ifdef FABLE_RETAIL_CENGINEMAP_ONGAMEMAPLOADED
    // NF56 opt-in (row 0x00B401E0 CEngineWorldMap::OnGameMapLoaded, which expands both): Ego_r.pdb / FableWin.pdb
    // ?SetGameMap@CEngineMap@@QAEXPBVCIDrawMap@@@Z (the GameMap +0x08 store) and ?OnGameMapLoaded@CEngineMap@@QAEXJ@Z (Ego_r.pdb 0x009DD5D0 /
    // FableWin.pdb 0x02D5C390; retail has no out-of-line copy: 0x00B401E0 expands it). The row supplies the inline body. Declarations
    // (+ the one-store setter); no layout change.
    void SetGameMap(const CIDrawMap* map) { GameMap = map; }
    void OnGameMapLoaded(long index);
#endif
#ifdef FABLE_RETAIL_CENGINEMAP_ONGAMEMAPUNLOADED
    // NF160 opt-in (row 0x00B40230 CEngineWorldMap::OnGameMapUnloaded, which expands it): Ego_r.pdb 0x009DD2B0 / FableWin.pdb 0x02D5C630
    // ?OnGameMapUnloaded@CEngineMap@@QAEXXZ (no retail out-of-line copy: 0x00B40230 tests LandscapeMap +0x00, calls its
    // OnGameMapUnloaded 0x00BDBFF0 and clears GameMap +0x08). The row supplies the inline body. Declaration only.
    void OnGameMapUnloaded();
#endif
#ifdef FABLE_RETAIL_CENGINEMAP_SETMAPVISIBILITY
    // NF160 opt-in (rows 0x00B42390 / 0x00B423B0 CEngineWorldMap::SetMapVisibility / SetMapVisibilityAllMaps): Ego_r.pdb 0x009E2440 /
    // FableWin.pdb 0x02D5B370 ?SetMapVisibility@CEngineMap@@QAEXAAV?$list@PAVCEngineMap@@V?$allocator@PAVCEngineMap@@@std@@@std@@_N@Z, out of
    // line at 0x00B41DB0 (both callers pass the world map's VisibleMapList +0x2C). Declaration only; needs stlport/fable_containers.h first.
    void SetMapVisibility(std::list<CEngineMap*>& visible_maps, bool visible);   // 0x00B41DB0
#endif
#ifdef FABLE_RETAIL_CENGINEMAP
    // Retail opt-in (chunk 723): the inline LandscapeMap accessors (Ego_r.pdb / FableWin.pdb ?GetLandscapeMap@CEngineMap@@QBEPAVCEngineLandscapeMap@@XZ,
    // ?SetLandscapeMap@CEngineMap@@QAEXPAVCEngineLandscapeMap@@@Z); ~CEngineLandscapeRenderer 0x00B67920 expands both on +0x00.
    CEngineLandscapeMap* GetLandscapeMap() const { return LandscapeMap; }
    void SetLandscapeMap(CEngineLandscapeMap* map) { LandscapeMap = map; }
#endif
#ifdef FABLE_RETAIL_CENGINEMAP_PEEK
    // Retail opt-in (chunk 1013b, rows 0x00B43720 PeekThemeId / 0x00B43870 PeekThemeBlend / 0x00B439C0 PeekLandscapeHeight):
    // the FableWin.pdb inline accessors ?PeekWidth / ?PeekHeight / ?PeekWorldX / ?PeekWorldY @CEngineMap@@QBEJXZ (the unsigned
    // short members widened to long: `movzx`), ?PeekGameMap@CEngineMap@@QBEPBVCIDrawMap@@XZ and ?MapHasDataAtPoint@CEngineMap@@
    // QBE_NJJ@Z (out of line in FableWin only; Ego_r.pdb has no copy, so it is always expanded in release), the three Peek
    // members (Ego_r + FableWin ?PeekThemeId@CEngineMap@@QBEJJJJ@Z, Ego_r ?PeekThemeBlend@CEngineMap@@QBEEJJJ@Z, Ego_r + FableWin
    // ?PeekLandscapeHeight@CEngineMap@@QBEMJJ@Z) and the private inline ?QuantiseLandscapeHeight@CEngineMap@@ABEMM@Z (FableWin).
    long PeekWidth() const { return MapWidth; }
    long PeekHeight() const { return MapHeight; }
#ifdef FABLE_RETAIL_LANDSCAPE_BUILD_DEFAULT_SECTION_PASS28
    // FableWin PeekWorldPosition and Ego_r MapWorldPosition, loaded at +0x0c in 0x00BE6880.
    const C3DVector& PeekWorldPosition() const { return MapWorldPosition; }
#endif
    long PeekWorldX() const { return WorldPosX; }
    long PeekWorldY() const { return WorldPosY; }
    const CIDrawMap* PeekGameMap() const { return GameMap; }
    bool MapHasDataAtPoint(long x, long y) const
    {
#ifdef FABLE_RETAIL_CENGINEMAP_MAPHASDATA_IFRETURN
        // Retail opt-in (chunk 2037): the out-of-range test returns false and the last test is returned as a value, which keeps
        // retail's `test ; setne al` (with the out-of-range paths on the caller's shared `xor al,al`) where the result is returned
        // or tested: CEngineWorldMap::MapHasDataAtPoint's COMDAT 0x00B56880 (0x00B56915..0x00B5691D) grades MATCH with this body,
        // RELOCATION-masked DIFFER(533v540) with the && chain (the chain's 0 / 1 are materialised as `xor eax,eax` / `mov eax,1`
        // and the four returns merged into one), and ClipBoxToLandscape 0x00B52F7F..0x00B52F84 / 0x00B530D0 tests the setne value.
        if (x < PeekWidth() && y < PeekHeight() && x >= 0 && y >= 0)
            return GameMap != 0;
        return false;
#else
        return x < PeekWidth() && y < PeekHeight() && x >= 0 && y >= 0 && GameMap != 0;
#endif
    }
    long PeekThemeId(long x, long y, long layer) const;
    unsigned char PeekThemeBlend(long x, long y, long layer) const;
    float PeekLandscapeHeight(long x, long y) const;
#ifdef FABLE_RETAIL_CENGINEMAP_PEEKNORMAL
    // Nested opt-in (chunk 1013b, row 0x00B3FC40): Ego_r.pdb / FableWin.pdb ?PeekMapNormal@CEngineMap@@QBE?AVC3DVector@@JJ@Z (retail
    // 0x00B3F5E0) and ?PeekInterpolatedMapNormal@CEngineMap@@QBE?AVC3DVector@@MM@Z (the row).
    C3DVector PeekMapNormal(long x, long y) const;
    C3DVector PeekInterpolatedMapNormal(float x, float y) const;
#endif
#ifdef FABLE_RETAIL_CENGINEMAP_PEEKWATER
    // Nested opt-in (chunk 1013b, row 0x00B41770 PeekInterpolatedWaterHeight): Ego_r.pdb / FableWin.pdb ?PeekHasWater@CEngineMap@@QBE_NJJ@Z
    // (retail 0x00B412A0), ?PeekHasWaterFast@..@@QBE_NJJ@Z (0x00B40260), ?PeekWaterDepth@..@@QBEMJJ@Z (0x00B40BD0) and the row itself.
    bool PeekHasWater(long x, long y) const;
#ifdef FABLE_RETAIL_MAP_WATER_TYPE_PASS29
    EWaterType PeekWaterType(long mapx,long mapy) const;
#endif
    bool PeekHasWaterFast(long x, long y) const;
    float PeekWaterDepth(long x, long y) const;
    float PeekInterpolatedWaterHeight(long x, long y, long radius) const;
#endif
#ifdef FABLE_RETAIL_CENGINEMAP_PEEKWATERHEIGHT
    // hand3c opt-in (row 0x00B41A80): Ego_r.pdb / FableWin.pdb ?PeekWaterHeight@CEngineMap@@QBEMJJ@Z (retail 0x00B41A80, `ret 8`).
    float PeekWaterHeight(long x, long y) const;
#endif
private:
    // 0x00B43AF2: `fmul [128.0f] ; fstp/fld ; fistp ; fild ; fmul [1/128.0f]` -- heights snap to 1/128 of a unit.
    float QuantiseLandscapeHeight(float height) const
    {
        return (float)GFFloatToLongNear(height * 128.0f) * (1.0f / 128.0f);
    }
#endif
private:
    // Layout: original nested struct TPI 0x5a920/complete record. Every
    // destination offset is directly checked against this row's stores.
    struct CStaticMapInfoBlock {
        long VersionID,BankFileIndex,EdgeHeightFilePtr,EdgeHeightFileSize;
        long LandscapeMapPtr,LocalDetailMapPtr,ChecksumBlockFilePtr,ChecksumBlockFileSize;
        long MapWidth,MapHeight,WorldX,WorldY,Quality,ShorePointArraySize,ShorePointArrayStart;
        unsigned long LevelChecksum;
        C3DBoundingBox CameraMapBounds;
        long HeaderEndPtr;
    };
    static void __fastcall ReadMapInfoBlock(CStaticMapInfoBlock&,CDataInputStream&);
    CEngineLandscapeMap* LandscapeMap;
    CLocalDetailCacheMap* LocalDetailCacheMap;
    const CIDrawMap* GameMap;
    C3DVector MapWorldPosition;
    long MapUID;
    CCharString StaticMapName;
    STATIC_MAP_OPEN_STATE StaticMapOpenState;
    C3DBoundingBox CameraMapBounds;
    CCountedPointer<CBankFileAsyncEntry> AsyncFileEntry;
    CEngineStaticMapFileBlockManager FileBlockManager;
    std::list<CEngineMap*>::iterator VisibleMapListIterator;
    unsigned short MapWidth,MapHeight,WorldPosX,WorldPosY;
    bool IsInVisibleMapList;
    CEngineStaticMapEdgeHeights* EdgeHeights;
    CArray<C2DCoordI> ShorePoints;
    bool ShorePointsLoadedForThisMap,MapChecksumPassed,DefChecksumPassed,NeedsRebuild;
    long EdgeHeightFilePtr,EdgeHeightFileSize,ShorePointFilePtr,ShorePointFileSize;
    CArray<long> StaticMapGenerationVisibilityInfoObservingMaps;
};
#ifdef FABLE_RETAIL_CENGINEMAP_PEEK_BODIES
// Retail opt-in (chunk 2021, rows 0x00B40BD0 PeekWaterDepth / 0x00B3F5E0 PeekMapNormal): the bodies of the three header inline
// Peek members (Ego_r.pdb / FableWin.pdb ?PeekThemeId@CEngineMap@@QBEJJJJ@Z, ?PeekThemeBlend@CEngineMap@@QBEEJJJ@Z,
// ?PeekLandscapeHeight@CEngineMap@@QBEMJJ@Z; retail keeps their COMDAT copies in this TU's inline run 0x00B43720 / 0x00B43870 /
// 0x00B439C0, after the TU's ordinary members). A cell outside this map (or a map without its game map) is looked up in the
// neighbouring map that holds it (the self call: a loop in the out-of-line copy, a call to that copy where the body is expanded),
// else clamped to this map's edge.
// - Plain inline, not a forced inline: other TUs call the COMDAT copy at plain-inline sites (CWaterPatchMesh::GetWaterType
//   0x00BEAB40 calls PeekThemeId 0x00B43720 three times inside its loops, 0x00BEAB8A..0x00BEABAE), which a forced inline would
//   have expanded. The CEngineMap TU expands them at every site (inline score 403..575 for PeekThemeId / PeekThemeBlend, 443 for
//   PeekLandscapeHeight): its rows carry a `/d2inlT<N>` flag (EXTRA_RULES 2026-11-08 inline-budget device).
// - g_pEngineWorldMap is read at each use, not copied into a local: a local copy changes the first expanded copy's allocation
//   (row 0x00B40BD0: the MapWidth CSE is rematerialised instead of spilled to [esp+38h], 0x00B40BF3, and the x / y copies take
//   other registers); with the global read in place the copy uses the global's own allocation candidate.
// Needs FABLE_RETAIL_CENGINEMAP_PEEK, FABLE_RETAIL_CIDRAWMAP(_DRAW) and FABLE_RETAIL_CENGINEWORLDMAP(_GETMAPXY).
#include "engine/CEngineWorldMap.h"
#include "engine/CIDrawMap.h"
#ifdef FABLE_RETAIL_CENGINEMAP_GPENGINEWORLDMAP
// Opt-in (NF44 round 6, rows 0x00B3F5E0 / 0x00B40BD0 / 0x00B52DD0): the global at 0x01436E8C is Ego_r.pdb / FableWin.pdb
// ?GPEngineWorldMap@NEngineInternal@@3PAVCEngineWorldMap@@A; the bodies below read it through FABLE_CENGINEMAP_WORLDMAP.
namespace NEngineInternal { extern CEngineWorldMap* GPEngineWorldMap; }   // 0x01436E8C
#define FABLE_CENGINEMAP_WORLDMAP NEngineInternal::GPEngineWorldMap
#else
extern CEngineWorldMap* g_pEngineWorldMap;   // 0x01436E8C (landed spelling, CEngineWorldMap::SetWorld 0x00B42ED0)
#define FABLE_CENGINEMAP_WORLDMAP g_pEngineWorldMap
#endif

#ifdef FABLE_RETAIL_CENGINEMAP_PEEKTHEMEID_ORIGINAL_PASS27
// FableWin 0x02D5CE80 and Pass26 retail 0x00B43720: GetMap receives world coordinates
// and handles the map origin itself. Shared by callers that expand this inline member.
inline long CEngineMap::PeekThemeId(long xp, long yp, long level) const
{
    if (!MapHasDataAtPoint(xp,yp)) {
        CEngineMap* map = NEngineInternal::GPEngineWorldMap->GetMap(
            PeekWorldX() + xp,
            PeekWorldY() + yp);
        if (map && map->MapHasDataAtPoint(PeekWorldX() + xp - map->PeekWorldX(), PeekWorldY() + yp - map->PeekWorldY()))
            return map->PeekThemeId(PeekWorldX() + xp - map->PeekWorldX(), PeekWorldY() + yp - map->PeekWorldY(), level);
        if (xp < 0)
            xp = 0;
        else if (xp > PeekWidth() - 1)
            xp = PeekWidth() - 1;
        if (yp < 0)
            yp = 0;
        else if (yp > PeekHeight() - 1)
            yp = PeekHeight() - 1;
    }
    if (GameMap)
        return GameMap->DrawGetEngineThemeAt(xp, yp, level);
    return 0;
}
#else
inline long CEngineMap::PeekThemeId(long x, long y, long layer) const
{
    if (x >= PeekWidth() || y >= PeekHeight() || x < 0 || y < 0 || !GameMap) {
        long worldX = PeekWorldX() - FABLE_CENGINEMAP_WORLDMAP->MapOffsetX + x;
        long worldY = PeekWorldY() - FABLE_CENGINEMAP_WORLDMAP->MapOffsetY + y;
        const CEngineMap* map = FABLE_CENGINEMAP_WORLDMAP->GetMap(worldX, worldY);
#ifdef FABLE_RETAIL_CENGINEMAP_MAPHASDATA_IFRETURN
        // The neighbour test written out as in PeekLandscapeHeight below (out-of-line copy: same object as the call form).
        if (map) {
            long mapY = PeekWorldY() - map->PeekWorldY() + y;
            long mapX = PeekWorldX() - map->PeekWorldX() + x;
            if (mapX < map->PeekWidth() && mapY < map->PeekHeight() && mapX >= 0 && mapY >= 0 && map->GameMap)
                return map->PeekThemeId(PeekWorldX() - map->PeekWorldX() + x, PeekWorldY() - map->PeekWorldY() + y, layer);
        }
#else
        if (map && map->MapHasDataAtPoint(PeekWorldX() - map->PeekWorldX() + x, PeekWorldY() - map->PeekWorldY() + y))
            return map->PeekThemeId(PeekWorldX() - map->PeekWorldX() + x, PeekWorldY() - map->PeekWorldY() + y, layer);
#endif
        if (x < 0)
            x = 0;
        else if (x > PeekWidth() - 1)
            x = PeekWidth() - 1;
        if (y < 0)
            y = 0;
        else if (y > PeekHeight() - 1)
            y = PeekHeight() - 1;
    }
    if (GameMap)
        return GameMap->DrawGetEngineThemeAt(x, y, layer);
    return 0;
}

#endif

inline unsigned char CEngineMap::PeekThemeBlend(long x, long y, long layer) const
{
    if (x >= PeekWidth() || y >= PeekHeight() || x < 0 || y < 0 || !GameMap) {
        long worldX = PeekWorldX() - FABLE_CENGINEMAP_WORLDMAP->MapOffsetX + x;
        long worldY = PeekWorldY() - FABLE_CENGINEMAP_WORLDMAP->MapOffsetY + y;
        const CEngineMap* map = FABLE_CENGINEMAP_WORLDMAP->GetMap(worldX, worldY);
#ifdef FABLE_RETAIL_CENGINEMAP_MAPHASDATA_IFRETURN
        // The neighbour test written out as in PeekLandscapeHeight below (out-of-line copy: same object as the call form).
        if (map) {
            long mapY = PeekWorldY() - map->PeekWorldY() + y;
            long mapX = PeekWorldX() - map->PeekWorldX() + x;
            if (mapX < map->PeekWidth() && mapY < map->PeekHeight() && mapX >= 0 && mapY >= 0 && map->GameMap)
                return map->PeekThemeBlend(PeekWorldX() - map->PeekWorldX() + x, PeekWorldY() - map->PeekWorldY() + y, layer);
        }
#else
        if (map && map->MapHasDataAtPoint(PeekWorldX() - map->PeekWorldX() + x, PeekWorldY() - map->PeekWorldY() + y))
            return map->PeekThemeBlend(PeekWorldX() - map->PeekWorldX() + x, PeekWorldY() - map->PeekWorldY() + y, layer);
#endif
        if (x < 0)
            x = 0;
        else if (x > PeekWidth() - 1)
            x = PeekWidth() - 1;
        if (y < 0)
            y = 0;
        else if (y > PeekHeight() - 1)
            y = PeekHeight() - 1;
    }
    if (GameMap)
        return GameMap->DrawGetEngineBlendAt(x, y, layer);
    return 0;
}

inline float CEngineMap::PeekLandscapeHeight(long x, long y) const
{
    if (x >= PeekWidth() || y >= PeekHeight() || x < 0 || y < 0 || !GameMap) {
        long worldX = PeekWorldX() - FABLE_CENGINEMAP_WORLDMAP->MapOffsetX + x;
        long worldY = PeekWorldY() - FABLE_CENGINEMAP_WORLDMAP->MapOffsetY + y;
        const CEngineMap* map = FABLE_CENGINEMAP_WORLDMAP->GetMap(worldX, worldY);
#ifdef FABLE_RETAIL_CENGINEMAP_MAPHASDATA_IFRETURN
        // With the if-return MapHasDataAtPoint body a call here would leave `setne al ; test al,al` behind; retail's neighbour test is
        // pure branches with the neighbour's y cell formed first (COMDAT 0x00B566B0: 0x00B567AF..0x00B567E0, RELOCATION_MATCH): the
        // test is written out on named cells (mapY first), while the recursive call keeps its own argument expressions (call-argument
        // temporaries that c2 CSEs with the cells). Passing the named cells to the call instead gives them the call's references too:
        // ClipBoxToLandscape 0x00B52DD0's last expanded copy then keeps them in eax / edx (corner 4, 0x00B53E04..0x00B53F13) and
        // the out-of-line 0x00B439C0 scores 84 / 132; with the expressions retail's spills come back (0x00B52DD0 RELOCATION_MATCH)
        // and 0x00B439C0 / 0x00B43720 / 0x00B43870 compile to the same object as the c2021 call form below.
        if (map) {
            long mapY = PeekWorldY() - map->PeekWorldY() + y;
            long mapX = PeekWorldX() - map->PeekWorldX() + x;
            if (mapX < map->PeekWidth() && mapY < map->PeekHeight() && mapX >= 0 && mapY >= 0 && map->GameMap)
                return map->PeekLandscapeHeight(PeekWorldX() - map->PeekWorldX() + x, PeekWorldY() - map->PeekWorldY() + y);
        }
#else
        if (map && map->MapHasDataAtPoint(PeekWorldX() - map->PeekWorldX() + x, PeekWorldY() - map->PeekWorldY() + y))
            return map->PeekLandscapeHeight(PeekWorldX() - map->PeekWorldX() + x, PeekWorldY() - map->PeekWorldY() + y);
#endif
        if (x < 0)
            x = 0;
        else if (x > PeekWidth() - 1)
            x = PeekWidth() - 1;
        if (y < 0)
            y = 0;
        else if (y > PeekHeight() - 1)
            y = PeekHeight() - 1;
    }
    if (GameMap)
        return QuantiseLandscapeHeight(GameMap->DrawBlockGetSizeZAt(x, y, 1.0f));
    return 0.0f;
}
#ifdef FABLE_RETAIL_CENGINEWORLDMAP_PEEKLANDSCAPE
// Retail opt-in (chunk 2037, row 0x00B52DD0 CEngineWeatherRenderer::ClipBoxToLandscape, which expands both four times): the bodies of
// FableWin.pdb ?MapHasDataAtPoint@CEngineWorldMap@@QBE_NJJ@Z and ?PeekLandscapeHeight@CEngineWorldMap@@QBEMJJ@Z. A world cell on a
// map edge belongs to up to four maps, so MapHasDataAtPoint asks the grid for the map owning (x, y), then (x - 1, y), (x, y - 1) and
// (x - 1, y - 1) (0x00B52E73..0x00B530D0); PeekLandscapeHeight asks only for (x, y) and reads 0 off the maps (0x00B530D8..0x00B53242).
FABLE_FORCEINLINE bool CEngineWorldMap::MapHasDataAtPoint(long x, long y) const
{
    long mapX = x - MapOffsetX;
    long mapY = y - MapOffsetY;
    const CEngineMap* map = GetMap(mapX, mapY);
    if (map)
        return map->MapHasDataAtPoint(x - map->PeekWorldX(), y - map->PeekWorldY());
    map = GetMap(mapX - 1, mapY);
    if (map)
        return map->MapHasDataAtPoint(x - map->PeekWorldX(), y - map->PeekWorldY());
    map = GetMap(mapX, mapY - 1);
    if (map)
        return map->MapHasDataAtPoint(x - map->PeekWorldX(), y - map->PeekWorldY());
    map = GetMap(mapX - 1, mapY - 1);
    if (map)
        return map->MapHasDataAtPoint(x - map->PeekWorldX(), y - map->PeekWorldY());
    return false;
}

FABLE_FORCEINLINE float CEngineWorldMap::PeekLandscapeHeight(long x, long y) const
{
    long mapX = x - MapOffsetX;
    long mapY = y - MapOffsetY;
    const CEngineMap* map = GetMap(mapX, mapY);
    if (map)
        return map->PeekLandscapeHeight(x - map->PeekWorldX(), y - map->PeekWorldY());
    return 0.0f;
}
#endif
#endif
#endif
