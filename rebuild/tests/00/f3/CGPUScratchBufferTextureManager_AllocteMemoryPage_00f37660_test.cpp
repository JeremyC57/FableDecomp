// Behaviour test for 0x00F37660 CGPUScratchBufferTextureManager::AllocteMemoryPage: the row's logic over std::vector (the CArray
// base); checks the append path, the reuse of the last free slot (and its removal), and the page fields copied from the pool.
#include <cstdio>
#include <vector>
struct CScratchPool { unsigned char* Buffer; long BufferSize; };
struct CResourcePage {
    long ScratchPool; std::vector<long> ResourceList; unsigned char* NextFreeBufferAddress; long FreeSpaceSize; bool EnableAutoRestore;
};
struct CGPUScratchBufferTextureManager {
    std::vector<CResourcePage> MemoryPages;
    std::vector<CScratchPool> ScratchPools;
    std::vector<long> FreeTexturePageSlots;
    long AllocteMemoryPage(long scratchPool, bool enableAutoRestore)
    {
        CScratchPool& pool = ScratchPools[scratchPool];
        CResourcePage page;
        page.ScratchPool = scratchPool;
        page.NextFreeBufferAddress = pool.Buffer;
        page.FreeSpaceSize = pool.BufferSize;
        page.EnableAutoRestore = enableAutoRestore;
        if (FreeTexturePageSlots.size() > 0) {
            long index = FreeTexturePageSlots[FreeTexturePageSlots.size() - 1];
            FreeTexturePageSlots.pop_back();
            MemoryPages[index] = page;
            return index;
        }
        long index = (long)MemoryPages.size();
        MemoryPages.push_back(page);
        return index;
    }
};
int main()
{
    static unsigned char a[64], b[32];
    CGPUScratchBufferTextureManager m;
    CScratchPool p0 = { a, 64 }, p1 = { b, 32 };
    m.ScratchPools.push_back(p0); m.ScratchPools.push_back(p1);
    bool ok = m.AllocteMemoryPage(0, true) == 0 && m.AllocteMemoryPage(1, false) == 1 && m.AllocteMemoryPage(0, false) == 2;
    ok = ok && m.MemoryPages[1].NextFreeBufferAddress == b && m.MemoryPages[1].FreeSpaceSize == 32 && !m.MemoryPages[1].EnableAutoRestore;
    m.MemoryPages[0].ResourceList.push_back(5);
    m.FreeTexturePageSlots.push_back(2); m.FreeTexturePageSlots.push_back(0);
    ok = ok && m.AllocteMemoryPage(1, true) == 0 && m.FreeTexturePageSlots.size() == 1;
    ok = ok && m.MemoryPages[0].ScratchPool == 1 && m.MemoryPages[0].ResourceList.empty() && m.MemoryPages[0].EnableAutoRestore
            && m.MemoryPages[0].NextFreeBufferAddress == b;
    ok = ok && m.AllocteMemoryPage(0, false) == 2 && m.FreeTexturePageSlots.empty() && m.AllocteMemoryPage(0, false) == 3;
    std::printf(ok ? "00f37660_TEST PASS\n" : "00f37660_TEST FAIL\n");
    return ok ? 0 : 1;
}