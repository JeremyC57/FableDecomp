// Behaviour test for 009f2a40: src/009f2a40.cpp copied verbatim (shared headers expanded inline), callees stubbed below so calls
// and arguments are observable. CMemoryAllocator::InternalAllocate(long size, unsigned long type, bool flag)
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CMemoryAllocator::InternalAllocate(long size, unsigned long type, bool flag) -- allocate a tracked block: clamp the size to >= 1, add the 0x14 bytes of header/debug trailer, pick the size-class allocator of the allocator type (AllocSizeToAllocator[type][min(total, MaxFixedAllocationSizes[type] + 1)]), stamp its ID in the byte before the block when the block lies outside the reserved region, count it, and fill the debug info of the payload (block + 4)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009F2A40, length 122 bytes.
// Callees: the size-class allocator's virtual slot 0 (Allocate), CMemoryAllocator::InitDebugInfo @ 0x009F28D0 (landed).
// Globals: the reserved region bounds 0x013CAAA4 / 0x013CAAA8.
// Layout (PDB CMemoryAllocator): NoAllocations +0x40c, MaxFixedAllocationSizes[15] +0x424, AllocSizeToAllocator*[15] +0x460;
//   CAllocSizeToAllocator (8 bytes) PAllocator +0, ID +4.
// Shape: the clamp runs on the running total itself: `total = size; if ((long)total < 1) total = 1; size = total;
//   total += 0x14;` loads the size into eax before the register pushes, clamps it there with `jge`, copies it to ebp and
//   adds the header in place (retail order). Clamping `size` first (if, ?:, a Max inline) keeps it in ebp and builds the
//   total with `lea eax,[ebp+14h]`; a Max(a, b) `a > b` inline also gives `jg`.
extern unsigned char* g_pRegionStart;       // 0x013CAAA4
extern unsigned char* g_pRegionEnd;         // 0x013CAAA8
struct CMemoryAllocatorBase {
    virtual void* Allocate(unsigned long size);
};
struct CAllocSizeToAllocator {
    CMemoryAllocatorBase* PAllocator;
    unsigned char ID;
};
struct CMemoryAllocator {
    unsigned long MemoryAllocatedPerTrackingID[256];
    unsigned long MemoryAllocated;
    void** MemoryAuditBuffer;
    void* Heap;
    unsigned long NoAllocations;
    unsigned long NoAllocators;
    unsigned char CurrentTrackingID;
    bool AllocatorExists[15];
    long MaxFixedAllocationSizes[15];
    CAllocSizeToAllocator* AllocSizeToAllocator[15];
    void InitDebugInfo(void* block, bool flag, long size);
    void* InternalAllocate(long size, unsigned long type, bool flag);
};
void* CMemoryAllocator::InternalAllocate(long size, unsigned long type, bool flag)
{
    unsigned long total = size;
    if ((long)total < 1)
        total = 1;
    size = total;
    total += 0x14;
    unsigned long index = total;
    if (total > (unsigned long)(MaxFixedAllocationSizes[type] + 1))
        index = MaxFixedAllocationSizes[type] + 1;
    CAllocSizeToAllocator* entry = &AllocSizeToAllocator[type][index];
    unsigned char* mem = (unsigned char*)entry->PAllocator->Allocate(total);
    if (mem) {
        if (mem < g_pRegionStart || mem >= g_pRegionEnd)
            mem[-1] = entry->ID;
        ++NoAllocations;
        mem += 4;
        InitDebugInfo(mem, flag, size);
    }
    return mem;
}

// ---- stubs and driver ----
unsigned char* g_pRegionStart;
unsigned char* g_pRegionEnd;
static unsigned char g_heap[4096];
static int g_allocCalls; static unsigned long g_allocSize; static const void* g_allocThis; static unsigned char* g_next;
void* CMemoryAllocatorBase::Allocate(unsigned long size) { ++g_allocCalls; g_allocSize = size; g_allocThis = this; return g_next; }
static int g_initCalls; static void* g_initBlock; static bool g_initFlag; static long g_initSize;
void CMemoryAllocator::InitDebugInfo(void* block, bool flag, long size) { ++g_initCalls; g_initBlock = block; g_initFlag = flag; g_initSize = size; }
static void Reset() { g_allocCalls = g_initCalls = 0; g_allocSize = 0; g_allocThis = 0; g_initBlock = 0; g_initSize = -999; }
int main()
{
    static CMemoryAllocator m; memset(&m, 0, sizeof m);
    static CMemoryAllocatorBase allocators[4];
    static CAllocSizeToAllocator table[2][300];
    for (int t = 0; t < 2; ++t)
        for (int i = 0; i < 300; ++i) { table[t][i].PAllocator = &allocators[(t * 2 + (i > 101)) % 4]; table[t][i].ID = (unsigned char)(i + t * 7); }
    m.AllocSizeToAllocator[3] = table[0]; m.MaxFixedAllocationSizes[3] = 100;
    m.AllocSizeToAllocator[5] = table[1]; m.MaxFixedAllocationSizes[5] = 250;
    g_pRegionStart = g_heap + 2048; g_pRegionEnd = g_heap + 3072;
    // a small block outside the reserved region: size + 0x14 picks the entry, its ID goes in the byte before the block
    Reset(); g_next = g_heap + 64; g_heap[63] = 0;
    void* p = m.InternalAllocate(10, 3, true);
    CHECK(g_allocCalls == 1 && g_allocSize == 30 && g_allocThis == &allocators[0]);
    CHECK(g_heap[63] == 30 && m.NoAllocations == 1 && p == g_heap + 68);
    CHECK(g_initCalls == 1 && g_initBlock == g_heap + 68 && g_initFlag == true && g_initSize == 10);
    // zero and negative sizes are clamped to one byte (and the debug info sees the clamped size)
    Reset(); g_next = g_heap + 128; p = m.InternalAllocate(0, 3, false);
    CHECK(g_allocSize == 21 && g_heap[127] == 21 && g_initSize == 1 && !g_initFlag && p == g_heap + 132);
    Reset(); g_next = g_heap + 192; p = m.InternalAllocate(-5, 3, false);
    CHECK(g_allocSize == 21 && g_initSize == 1 && m.NoAllocations == 3);
    // totals past the largest fixed size use the overflow entry (max + 1) but still ask for the full total
    Reset(); g_next = g_heap + 256; p = m.InternalAllocate(200, 3, true);
    CHECK(g_allocSize == 220 && g_heap[255] == 101 && g_allocThis == &allocators[0] && g_initSize == 200);
    Reset(); g_next = g_heap + 320; p = m.InternalAllocate(81, 3, true);    // total 101 == max + 1: the entry itself
    CHECK(g_allocSize == 101 && g_heap[319] == 101);
    Reset(); g_next = g_heap + 384; p = m.InternalAllocate(79, 3, true);    // total 99: below the limit
    CHECK(g_allocSize == 99 && g_heap[383] == 99);
    // the bound compare is unsigned: a huge size still lands on the overflow entry
    Reset(); g_next = g_heap + 448; p = m.InternalAllocate(0x7fffffff, 3, true);
    CHECK(g_allocSize == 0x80000013UL && g_heap[447] == 101);
    // another type uses its own table and limit
    Reset(); g_next = g_heap + 512; p = m.InternalAllocate(150, 5, false);
    CHECK(g_allocSize == 170 && g_heap[511] == (unsigned char)(170 + 7) && g_allocThis == &allocators[3]);
    // inside the reserved region: no ID stamp; at the region end: stamped
    Reset(); g_next = g_heap + 2100; g_heap[2099] = 0xAB; p = m.InternalAllocate(10, 3, true);
    CHECK(g_heap[2099] == 0xAB && p == g_heap + 2104 && g_initCalls == 1);
    Reset(); g_next = g_heap + 3072; g_heap[3071] = 0xAB; p = m.InternalAllocate(10, 3, true);
    CHECK(g_heap[3071] == 30);
    Reset(); g_next = g_heap + 2048; g_heap[2047] = 0xAB; p = m.InternalAllocate(10, 3, true);
    CHECK(g_heap[2047] == 0xAB);
    // allocation failure: null back, nothing counted or initialised
    unsigned long before = m.NoAllocations;
    Reset(); g_next = 0; p = m.InternalAllocate(10, 3, true);
    CHECK(p == 0 && g_allocCalls == 1 && g_initCalls == 0 && m.NoAllocations == before);
    if (fails == 0) printf("009f2a40_TEST PASS\n");
    return fails;
}
