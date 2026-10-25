// Behaviour test for 0x00AF87C0 CDynamicObjectHeap<156,2100>::CDynamicObjectHeap(bool): the base heap is constructed,
// the grow flag stored, the block link cleared, and all 2100 embedded 156-byte entries pushed onto the free list in
// order, so the list runs from the last entry back to the first (whose link is null).
#include <cstdio>
#include <new>

static int g_baseCtorCalls = 0;

struct HeapBase {
    long AllocCount;
    HeapBase() : AllocCount(0) { ++g_baseCtorCalls; }
};

template <unsigned int TSize, unsigned int TCount> struct Heap : HeapBase {
    struct Entry { Entry* NextEntry; unsigned char Pad[TSize - sizeof(Entry*)]; };
    struct Block { Entry Blocks[TCount]; Block* NextEntry; };
    Entry* FreeList;
    Block  StartBlock;
    bool   AllowHeapToGrow;
    Heap(bool allowHeapToGrow)
    {
        AllowHeapToGrow = allowHeapToGrow;
        FreeList = 0;
        StartBlock.NextEntry = 0;
        for (long i = 0; i < (long)TCount; ++i) {
            StartBlock.Blocks[i].NextEntry = FreeList;
            FreeList = &StartBlock.Blocks[i];
        }
    }
};

typedef Heap<156, 2100> BigHeap;
static double g_storage[sizeof(BigHeap) / sizeof(double) + 1];

int main()
{
    unsigned char* raw = (unsigned char*)g_storage;
    for (unsigned int i = 0; i < sizeof(g_storage); ++i) raw[i] = 0xAB;          // garbage before construction
    BigHeap* heap = new (g_storage) BigHeap(true);
    int ok = 1;
    if (g_baseCtorCalls != 1 || !heap->AllowHeapToGrow || heap->StartBlock.NextEntry != 0) ok = 0;
    if (heap->FreeList != &heap->StartBlock.Blocks[2099]) ok = 0;
    unsigned int count = 0;
    BigHeap::Entry* expect = &heap->StartBlock.Blocks[2099];
    for (BigHeap::Entry* e = heap->FreeList; e; e = e->NextEntry) {
        if (e != expect) { ok = 0; break; }
        --expect;
        if (++count > 2100) { ok = 0; break; }
    }
    if (count != 2100) ok = 0;
    BigHeap* small = new (g_storage) BigHeap(false);
    if (small->AllowHeapToGrow || g_baseCtorCalls != 2 || small->StartBlock.Blocks[0].NextEntry != 0) ok = 0;
    std::printf(ok ? "00af87c0_TEST PASS\n" : "00af87c0_TEST FAIL\n");
    return ok ? 0 : 1;
}