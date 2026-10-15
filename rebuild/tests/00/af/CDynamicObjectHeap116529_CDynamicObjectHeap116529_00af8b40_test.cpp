#include <cstdio>
#include <cstring>
#include <new>
static int g_baseCalls = 0;
struct CDynamicObjectHeapBase {
    virtual void v00() { }
    long AllocCount, TotalAllocatorBlocks; bool RefCountReleaseEnabled; CDynamicObjectHeapBase* HeapNext;
    CDynamicObjectHeapBase() : AllocCount(0), TotalAllocatorBlocks(0), RefCountReleaseEnabled(false), HeapNext(0) { ++g_baseCalls; }
};
struct CObjectEntry_116 { CObjectEntry_116* NextEntry; unsigned char Pad[0x70]; };
struct CObjectAllocatorBlockEntry_116_529 { CObjectEntry_116 Blocks[529]; CObjectAllocatorBlockEntry_116_529* NextEntry; };
struct CDynamicObjectHeap_116_529 : CDynamicObjectHeapBase {
    CObjectEntry_116* FreeList;
    CObjectAllocatorBlockEntry_116_529 StartBlock;
    bool AllowHeapToGrow;
    virtual void v00() { }
    CDynamicObjectHeap_116_529(bool allowHeapToGrow)
    {
        AllowHeapToGrow = allowHeapToGrow;
        FreeList = 0;
        StartBlock.NextEntry = 0;
        for (long i = 0; i < 529; ++i) {
            CObjectEntry_116* entry = &StartBlock.Blocks[i];
            entry->NextEntry = FreeList;
            FreeList = entry;
        }
    }
};
static unsigned char g_buf[sizeof(CDynamicObjectHeap_116_529)];
int main()
{
    std::memset(g_buf, 0xCD, sizeof(g_buf));
    CDynamicObjectHeap_116_529* h = new (g_buf) CDynamicObjectHeap_116_529(true);
    bool ok = g_baseCalls == 1 && h->AllowHeapToGrow && h->StartBlock.NextEntry == 0
        && h->FreeList == &h->StartBlock.Blocks[528] && h->StartBlock.Blocks[0].NextEntry == 0
        && h->StartBlock.Blocks[1].NextEntry == &h->StartBlock.Blocks[0];
    long n = 0;
    for (CObjectEntry_116* e = h->FreeList; e && n < 1000; e = e->NextEntry) ++n;
    if (n != 529) ok = false;
    if (!ok) { std::printf("00af8b40_TEST FAIL\n"); return 1; }
    std::printf("00af8b40_TEST PASS\n");
    return 0;
}