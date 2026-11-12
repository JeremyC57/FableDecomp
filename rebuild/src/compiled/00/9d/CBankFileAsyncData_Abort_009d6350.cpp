// CBankFileAsyncData::Abort() -- under the bank lock: a finished read is trivially aborted; otherwise drop a pending restart
// or remove the queued threaded-file object (failure = cannot abort yet), unregister the instance, free its pool memory and
// mark it aborted.
// Retail: Fable.exe (TLC, VC7.1) @ 0x009D6350, 196 bytes (+12 int3), returns bool (Ego_r.pdb ?Abort@CBankFileAsyncData@@QAE_NXZ).
// Leaf: ?Abort@CBankFileAsyncData@@QAE_NXZ
// Callees: CCriticalSectionLock ctor/dtor @ 0x00A62920 / 0x00A629A0, set<CBankFileAsyncData*>::erase(const key&) (the
//   _Rb_tree's, out of line) @ 0x009D7F40, CThreadedFile::RemoveObject(unsigned long) @ 0x0098EC90 (Ego_r.pdb
//   ?RemoveObject@CThreadedFile@@QAE_NK@Z; a thunk onto CThreadedFileManager::RemoveObject 0x0098EB20 of the global manager),
//   CMovableResourceMemoryPool::FreeResource(long) @ 0x00A62ED0 (?FreeResource@CMovableResourceMemoryPool@@QAEXJ@Z, a tail `jmp`
//   onto CHandleBasedMemoryPool::Free 0x00A02B30; the same callee as the landed ~C3DMesh2 0x00A895F0).
// Shape: the result is a bool in bl set true in both arms (restart dropped / object removed) and tested once: an `if (result)`
//   block holds the unregister, free and Aborted store, so Aborted gets an immediate 1 (a single `result = true` before the
//   erase stores bl); the early `return true` for a finished read keeps its own lock-dtor tail; Finished is volatile (header).
// Layout: engine/CBankFileAsyncData.h + engine/CBankFileAsync.h (FABLE_RETAIL_CBANKFILEASYNC_DATA; Ego_r.pdb members).
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
#define FABLE_RETAIL_CBANKFILEASYNCDATA_ABORT
#include "engine/CBankFileAsyncData.h"
#include "engine/CBankFileAsync.h"
#include "engine/CCriticalSectionLock.h"
#include "engine/CMovableResourceMemoryPool.h"
class CThreadedFile {                              // no shared header: the one member this row calls
public:
    bool RemoveObject(unsigned long uid);          // 0x0098EC90 ?RemoveObject@CThreadedFile@@QAE_NK@Z
};

bool CBankFileAsyncData::Abort()
{
    CCriticalSectionLock lock(BankFile.CriticalSection, false);
    if (Finished)
        return true;
    bool result = false;
    if (BankFile.ReadsToRestart.erase(this) != 0)
        result = true;
    else if (BankFile.ThreadedFile->RemoveObject(ThreadedFileUID)) {
        ThreadedFileUID = 0;
        result = true;
    }
    if (result) {
        BankFile.AsyncDataInstances.erase(this);
        if (MemoryPool && MemoryHandle != -1) {
            MemoryPool->FreeResource(MemoryHandle);
            MemoryHandle = -1;
            Buffer = 0;
            Data = 0;
        }
        Aborted = true;
    }
    return result;
}