// Behaviour test for CEngineStaticMapFileBlockManager::AddRequest (0x00BE06B0): a new block starts a read and enters the map
// with RefCount 1; a known block is re-counted and, for a normal request on a low-priority block, its read priority raised.
#include <cstdio>
#include <map>
enum EThreadedFilePriority { THREADED_FILE_PRIORITY_LOW = 1, THREADED_FILE_PRIORITY_BELOW_NORMAL = 2, THREADED_FILE_PRIORITY_NORMAL = 3, THREADED_FILE_PRIORITY_HIGH = 4 };
static EThreadedFilePriority g_HighLoadingPriority = THREADED_FILE_PRIORITY_HIGH;
class CBankFileAsyncData { public:
    long Offset; long Size; EThreadedFilePriority Priority; int Changes;
    void ChangeLoadingPriority(EThreadedFilePriority priority) { Priority = priority; ++Changes; }
};
static int g_reads = 0;
static CBankFileAsyncData g_pool[8];
class CBankFileAsyncEntry { public:
    CBankFileAsyncData* BeginReadDataAutoAlloc(unsigned long offset, unsigned long size, EThreadedFilePriority priority)
    {
        CBankFileAsyncData* data = &g_pool[g_reads++];
        data->Offset = (long)offset; data->Size = (long)size; data->Priority = priority; data->Changes = 0;
        return data;
    }
};
class CEngineStaticMapFileBlockManager { public:
    struct CFileBlock { long RefCount; bool LowPriority; CBankFileAsyncData* LoadingData; };
    CBankFileAsyncEntry* AsyncBankEntry;
    std::map<long, CFileBlock> Blocks;
    void AddRequest(long block, long size, bool lowPriority)
    {
        std::map<long, CFileBlock>::iterator it = Blocks.find(block);
        if (it == Blocks.end()) {
            CBankFileAsyncData* data = AsyncBankEntry->BeginReadDataAutoAlloc(block, size, lowPriority ? THREADED_FILE_PRIORITY_BELOW_NORMAL : g_HighLoadingPriority);
            CFileBlock fileBlock = { 1, lowPriority, data };
            Blocks.insert(std::pair<const long, CFileBlock>(block, fileBlock));
        } else {
            if (!lowPriority && it->second.LowPriority)
                it->second.LoadingData->ChangeLoadingPriority(g_HighLoadingPriority);
            ++it->second.RefCount;
        }
    }
};
int main()
{
    CBankFileAsyncEntry entry;
    CEngineStaticMapFileBlockManager manager; manager.AsyncBankEntry = &entry;
    bool ok = true;
    manager.AddRequest(0x100, 64, true);                                  // new low-priority block
    ok = ok && g_reads == 1 && manager.Blocks.size() == 1;
    ok = ok && g_pool[0].Offset == 0x100 && g_pool[0].Size == 64 && g_pool[0].Priority == THREADED_FILE_PRIORITY_BELOW_NORMAL;
    ok = ok && manager.Blocks[0x100].RefCount == 1 && manager.Blocks[0x100].LowPriority && manager.Blocks[0x100].LoadingData == &g_pool[0];
    manager.AddRequest(0x100, 64, true);                                  // again, still low: only the count moves
    ok = ok && g_reads == 1 && manager.Blocks[0x100].RefCount == 2 && g_pool[0].Changes == 0;
    manager.AddRequest(0x100, 64, false);                                 // a normal request raises the read; the flag stays
    ok = ok && g_reads == 1 && manager.Blocks[0x100].RefCount == 3 && g_pool[0].Changes == 1 && g_pool[0].Priority == THREADED_FILE_PRIORITY_HIGH;
    ok = ok && manager.Blocks[0x100].LowPriority;
    manager.AddRequest(0x200, 32, false);                                 // new normal block
    ok = ok && g_reads == 2 && manager.Blocks.size() == 2 && g_pool[1].Priority == THREADED_FILE_PRIORITY_HIGH && g_pool[1].Size == 32;
    ok = ok && manager.Blocks[0x200].RefCount == 1 && !manager.Blocks[0x200].LowPriority;
    manager.AddRequest(0x200, 32, false);                                 // normal request on a normal block: no priority change
    ok = ok && manager.Blocks[0x200].RefCount == 2 && g_pool[1].Changes == 0;
    if (!ok) { std::printf("00be06b0_TEST FAIL\n"); return 1; }
    std::printf("00be06b0_TEST PASS\n");
    return 0;
}