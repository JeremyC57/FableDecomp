// behaviour test for 0x00AF8B40 CDynamicObjectHeap<116,529>::CDynamicObjectHeap(bool), linked against the built
// constructor: base constructor called once, grow flag stored, block link cleared, all 529 entries on the free list
// (last entry first out)
#include <cstdio>
#include <cstdlib>
#include <new>
#include <cstring>
#include <cstddef>
static int g_baseCalls = 0;
class CDynamicObjectHeapBase {
public:
    virtual ~CDynamicObjectHeapBase();
    long                    AllocCount;
    long                    TotalAllocatorBlocks;
    bool                    RefCountReleaseEnabled;
    CDynamicObjectHeapBase* HeapNext;
    CDynamicObjectHeapBase();
};
CDynamicObjectHeapBase::CDynamicObjectHeapBase() : AllocCount(0), TotalAllocatorBlocks(0), RefCountReleaseEnabled(false), HeapNext(0) { ++g_baseCalls; }
CDynamicObjectHeapBase::~CDynamicObjectHeapBase() { }

template <long ObjectSize, long BlockCount>
class CDynamicObjectHeap : public CDynamicObjectHeapBase {
public:
    struct CObjectEntry {
        CObjectEntry* NextEntry;
        unsigned char Pad[ObjectSize - 4];
    };
    struct CObjectAllocatorBlockEntry {
        CObjectEntry                Blocks[BlockCount];
        CObjectAllocatorBlockEntry* NextEntry;
    };
    CObjectEntry*              FreeList;
    CObjectAllocatorBlockEntry StartBlock;
    bool                       AllowHeapToGrow;
    CDynamicObjectHeap(bool allowHeapToGrow);
    virtual ~CDynamicObjectHeap();
};
template <long ObjectSize, long BlockCount>
CDynamicObjectHeap<ObjectSize, BlockCount>::~CDynamicObjectHeap() { }
template CDynamicObjectHeap<116, 529>::~CDynamicObjectHeap();   // the built object's deleting destructor needs it

typedef CDynamicObjectHeap<116, 529> THeap;
int main()
{
    if (sizeof(THeap) != 0xefd4 || offsetof(THeap, AllowHeapToGrow) != 0xefd0) { std::printf("00af8b40_TEST FAIL layout\n"); return 1; }
    void* mem = std::malloc(sizeof(THeap));
    std::memset(mem, 0xCD, sizeof(THeap));
    THeap* h = new (mem) THeap(true);
    bool ok = g_baseCalls == 1 && h->AllowHeapToGrow && h->StartBlock.NextEntry == 0
        && h->FreeList == &h->StartBlock.Blocks[528] && h->StartBlock.Blocks[0].NextEntry == 0;
    long n = 0;
    for (THeap::CObjectEntry* e = h->FreeList; e && n < 1000; e = e->NextEntry) {
        if (e != &h->StartBlock.Blocks[528 - n]) ok = false;
        ++n;
    }
    if (n != 529) ok = false;
    h->~THeap();
    THeap* g = new (mem) THeap(false);
    if (g->AllowHeapToGrow || g_baseCalls != 2) ok = false;
    g->~THeap();
    std::free(mem);
    if (!ok) { std::printf("00af8b40_TEST FAIL\n"); return 1; }
    std::printf("00af8b40_TEST PASS\n");
    return 0;
}
