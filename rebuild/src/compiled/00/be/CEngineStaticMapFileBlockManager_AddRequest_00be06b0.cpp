// CEngineStaticMapFileBlockManager::AddRequest(long block, long size, bool lowPriority) -- request a static-map file block. A
// block not yet in Blocks starts an asynchronous auto-allocating read (below-normal priority for a low-priority request, else
// the high loading priority) and is inserted with RefCount 1; a known block gets its RefCount bumped and, when a normal request
// meets a block still marked low priority, its read raised to the high loading priority.
// Leaf: ?AddRequest@CEngineStaticMapFileBlockManager@@QAEXJJ_N@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BE06B0, length 322 bytes, `ret 0Ch`.
// Callees: STLport _Rb_tree<long, pair<const long,CFileBlock> >::find 0x00BE08E0 and insert_unique(const V&) 0x00BE0BF0 (both out
// of line), CBankFileAsyncEntry::BeginReadDataAutoAlloc 0x009D6100, CBankFileAsyncData::ChangeLoadingPriority 0x009D5630,
// operator delete 0x00BFE9BC; g_HighLoadingPriority 0x0139A728.
// Layout: engine/CEngineStaticMapFileBlockManager.h (AsyncBankEntry +0, Blocks +8; CFileBlock RefCount / LowPriority / LoadingData).
// Shape: `data` is default-constructed and then assigned the returned pointer (CCountedPointer::operator= -> ShareData: Data and
// Info are taken only when the returned Info is not null, then the temporary dies); the CFileBlock is an aggregate initialised
// from { 1, lowPriority, data } (a copy construction: `test esi,esi ; je ; inc [esi]`, where a member-wise assignment expands
// ShareData again), and the inserted pair copies it once more. Three releases follow the insert: the pair's, the block's and
// data's. The STLport insert_unique / _M_insert bodies must be visible (FABLE_RETAIL_STLP_MAP + _ORIGINAL_INSERT_BODY) for the
// retail allocation (`this` in edi, lowPriority kept in bl); declaration-only bodies give 333v322.
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CMOVABLERESOURCE
#define FABLE_RETAIL_CBANKFILEASYNCDATA_CHANGELOADINGPRIORITY
#define FABLE_RETAIL_CENGINESTATICMAPFILEBLOCKMANAGER_ADDREQUEST
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_MAP_ORIGINAL_INSERT_BODY
#include "engine/EThreadedFilePriority.h"
#include "engine/CBankFileAsyncData.h"
#include "engine/CEngineStaticMapFileBlockManager.h"
class CBankFileAsyncEntry {                        // no shared header: the one member this row calls
public:
    CCountedPointer<CBankFileAsyncData> BeginReadDataAutoAlloc(unsigned long offset, unsigned long size, EThreadedFilePriority priority);   // 0x009D6100
};
extern EThreadedFilePriority g_HighLoadingPriority;                       // 0x0139A728
void CEngineStaticMapFileBlockManager::AddRequest(long block, long size, bool lowPriority)
{
    std::map<long,CFileBlock>::iterator it = Blocks.find(block);
    if (it == Blocks.end()) {
        CCountedPointer<CBankFileAsyncData> data;
        data = AsyncBankEntry->BeginReadDataAutoAlloc(block, size, lowPriority ? THREADED_FILE_PRIORITY_BELOW_NORMAL : g_HighLoadingPriority);
        CFileBlock fileBlock = { 1, lowPriority, data };
        Blocks.insert(std::pair<const long,CFileBlock>(block, fileBlock));
    } else {
        if (!lowPriority && it->second.LowPriority)
            it->second.LoadingData->ChangeLoadingPriority(g_HighLoadingPriority);
        ++it->second.RefCount;
    }
}