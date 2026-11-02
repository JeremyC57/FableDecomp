#ifndef FABLE_ENGINE_CENGINEMAP_H
#define FABLE_ENGINE_CENGINEMAP_H
#include "engine/C3DBoundingBox.h"
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CEngineStaticMapFileBlockManager.h"
#include "engine/CArray.h"
#include "engine/C2DCoordI.h"
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
