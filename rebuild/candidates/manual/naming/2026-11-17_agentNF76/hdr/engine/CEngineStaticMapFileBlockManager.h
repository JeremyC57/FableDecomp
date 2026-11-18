#ifndef FABLE_ENGINE_CENGINESTATICMAPFILEBLOCKMANAGER_H
#define FABLE_ENGINE_CENGINESTATICMAPFILEBLOCKMANAGER_H
#include "engine/CCountedPointer.h"
#include "stlport/fable_map.h"
class CBankFileAsyncEntry;
class CBankFileAsyncData;
// Layout: donor TPI supplies every field/type. Retail's three-word STLport
// tree replaces the donor four-word tree; owning CEngineMap's widths follow
// this member at +0x5c, corroborating the 20-byte retail manager.
class CEngineStaticMapFileBlockManager {
public:
    ~CEngineStaticMapFileBlockManager();
#ifdef FABLE_RETAIL_CENGINESTATICMAPFILEBLOCKMANAGER_ADDREQUEST
    // Retail opt-in (chunk F2, row 0x00BE06B0): Ego_r.pdb ?AddRequest@CEngineStaticMapFileBlockManager@@QAEXJJ_N@Z. Declaration only.
    void AddRequest(long block, long size, bool lowPriority);            // 0x00BE06B0
#endif
#ifdef FABLE_RETAIL_CENGINESTATICMAPFILEBLOCKMANAGER_SETBANKFILEENTRY
    // NF76 opt-in (row 0x00B42530 CEngineMap::OpenDevelopmentStaticMap): Ego_r.pdb
    // ?SetBankFileEntry@CEngineStaticMapFileBlockManager@@QAEXV?$CCountedPointer@VCBankFileAsyncEntry@@@@@Z (out of line at 0x00BE03A0; the
    // counted pointer by value: `sub esp,8 ; mov ecx,esp ; push ; call <copy ctor>` at 0x00B426B5). Declaration only.
    void SetBankFileEntry(CCountedPointer<CBankFileAsyncEntry> entry);   // 0x00BE03A0
#endif
private:
    struct CFileBlock {
        long RefCount;
        bool LowPriority;
        CCountedPointer<CBankFileAsyncData> LoadingData;
    };
    CCountedPointer<CBankFileAsyncEntry> AsyncBankEntry;
    std::map<long,CFileBlock> Blocks;
};
FABLE_STATIC_ASSERT_AT(26, sizeof(CEngineStaticMapFileBlockManager)==20);
#endif
