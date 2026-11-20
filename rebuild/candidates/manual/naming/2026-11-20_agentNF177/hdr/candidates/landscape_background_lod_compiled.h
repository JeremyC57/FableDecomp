#ifndef FABLETLC_LANDSCAPE_BACKGROUND_LOD_COMPILED_H
#define FABLETLC_LANDSCAPE_BACKGROUND_LOD_COMPILED_H

#include <stddef.h>
#include "rebuild_abi.h"

enum LS_LOADING_STATUS
{
    LS_LOADING_READY = 1,
    LS_LOADING_PENDING = 2
};

class CLandscapeGPUUpdateQueue {};

class CEngineMap
{
public:
    fable_u8 pad_00[8];
    void* EdgeHeightFile;
    fable_u8 pad_0c[0x50];
    unsigned short BackgroundWidth;
    unsigned short BackgroundHeight;
};

class CEngineStaticMapFileBlockManager
{
public:
    void AddRequest(long blockId, unsigned long blockSize, bool immediate);
#ifdef FABLE_RETAIL_CENGINESTATICMAPFILEBLOCKMANAGER_ISBLOCKLOADED_CONST
    // NF177 opt-in (PRIORITY q12088, row 0x00BDC180): Ego_r.pdb 0x00A41DE0 / FableWin.pdb 0x02DF7220
    // ?IsBlockLoaded@CEngineStaticMapFileBlockManager@@QBE_NJ@Z (const), the spelling of the self row 0x00BE0480 and its other callers.
    bool IsBlockLoaded(long blockId) const;                              // 0x00BE0480
#else
    bool IsBlockLoaded(long blockId);
#endif
#ifdef FABLE_RETAIL_CENGINESTATICMAPFILEBLOCKMANAGER_GETBLOCKDATA
    // NF131 opt-in (2026-11-20, q6182, row 0x00BDC180): Ego_r.pdb 0x00A41E20 + FableWin.pdb 0x02DF74B0
    // ?GetBlockData@CEngineStaticMapFileBlockManager@@QBEPBEJ@Z (one argument, const; retail 0x00BE04C0 `ret 4`), the spelling the other landed
    // callers bind. The two-argument form below only reproduced the bytes because the block size it pushed is the stream ctor's second argument.
    const unsigned char* GetBlockData(long blockId) const;               // 0x00BE04C0
#else
    unsigned char* GetBlockData(long blockId, unsigned long blockSize);
#endif
    void BankFileList_RemoveFinished();
};

class CMemoryDataInputStream
{
public:
    fable_u8 storage[0x18];
#ifdef FABLE_RETAIL_CMEMORYDATAINPUTSTREAM_LOD_CTOR
    // NF131 opt-in (2026-11-20, q6182, row 0x00BDC180): Ego_r.pdb 0x00AFCA20 + FableWin.pdb 0x030849C0
    // ??0CMemoryDataInputStream@@QAE@PBXK@Z (retail 0x00994930, `ret 8`): data and size.
    CMemoryDataInputStream(const void* data, unsigned long size);        // 0x00994930
    fable_u8 storage_tail[0x4];                                          // Ego_r.pdb sizeof CMemoryDataInputStream 0x1C (retail frame `sub esp,1Ch`)
#else
    explicit CMemoryDataInputStream(unsigned char* data);
#endif
    ~CMemoryDataInputStream();
};

#ifdef FABLE_RETAIL_CMEMORYDATAINPUTSTREAM_LOD_CTOR
FABLE_STATIC_ASSERT_AT(57, sizeof(CMemoryDataInputStream) == 0x1C);
#else
FABLE_STATIC_ASSERT_AT(42, sizeof(CMemoryDataInputStream) == 0x18);
#endif

class CEngineLandscapeMap;
void* __fastcall AllocateLandscapeBackgroundTreeNode(unsigned long bytes, CEngineLandscapeMap* map);

class CLandscapeBackgroundTreeNode
{
public:
    fable_u8 storage[0xE8];
    CLandscapeBackgroundTreeNode(CEngineLandscapeMap* map, void* parent, long mapIndex);
    void BuildDefaultSection(unsigned long x, unsigned long y, unsigned long width, unsigned long height);
    void LoadHeader(CMemoryDataInputStream& stream);
    LS_LOADING_STATUS UpdateStaticLOD(
        double distanceToCamera,
        bool forceUpdate,
        CEngineStaticMapFileBlockManager* manager);
};

class CEngineLandscapeMap
{
public:
    fable_u8 pad_00[4];
    CLandscapeBackgroundTreeNode* BackgroundTree;
    fable_u8 pad_08[0x0C];
    CEngineMap* OwnerMap;
    void* BackgroundLoadEntry;
    fable_u8 pad_1c[4];
    CEngineStaticMapFileBlockManager* StaticMapFileBlockManager;
    fable_u8 pad_24[0x0C];
    long BackgroundHeaderBlockId;
    unsigned long BackgroundHeaderBlockSize;
    fable_u8 pad_38[0x20];
    long LandscapeMapIndex;
    bool LandscapeUpdateActive;
    bool ForegroundLODUpdateActive;
    bool BackgroundHeaderRequestIssued;

    LS_LOADING_STATUS UpdateBackgroundLOD(
        CLandscapeGPUUpdateQueue& gpuUpdateQueue,
        double distanceToCamera,
        bool forceUpdate);
    void ReleaseFileBlockLoad();
};

FABLE_STATIC_ASSERT_AT(86, offsetof(CEngineLandscapeMap, BackgroundTree) == 0x04);
FABLE_STATIC_ASSERT_AT(87, offsetof(CEngineLandscapeMap, OwnerMap) == 0x14);
FABLE_STATIC_ASSERT_AT(88, offsetof(CEngineLandscapeMap, BackgroundLoadEntry) == 0x18);
FABLE_STATIC_ASSERT_AT(89, offsetof(CEngineLandscapeMap, StaticMapFileBlockManager) == 0x20);
FABLE_STATIC_ASSERT_AT(90, offsetof(CEngineLandscapeMap, BackgroundHeaderBlockId) == 0x30);
FABLE_STATIC_ASSERT_AT(91, offsetof(CEngineLandscapeMap, LandscapeMapIndex) == 0x58);
FABLE_STATIC_ASSERT_AT(92, offsetof(CEngineLandscapeMap, LandscapeUpdateActive) == 0x5C);
FABLE_STATIC_ASSERT_AT(93, offsetof(CEngineLandscapeMap, BackgroundHeaderRequestIssued) == 0x5E);

#endif
