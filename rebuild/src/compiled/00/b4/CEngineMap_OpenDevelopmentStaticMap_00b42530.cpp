// CEngineMap::OpenDevelopmentStaticMap(CStaticMapBankFile& bank, long mapID) -- open a development static map from the bank: close the current map, find the map's entry by name, read its header block into a memory stream and parse the map info block; when the info's dimensions match this map's, take the map extents, and for map 1 open the bank entry asynchronously, hand it to the block manager and open the landscape (and the detail cache, when present) from their stream offsets; finally record the map ID
// Leaf: ?OpenDevelopmentStaticMap@CEngineMap@@QAEXAAVCStaticMapBankFile@@W4STATIC_MAP_OPEN_STATE@1@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B42530, `ret 8`.
// NF76 port (2026-11-17): on the shared headers: engine/CEngineMap.h (the retail member set: StaticMapName +0x1C, StaticMapOpenState +0x20,
//   CameraMapBounds +0x24, AsyncFileEntry +0x3C, FileBlockManager +0x44, MapWidth .. WorldPosY +0x5C, the nested CStaticMapInfoBlock and
//   the static ReadMapInfoBlock 0x00B3EFA0; OPENDEVELOPMENTSTATICMAP declares the self), CStaticMapBankFile.h CLASS on CBankFileAsync.h
//   (GETASYNCENTRY) / CBankFile.h (ENTRIES, ADDENTRYMULTIPLEFILES), CBankFileEntryUpdateData.h, CMemoryDataInputStream.h / CDataInputStream.h
//   retail branches, CArray.h, CCountedPointer.h, CEngineStaticMapFileBlockManager.h, CLocalDetailCacheMap.h, and my overlay opt-ins:
//   CENGINEMAP_CLOSESTATICMAP, CENGINESTATICMAPFILEBLOCKMANAGER_SETBANKFILEENTRY, CLOCALDETAILCACHEMAP_OPENSTATICMAP,
//   CBANKFILE_PEEKENTRYUPDATEDATA, CBANKFILEENTRYUPDATEDATA_READINFO, CCOUNTEDPOINTER_COPY_DECL_ONLY (with DTOR_DECL_ONLY /
//   ASSIGN_DECL_ONLY: this TU calls the CCountedPointer<CBankFileAsyncEntry> COMDATs 0x009D69A0 / 0x009C9FC0 / 0x00A5F6D0).
//   The local CCharString / CByteVector / CDataInputStream / CMemoryDataInputStream / CBankEntryRecord / CAsyncEntryPointer /
//   CStaticMapBankFile / CStaticMapInfoBlock / CEngineStaticMapFileBlockManager / CLocalDetailCacheMap / CEngineMap models are gone;
//   CEngineLandscapeMap has no shared header and stays a local declaration (Ego_r.pdb OpenStaticMap). 0x009D58D0 binds the queued
//   ?GetAsyncEntry@CBankFileAsync@@QAE?AV?$CCountedPointer@VCBankFileAsyncEntry@@@@K@Z (q10881). Names Ego_d.pdb (file, open_state,
//   smap_index, ifblock, entry, info_buffer, info_size, ifstream).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_STLP_TREE_MFIND_DECL
#define FABLE_RETAIL_CDATAINPUTSTREAM
#define FABLE_RETAIL_CMEMORYDATAINPUTSTREAM
#define FABLE_RETAIL_CCOUNTEDPOINTER_DTOR_DECL_ONLY
#define FABLE_RETAIL_CCOUNTEDPOINTER_ASSIGN_DECL_ONLY
#define FABLE_RETAIL_CCOUNTEDPOINTER_COPY_DECL_ONLY
#define FABLE_RETAIL_CENGINEMAP_OPENDEVELOPMENTSTATICMAP
#define FABLE_RETAIL_CENGINEMAP_CLOSESTATICMAP
#define FABLE_RETAIL_CENGINESTATICMAPFILEBLOCKMANAGER_SETBANKFILEENTRY
#define FABLE_RETAIL_CLOCALDETAILCACHEMAP_OPENSTATICMAP
#define FABLE_RETAIL_CBANKFILE_ENTRIES
#define FABLE_RETAIL_CBANKFILE_FILENAMEINDICES
#define FABLE_RETAIL_CBANKFILE_ADDENTRYMULTIPLEFILES
#define FABLE_RETAIL_CBANKFILE_PEEKENTRYUPDATEDATA
#define FABLE_RETAIL_CBANKFILEENTRYUPDATEDATA_RETAIL
#define FABLE_RETAIL_CBANKFILEENTRYUPDATEDATA_READINFO
#define FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS
#define FABLE_RETAIL_CBANKFILEASYNC_DATA
#define FABLE_RETAIL_CBANKFILEASYNC_GETASYNCENTRY
#define FABLE_RETAIL_CSTATICMAPBANKFILE_CLASS
#include "stlport/fable_containers.h"
#include "stlport/fable_map.h"
#include "fable_string.h"
#include "engine/CArray.h"
#include "engine/CPackedUIntArray.h"
#include "engine/CCountedPointer.h"
#include "engine/CMemoryDataInputStream.h"
#include "engine/CBankFileEntryUpdateData.h"
#include "engine/CBankFile.h"
#include "engine/CBankFileAsync.h"
#include "engine/CStaticMapBankFile.h"
#include "engine/CEngineStaticMapFileBlockManager.h"
#include "engine/CLocalDetailCacheMap.h"
#include "engine/CEngineMap.h"

class CEngineLandscapeMap {                      // no shared header
public:
    void OpenStaticMap(CEngineStaticMapFileBlockManager* block_manager, CDataInputStream& stream);   // 0x00BDD0E0 (Ego_r.pdb)
};

void CEngineMap::OpenDevelopmentStaticMap(CStaticMapBankFile& file, STATIC_MAP_OPEN_STATE open_state)
{
    if (StaticMapOpenState == open_state)
        return;
    CloseStaticMap();
    unsigned long smap_index = file.FindIndexBySymbol(StaticMapName);
    if (!smap_index)
        return;
    CArray<char> info_buffer;
    CMemoryDataInputStream ifstream;
    const CBankFileEntryUpdateData* entry = file.PeekEntryUpdateData(smap_index);
    unsigned long info_size = entry->GetInfoSize();
    info_buffer.resize(info_size);
    entry->ReadInfo(0, info_buffer.GetAsCArray(), info_size);
    ifstream.OpenMemorySource(info_buffer.GetAsCArray(), info_size);
    CStaticMapInfoBlock ifblock;
    ReadMapInfoBlock(ifblock, ifstream);
    if (ifblock.MapWidth == MapWidth && ifblock.MapHeight == MapHeight && ifblock.WorldX == WorldPosX && ifblock.WorldY == WorldPosY) {
        CameraMapBounds = ifblock.CameraMapBounds;
        if (open_state == STATIC_MAP_OPEN_STATE_OPEN_FOR_USE) {
            AsyncFileEntry = file.GetAsyncEntry(smap_index);
            FileBlockManager.SetBankFileEntry(AsyncFileEntry);
            ifstream.SetPosition(ifblock.LandscapeMapPtr);
            LandscapeMap->OpenStaticMap(&FileBlockManager, ifstream);
            if (LocalDetailCacheMap) {
                ifstream.SetPosition(ifblock.LocalDetailMapPtr);
                LocalDetailCacheMap->OpenStaticMap(&FileBlockManager, ifstream);
            }
        }
        ifstream.SetPosition(ifblock.HeaderEndPtr);
        StaticMapOpenState = open_state;
    }
}
