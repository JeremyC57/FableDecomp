// CBankFileAsync::HandleFailedAllocation() -- under the bank lock, walk AsyncDataInstances round-robin from just after the last
// restarted read; the first finished read that still allows a restart is reset (Finished cleared, its buffer handed back to the
// movable pool), queued in ReadsToRestart and remembered in LastRestartingData. Returns whether one was found.
// Leaf: ?HandleFailedAllocation@CBankFileAsync@@QAE_NXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009D5BF0, length 314 bytes, plain `ret`.
// Callees: CCriticalSectionLock ctor 0x00A62920 / dtor 0x00A629A0, CMovableResourceMemoryPool::FreeResource 0x00A62ED0 (through the
// expanded CBankFileAsyncData::FreeBuffer, whose out-of-line copy is the landed 0x009D5410; its real body is repeated here as the
// inline it is in retail), the STLport _Rb_tree<CBankFileAsyncData*>::insert_unique(const V&) 0x009D7CF0 (out of line).
// Layout: engine/CBankFileAsync.h (AsyncDataInstances +0x128, ReadsToRestart +0x134, LastRestartingData +0x140, CriticalSection
// +0x14C) and engine/CBankFileAsyncData.h (Finished +0x44 / AllowRestart +0x46, declared there as retail has them).
// Shape: set::upper_bound and the iterator's ++ (_Rb_global::_M_increment) are expanded; end() is hoisted into edi. The STLport
// insert_unique / _M_insert bodies must be visible (FABLE_RETAIL_STLP_MAP + FABLE_RETAIL_STLP_MAP_ORIGINAL_INSERT_BODY, as in every
// retail TU that includes <set>): VC7.1 then knows the call does not write the key through its const reference, keeps `data` in
// esi across it (`mov [edi+140h],esi` with no reload), reads the two flags through cl and homes `this` in the frame word
// the insert result later reuses. With declaration-only bodies `data` is reloaded after the call and `this` takes ebp (305v314).
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CMOVABLERESOURCE
#define FABLE_RETAIL_CHANDLEBASEDMEMORYPOOL
#define FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS
#define FABLE_RETAIL_CBANKFILEASYNC_DATA
#define FABLE_RETAIL_CCRITICALSECTIONLOCK
#define FABLE_RETAIL_CMOVABLERESOURCEMEMORYPOOL
#define FABLE_RETAIL_CMOVABLERESOURCEMEMORYPOOL_FREERESOURCE
#define FABLE_RETAIL_STLP_TREE_ERASE
#define FABLE_RETAIL_STLP_TREE_ERASE_KEY
#define FABLE_RETAIL_STLP_SET
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_MAP_ORIGINAL_INSERT_BODY
#define FABLE_RETAIL_STLP_SET_UPPER_BOUND
#define FABLE_RETAIL_CBANKFILEASYNC_HANDLEFAILEDALLOCATION
#define FABLE_RETAIL_CBANKFILEASYNCDATA_FREEBUFFER
#include "engine/CBankFileAsyncData.h"
#include "engine/CBankFileAsync.h"
#include "engine/CCriticalSectionLock.h"
#include "engine/CMovableResourceMemoryPool.h"
inline void CBankFileAsyncData::FreeBuffer()       // 0x009D5410 (landed; expanded here)
{
    if (MemoryPool && MemoryHandle != -1) {
        MemoryPool->FreeResource(MemoryHandle);
        MemoryHandle = -1;
        Buffer = 0;
        Data = 0;
    }
}
bool CBankFileAsync::HandleFailedAllocation()
{
    CCriticalSectionLock lock(CriticalSection, false);
    unsigned long count = AsyncDataInstances.size();
    if (count == 0)
        return false;
    std::set<CBankFileAsyncData*>::iterator it = AsyncDataInstances.upper_bound(LastRestartingData);
    for (unsigned long i = 0; i < count; ++i) {
        if (it == AsyncDataInstances.end())
            it = AsyncDataInstances.begin();
        CBankFileAsyncData* data = *it;
        if (data->Finished && data->AllowRestart) {
            data->Finished = false;
            data->FreeBuffer();
            ReadsToRestart.insert(data);
            LastRestartingData = data;
            return true;
        }
        ++it;
    }
    return false;
}