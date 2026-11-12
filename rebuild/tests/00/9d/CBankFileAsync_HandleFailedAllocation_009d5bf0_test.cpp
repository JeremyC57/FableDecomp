// Behaviour test for CBankFileAsync::HandleFailedAllocation (0x009D5BF0): round-robin search, from just after the last restarted
// read, for the first finished read that allows a restart; it is reset, queued and remembered.
#include <cstdio>
#include <set>
struct CPool { int Freed; long LastHandle; void FreeResource(long handle) { ++Freed; LastHandle = handle; } };
class CBankFileAsyncData { public:
    void* Data; void* Buffer; CPool* MemoryPool; long MemoryHandle; volatile bool Finished; volatile bool AllowRestart;
    void FreeBuffer()
    {
        if (MemoryPool && MemoryHandle != -1) {
            MemoryPool->FreeResource(MemoryHandle);
            MemoryHandle = -1;
            Buffer = 0;
            Data = 0;
        }
    }
};
class CBankFileAsync { public:
    std::set<CBankFileAsyncData*> AsyncDataInstances;
    std::set<CBankFileAsyncData*> ReadsToRestart;
    CBankFileAsyncData* LastRestartingData;
    bool HandleFailedAllocation()
    {
        unsigned long count = (unsigned long)AsyncDataInstances.size();
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
};
int main()
{
    CPool pool = { 0, 0 };
    CBankFileAsyncData reads[4];
    int marker;
    for (int i = 0; i < 4; ++i) {
        reads[i].Data = &marker; reads[i].Buffer = &marker; reads[i].MemoryPool = &pool; reads[i].MemoryHandle = 10 + i;
        reads[i].Finished = false; reads[i].AllowRestart = false;
    }
    CBankFileAsync bank; bank.LastRestartingData = 0;
    bool ok = true;
    ok = ok && !bank.HandleFailedAllocation();                           // empty set
    for (int i = 0; i < 4; ++i) bank.AsyncDataInstances.insert(&reads[i]);
    ok = ok && !bank.HandleFailedAllocation() && pool.Freed == 0;         // nothing finished
    reads[1].Finished = true;                                             // finished but no restart allowed
    reads[0].Finished = true; reads[0].AllowRestart = true;
    reads[2].Finished = true; reads[2].AllowRestart = true;
    reads[3].AllowRestart = true;                                         // restart allowed but not finished
    ok = ok && bank.HandleFailedAllocation() && bank.LastRestartingData == &reads[0];
    ok = ok && !reads[0].Finished && reads[0].MemoryHandle == -1 && reads[0].Buffer == 0 && reads[0].Data == 0;
    ok = ok && pool.Freed == 1 && pool.LastHandle == 10 && bank.ReadsToRestart.count(&reads[0]) == 1;
    reads[0].Finished = true; reads[0].MemoryPool = 0; reads[0].MemoryHandle = 10;                    // candidate again, but the walk starts after it
    ok = ok && bank.HandleFailedAllocation() && bank.LastRestartingData == &reads[2] && pool.LastHandle == 12;
    ok = ok && bank.HandleFailedAllocation() && bank.LastRestartingData == &reads[0];   // wraps round; no pool: nothing freed
    ok = ok && pool.Freed == 2 && reads[0].MemoryHandle == 10 && bank.ReadsToRestart.size() == 2;
    ok = ok && !bank.HandleFailedAllocation() && reads[1].Finished;
    if (!ok) { std::printf("009d5bf0_TEST FAIL\n"); return 1; }
    std::printf("009d5bf0_TEST PASS\n");
    return 0;
}