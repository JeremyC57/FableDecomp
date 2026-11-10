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
    long PeekWorldX() const { return WorldPosX; }
    long PeekWorldY() const { return WorldPosY; }
    const CIDrawMap* PeekGameMap() const { return GameMap; }
    bool MapHasDataAtPoint(long x, long y) const
    {
        return x < PeekWidth() && y < PeekHeight() && x >= 0 && y >= 0 && GameMap != 0;
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
    bool PeekHasWaterFast(long x, long y) const;
    float PeekWaterDepth(long x, long y) const;
    float PeekInterpolatedWaterHeight(long x, long y, long radius) const;
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
#endif
